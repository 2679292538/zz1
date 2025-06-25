#include "ckernel.h"
#include"QDebug"
#include"md5.h"
#include<QMessageBox>
#include<QTime>

#define NetPackMap(a) m_netPackMap[a-DEF_PACK_BASE]
//设置协议映射关系
void Ckernel::slot_setNetPackMap()
{
    memset(m_netPackMap,0,sizeof(m_netPackMap));
    //m_netPackMap[_DEF_PACK_LOGIN_RS-_DEF_PACK_BASE]=&Ckernel::slot_dealLoginRs;
    NetPackMap(DEF_PACK_LOGIN_RS)      = &Ckernel::slot_dealLoginRs;
    NetPackMap(DEF_PACK_REGISTER_RS)   = &Ckernel::slot_dealRegisterRs;
    NetPackMap(DEF_PACK_CREATEROOM_RS) = &Ckernel::slot_dealcreateroomRs;
    NetPackMap(DEF_PACK_JOINROOM_RS)   = &Ckernel::slot_dealjoinroomRs;
    NetPackMap(DEF_PACK_ROOM_MEMBER)   = &Ckernel::slot_dealmemberRq;
    NetPackMap(DEF_PACK_LEAVEROOM_RQ)  = &Ckernel::slot_dealLeaveRoomRq;
    NetPackMap(DEF_PACK_AUDIO_FRAME)  = &Ckernel::slot_dealAudioFrameRq;
     NetPackMap(DEF_PACK_VIDEO_FRAME)  = &Ckernel::slot_dealVideoFrameRq;

}
#include<QSettings>
#include<QApplication>
#include<QFileInfo>
void Ckernel::initconfig()
{
    m_serverip=_DEF_SERVERIP;

    //加载配置文件 ip设置为配置文件里面的ip
    //路径设置 exe同级的目录->applicationdirpath，config.ini
    QString path=QApplication::applicationDirPath()+"/config.ini";
   //判断是否存在
   QFileInfo info(path);
   QSettings settings(path,QSettings::IniFormat,NULL);//有的话打开，没有创建
   if(info.exists())
   {
       //打开配置文件，移动到Net组
     settings.beginGroup("Net");
      //读取赋值
      QVariant ip= settings.value("ip");

      QString strip=ip.toString();

      //结束
      settings.endGroup();
      if(!strip.isEmpty())
      {
          m_serverip=strip;

     }
   }
   else
   {
      settings.beginGroup("Net");
      settings.setValue("ip",m_serverip);
       settings.endGroup();
   }
   qDebug()<<m_serverip;
   qDebug()<<_DEF_SERVERIP;
   //没有配置文件，写为默认的ip


}



