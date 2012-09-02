# -------------------------------------------------
# Project created by QtCreator 2011-10-03T15:07:00
# -------------------------------------------------
QT -= gui
TARGET = BottleNeck
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp
LIBS = $(SUBLIBS)  -L/usr/lib -lQtCore -lpthread -L/usr/lib/X11 -lX11
