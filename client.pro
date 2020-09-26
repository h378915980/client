QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix|win32: LIBS += -L$$PWD/tool/compress/lib/ -lquazip
INCLUDEPATH += $$PWD/tool/compress/include/quazip/
DEPENDPATH += $$PWD/tool/compress/include/quazip/


SOURCES += \
    calibration/position/UPositionMap.cpp \
    main.cpp \
    tool/fileOperator.cpp \
    tool/inifile.cpp \
    ui/confirmmasspoint.cpp \
    ui/energycalibration.cpp \
    ui/maincalibration.cpp \
    ui/positionmap.cpp \
    utcpsocket.cpp \
    widget.cpp

HEADERS += \
    calibration/position/UAcquireDef.h \
    calibration/position/UPositionMap.h \
    calibration/position/UTypeDef.h \
    calibration/position/libAcquirePET_global.h \
    maincalibration.h \
    tool/compress/include/quazip/JlCompress.h \
    tool/compress/include/quazip/crypt.h \
    tool/compress/include/quazip/ioapi.h \
    tool/compress/include/quazip/quaadler32.h \
    tool/compress/include/quazip/quachecksum32.h \
    tool/compress/include/quazip/quacrc32.h \
    tool/compress/include/quazip/quagzipfile.h \
    tool/compress/include/quazip/quaziodevice.h \
    tool/compress/include/quazip/quazip.h \
    tool/compress/include/quazip/quazip_global.h \
    tool/compress/include/quazip/quazipdir.h \
    tool/compress/include/quazip/quazipfile.h \
    tool/compress/include/quazip/quazipfileinfo.h \
    tool/compress/include/quazip/quazipnewinfo.h \
    tool/compress/include/quazip/unzip.h \
    tool/compress/include/quazip/zip.h \
    tool/fileOperator.h \
    tool/inifile.h \
    ui/confirmmasspoint.h \
    ui/energycalibration.h \
    ui/maincalibration.h \
    ui/positionmap.h \
    utcpsocket.h \
    widget.h

FORMS += \
    ui/confirmmasspoint.ui \
    ui/energycalibration.ui \
    ui/maincalibration.ui \
    ui/positionmap.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    iconQrc.qrc

