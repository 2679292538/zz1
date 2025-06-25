QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(./netapi/netapi.pri)
INCLUDEPATH+=./netapi

include(./AudioApi/audioapi.pri)
INCLUDEPATH+=./md5

include(./md5/md5.pri)
INCLUDEPATH+=./AudioApi

include(./VideoApi/videoapi.pri)
INCLUDEPATH+=./VideoApi

SOURCES += \
    ckernel.cpp \
    loginindialog.cpp \
    main.cpp \
    roomdialog.cpp \
    usershow.cpp \
    wechatdialog.cpp

HEADERS += \
    ckernel.h \
    loginindialog.h \
    roomdialog.h \
    usershow.h \
    wechatdialog.h

FORMS += \
    loginindialog.ui \
    roomdialog.ui \
    usershow.ui \
    wechatdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
