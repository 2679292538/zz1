#include "loginindialog.h"
#include "ui_loginindialog.h"

LogininDialog::LogininDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogininDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("注册&登录");
    ui->tw_page->setCurrentIndex(0);//第一时间显示登录：0，
    //ui->tw_page->setCurrentIndex(0);
}

LogininDialog::~LogininDialog()
{
    delete ui;
}

void LogininDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    Q_EMIT SIG_close();
}

#include<QRegExp>
#include<QMessageBox>
//提交登录
void LogininDialog::on_pb_commit_clicked()
{
    QString strtel=ui->le_tel->text();
    QString strpassword=ui->le_password->text();

    //校验
    //非空校验，清掉空格，不能是空字符串
    QString tmptel=strtel,tmppass=strpassword;
    if(tmptel.remove(" ").isEmpty()||tmppass.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"提示","手机号码不能为空格");
        return;
    }
    //1.校验手机号  正则表达式
    QRegExp reg("^1[3-9][0-9]{6,9}$");
    bool res=reg.exactMatch(strtel);
    if(!res)
    {
        QMessageBox::about(this,"提示","手机号格式错误，8-11位手机号");
        return;
    }
    //2.校验密码  长度不超过20
    if(strpassword.length()>20)
    {
        QMessageBox::about(this,"提示","密码过长，长度不超过20");
        return;
    }
    Q_EMIT SIG_loginCommit(strtel,strpassword);
}

void LogininDialog::on_pb_clear_register_clicked()
{
ui->le_confirm_register->setText("");
ui->le_name_register->setText("");
ui->le_tel_register->setText("");
ui->le_password_register->setText("");
}


void LogininDialog::on_pb_commit_register_clicked()
{
    QString strtel=ui->le_tel_register->text();
    QString strpassword=ui->le_password_register->text();
     QString strconfirm=ui->le_confirm_register->text();
      QString strname=ui->le_name_register->text();

    //校验
    //非空校验，清掉空格，不能是空字符串
    QString tmptel=strtel,tmppass=strpassword,tmpname=strname;
    if(tmptel.remove(" ").isEmpty()||tmppass.remove(" ").isEmpty()||tmpname.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"提示","手机号码不能为空格");
    }
    //1.校验手机号  正则表达式
    QRegExp reg("^1[3-8][0-9]{6,9}$");
    bool res=reg.exactMatch(strtel);
    if(!res)
    {
        QMessageBox::about(this,"提示","手机号格式错误，8-11位手机号");
        return;
    }
    //2.校验密码  长度不超过20
    if(strpassword.length()>20)
    {
        QMessageBox::about(this,"提示","密码过长，长度不超过20");
        return;
    }
    //输入是否一致
    if(strpassword!=strconfirm)
    {
        QMessageBox::about(this,"提示","两次密码不一致");
        return;
    }
    if(strname.length()>10)
    {
        QMessageBox::about(this,"提示","长度不超过10");
        return;
    }
    Q_EMIT SIG_registerCommit(strtel,strpassword,strname);

}

//清空登录信息
void LogininDialog::on_pb_clear_clicked()
{

    ui->le_tel->setText("");
    ui->le_password->setText("");
}



