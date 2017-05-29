#include <QCoreApplication>
#include <QDebug>
#include "../network/network.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // udp network instance
    // add QT += network to your project file
    Network net("hallo");

    /*Network class provides following signals/slots & functions
     *
     * signal: void clientGotInactive(QHostAddress address)
     *         returns QHostAddress of client, who got
     *         inactive due to timeout/no response
     *
     * signal: void newClientAdded(QHostAddress address)
     *         return QHostAddress of client, who recently
     *         joined the network
     *
     * signal: void clientListChanged()
     *         signal is emitted if either a client joined
     *         or got inactive
     *
     * slot:   void removeClientExtern(QHostAddress address)
     *         removes client from list due to an external
     *         reason (f.e. no TCP response)
     *         when the client reconnects, the signals
     *         "newClientAdded" and "clientListChanged"
     *         are emitted again
     *
     * func:   QList<QHostAddress> getClientList();
     *         returns QList of all active clients
     *         To catch all changes, call function
     *         after "clientListChanged()" is emitted
    */

    /* static public function of myudp to get ipadress
    *  function returns first ip4 address address, which
    *  is not loopback address, so if more than one network
    *  devices are installed the first non loopback device
    *  is taken. */
    // QHostAddress address = MyUDP::getOwnAddress();

    /*static function to check if two QHostAddresses are equal
     * returns true if equal and false if not equal */
    bool result = ClientList::compareHostAddress(QHostAddress::LocalHost,
                                                QHostAddress::LocalHost);
    // QList may be empty
    QList<QHostAddress> list = net.getClientList();



    return a.exec();
}
