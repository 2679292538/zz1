HEADERS += \
    $$PWD/common.h \
    $$PWD/myfacedetect.h \
    $$PWD/screenread.h \
    $$PWD/threadworker.h \
    $$PWD/videoread.h

SOURCES += \
    $$PWD/myfacedetect.cpp \
    $$PWD/screenread.cpp \
    $$PWD/threadworker.cpp \
    $$PWD/videoread.cpp


INCLUDEPATH+=D:\qt\opencv-release/include/opencv2\
 D:\qt\opencv-release/include


LIBS+=D:\qt\opencv-release\lib\libopencv_calib3d420.dll.a\
 D:\qt\opencv-release\lib\libopencv_core420.dll.a\
 D:\qt\opencv-release\lib\libopencv_features2d420.dll.a\
 D:\qt\opencv-release\lib\libopencv_flann420.dll.a\
 D:\qt\opencv-release\lib\libopencv_highgui420.dll.a\
 D:\qt\opencv-release\lib\libopencv_imgproc420.dll.a\
 D:\qt\opencv-release\lib\libopencv_ml420.dll.a\
 D:\qt\opencv-release\lib\libopencv_objdetect420.dll.a\
 D:\qt\opencv-release\lib\libopencv_video420.dll.a\
 D:\qt\opencv-release\lib\libopencv_videoio420.dll.a
