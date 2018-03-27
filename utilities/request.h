//
// Created by mhghasemi on 3/17/18.
//

#ifndef BNB_CLI_REQUEST_H
#define BNB_CLI_REQUEST_H

#include <jsoncpp/json/json.h>

#include <zmq.h>
#include <zmq.hpp>

#define REQUEST_TIMEOUT     2500    //  msecs, (> 1000!)
#define REQUEST_RETRIES     3       //  Before we abandon

class Connection
{
public:
    static zmq::context_t& get_context(void);
    ~Connection()
    {
        cntxt.close();
    }
private:
    Connection(){}
    static zmq::context_t cntxt;
};


class Postman {
public:
    Postman();
    ~Postman()
    {
        if(client)
            delete client;
    }
    Json::Value post(Json::Value rqst);

    void s_client_socket(zmq::context_t &context);

private:

    Json::Value request;
    Json::Value reply;
    std::string requestMessage;
    std::string replyMessage;
    zmq::socket_t *client;
};
#endif //BNB_CLI_REQUEST_H
