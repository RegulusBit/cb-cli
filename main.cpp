#include <iostream>
#include "utilities/parser.h"


int main(int argc, char* argv[]) {

    Parameters params(argc, argv);

    try
    {
        params.ParseParameters();
        params.ParseMethod();
        params.CheckSanity();
        params.toJSON();

    }catch(Exception& e){
        e.Log();
        exit(EXIT_FAILURE);
    }catch (...){
        exit(EXIT_FAILURE);
    }






    return 0;
}