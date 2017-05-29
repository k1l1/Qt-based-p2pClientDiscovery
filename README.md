# Qt-based-p2pClientDiscovery
Qt based network discovery service for 2p2 clients

## how to include
add
```
QT += network
```
to your project file

```
#include "network/network.h"
```
create instance of "Network" class and set an identify message for the p2p clients

## usage
Network class provides following signals/slots & functions

signal: 

```void clientGotInactive(QHostAddress address);```

returns QHostAddress of client, who got
inactive due to timeout/no response.
    
signal: 

```void newClientAdded(QHostAddress address);```

return QHostAddress of client, who recently
joined the network.
    
signal:

```void clientListChanged();```

signal is emitted if either a client joined
or got inactive.
    
slot:

```void removeClientExtern(QHostAddress address);```

removes client from list due to an external
reason (f.e. no TCP response)
when the client reconnects, the signals
"newClientAdded" and "clientListChanged"
are emitted again.
    
func:

```QList<QHostAddress> getClientList();```

returns QList of all active clients.
To catch all changes, call function
after "clientListChanged()" is emitted.
    
## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
