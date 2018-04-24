//
// Created by reza on 4/19/18.
//

#include "commands.h"


// utilities needed by methods TODO: create independent utility cpp and headers

inline bool isInteger(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

    char * p ;
    strtol(s.c_str(), &p, 10) ;

    return (*p == 0) ;
}

Json::Value template_creator(const std::vector<std::string> & input)
{
    Json::Value request;
    Json::Value params;
    std::string method;

    method.append(input[1]);
    request["Id"] = input[0];
    // omitting the first character of command: '.'
    request["Method"] = method.erase(0, 1);
    request["Parameters"] = params;

    return request;
}


// any new commands for UI must be defined here




unsigned info(const std::vector<std::string> &) {
    std::cout << "Welcome to the example console. This command does not really\n"
              << "do anything aside from printing this statement. Thus it does\n"
              << "not need to look into the arguments that are passed to it.\n";
    return ret::Ok;
}

unsigned get_accounts(const std::vector<std::string> & input)
{
    try {
        // request must be defined as pointer to prevent some errors in Jsoncpp library
        Json::Value request = template_creator(input);

        Postman postman;
        postman.s_client_socket(Connection::get_context());
        postman.post(request);
    }catch (...)
    {
        return ret::Error;
    }
    return ret::Ok;
}

unsigned purge_accounts(const std::vector<std::string> & input)
{
    try {
        // request must be defined as pointer to prevent some errors in Jsoncpp library
        Json::Value request = template_creator(input);

        Postman postman;
        postman.s_client_socket(Connection::get_context());
        postman.post(request);
    }catch (...)
    {
        return ret::Error;
    }
    return ret::Ok;
}

unsigned add_account(const std::vector<std::string> & input)
{
    try {

        Json::Value request = template_creator(input);
        Json::Value params;

        if(input.size() > 2 && isInteger(input[2])) {
            params["-b"] = input[2];
            request["Parameters"] = params;
        }


        Postman postman;
        postman.s_client_socket(Connection::get_context());
        postman.post(request);
    }catch (...)
    {
        return ret::Error;
    }
    return ret::Ok;
}

unsigned test(const std::vector<std::string> & input)
{

    std::cout << "created hash is: " << input[0] << std::endl;

    return ret::Ok;
}