Ckernel::Ckernel(QObject *parent) : QObject(parent),m_id(0),m_roomid(0)
{
    qDebug()<<"mainthread::"<<QThread::currentThreadId();
    slot_setNetPackMap();
    initconfig();
    m_wechatdilog=new WeChatDialog;
    connect(m_wechatdilog,SIGNAL(SIG_close()),this,SLOT(slot_destory()));
    connect(m_wechatdilog,SIGNAL(SIG_createroom()),this,SLOT(slot_createroom()));
    connect(m_wechatdilog,SIGNAL(SIG_joinroom()),this,SLOT(slot_joinroom()));
    //m_wechatdilog->show();
    m_plogindialog=new LogininDialog;
    connect(m_plogindialog,SIGNAL(SIG_loginCommit(QString,QString)),this,SLOT(slot_loginCommit(QString,QString)));
    connect(m_plogindialog,SIGNAL(SIG_close()),this,SLOT(slot_destory()));
    connect(m_plogindialog,SIGNAL(SIG_registerCommit(QString,QString,QString)),this,SLOT(slot_registerCommit(QString,QString,QString)));
    m_plogindialog->show();
    m_roomdialog=new RoomDialog;
     connect(m_roomdialog,SIGNAL(SIG_close()),this,SLOT(slot_quitroom()));
     connect(m_roomdialog,SIGNAL(SIG_audioPause()),this,SLOT(slot_pauseAudio()));
     connect(m_roomdialog,SIGNAL(SIG_audioStart()),this,SLOT(slot_startAudio()));
     connect(m_roomdialog,SIGNAL(SIG_videoPause()),this,SLOT(slot_pauseVideo()));
     connect(m_roomdialog,SIGNAL(SIG_videoStart()),this,SLOT(slot_startVideo()));
     connect(m_roomdialog,SIGNAL(SIG_screenStart()),this,SLOT(slot_startScreen()));
     connect(m_roomdialog,SIGNAL(SIG_screenPause()),this,SLOT(slot_pauseScreen()));
    m_pclient=new TcpClientMediator;
    m_pclient->OpenNet(_DEF_SERVERIP,_DEF_PORT);
    //m_pclient->OpenNet(m_serverip.toStdString().c_str());


    connect(m_pclient , SIGNAL(SIG_ReadyData(uint,char*,int)) , this , SLOT(slot_dealdata(uint,char*,int)));
    //音频视频的连接
for(int i=0;i<2;i++)
{
    m_pavclient[i]=new TcpClientMediator;

    m_pavclient[i]->OpenNet(_DEF_SERVERIP,_DEF_PORT);
    //m_pclient->OpenNet(m_serverip.toStdString().c_str());


    connect(m_pavclient[i] , SIGNAL(SIG_ReadyData(uint,char*,int)) , this , SLOT(slot_dealdata(uint,char*,int)));
}

     m_pAudioRead=new AudioRead;
     connect(m_pAudioRead , SIGNAL(SIG_audioFrame(QByteArray)) , this , SLOT(slot_audioFrame(QByteArray)));

     m_pVideoRead=new VideoRead;
      connect(m_pVideoRead , SIGNAL(SIG_sendVideoFrame(QImage)) , this , SLOT(slot_sendVideoFrame(QImage)));

     m_pScreenRead=new ScreenRead;
     connect(m_pScreenRead , SIGNAL(SIG_getScreenFrame(QImage)) , this , SLOT(slot_sendVideoFrame(QImage)));

     m_psendvideoworker=QSharedPointer<SendVideoWorker>(new SendVideoWorker);
     connect(this,SIGNAL(SIG_SendVideo(char*,int)),m_psendvideoworker.data(),SLOT(slot_sendVideo(char*,int)));


     //设置萌拍效果
     connect(m_roomdialog,SIGNAL(SIG_setMoji(int)),m_pVideoRead,SLOT(slot_setMoji(int)));
}


//回收
void Ckernel::slot_destory()
{
    if(m_wechatdilog)
    {
        m_wechatdilog->hide();
        delete m_wechatdilog;
        m_wechatdilog=NULL;
    }
    if(m_plogindialog)
    {
        m_plogindialog->hide();
        delete m_plogindialog;
        m_plogindialog=NULL;
    }
    if(m_roomdialog)
    {
        m_roomdialog->hide();
        delete m_roomdialog;
        m_roomdialog=NULL;
    }
    if(m_pAudioRead)
    {
      m_pAudioRead->pause();
      delete  m_pAudioRead;
      m_pAudioRead=NULL;
    }
    if(m_pclient)
    {
        m_pclient->CloseNet();
        delete m_pclient;
        m_pclient=NULL;
    }
    exit(0);
}



void Ckernel::slot_dealdata(uint sock, char *buf, int nlen)
{
    int type=*(int*)buf;
    if(type>=DEF_PACK_BASE&&type<DEF_PACK_BASE+DEF_PACK_COUNT)
    {
        //取得协议头，根据协议关系，找到函数指针
        pfun pf=NetPackMap(type);
        if(pf)
        {
            (this->*pf)(sock,buf,nlen);
        }
        delete[]buf;
    }
}

