//
// Created by mhghasemi on 3/17/18.
//

#include "request.h"
#include "zhelpers.hpp"
#include "zmsg.hpp"
#include "logger.h"
#include <iostream>



zmq::context_t Connection::cntxt(1);

zmq::context_t& Connection::get_context()
{
    return cntxt;
}


Postman::Postman()
{
    srandom((unsigned)time(NULL));
    client = NULL;
}

void Postman::s_client_socket(zmq::context_t & context) {
    if(client)
        delete client;
    logger::log( "connecting to daemon…" );
    client = new zmq::socket_t (context, ZMQ_REQ);
    client->connect ("tcp://localhost:9155");

    //  Configure socket to not wait at close time
    int linger = 0;
    client->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));

    s_set_id(*client);
}


Json::Value Postman::post(Json::Value rqst)
{
    logger::log("starting to post the request...");

    int retries_left = REQUEST_RETRIES;

    Json::FastWriter fstw;
    Json::Reader rdr;
    Json::StyledWriter stwr;

    requestMessage = fstw.write(rqst);

    zmsg msg(requestMessage.c_str());
    zmsg zreply;
    //s_send(*client, requestMessage);
    msg.send(*client);

    bool expect_reply = true;
        while(expect_reply)
        {
            zmq_pollitem_t items[] = {{(void*)*client, 0, ZMQ_POLLIN, 0}};
            zmq::poll(&items[0], 1, REQUEST_TIMEOUT);

            if(items[0].revents && ZMQ_POLLIN)
            {
                //process the response
                //replyMessage = s_recv(*client);

                zreply.recv(*client);
                replyMessage = zreply.body();

                rdr.parse(replyMessage, reply);
                std::cout << stwr.write(reply) << std::endl;

                expect_reply = false;
            }
            else{
                if (--retries_left == 0) {
                    logger::log("daemon seems to be offline, abandoning");
                    expect_reply = false;
                    break;
                } else {
                    logger::log("no response from daemon, retrying…");
                    //  Old socket will be confused; close it and open a new one
                    s_client_socket(Connection::get_context());
                    //  Send request again, on new socket
                    msg.send(*client);
                }
            }
        }

}