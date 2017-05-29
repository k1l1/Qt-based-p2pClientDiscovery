#include "clientlist.h"
#define NOT_IN_VECTOR -1

ClientList::ClientList(int timeoutInterval, QObject *parent) : QObject(parent)
{
    ClientList::timeoutInterval = timeoutInterval;
}

/* pushes back received address (if new) in QHostAddress list and
 * sets timer for timeout checks */
void ClientList::pushBack(QHostAddress newAddress)
{
    int result = isEntryOfVector(newAddress);
    if(result == NOT_IN_VECTOR)
    {
        QTimer *timer = new QTimer();
        hostAddressList.push_back(newAddress);
        timerList.push_back(timer);
        timerList.constLast()->setInterval(9000);
        timerList.constLast()->start();
        connect(timerList.constLast(),SIGNAL(timeout()),this,SLOT(removeHost()));
        qDebug()<< "[clientFound@"<< newAddress.toString() << "]";
        emit newClientAdded(newAddress);
        emit clientListchanged();
    }
    else
    {
        timerList.at(result)->start();
    }
}

/*static function to check if two QHostAddresses are equal
 * returns true if equal and false if not equal */
bool ClientList::compareHostAddress(QHostAddress address1, QHostAddress address2)
{
    QString string1 = address1.toString();
    QString string2 = address2.toString();
    int result;

    result = QString::compare(string1,string2,Qt::CaseSensitive);

    if(result != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/* checks if hostAddress is in vector, returns -1 if not,
 * otherwise returns index of vector [0,1,2,...] */
int ClientList::isEntryOfVector(QHostAddress address)
{
    int result;
    for (int i=0;i< hostAddressList.size();i++)
    {
        result = compareHostAddress(hostAddressList.at(i),address);
        if (result)
        {
            return i;
        }
    }
    return NOT_IN_VECTOR;
}

/* slot which is called if one of the host timers has a timeout
 * the timer is stopped and removed, like the address from
 * the vector, appropriate signals are emitted */
void ClientList::removeHost()
{
    int index = NOT_IN_VECTOR;
    QObject *timer = QObject::sender();
    index = timerList.indexOf(reinterpret_cast<QTimer*>(timer));
    disconnect(timerList.at(index),SIGNAL(timeout()),this,SLOT(removeHost()));

    qDebug() << "[removed:"<< hostAddressList.at(index).toString() << ",reason: timeout]";
    emit clientGotInactive(hostAddressList.at(index));

    timerList.removeAt(index);
    hostAddressList.removeAt(index);
    timer->deleteLater();

    emit clientListchanged();
}

/* removes client from list due to an external
*  reason (f.e. no TCP response)
*  when the client reconnects, the signals
*  "newClientAdded" and "clientListChanged"
*  are emitted again. the timer is stopped and
*  removed, like the address from
*  the vector, appropriate signals are emitted */
void ClientList::removeHostExtern(QHostAddress removeAddress)
{
    int index = isEntryOfVector(removeAddress);

    if(index == NOT_IN_VECTOR)
    {
        qDebug() << removeAddress.toString() << "not found in active host list.";
    }
    else
    {
        disconnect(timerList.at(index),SIGNAL(timeout()),this,SLOT(removeHost()));

        qDebug() << "[removed: "<<hostAddressList.at(index).toString() <<
                    ",reason: ext]";
        timerList.at(index)->deleteLater();
        timerList.removeAt(index);
        hostAddressList.removeAt(index);

        emit clientListchanged();
    }
}

QList<QHostAddress> ClientList::getClientList()
{
    return hostAddressList;
}

