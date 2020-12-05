#ifndef ZSTREAMSOCKET_H
#define ZSTREAMSOCKET_H

#include "zsocket.h"
#include "zconnection.h"

namespace LibChaos {

// For TCP sockets
// Paired with ZConnection
class ZStreamSocket : private ZSocket {
public:
    ZStreamSocket();

    bool open(ZAddress port);
    void close();
    bool isOpen() const;

    void setReadBuffer(zu64 size);

    bool connect(ZAddress addr, ZConnection &conn);
    bool listen();
    bool accept(ZConnection &conn);

    zsocktype getSocket() const {
        return ZSocket::getSocket();
    }

    ZAddress getBoundAddress() const {
        return ZSocket::getBoundAddress();
    }

    ZError getError() const {
        return ZSocket::getError();
    }

};

}

#endif // ZSTREAMSOCKET_H
