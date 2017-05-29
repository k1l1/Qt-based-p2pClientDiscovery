#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QNetworkInterface>
#include <QMutex>

#include "clientlist.h"

class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QString identityMessage,
                   QObject *parent,
                   int port,
                   int broadcastInterval,
                   int timeoutInterval);

    ClientList *cList;
    static QHostAddress getOwnAddress();

signals:
    void clientGotInactive(QHostAddress address);
    void newClientAdded(QHostAddress address);

private slots:
    void recieveMessage();
    void broadcastService();

private:
    //functions

    bool compareHostAdress(QHostAddress address1,QHostAddress address2);

    //variables
    QUdpSocket *socketRead;
    QUdpSocket *socketSend;

    QTimer *broadcastTimer;

    QHostAddress ownAddress;

    QString message;
    int port = 1234;

private slots:
    void removeHostExtern(QHostAddress address);
};

#endif // MYUDP_H
