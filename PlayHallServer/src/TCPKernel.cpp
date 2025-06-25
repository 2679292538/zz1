#include<TCPKernel.h>
#include "packdef.h"
#include<stdio.h>
#include<sys/time.h>


using namespace std;


//设置网络协议映射
void TcpKernel::setNetPackMap()
{
    //清空映射
    bzero( m_NetPackMap , sizeof(m_NetPackMap) );
    NetPackMap(DEF_PACK_REGISTER_RQ)    = &TcpKernel::RegisterRq;
    NetPackMap(DEF_PACK_LOGIN_RQ)       = &TcpKernel::LoginRq;
    NetPackMap(DEF_PACK_CREATEROOM_RQ)  = &TcpKernel::CreateRoomRq;
    NetPackMap(DEF_PACK_JOINROOM_RQ)    = &TcpKernel::JoinRoomRq;
    NetPackMap(DEF_PACK_LEAVEROOM_RQ)   = &TcpKernel::LeaveRoomRq;
    NetPackMap(DEF_PACK_AUDIO_FRAME)    = &TcpKernel::AudioFrameRq;
    NetPackMap(DEF_PACK_VIDEO_FRAME)    = &TcpKernel::VideoFrameRq;
    NetPackMap(DEF_PACK_AUDIO_REGISTER)    = &TcpKernel::AudioRegister;
    NetPackMap(DEF_PACK_VIDEO_REGISTER)    = &TcpKernel::VideoRegister;
    //协议映射赋值
    //m_logic->setNetPackMap();

}


TcpKernel::TcpKernel()
{

}

TcpKernel::~TcpKernel()
{
   
}

TcpKernel *TcpKernel::GetInstance()
{
    static TcpKernel kernel;
    return &kernel;
}

int TcpKernel::Open( int port)
{
    initRand();

    m_sql = new CMysql;
    // 数据库 使用127.0.0.1 地址  用户名root 密码colin123 数据库 wechat 没有的话需要创建 不然报错
    if(  !m_sql->ConnectMysql( DEF_DB_IP , DEF_DB_USER, DEF_DB_PWD, DEF_DB_NAME )  )
    {
        printf("Conncet Mysql Failed...\n");
        return FALSE;
    }
    else
    {
        printf("MySql Connect Success...\n");
    }
    //初始网络
    m_tcp = new Block_Epoll_Net;
    bool res = m_tcp->InitNet( port , &TcpKernel::DealData ) ;
    if( !res )
        err_str( "net init fail:" ,-1);

    
    setNetPackMap();

    return TRUE;
}

void TcpKernel::Close()
{
    m_sql->DisConnect();

}

//随机数初始化
void TcpKernel::initRand()
{
    struct timeval time;
    gettimeofday( &time , NULL);
    srand( time.tv_sec + time.tv_usec );
}

void TcpKernel::DealData(sock_fd clientfd,char *szbuf,int nlen)
{
    PackType type = *(PackType*)szbuf;
    if( (type >= DEF_PACK_BASE) && ( type < DEF_PACK_BASE + DEF_PACK_COUNT) )
    {
        PFUN pf = NetPackMap( type );
        if( pf )
        {
            (TcpKernel::GetInstance()->*pf)( clientfd , szbuf , nlen);
        }
    }

    return;
}

void TcpKernel::EventLoop()
{
    printf("event loop\n");
    m_tcp->EventLoop();
}

void TcpKernel::SendData(sock_fd clientfd, char *szbuf, int nlen)
{
    m_tcp->SendData(clientfd , szbuf ,nlen );
}

void TcpKernel::LoginRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("%d",clientfd);


    STRU_LOGIN_RQ *rq=(STRU_LOGIN_RQ*)szbuf;

    STRU_LOGIN_RS rs;

    char strsql[1024]="";
    list<string>lstres;
    sprintf(strsql,"select passw,id,name from t_user where tel='%s';",rq->m_tel);
    if(!m_sql->SelectMysql(strsql,3,lstres))
    {
       printf("select error;%s\n",strsql);
       return;
    }
    if(lstres.size()==0)
    {
        rs.m_lResult=user_not_exist;

    }else
    {
        if(strcmp(rq->m_password,lstres.front().c_str())!=0)
        {
            rs.m_lResult=password_error;
        }
        else
        {
             lstres.pop_front();
             int id=atoi(lstres.front().c_str());
             lstres.pop_front();
             //sock保护起来，id->sock
             UserInfo*pinfo=new UserInfo;
             pinfo->m_id=id;
             pinfo->m_roomid=0;
             pinfo->m_sockfd=clientfd;
             strcpy(pinfo->m_userName,lstres.front().c_str());
                 lstres.pop_front();
                 if(m_mapIDToUserInfo.IsExist(pinfo->m_id))
                {

                }

                 m_mapIDToUserInfo.insert(pinfo->m_id,pinfo);

             rs.m_userid=id;
             rs.m_lResult=login_success;
             strcpy(rs.m_name,pinfo->m_userName);
        }
    }
    SendData(clientfd,(char*)&rs,sizeof(rs));

}

