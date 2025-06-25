#ifndef WORLD_H
#define WORLD_H
#include<QAudioInput>
#include<QAudioOutput>
#include<QIODevice>
#include<QTimer>
#include<QMessageBox>
#include<QDebug>
#include"WebRtc_Vad/webrtc_vad.h"
#define USE_VAD (1)
enum ENUM_PLAY_STATE {stopped,playing,pausing};
#include"speex/include/speex.h"
#define SPEEX_QUALITY (8)
#define USE_SPEEX (1)
#endif // WORLD_H
