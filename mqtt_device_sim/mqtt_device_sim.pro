QT       += core gui mqtt network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    dialog.cpp \
    dialog2.cpp \
    main.cpp \
    mainwindow.cpp \
    mqttdevice.cpp

HEADERS += \
    aboutdialog.h \
    dialog.h \
    dialog2.h \
    mainwindow.h \
    mqttdevice.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    aboutdialog.ui \
    dialog.ui \
    dialog2.ui



