#include "libzmqm.h"



ZmqConnection::ZmqConnection(){}

zmq::context_t ZmqConnection::context(1);

zmq::context_t& ZmqConnection::getContext()
{
    return context;
}

