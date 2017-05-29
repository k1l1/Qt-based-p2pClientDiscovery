QT += core
QT -= gui
QT += network

CONFIG += c++11

TARGET = peerToPeerNetworkDiscoveryService
CONFIG += console
CONFIG -= app_bundle


TEMPLATE = app

SOURCES += \
    main.cpp \
    ../network/network.cpp \
    ../network/myudp.cpp \
    ../network/clientlist.cpp

HEADERS += \
    ../network/network.h \
    ../network/myudp.h \
    ../network/clientlist.h \
    ../network/clientlist.h

