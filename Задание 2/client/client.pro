QT += core network
QT -= gui

TARGET = client
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
           client.cpp

HEADERS += client.h