void Ckernel::slot_dealLoginRs(uint sock, char *buf, int nlen)
{

    qDebug()<<__func__;
    //拆包
    STRU_LOGIN_RS *rs=(STRU_LOGIN_RS*)buf;
    //
    switch(rs->m_lResult)
    {
    case user_not_exist:
        QMessageBox::about(m_plogindialog,"提示","用户不存在,登陆失败");
        break;
    case password_error:
         QMessageBox::about(m_plogindialog,"提示","密码错误,登陆失败");
        break;
    case login_success:
    {
        //QString strname=QString("用户[%1]登陆成功").arg(rs->m_name);
        // QMessageBox::about(m_plogindialog,"提示",strname);
        m_name=QString::fromStdString(rs->m_name);
        m_wechatdilog->setinfo(m_name);
         m_id=rs->m_userid;
         m_plogindialog->hide();
         m_wechatdilog->showNormal();

         //注册视频和音频的fd
//         STRU_AUDIO_REGISTER rq_audio;
//         rq_audio.m_userid=m_id;
//         STRU_VIDEO_REGISTER rq_video;
//         rq_video.m_userid=m_id;

//         m_pavclient[audio_client]->SendData(0,(char*)&rq_audio,sizeof(rq_audio));

//         m_pavclient[video_client]->SendData(0,(char*)&rq_video,sizeof(rq_video));
    }
        break;
    default:break;
    }
}

void Ckernel::slot_dealRegisterRs(uint sock, char *buf, int nlen)
{
//拆包
    STRU_REGISTER_RS* rs=(STRU_REGISTER_RS*)buf;
    switch(rs->m_lResult)
    {
    case tel_is_exist:
        QMessageBox::about(m_plogindialog,"提示","手机号已存在");
        break;
    case register_success:
         QMessageBox::about(m_plogindialog,"提示","注册成功");
        break;
    case name_is_exist:
         QMessageBox::about(m_plogindialog,"提示","姓名已存在");
        break;
    default:break;
    }
}

void Ckernel::slot_dealcreateroomRs(uint sock, char *buf, int nlen)
{

    STRU_CREATEROOM_RS *rs=(STRU_CREATEROOM_RS*)buf;

    m_roomdialog->slot_setInfo(QString::number(rs->m_RoomID));

//把自己的信息放到房间 做显示
   UserShow*user=new UserShow;

   connect(user,SIGNAL(SIG_itemClicked(int,QString)),m_roomdialog,SLOT(slot_setBigImgID(int,QString)));
   user->slot_setinfo(m_id,m_name);
   m_roomdialog->slot_addUserShow(user);
   m_roomid=rs->m_RoomID;



    m_roomdialog->showNormal();
    //音频初始化
    m_roomdialog->slot_setAudioCheck(false);
    //视频初始化  todo
    m_roomdialog->slot_setVideoCheck(false);
}

void Ckernel::slot_dealjoinroomRs(uint sock, char *buf, int nlen)
{
     STRU_JOINROOM_RS *rs=(STRU_JOINROOM_RS*)buf;
     if(rs->m_lResult==0)
     {
        QMessageBox::about(m_wechatdilog,"提示","房间id不存在，加入失败");
        return;
     }

     m_roomdialog->slot_setInfo(QString::number(rs->m_RoomID));
     m_roomid=rs->m_RoomID;

     m_roomdialog->showNormal();

     //音频初始化
     m_roomdialog->slot_setAudioCheck(false);
     //视频初始化  todo
     m_roomdialog->slot_setVideoCheck(false);
}

void Ckernel::slot_dealmemberRq(uint sock, char *buf, int nlen)
{
    STRU_ROOM_MEMBER_RQ* rq=(STRU_ROOM_MEMBER_RQ*)buf;

    UserShow*user=new UserShow;

    user->slot_setinfo(rq->m_UserID,QString::fromStdString(rq->m_szUser));
    connect(user,SIGNAL(SIG_itemClicked(int,QString)),m_roomdialog,SLOT(slot_setBigImgID(int,QString)));
    m_roomdialog->slot_addUserShow(user);


    //音频
    AudioWrite *aw=NULL;

   if(m_mapIDToAudioWrite.count(rq->m_UserID)==0)
   {
       aw=new AudioWrite;
       m_mapIDToAudioWrite[rq->m_UserID]=aw;
   }



    //视频 todo


}
#define MD5_KEY (1234)
static std::string GetMD5(QString value)
{
    QString str=QString("%1_%2").arg(value).arg(MD5_KEY);
    std::string strsrc=str.toStdString();
    MD5 md5(strsrc);
    return md5.toString();
}

