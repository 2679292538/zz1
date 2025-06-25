#include "wechatdialog.h"
#include "ui_wechatdialog.h"
#include<QMessageBox>
#include<QDebug>
WeChatDialog::WeChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WeChatDialog)
{
    ui->setupUi(this);
}

WeChatDialog::~WeChatDialog()
{
    delete ui;
}
//关闭
void WeChatDialog::closeEvent(QCloseEvent *event)
{
    qDebug()<<__func__;
    if(QMessageBox::question(this,"提示","是否退出")==QMessageBox::Yes)
    {
        Q_EMIT SIG_close();
        event->accept();

    }
    else
    {
        event->ignore();
    }
}

void WeChatDialog::setinfo(QString name, int icon)
{
    ui->lb_name->setText(name);
}

void WeChatDialog::on_pb_create_clicked()
{
  Q_EMIT SIG_createroom();
}


void WeChatDialog::on_pb_join_clicked()
{
  Q_EMIT SIG_joinroom();
}

