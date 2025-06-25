#include "usershow.h"
#include "ui_usershow.h"
#include<QPainter>

UserShow::UserShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserShow),m_defaultImg(":/bq/000.png")
{
    ui->setupUi(this);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(slot_checkTimer()));
    m_lastTime=QTime::currentTime();
    m_timer.start(1000);
}

UserShow::~UserShow()
{
    delete ui;
}

void UserShow::slot_setinfo(int id, QString name)
{
    m_id=id;
    m_username=name;
    QString text=QString("用户名:%1").arg(m_username);
    ui->lb_name->setText(text);
}

//重绘事件
void UserShow::paintEvent(QPaintEvent *event)
{
    //画黑背景
    QPainter painter(this);
    painter.setBrush(Qt::black);//设置黑色画刷
    painter.drawRect(-1,0,this->width()+1,this->height());

    if( m_img.size().height()<= 0 ) return;
    //画视频帧
    //等比例缩放
    m_img=m_img.scaled(this->width(),this->height()-ui->lb_name->height(),Qt::KeepAspectRatio);
    QPixmap pix=QPixmap::fromImage(m_img);
    int x=this->width()-pix.width();
    x=x/2;
    int y=this->height()-pix.height()-ui->lb_name->height();
    y=ui->lb_name->height()+y/2;
    painter.drawPixmap(QPoint(x,y), pix);
    painter.end();
}

void UserShow::slot_setImage(QImage &img)
{
    m_img=img;
    m_lastTime=QTime::currentTime();
    this->update();//触发重绘

}

void UserShow::mousePressEvent(QMouseEvent *event)
{
   event->accept();
   Q_EMIT SIG_itemClicked(m_id,m_username);
}

void UserShow::slot_checkTimer()
{
    if(m_lastTime.secsTo(QTime::currentTime())>5)
    {
        slot_setImage(m_defaultImg);
        //m_lastTime=QTime::currentTime();
    }
}
