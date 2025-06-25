#ifndef USERSHOW_H
#define USERSHOW_H

#include <QWidget>
#include<QPaintEvent>
#include<QImage>
#include<QMouseEvent>
#include<QTimer>
#include<QTime>
namespace Ui {
class UserShow;
}

class UserShow : public QWidget
{
    Q_OBJECT

public:
    explicit UserShow(QWidget *parent = nullptr);
    ~UserShow();
signals:
    void SIG_itemClicked(int id,QString name);
public slots:
    void slot_setinfo(int id,QString name);
    void paintEvent(QPaintEvent*event);
    void slot_setImage(QImage &img);

    void mousePressEvent(QMouseEvent*event);
    void slot_checkTimer();
private:
    Ui::UserShow *ui;
    int m_id;
    QString m_username;
    QImage m_img;
    friend class RoomDialog;
    QTime m_lastTime;
    QTimer m_timer;
    QImage m_defaultImg;
};

#endif // USERSHOW_H
