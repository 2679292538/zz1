#ifndef LOGININDIALOG_H
#define LOGININDIALOG_H

#include <QDialog>
#include<QCloseEvent>
namespace Ui {
class LogininDialog;
}

class LogininDialog : public QDialog
{
    Q_OBJECT
signals:
    void SIG_loginCommit(QString tel,QString password);
    void SIG_registerCommit(QString tel,QString password,QString name);
    void SIG_close();
public:
    explicit LogininDialog(QWidget *parent = nullptr);
    ~LogininDialog();
    void closeEvent(QCloseEvent*event);

private slots:


    void on_pb_clear_register_clicked();

    void on_pb_commit_register_clicked();

    void on_pb_clear_clicked();

    void on_pb_commit_clicked();

private:
    Ui::LogininDialog *ui;
};

#endif // LOGININDIALOG_H