void TcpKernel::RegisterRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("%d",clientfd);
    //1.拆包
    STRU_REGISTER_RQ* rq=(STRU_REGISTER_RQ*)szbuf;
    STRU_REGISTER_RS rs;

    //获取tel password name
    //查表t——user，根据tel查tel

    char sqlstr[1024]={0};
    sprintf(sqlstr,"select tel from t_user where tel='%s';",rq->m_tel);
    list<string>resList;
    if(!m_sql->SelectMysql(sqlstr,1,resList))
    {
        printf("SelectMysql error:%s\n",sqlstr);
        return;
    }
    //user
    if(resList.size()>0)
    {
        rs.m_lResult=tel_is_exist;
    }
    else
    {
        char sqlstr[1024]={0};
        resList.clear();
        sprintf(sqlstr,"select name from t_user where name='%s';",rq->m_name);
        if(!m_sql->SelectMysql(sqlstr,1,resList))
        {
            printf("Selectmysql error:%s\n",sqlstr);
            return;

        }
        if(resList.size()>0)
        {
            rs.m_lResult=name_is_exist;
        }
        else
        {

            rs.m_lResult=register_success;
            sprintf(sqlstr,"insert into t_user(tel,passw,name,iconid,feeling)values ('%s','%s','%s','%d','%s');",rq->m_tel,rq->m_password,rq->m_name,1,"bijiaolan");

            if(!m_sql->UpdataMysql(sqlstr))
            {
                printf("updatamysql error:%s\n",sqlstr);
            }

        }
    }
    m_tcp->SendData(clientfd,(char*)&rs,sizeof(rs));

    //有 tel存在
    //没有 name有没有
    //有 name存在
    //没有 写表 tel，pass， name 头像和签名的默认值返回注册成功
}

void TcpKernel::CreateRoomRq(int clientfd, char *szbuf, int len)
{
    printf("clientfd:%d Createroomrq\n",clientfd);
    //chaibao
    STRU_CREATEROOM_RQ *rq=(STRU_CREATEROOM_RQ*)szbuf;
    //
    int roomid=0;
    do
    {
        roomid=rand()%99999999+1;
    }
    while(m_mapIDToRoomid.IsExist(roomid));
    list<int>lst;
    lst.push_back(rq->m_UserID);
    m_mapIDToRoomid.insert(roomid,lst);
    printf("roomid=%d\n",roomid);
    //huifu
    STRU_CREATEROOM_RS rs;
    rs.m_RoomID=roomid;
    rs.m_lResult=1;
    SendData(clientfd,(char*)&rs,sizeof (rs));

}

void TcpKernel::JoinRoomRq(int clientfd, char *szbuf, int len)
{
    printf("clientfd:%d joinroomrq\n",clientfd);

    STRU_JOINROOM_RQ*rq=(STRU_JOINROOM_RQ*)szbuf;
    STRU_JOINROOM_RS rs;

    if(!m_mapIDToRoomid.IsExist( rq->m_RoomID))
    {
        rs.m_lResult=0;
        SendData(clientfd,(char*)&rs,sizeof(rs));

        return;
    }
    //cunzaizefanhuichenggong
    rs.m_lResult=1;
    rs.m_RoomID=rq->m_RoomID;

    SendData(clientfd,(char*)&rs,sizeof(rs));

    if(!m_mapIDToUserInfo.IsExist(rq->m_UserID))

        return;

    UserInfo*joiner=m_mapIDToUserInfo.find(rq->m_UserID);

    STRU_ROOM_MEMBER_RQ joinrq;
    joinrq.m_UserID=rq->m_UserID;
    strcpy(joinrq.m_szUser,joiner->m_userName);

    //geng xin zi ji xin xi

    SendData(clientfd,(char*)&joinrq,sizeof(joinrq));

    list<int>lstRoomMem=m_mapIDToRoomid.find(rq->m_RoomID);


    for(auto ite=lstRoomMem.begin();ite!=lstRoomMem.end();++ite)
    {

        int Memid=*ite;


        if(!m_mapIDToUserInfo.IsExist(Memid))continue;

        UserInfo*meminfo=m_mapIDToUserInfo.find(Memid);

        STRU_ROOM_MEMBER_RQ memrq;
        memrq.m_UserID=meminfo->m_id;

        strcpy(memrq.m_szUser,meminfo->m_userName);

        SendData(meminfo->m_sockfd,(char*)&joinrq,sizeof(joinrq));

        SendData(clientfd,(char*)&memrq,sizeof (memrq));
    }

    //jiaren
    lstRoomMem.push_back(rq->m_UserID);
    m_mapIDToRoomid.insert(rq->m_RoomID,lstRoomMem);
}

