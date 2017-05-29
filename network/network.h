#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QThread>
#include <QCoreApplication>
#include <cstring>
#include <QtEndian>
#include <QList>

#include "myudp.h"

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QString identityMessage,
                     QObject *parent = 0,
                     int port = 1234,
                     int broadcastInterval = 3000,
                     int timoutInterval = 9000);
    ~Network();


private:

    MyUDP *udp;
    QThread *udpThread;

public:
    QList<QHostAddress> getClientList();

signals:
    void clientGotInactive(QHostAddress address);
    void newClientFound(QHostAddress address);
    void clientListChanged();
    void removeClientExternSIGNAL(QHostAddress address);

private slots:
    void clientGotInactiveSLOT(QHostAddress address);
    void newClientFoundSLOT(QHostAddress address);
    void clientListChangedSLOT();

public slots:
    void removeClientExtern(QHostAddress address);
};

#endif // NETWORK_H
