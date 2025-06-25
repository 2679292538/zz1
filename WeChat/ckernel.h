#ifndef CKERNEL_H
#define CKERNEL_H
#include"packdef.h"
#include <QObject>
#include"wechatdialog.h"
#include"TcpClientMediator.h"
#include"loginindialog.h"
#include"roomdialog.h"

#include"audioread.h"
#include"audiowrite.h"
#include"videoread.h"
#include"screenread.h"
#include"threadworker.h"
//协议映射表使用的类型
class Ckernel;
typedef void (Ckernel::*pfun)(uint sock,char*buf,int nlen);



class SendVideoWorker;
class Ckernel : public QObject
{
    Q_OBJECT
public:
    explicit Ckernel(QObject *parent = nullptr);
static Ckernel* GetInstance()
{

    static Ckernel kernel;
    return &kernel;

}

signals:
void SIG_SendVideo(char*buf,int nlen);
public slots:

void slot_setNetPackMap();
//初始化配置
void initconfig();
//回收
    void slot_destory();
    //发送注册信息
    void slot_registerCommit(QString tel,QString pass,QString name);

    void slot_joinroom();
    void slot_createroom();
    //网络信息处理
   void slot_dealdata(uint sock,char*buf,int nlen);
   //登录回复
   void slot_dealLoginRs(uint sock,char*buf,int nlen);
   //注册回复
   void slot_dealRegisterRs(uint sock,char*buf,int nlen);

   void slot_dealcreateroomRs(uint sock,char*buf,int nlen);

   void slot_dealjoinroomRs(uint sock,char*buf,int nlen);
   //房间成员请求处理
   void slot_dealmemberRq (uint sock,char*buf,int nlen);
//发送登录信息
void slot_loginCommit(QString tel,QString pass);
void slot_quitroom();
void slot_startAudio();
void slot_pauseAudio();
void slot_startVideo();
void slot_pauseVideo();
void slot_startScreen();
void slot_pauseScreen();
void slot_refreshVideo(int id,QImage &img);

void slot_audioFrame(QByteArray ba);

void slot_sendVideoFrame(QImage img);
//离开房间的请求处理
void slot_dealLeaveRoomRq(uint sock,char*buf,int nlen);
//音频帧处理
void slot_dealAudioFrameRq(uint sock,char*buf,int nlen);

void slot_dealVideoFrameRq(uint sock,char*buf,int nlen);
//多线程发送视频
void slot_SendVideo(char*buf,int nlen);
private:
    WeChatDialog*m_wechatdilog;
    INetMediator *m_pclient;

    LogininDialog*m_plogindialog;
    pfun m_netPackMap[DEF_PACK_COUNT];

    QString m_serverip;
    int m_id;
    QString m_name;
    int m_roomid;
    RoomDialog *m_roomdialog;
    ////////
    ///音频
    AudioRead*m_pAudioRead;
    std::map<int,AudioWrite*>m_mapIDToAudioWrite;
    ////////
    ///视频采集
    ///
    ///
    VideoRead*m_pVideoRead;
    ScreenRead *m_pScreenRead;

    enum client_type{audio_client=0,video_client};
    INetMediator*m_pavclient[2];
    QSharedPointer<SendVideoWorker>m_psendvideoworker;
};
class SendVideoWorker:public ThreadWorker
{
    Q_OBJECT
public slots:
    void slot_sendVideo(char *buf,int len)
    {
        Ckernel::GetInstance()->slot_SendVideo(buf,len);
    }

};

#endif // CKERNEL_H
