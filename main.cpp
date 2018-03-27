#include <iostream>
#include "utilities/parser.h"
#include "utilities/request.h"
#include "utilities/logger.h"


int main(int argc, char* argv[]) {


    Parameters params(argc, argv);
    zmq::context_t context(1);
    Postman postman;




    try
    {
        params.parseParameters();
        params.parseMethod();
        params.checkSanity();
        params.toJSON();
        postman.s_client_socket(Connection::get_context());
        postman.post(params.getJson());


    }catch(Exception& e){
        e.Log();
        exit(EXIT_FAILURE);
    }catch (...){

        exit(EXIT_FAILURE);
    }



    return 0;
}