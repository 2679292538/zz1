#include "videoread.h"
#include<QMessageBox>
#include<QDebug>
#include<QPainter>
VideoRead::VideoRead(QObject *parent) : QObject(parent),m_tuer(":/images/tuer.png"),m_hat(":/images/hat.png")
{
    m_timer=new QTimer;
    //connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_getVideoFrame()));
    m_pvideoworker=QSharedPointer<VideoWorker>(new VideoWorker) ;
    m_pvideoworker.data()->slot_setInfo(this);
    connect(m_timer,SIGNAL(timeout()),m_pvideoworker.data(),SLOT(slot_dowork()));
    MyFaceDetect::FaceDetectInit();
    //加载萌拍图片
    m_moji=0;

}

VideoRead::~VideoRead()
{
    if(m_timer)
    {
        m_timer->stop();
        delete m_timer;
        m_timer=NULL;
    }
}

void VideoRead::slot_getVideoFrame()
{
    //qDebug()<<"slot_getVideoFrame::"<<QThread::currentThreadId();
    Mat frame;
    if( !cap.read(frame) )
    {
     return;
    }
    //将 opencv 采集的 BGR 的图片类型转化为 RGB24 的类型

    //加滤镜，人脸识别等
    std::vector<Rect>faces;
    //存储上一次识别的矩形
    m_vecLastFace;

    if(m_moji!=0)
    {
     MyFaceDetect::detectAndDisplay(frame,faces);
    }
    cvtColor(frame,frame,CV_BGR2RGB);
    //定义 QImage 对象, 用于发送数据以及图片显示
    QImage image ((unsigned const
    char*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888);

    QImage tmpImg;
    switch (m_moji) {
       case moji_tuer:
        tmpImg=m_tuer;break;
       case moji_hat:
        tmpImg=m_hat;break;

    }
    if(faces.size()>0)
    {
        m_vecLastFace=faces;
    }
    if(m_moji==moji_tuer||m_moji==moji_hat)
    {
        QPainter paint(&image);
        //遍历所有的人脸，话道具
        for(int i=0;i<m_vecLastFace.size();++i)
        {
            Rect rct=m_vecLastFace[i];
            int x=rct.x+rct.width*0.5-tmpImg.width()*0.5+20;
            int y=rct.y-tmpImg.height();
            QPoint p(x,y);
            paint.drawImage(p,tmpImg);
            //paint.drawImage();
        }
    }
    //转化为大小更小的图片
    image = image.scaled( IMAGE_WIDTH,IMAGE_HEIGHT, Qt::KeepAspectRatio );
    //发送图片
    Q_EMIT SIG_sendVideoFrame( image );
    qDebug()<<"2";
}

void VideoRead::slot_openVideo()
{
    qDebug()<<"1";
    m_timer->start(1000/FRAME_RATE-10);
    //打开摄像头
    cap.open(0);//打开默认摄像头
    if(!cap.isOpened()){
     QMessageBox::information(NULL,tr("提示"),tr("视频没有打开"));
     return;
    }
}

void VideoRead::slot_closeVideo()
{
    m_timer->stop();

    //关闭摄像头
    if(cap.isOpened())
        cap.release();
}

void VideoRead::slot_setMoji(int newMoji)
{
    m_moji = newMoji;
}
