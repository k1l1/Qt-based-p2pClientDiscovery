#include "myudp.h"

MyUDP::MyUDP(QString identityMessage, QObject *parent, int port, int broadcastInterval, int timeoutInterval) :
    QObject(parent)
{
    MyUDP::port = port;

    cList = new ClientList(timeoutInterval,0);
    message = identityMessage;

    //send socket configuration
    socketSend = new QUdpSocket(this);
    socketSend->bind(QHostAddress::Broadcast,port);

    //read socket configuration
    socketRead = new QUdpSocket(this);
    socketRead->bind(port,QUdpSocket::ShareAddress);

    broadcastTimer = new QTimer(this);
    broadcastTimer->start(broadcastInterval);

    //connect Signal
    connect(socketRead,SIGNAL(readyRead()),this,SLOT(recieveMessage()));
    connect(broadcastTimer,SIGNAL(timeout()),this,SLOT(broadcastService()));

    ownAddress = getOwnAddress();
}

//broadcasts info message over network in defined time interval
void MyUDP::broadcastService()
{
    QByteArray Data;
    Data.append(message);
    socketSend->writeDatagram(Data,QHostAddress::Broadcast,port);
}

// recieves message from socket und compares to preset identity message
void MyUDP::recieveMessage()
{
    QByteArray buffer;
    QByteArray compareArray;
    compareArray = message.toUtf8();

    buffer.resize(socketRead->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socketRead->readDatagram(buffer.data(),buffer.size(),&sender,&senderPort);

    //check if hello Message is correct;
    if (buffer == compareArray)
    {
        //check if not loopback
        if (!sender.toString().contains(ownAddress.toString()))
        {
              cList->pushBack(sender);
        }
    }
}

/* static public function of myudp to get ipadress
*  function returns first ip4 address address, which
*  is not loopback address, so if more than one network
*  devices are installed the first non loopback device
*  is taken. */
QHostAddress MyUDP::getOwnAddress()
{
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
            qDebug() << "[ownClientIP: " << address.toString()<<"]";
            return address;
        }
    }
    QHostAddress adress;
    return adress;
}

void MyUDP::removeHostExtern(QHostAddress address)
{
    cList->removeHostExtern(address);
}