void TcpKernel::LeaveRoomRq(int clientfd, char *szbuf, int len)
{
     printf("clientfd:%d leaveroomrq\n",clientfd);
    STRU_LEAVEROOM_RQ *rq=(STRU_LEAVEROOM_RQ*)szbuf;

    if(!m_mapIDToRoomid.IsExist(rq->m_RoomId))
        return;

    //bianlimeigeyonghu
    list<int>lst=m_mapIDToRoomid.find(rq->m_RoomId);

    for(auto ite=lst.begin();ite!=lst.end();)
    {

        //shubushiziji
        int userid=*ite;
        //shiziji
        if(userid==rq->m_nUserId)
        {
            ite=lst.erase(ite);

        }
        else
        {

            if(m_mapIDToUserInfo.IsExist(userid))
            {

                UserInfo*info=m_mapIDToUserInfo.find(userid);
                SendData(info->m_sockfd,szbuf,len);

            }
            ++ite;
        }
    }
    if(lst.size()==0)
    {
        m_mapIDToRoomid.erase(rq->m_RoomId);
        return;
    }

    m_mapIDToRoomid.insert(rq->m_RoomId,lst);
}

///int type;
///int userId;
///int roomId;
///int min;
///int sec;
/// int msec;
/// QByteArray audioFrame;
void TcpKernel::AudioFrameRq(int clientfd, char *szbuf, int len)
{
    printf("clientfd:%d audioframerq\n",clientfd);
    char*tmp=szbuf;
    //tiao guo type
    tmp+=sizeof (int);
    //du qu userid
    int uid=*(int*)tmp;
    //tiao guo userid
    tmp+=sizeof(int);

    int roomid=*(int*)tmp;

    if(!m_mapIDToRoomid.IsExist(roomid))return;
    list<int>lst=m_mapIDToRoomid.find(roomid);
    for(auto ite=lst.begin();ite!=lst.end();++ite)
    {
        int userid=*ite;
        //ping bi zi ji
        if(uid==userid)continue;
        if(!m_mapIDToUserInfo.IsExist(userid))continue;
        UserInfo *userinfo=m_mapIDToUserInfo.find(userid);

        SendData(userinfo->m_sockfd,szbuf,len);
          printf("1");
    }


}

void TcpKernel::VideoFrameRq(int clientfd, char *szbuf, int len)
{
    printf("clientfd:%d Videoframerq\n",clientfd);
    char*tmp=szbuf;
    //tiao guo type
    tmp+=sizeof (int);
    //du qu userid
    int uid=*(int*)tmp;
    //tiao guo userid
    tmp+=sizeof(int);

    int roomid=*(int*)tmp;

    if(!m_mapIDToRoomid.IsExist(roomid))return;
    list<int>lst=m_mapIDToRoomid.find(roomid);
    for(auto ite=lst.begin();ite!=lst.end();++ite)
    {
        int userid=*ite;
        //ping bi zi ji
        if(uid==userid)continue;
        if(!m_mapIDToUserInfo.IsExist(userid))continue;
        UserInfo *userinfo=m_mapIDToUserInfo.find(userid);

        SendData(userinfo->m_sockfd,szbuf,len);

    }

}

void TcpKernel::AudioRegister(int clientfd, char *szbuf, int len)
{
     printf("clientfd:%d audioregister\n",clientfd);

     STRU_AUDIO_REGISTER*rq=(STRU_AUDIO_REGISTER*)szbuf;
     int userid=rq->m_userid;
     if(m_mapIDToUserInfo.IsExist(userid))
     {
         UserInfo*info= m_mapIDToUserInfo.find(userid);
         info->m_audiofd=clientfd;
     }
}

void TcpKernel::VideoRegister(int clientfd, char *szbuf, int len)
{
     printf("clientfd:%d audioregister\n",clientfd);

     STRU_VIDEO_REGISTER*rq=(STRU_VIDEO_REGISTER*)szbuf;
     int userid=rq->m_userid;
     if(m_mapIDToUserInfo.IsExist(userid))
     {
         UserInfo*info= m_mapIDToUserInfo.find(userid);
         info->m_videofd=clientfd;
     }
}
