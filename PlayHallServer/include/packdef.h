#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "err_str.h"
#include <malloc.h>

#include<iostream>
#include<map>
#include<list>


//边界值
#define _DEF_SIZE           45
#define _DEF_BUFFERSIZE     1000
#define _DEF_PORT           8000
#define _DEF_SERVERIP       "0.0.0.0"
#define _DEF_LISTEN         128
#define _DEF_EPOLLSIZE      4096
#define _DEF_IPSIZE         16
#define DEF_COUNT          10
#define DEF_TIMEOUT        10
#define _DEF_SQLIEN         400
#define TRUE                true
#define FALSE               false



/*-------------数据库信息-----------------*/
#define DEF_DB_NAME    "wechat"
#define DEF_DB_IP      "localhost"
#define DEF_DB_USER    "root"
#define DEF_DB_PWD     "colin123"
/*--------------------------------------*/
#define MAX_PATH           (260)
#define DEF_BUFFER         (4096)
#define DEF_CONTENT_SIZE	(1024)
#define MAX_SIZE           (60)

//自定义协议   先写协议头 再写协议结构
//登录 注册 获取好友信息 添加好友 聊天 发文件 下线请求
#define BOOL bool
#define DEF_PACK_BASE	(10000)
#define DEF_PACK_COUNT (100)

//注册
#define DEF_PACK_REGISTER_RQ	(DEF_PACK_BASE + 0 )
#define DEF_PACK_REGISTER_RS	(DEF_PACK_BASE + 1 )
//登录
#define DEF_PACK_LOGIN_RQ	(DEF_PACK_BASE + 2 )
#define DEF_PACK_LOGIN_RS	(DEF_PACK_BASE + 3 )
//创建房间
#define DEF_PACK_CREATEROOM_RQ	(DEF_PACK_BASE + 4 )
#define DEF_PACK_CREATEROOM_RS	(DEF_PACK_BASE + 5 )
//加入房间
#define DEF_PACK_JOINROOM_RQ	(DEF_PACK_BASE + 6 )
#define DEF_PACK_JOINROOM_RS	(DEF_PACK_BASE + 7 )
//房间列表请求
#define DEF_PACK_ROOM_MEMBER	(DEF_PACK_BASE + 8 )
//音频数据
#define DEF_PACK_AUDIO_FRAME	(DEF_PACK_BASE + 9 )
//视频数据
#define DEF_PACK_VIDEO_FRAME	(DEF_PACK_BASE + 10 )
//退出房间请求
#define DEF_PACK_LEAVEROOM_RQ	(DEF_PACK_BASE + 11 )
//退出房间回复
#define DEF_PACK_LEAVEROOM_RS	(DEF_PACK_BASE + 12 )

#define DEF_PACK_AUDIO_REGISTER	(DEF_PACK_BASE + 13 )

#define DEF_PACK_VIDEO_REGISTER	(DEF_PACK_BASE + 14 )
//返回的结果
//注册请求的结果
#define tel_is_exist		(0)
#define register_success	(1)
#define name_is_exist       (2)
//登录请求的结果
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)
//
#define room_is_exist 0


typedef int PackType;

//协议结构
//注册
typedef struct STRU_REGISTER_RQ
{
    STRU_REGISTER_RQ()
    {
        m_nType=DEF_PACK_REGISTER_RQ;
        memset( m_tel  , 0, MAX_SIZE);
        memset( m_name  , 0, MAX_SIZE);
        memset( m_password , 0, MAX_SIZE );
    }
    //需要手机号码 , 密码, 昵称
    PackType m_nType;
    char m_tel[MAX_SIZE];
    char m_name[MAX_SIZE];
    char m_password[MAX_SIZE];

}STRU_REGISTER_RQ;

typedef struct STRU_REGISTER_RS
{
    //回复结果
    STRU_REGISTER_RS()
    {
        m_nType=DEF_PACK_REGISTER_RS;
        m_lResult=0;
    }
    PackType m_nType;
    int m_lResult;

}STRU_REGISTER_RS;

