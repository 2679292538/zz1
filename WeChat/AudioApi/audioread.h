#ifndef AUDIOREAD_H
#define AUDIOREAD_H

#include <QObject>
#include"world.h"
class AudioRead : public QObject
{
    Q_OBJECT
public:
    explicit AudioRead(QObject *parent = nullptr);
    //定时超时，从缓冲区读取音频数据
    ~AudioRead();
public slots:
    void RedaMore();

    void start();

    void pause();

signals:
    void SIG_audioFrame(QByteArray bt);
private:
    QAudioInput *m_audio_in;//采集声卡 音频输入
    QIODevice*m_buffer_in;//对应的缓冲区
    QTimer *m_timer;
     QAudioFormat format;
    int m_audioState;

    //speex 相关变量
    SpeexBits bits_enc;
    void *Enc_State;

    VadInst *handle;
};

#endif // AUDIOREAD_H
