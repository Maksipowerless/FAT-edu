QT += core
QT -= gui

TARGET = FAT
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    filesystemdriver.cpp

HEADERS += \
    superblock.h \
    filesystemdriver.h \
    file.h