//登录
typedef struct STRU_LOGIN_RQ
{
    //登录需要: 手机号 密码
    STRU_LOGIN_RQ()
    {
        m_nType=DEF_PACK_LOGIN_RQ;
        memset( m_tel , 0, MAX_SIZE );
        memset( m_password , 0, MAX_SIZE );
    }
    PackType m_nType;//包类型
    char m_tel[MAX_SIZE];
    char m_password[MAX_SIZE];

}STRU_LOGIN_RQ;

typedef struct STRU_LOGIN_RS
{
    // 需要 结果 , 用户的id
    STRU_LOGIN_RS()
    {
        m_nType=DEF_PACK_LOGIN_RS;
        m_userid=0;
        m_lResult=0;
        memset(m_name,0,MAX_SIZE);
    }
    PackType m_nType;
    int m_lResult;
    int m_userid;
   char m_name[MAX_SIZE];
}STRU_LOGIN_RS;

typedef struct UserInfo
{

    UserInfo()
    {
        m_sockfd=0;
        m_id=0;
        m_roomid=0;
        memset(m_userName,0,MAX_SIZE);
        m_videofd=0;
        m_videofd=0;
    }
    int m_sockfd;
    int m_id;
    int m_roomid;
    char m_userName[MAX_SIZE];


    int m_videofd;
    int m_audiofd;
}UserInfo;

typedef struct STRU_CREATEROOM_RQ
{
    STRU_CREATEROOM_RQ()
    {
        m_nType=DEF_PACK_CREATEROOM_RQ;
        m_UserID=0;
    }
    PackType m_nType;
    int m_UserID;
}STRU_CREATEROOM_RQ;

typedef struct STRU_CREATEROOM_RS
{
    STRU_CREATEROOM_RS()
    {
        m_nType=DEF_PACK_CREATEROOM_RS;
        m_lResult=0;
        m_RoomID=0;
    }
    PackType m_nType;
    int m_lResult;
    int m_RoomID;
}STRU_CREATEROOM_RS;

typedef struct STRU_JOINROOM_RQ
{
    STRU_JOINROOM_RQ()
    {
        m_nType=DEF_PACK_JOINROOM_RQ;
        m_UserID=0;
        m_RoomID=0;
    }
    PackType m_nType;
    int m_UserID;
    int m_RoomID;
}STRU_JOINROOM_RQ;

typedef struct STRU_JOINROOM_RS
{
    STRU_JOINROOM_RS()
    {
        m_nType=DEF_PACK_JOINROOM_RS;
        m_lResult=0;
        m_RoomID=0;
    }
    PackType m_nType;
    int m_lResult;
    int m_RoomID;
}STRU_JOINROOM_RS;

typedef struct STRU_ROOM_MEMBER_RQ
{
    STRU_ROOM_MEMBER_RQ()
    {
        m_nType=DEF_PACK_ROOM_MEMBER;
        m_UserID=0;
        memset(m_szUser,0,MAX_SIZE);
    }
    PackType m_nType;
    int m_UserID;
    char m_szUser[MAX_SIZE];
}STRU_ROOM_MEMBER_RQ;
typedef struct STRU_LEAVEROOM_RQ
{
    STRU_LEAVEROOM_RQ()
    {
        m_nType=DEF_PACK_LEAVEROOM_RQ;
        m_nUserId=0;
        m_RoomId=0;
        memset(szUserName,0,MAX_SIZE);
    }
    PackType m_nType;
    int m_nUserId;
    int m_RoomId;
    char szUserName[MAX_SIZE];
}STRU_LEAVEROOM_RQ;


struct STRU_AUDIO_REGISTER
{
    STRU_AUDIO_REGISTER():m_nType(DEF_PACK_AUDIO_FRAME)
    {
        m_userid=0;
    }
    PackType m_nType;
    int m_userid;

};
struct STRU_VIDEO_REGISTER
{
    STRU_VIDEO_REGISTER():m_nType(DEF_PACK_VIDEO_FRAME)
    {
        m_userid=0;
    }
    PackType m_nType;
    int m_userid;

};
///音频数据帧
///int type;
///int userId;
///int roomId;
///int min;
///int sec;
/// int msec;
/// QByteArray audioFrame;-->char frame[];
///


///视频数据帧
///int type;
/// int userId;
/// int roomId;
/// int min;
/// int sec;
/// int mxec;
/// QByteArray videoFrame;