//提交登录信息
void Ckernel::slot_loginCommit(QString tel, QString pass)
{

    std::string strtel=tel.toStdString();
    //std::string strpass=pass.toStdString();
    STRU_LOGIN_RQ rq;
    strcpy(rq.m_tel,strtel.c_str());
    std::string passMD5=GetMD5(pass);
    qDebug()<<passMD5.c_str();
    strcpy(rq.m_password,passMD5.c_str());

    //int nlen=sizeof(rq.m_szPassword);
    //MD5 md5(strpass);
    //std::string strpassMD5=md5.toString();
    //qDebug()<<strpassMD5.c_str();
    m_pclient->SendData(0,(char*)&rq,sizeof(rq));


}

void Ckernel::slot_quitroom()
{
    qDebug()<<__func__;
    //发退出包
    STRU_LEAVEROOM_RQ rq;
    rq.m_nUserId=m_id;
    rq.m_RoomId=m_roomid;
    std::string name=m_name.toStdString();
    strcpy(rq.szUserName,name.c_str());
    m_pclient->SendData(0,(char*)&rq,sizeof(rq));
     qDebug()<<m_roomid;
    //关闭音频视频
    m_pAudioRead->pause();
    m_pVideoRead->slot_closeVideo();
    m_pScreenRead->slot_closeVideo();
    m_roomdialog->slot_setAudioCheck(false);
    m_roomdialog->slot_setVideoCheck(false);
    m_roomdialog->slot_setScreenCheck(false);
    //回收所有人的audowrite
    for(auto ite=m_mapIDToAudioWrite.begin();ite!=m_mapIDToAudioWrite.end();)
    {
        AudioWrite* pwrite=ite->second;
        ite=m_mapIDToAudioWrite.erase(ite);
        delete pwrite;
    }
    //回收资源
    m_roomdialog->slot_clearUserShow();
    m_roomid=0;

}

void Ckernel::slot_startAudio()
{
    m_pAudioRead->start();
}

void Ckernel::slot_pauseAudio()
{
    m_pAudioRead->pause();
}

void Ckernel::slot_startVideo()
{
    m_pVideoRead->slot_openVideo();
}

void Ckernel::slot_pauseVideo()
{
    m_pVideoRead->slot_closeVideo();
}

void Ckernel::slot_startScreen()
{
     m_pScreenRead->slot_openVideo();
}

void Ckernel::slot_pauseScreen()
{
   m_pScreenRead->slot_closeVideo();
}

