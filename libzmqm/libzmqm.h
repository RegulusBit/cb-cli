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



class ClientDealer
{
public:
    ClientDealer(void);

    void Connect(std::string EndPoint);

private:
    zmq::socket_t requester;
};

class Service;

class Worker
{
public:
    Worker(std::string identity, Service* service = 0, int64_t expiry = 0);

    int64_t get_m_expiray(void){return m_expiry;}
    std::string get_m_identity(void){return m_identity;}
    Service* get_m_service(void){return m_service;}

private:
    std::string m_identity;   //  Address of worker
    Service* m_service;      //  Owning service, if known
    int64_t m_expiry;         //  Expires at unless heartbeat
};

class Service
{
public:
    Service(std::string name);
    ~Service();

    std::list<Worker*> get_m_waiting(){return m_waiting;}
    void worker_decreament(void){m_workers--;}

private:
    std::string m_name;             //  Service name
    std::deque<zmsg*> m_requests;   //  List of client requests
    std::list<Worker*> m_waiting;  //  List of waiting workers
    size_t m_workers;               //  How many workers we have

};

class Broker
{
public:
    //  Constructor for broker object

    Broker(int verbose);
    virtual ~Broker();

    //  Bind broker to endpoint, can call this multiple times
    //  We use a single socket for both clients and workers.
    void Bind(std::string endpoint);

    void Start(void);

private:
    zmq::context_t* m_context;                  //  0MQ context
    zmq::socket_t* m_socket;                    //  Socket for clients & workers
    int m_verbose;                               //  log activity
    std::string m_endpoint;                      //  Broker binds to this endpoint
    std::map<std::string, Service*> m_services;  //  Hash of known services
    std::map<std::string, Worker*> m_workers;    //  Hash of known workers
    std::set<Worker*> m_waiting;              //  List of waiting workers

    void purge_workers(void);
    void worker_delete(Worker *&wrk, int disconnect);

    //  Send message to worker
    //  If pointer to message is provided, sends that messag
    void worker_send(Worker *worker, char *command, std::string option, zmsg *msg);
};


#endif