#include "roomdialog.h"
#include "ui_roomdialog.h"
#include<QMessageBox>
#include<QDebug>
RoomDialog::RoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomDialog)
{
    ui->setupUi(this);

    m_mainLayout=new QVBoxLayout;
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(5);
    //设置一个数值布局的画布，可以向这个里面添加控件
    ui->wdg_list->setLayout(m_mainLayout);

//    for(int i=0;i<6;i++)
//    {
//        UserShow*user1=new UserShow;
//        user1->slot_setinfo(i+1,QString("测试%1").arg(i+1));
//        slot_addUserShow(user1);
//    }
}


RoomDialog::~RoomDialog()
{
    delete ui;
}

void RoomDialog::slot_setInfo(QString roomid)
{


    QString title = QString("房间号:%1").arg(roomid);

    setWindowTitle(title);

    ui->lb_title->setText(title);
}

void RoomDialog::slot_addUserShow(UserShow *user)
{
    m_mainLayout->addWidget(user);
    m_mapIDToUSerShow[user->m_id]=user;
}

void RoomDialog::slot_refreshUser(int id, QImage &img)
{
    if(ui->wdg_usershow->m_id==id)
    {
        ui->wdg_usershow->slot_setImage(img);
    }
    if(m_mapIDToUSerShow.count(id)>0)
    {
     UserShow*user=m_mapIDToUSerShow[id];
     user->slot_setImage(img);
    }
}

void RoomDialog::slot_removeUserShow(UserShow *user)
{
    user->hide();
    m_mainLayout->removeWidget(user);
}

void RoomDialog::slot_removeUserShow(int id)
{
    if(m_mapIDToUSerShow.count(id)>0)
    {
        UserShow *user=m_mapIDToUSerShow[id];
        slot_removeUserShow(user);
    }
}

void RoomDialog::slot_setAudioCheck(bool check)
{
    ui->cb_audio->setChecked(check);
}

void RoomDialog::slot_setVideoCheck(bool check)
{
    ui->cb_video->setChecked(check);
}
void RoomDialog::slot_setScreenCheck(bool check)
{
    ui->cb_desk->setChecked(check);
}


void RoomDialog::slot_clearUserShow()
{
    for(auto ite=m_mapIDToUSerShow.begin();ite!=m_mapIDToUSerShow.end();++ite)
    {
        slot_removeUserShow(ite->second);
    }
}

void RoomDialog::slot_setBigImgID(int id,QString name)
{
    qDebug()<<__func__;
ui->wdg_usershow->slot_setinfo(id,name);
}
//退出房间
void RoomDialog::on_pb_close_clicked()
{
    this->close();
}


void RoomDialog::on_pb_quit_clicked()
{
    this->close();
}

void RoomDialog::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this,"退出提示","是否退出会议")==QMessageBox::Yes)
    {
        //发送推出房间喜好

        Q_EMIT SIG_close();
        event->accept();
        return;
    }
    event->ignore();
}

//开启关闭音频
void RoomDialog::on_cb_audio_clicked()
{
    if(ui->cb_audio->isChecked())
    {
        //ui->cb_audio->setChecked(false);
          Q_EMIT SIG_audioStart();


    }
    else
    {
        // ui->cb_audio->setChecked(true);
       Q_EMIT SIG_audioPause();
    }
}


void RoomDialog::on_cb_video_clicked()
{
    if(ui->cb_video->isChecked())
    {
          ui->cb_desk->setChecked(false);
          Q_EMIT SIG_videoStart();
          Q_EMIT SIG_screenPause();

    }
    else
    {
       Q_EMIT SIG_videoPause();
    }
}


void RoomDialog::on_cb_desk_clicked()
{
    if(ui->cb_desk->isChecked())
    {
          ui->cb_video->setChecked(false);
          Q_EMIT SIG_screenStart();
          Q_EMIT SIG_videoPause();

    }
    else
    {
       Q_EMIT SIG_screenPause();
    }
}



void RoomDialog::on_cb_moji_currentIndexChanged(int index)
{

    Q_EMIT SIG_setMoji(index);
}