void Ckernel::slot_refreshVideo(int id,QImage &img)
{
    m_roomdialog->slot_refreshUser(id,img);
}
///音频数据帧
///int type;
///int userId;
///int roomId;
///int min;
///int sec;
/// int msec;
/// QByteArray audioFrame;
///
void Ckernel::slot_audioFrame(QByteArray ba)
{

    int nPackSize=6*sizeof(int)+ba.size();
    char*buf=new char[nPackSize];
    char *tmp=buf;
    //序列化
    int type=DEF_PACK_AUDIO_FRAME;
    int userId=m_id;
    int roomId=m_roomid;
    QTime tm=QTime::currentTime();
    int min=tm.minute();
    int sec=tm.second();
    int msec=tm.msec();
    *(int*)tmp=type;
    tmp+=sizeof(int);

    *(int*)tmp=userId;
    tmp+=sizeof(int);

    *(int*)tmp=roomId;
    tmp+=sizeof(int);

    *(int*)tmp=min;
    tmp+=sizeof(int);

    *(int*)tmp=sec;
    tmp+=sizeof(int);

    *(int*)tmp=msec;
    tmp+=sizeof(int);

    memcpy(tmp,ba.data(),ba.size());
//将视频发送单独变为一个信号，放到另一个线程执行
    //m_pclient->SendData(0,buf,nPackSize);
    m_pavclient[audio_client]->SendData(0,buf,nPackSize);
    delete [] buf;

}
#include<QBuffer>
void Ckernel::slot_sendVideoFrame(QImage img)
{
    //显示
     slot_refreshVideo(m_id,img);
    //压缩
     //压缩图片从 RGB24 格式压缩到 JPEG 格式, 发送出去
     QByteArray ba;
     QBuffer qbuf(&ba); // QBuffer 与 QByteArray 字节数组联立联系
     img.save( &qbuf , "JPEG" , 50 ); //将图片的数据写入 ba
     //使用 ba 对象, 可以获取图片对应的缓冲区
    // 可以使用 ba.data() , ba.size()将缓冲区发送出去
    //写视频帧  发送

    ///视频数据帧
    ///int type;
    /// int userId;
    /// int roomId;
    /// int min;
    /// int sec;
    /// int mxec;
    /// QByteArray videoFrame;
    int nPackSize=6*sizeof(int)+ba.size();
    char *buf=new char[nPackSize];
    char *tmp=buf;
    *(int*)tmp=DEF_PACK_VIDEO_FRAME;
    tmp+=sizeof (int);

    *(int*)tmp=m_id;
    tmp+=sizeof (int);

    *(int*)tmp=m_roomid;
    tmp+=sizeof (int);

    //用于延迟过久舍弃一些帧,参考时间
    QTime tm=QTime::currentTime();
    *(int*)tmp=tm.minute();
    tmp+=sizeof (int);

    *(int*)tmp=tm.second();
    tmp+=sizeof (int);

    *(int*)tmp=tm.msec();
    tmp+=sizeof (int);

    memcpy(tmp,ba.data(),ba.size());
    //发送是一个阻塞函数，如果服务器接收缓冲区由于数据量大，没及时取走缓冲区数据
    //滑动窗口变小，send函数阻塞，影响用户界面相应，出现未响应问题

    //m_pclient->SendData(0,buf,nPackSize);

    //delete [] buf;

     Q_EMIT SIG_SendVideo(buf,nPackSize);
}
//多线程发送视频
void Ckernel::slot_SendVideo(char *buf, int nlen)
{
    char *tmp=buf;
     tmp+=sizeof (int);
      tmp+=sizeof (int);
       tmp+=sizeof (int);
       int min=*(int*)tmp;
       tmp+=sizeof (int);
       int sec=*(int*)tmp;
       tmp+=sizeof (int);
       int msec=*(int*)tmp;
       tmp+=sizeof (int);

       QTime ctm=QTime::currentTime();
       QTime tm(ctm.hour(),min,sec,msec);


       if(tm.msecsTo(ctm)>300)
       {
           qDebug()<<"send fail";
           delete []buf;
           return;
       }
    //m_pclient->SendData(0,buf,nlen);
    m_pavclient[video_client]->SendData(0,buf,nlen);
    delete [] buf;
}
void Ckernel::slot_dealLeaveRoomRq(uint sock, char *buf, int nlen)
{
    qDebug()<<__func__;
    STRU_LEAVEROOM_RQ *rq=(STRU_LEAVEROOM_RQ*)buf;
    qDebug()<<rq->m_RoomId;
    qDebug()<<m_roomid;
    if(rq->m_RoomId==m_roomid)
    {
        m_roomdialog->slot_removeUserShow(rq->m_nUserId);

    }
    if(m_mapIDToAudioWrite.count(rq->m_nUserId)>0)
    {
        AudioWrite *aw=m_mapIDToAudioWrite[rq->m_nUserId];
        m_mapIDToAudioWrite.erase(rq->m_nUserId);
        delete aw;
    }
}

