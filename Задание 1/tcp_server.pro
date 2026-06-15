QT += core network
QT -= gui

TARGET = server
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
           server.cpp

HEADERS += server.h
