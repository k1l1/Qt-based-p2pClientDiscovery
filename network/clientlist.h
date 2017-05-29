#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <QObject>
#include <QTimer>
#include <QHostAddress>
#include <QList>

class ClientList : public QObject
{
    Q_OBJECT
public:
    explicit ClientList(int timeoutInterval,QObject *parent = 0);

signals:
    void clientGotInactive(QHostAddress address);
    void newClientAdded(QHostAddress address);
    void clientListchanged();

public:

    void pushBack(QHostAddress newAddress);  
    static bool compareHostAddress(QHostAddress address1, QHostAddress address2);

private:

    QList<QHostAddress> hostAddressList;
    QList<QTimer*> timerList;

    int isEntryOfVector(QHostAddress address);
    int timeoutInterval;

private slots:

    void removeHost();

public slots:
    void removeHostExtern(QHostAddress removeAddress);
    QList<QHostAddress> getClientList();

};

#endif // CLIENTLIST_H