void Ckernel::slot_dealAudioFrameRq(uint sock, char *buf, int nlen)
{
    qDebug()<<__func__;
    ///音频数据帧
    ///int type;
    ///int userId;
    ///int roomId;
    ///int min;
    ///int sec;
    /// int msec;
    /// QByteArray audioFrame;
    ///

    //序列化
  char *tmp=buf;
    int userId;
    int roomId;


    tmp+=sizeof(int);

    userId=*(int*)tmp;
    tmp+=sizeof(int);

    roomId=*(int*)tmp;
    tmp+=sizeof(int);

    tmp+=sizeof(int);

    tmp+=sizeof(int);

    tmp+=sizeof(int);

    int nbufLen=nlen-6*sizeof(int);
    QByteArray ba(tmp,nbufLen);

    if(m_roomid==roomId)
    {
        if(m_mapIDToAudioWrite.count(userId)>0)
        {
            AudioWrite*aw=m_mapIDToAudioWrite[userId];
            aw->slot_playaudio(ba);
        }
    }

}

void Ckernel::slot_dealVideoFrameRq(uint sock, char *buf, int nlen)
{
    ///视频数据帧
    ///int type;
    /// int userId;
    /// int roomId;
    /// int min;
    /// int sec;
    /// int mxec;
    /// QByteArray videoFrame;
    char*tmp=buf;
    tmp+=sizeof(int);

    int userId=*(int*)tmp;
    tmp+=sizeof(int);

    int roomId=*(int*)tmp;
    tmp+=sizeof(int);

    tmp+=sizeof(int);

    tmp+=sizeof(int);

    tmp+=sizeof(int);
    int datalen=nlen-6*sizeof(int);
    QByteArray bt(tmp,datalen);
    QImage img;
    img.loadFromData(bt);
    if(m_roomid==roomId)
        m_roomdialog->slot_refreshUser(userId,img);

}


void Ckernel::slot_registerCommit(QString tel,QString pass,QString name)
{
    std::string strtel=tel.toStdString();
    //std::string strpass=pass.toStdString();
    std::string strname=name.toStdString();
    STRU_REGISTER_RQ rq;
    strcpy(rq.m_tel,strtel.c_str());

    std::string passMD5=GetMD5(pass);
    qDebug()<<passMD5.c_str();

    //兼容中文
    strcpy(rq.m_name,strname.c_str());


    //
    strcpy(rq.m_password,passMD5.c_str());

    //int nlen=sizeof(rq.m_szPassword);
    //MD5 md5(strpass);
    //std::string strpassMD5=md5.toString();
    //qDebug()<<strpassMD5.c_str();
    m_pclient->SendData(0,(char*)&rq,sizeof(rq));

}
//创建房间
void Ckernel::slot_createroom()
{
    //判断是否在房间内 roomid
    if(m_roomid!=0)
    {
        QMessageBox::about(m_wechatdilog,"提示","在房间内无法创建，先退出");
        return;
    }
    //创建房间
    STRU_CREATEROOM_RQ rq;
    rq.m_UserID=m_id;
    m_pclient->SendData(0,(char*)&rq,sizeof(rq));

}

#include<QInputDialog>
#include"QRegExp"
//加入房间
void Ckernel::slot_joinroom()
{
    //判断是否在房间内
if(m_roomid!=0)
{
    QMessageBox::about(m_wechatdilog,"提示","在房间内无法加入");
    return;
}
    //弹出窗口，填房间号
    QString strroom= QInputDialog::getText(m_wechatdilog,"加入房间","输入房间号");
    //合理化判断
    QRegExp exp("^[0-9]{1,10}$");
    if(!exp.exactMatch(strroom))
    {
        QMessageBox::about(m_wechatdilog,"提示","房间号输入不合法");
        return;
    }
    //发命令加入房间
    qDebug()<<strroom;
    STRU_JOINROOM_RQ rq;
    rq.m_UserID=m_id;
    rq.m_RoomID=strroom.toInt();
    m_pclient->SendData(0,(char*)&rq,sizeof(rq));

}

