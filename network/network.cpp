#include "network.h"
Network::Network(QString identityMessage,QObject *parent, int port, int broadcastInterval, int timeoutInterval) : QObject(parent)
{ 
    qRegisterMetaType<QHostAddress>("QHostAddress");

    //creates Thread for UDP and moves QObject to thread
    udpThread = new QThread;
    udp = new MyUDP(identityMessage,0, port, broadcastInterval, timeoutInterval);
    udp->moveToThread(udpThread);
    udpThread->start();

    connect(udp->cList,SIGNAL(newClientAdded(QHostAddress)),this,SLOT(newClientFoundSLOT(QHostAddress)));
    connect(udp->cList,SIGNAL(clientGotInactive(QHostAddress)),this,SLOT(clientGotInactiveSLOT(QHostAddress)));
    connect(this,SIGNAL(removeClientExternSIGNAL(QHostAddress)),udp->cList,SLOT(removeHostExtern(QHostAddress)));
    connect(udp->cList,SIGNAL(clientListchanged()),this,SLOT(clientListChangedSLOT()));

}

Network::~Network()
{
    udp->deleteLater();
    udpThread->quit();
    udpThread->wait();
    udpThread->deleteLater();
}

/*
*  returns QList of all active clients
*  To catch all changes, call function
*  after "clientListChanged()" is emitted */
QList<QHostAddress> Network::getClientList()
{
    return udp->cList->getClientList();
}

/* removes client from list due to an external
*  reason (f.e. no TCP response)
*  when the client reconnects, the signals
*  "newClientAdded" and "clientListChanged"
*  are emitted again */
void Network::removeClientExtern(QHostAddress address)
{
    emit removeClientExternSIGNAL(address);
}


// internal signal routing
void Network::clientGotInactiveSLOT(QHostAddress address)
{
    emit clientGotInactive(address);
}

// internal signal routing
void Network::newClientFoundSLOT(QHostAddress address)
{
    emit newClientFound(address);
}

// internal signal routing
void Network::clientListChangedSLOT()
{
    emit clientListChanged();
}
