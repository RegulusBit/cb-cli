#include "libzmqm.h"

#define HEARTBEAT_LIVENESS  3       //  3-5 is reasonable
#define HEARTBEAT_INTERVAL  2500    //  msecs
#define HEARTBEAT_EXPIRY    HEARTBEAT_INTERVAL * HEARTBEAT_LIVENESS

ZmqConnection::ZmqConnection(){}

zmq::context_t ZmqConnection::context(1);

zmq::context_t& ZmqConnection::getContext()
{
    return context;
}

ClientDealer::ClientDealer():requester(ZmqConnection::getContext(), ZMQ_REQ)
{

}

void ClientDealer::Connect(std::string EndPoint)
{
    try {
        requester.connect(EndPoint);
    }catch(...)
    {
        throw;
    }

}


Worker::Worker(std::string identity, Service* service, int64_t expiry)
{
    m_identity = identity;
    m_service = service;
    m_expiry = expiry;
}

Service::Service(std::string name)
{
    m_name = name;
}

Service::~Service()
{
        for(size_t i = 0; i < m_requests.size(); i++) {
            delete m_requests[i];
        }
}

Broker::Broker(int verbose)
{
        //  Initialize broker state
        m_context = &ZmqConnection::getContext();
        m_socket = new zmq::socket_t(*m_context, ZMQ_ROUTER);
        m_verbose = verbose;

       //
}

Broker::~Broker()
{

        while (! m_services.empty())
        {
            delete m_services.begin()->second;
            m_services.erase(m_services.begin());
        }
        while (! m_workers.empty())
        {
            delete m_workers.begin()->second;
            m_workers.erase(m_workers.begin());
        }
}

void Broker::Bind(std::string endpoint)
{
    m_endpoint = endpoint;
    m_socket->bind(m_endpoint.c_str());

    // Log the event if it's mandatory
}

void Broker::worker_send(Worker *worker, char *command, std::string option, zmsg *msg)
{

    msg =(msg ? new zmsg(*msg) : new zmsg());

    //  Stack protocol envelope to start of message
    if (option.size()>0)
        {                 //  Optional frame after command
            msg->push_front((char*)option.c_str());
        }
    msg->push_front(command);
    msg->push_front((char*)MDPW_WORKER);
    //  Stack routing envelope to start of message
    msg->wrap(worker->get_m_identity().c_str(), "");

    if (m_verbose) {
        //log the event
        //s_console ("I: sending %s to worker",
        //           mdps_commands [(int) *command]);
        msg->dump ();
    }
    msg->send (*m_socket);
    delete msg;

}

void Broker::worker_delete(Worker *&wrk, int disconnect)
{
    assert(wrk);
    if (disconnect) {
        worker_send(wrk, (char*)MDPW_DISCONNECT, "", NULL);
    }

    if (wrk->get_m_service()) {
        for(std::list<Worker*>::iterator it = wrk->get_m_service()->get_m_waiting().begin();
            it != wrk->get_m_service()->get_m_waiting().end();) {
            if (*it == wrk) {
                it = wrk->get_m_service()->get_m_waiting().erase(it);
            }
            else {
                ++it;
            }
        }
        wrk->get_m_service()->worker_decreament();
    }
    m_waiting.erase(wrk);
    //  This implicitly calls the worker destructor
    m_workers.erase(wrk->get_m_identity());
    delete wrk;
}


void Broker::purge_workers()
{
    //  Delete any idle workers that haven't pinged us in a while.

        std::deque<Worker*> toCull;
        int64_t now = s_clock();
        for (std::set<Worker*>::iterator wrk = m_waiting.begin(); wrk != m_waiting.end(); ++wrk)
        {
            if ((*wrk)->get_m_expiray() <= now)
                toCull.push_back(*wrk);
        }
        for (std::deque<Worker*>::iterator wrk = toCull.begin(); wrk != toCull.end(); ++wrk)
        {
            if (m_verbose) {
                //log the event
                //s_console ("I: deleting expired worker: %s",
                //           (*wrk)->get_m_identity().c_str());
            }
            worker_delete(*wrk, 0);
        }

}

void Broker::Start()
{
    //First setting for Heartbeat implementation
    int64_t now = s_clock();
    int64_t heartbeat_at = now + HEARTBEAT_INTERVAL;

    //Now the Big loop begins! where's the zk-SNARK's?seriously?
    while(!s_interrupted)
    {
        // importent Note: zmq::socket_t must cast to void* befor passing to zmq::pollitem_t
        zmq::pollitem_t items[] = {{(void*)*m_socket,  0, ZMQ_POLLIN, 0 } };

        //don't know what this is?
        int64_t timeout = heartbeat_at - now;
        if(timeout < 0)
            timeout = 0;
        zmq::poll(items, 1, (long)timeout);

        //  Process next input message, if any
        if(items[0].revents & ZMQ_POLLIN)
        {
            zmsg *msg = new zmsg(*m_socket);
            if(m_verbose) {
                //log the event if it's mandatory
                msg->dump();
            }

            std::string sender = std::string((char*)msg->pop_front().c_str());
            msg->pop_front(); //empty message
            std::string header = std::string((char*)msg->pop_front().c_str());

//              std::cout << "sbrok, sender: "<< sender << std::endl;
//              std::cout << "sbrok, header: "<< header << std::endl;
//              std::cout << "msg size: " << msg->parts() << std::endl;
//              msg->dump();

            if (header.compare(MDPC_CLIENT) == 0) {
                //client_process(sender, msg);
            }
            else if (header.compare(MDPW_WORKER) == 0) {
                //worker_process(sender, msg);
            }
            else {
                //log the message
                // s_console("E: invalid message:");
                msg->dump();
                delete msg;
            }
        }//end of message processing


        //  Disconnect and delete any expired workers
        //  Send heartbeats to idle workers if needed
        now = s_clock();
        if (now >= heartbeat_at) {
            purge_workers();
                for (std::set<Worker*>::iterator it = m_waiting.begin();
                     it != m_waiting.end() && (*it)!=0; it++)
                {
                    worker_send(*it, (char*)MDPW_HEARTBEAT, "", NULL);
                }
            heartbeat_at += HEARTBEAT_INTERVAL;
            now = s_clock();
        }
    }
}
