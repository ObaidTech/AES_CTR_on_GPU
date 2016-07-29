#-------------------------------------------------
#
# Project created by QtCreator 2016-07-29T10:21:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AES_CTR_on_GPU
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    obencryptor.cpp

HEADERS  += mainwindow.h \
    obencryptor.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    images.qrc

win32:CONFIG(release, debug|release): LIBS += -L"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.5/lib/Win32/" -lOpenCL
else:win32:CONFIG(debug, debug|release): LIBS += -L"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.5/lib/Win32/" -lOpenCL
else:unix: LIBS += -L"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.5/lib/Win32/" -lOpenCL

INCLUDEPATH +=  "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.5/lib/Win32"\
                "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.5/include"
DEPENDPATH +=   "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.5/lib/Win32"\
                "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.5/include"
