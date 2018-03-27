#ifndef H_LIBZMQ
#define H_LIBZMQ

#include <vector>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <list>
#include <deque>
#include "zmsg.hpp"
#include "mdp.h"

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

class ZmqConnection
{
public:

    static zmq::context_t& getContext();

private:

    ZmqConnection();
    static zmq::context_t context;

public:
    ZmqConnection(ZmqConnection const&)  = delete;  //don't implement
    void operator=(ZmqConnection const&)  = delete; //don't implement
};



#endif