#include "threadworker.h"
#include<QDebug>
ThreadWorker::ThreadWorker(QObject *parent) : QObject(parent)
{
        m_pthread=new QThread;
        this->moveToThread(m_pthread);
        m_pthread->start();
}

ThreadWorker::~ThreadWorker()
{
    if(m_pthread)
    {
        m_pthread->quit();
        m_pthread->wait();
        if(m_pthread->isRunning())
        {
            m_pthread->terminate();
            m_pthread->wait(10);

        }
        delete  m_pthread;
        m_pthread=NULL;
    }

}

worker::~worker()
{
qDebug()<<"~worker";
}

void worker::slot_doWork()
{
    qDebug()<<"worker"<<QThread::currentThreadId();
}
