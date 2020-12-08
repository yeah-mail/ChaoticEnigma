#include "zstreamsocket.h"

namespace LibChaos {

ZStreamSocket::ZStreamSocket() : ZSocket(ZSocket::stream){

}

bool ZStreamSocket::open(ZAddress port){
    return ZSocket::open(port);
}

void ZStreamSocket::close(){
    ZSocket::close();
}

bool ZStreamSocket::isOpen() const {
    return ZSocket::isOpen();
}

bool ZStreamSocket::connect(ZAddress addr, ZConnection &conn){
    int connfd;
    ZAddress connaddr;
    bool ret = ZSocket::connect(addr, connfd, connaddr);
    conn = ZConnection(connfd, connaddr);
    return ret;
}

bool ZStreamSocket::listen(){
    return ZSocket::listen();
}

bool ZStreamSocket::accept(ZConnection &conn){
    int connfd;
    ZAddress connaddr;
    bool ret = ZSocket::accept(connfd, connaddr);
    conn = ZConnection(connfd, connaddr);
    return ret;
}

}
