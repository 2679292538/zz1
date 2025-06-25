#ifndef THREADWORKER_H
#define THREADWORKER_H
#include<QSharedPointer>
#include <QObject>
#include<QThread>

class ThreadWorker : public QObject
{
    Q_OBJECT
public:
    explicit ThreadWorker(QObject *parent = nullptr);
~ThreadWorker();
signals:

protected:
    QThread*m_pthread;
};
//例子
class worker:public ThreadWorker
{
    Q_OBJECT
public:
    ~worker();
public slots:
     void slot_doWork();
};

#endif // THREADWORKER_H
