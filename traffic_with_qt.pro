#-------------------------------------------------
#
# Project created by QtCreator 2019-02-23T12:32:00
#
#-------------------------------------------------

QT       += core gui network
QT       += multimedia multimediawidgets
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrafficX
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        overall.cpp \
        videowidget.cpp \
        tracker.cpp \
        detector.cpp \
        decide.cpp \
    tcpconnect.cpp

HEADERS += \
        mainwindow.h \
        overall.h \
        videowidget.h \
        tracker.h \
        detector.h \
        clipp.hpp \
        common.hpp \
        decide.h \
    tcpconnect.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH+= \
             /opt/intel/openvino_2019.2.242/opencv/include \
             /opt/intel/openvino_2019.2.242/opencv/include/opencv2 \
#             /opt/intel/openvino_2019.2.242/deployment_tools/inference_engine/src/extension \
#             /opt/intel/openvino_2019.2.242/deployment_tools/inference_engine/samples/common/format_reader \
#             /opt/intel/openvino_2019.2.242/deployment_tools/inference_engine/include \
#             /opt/intel/openvino_2019.2.242/deployment_tools/inference_engine/include/cpp
             /home/dylan/mxnet/cpp-package/include \
             /home/dylan/mxnet/include

MXNET_PATH = /home/dylan/mxnet

LIBS += \
        /opt/intel/openvino_2019.2.242/opencv/lib/*.so \
        -L/opt/intel/openvino_2019.2.242/opencv/lib -lopencv_core -lopencv_imgproc -lopencv_dnn -lopencv_videoio \
        -L/opt/intel/openvino_2019.2.242/deployment_tools/inference_engine/lib/intel64 -linference_engine\
        -L/opt/intel/openvino_2019.2.242/openvx/lib -liomp5 \
#        /home/curiosity/intel/computer_vision_sdk/opencv/lib/libopencv_core.so.4.0 \
#        /home/curiosity/intel/computer_vision_sdk/inference_engine/lib/ubuntu_18.04/intel64/libinference_engine.so \
#        /home/curiosity/intel/computer_vision_sdk/inference_engine/external/omp/lib/libiomp5.so \
        $${MXNET_PATH}/lib/*.so \
        $${MXNET_PATH}/lib/*.a
#        -L/home/dylan/mxnet/lib -lmkldnn -lmklml_intel -lmxnet
#        /usr/local/cuda/lib64/*.a \
#        /usr/local/cuda/lib64/*.so

RESOURCES += \
    image.qrc

