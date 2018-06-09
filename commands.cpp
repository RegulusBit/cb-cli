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

Json::Value template_creator(const std::vector<std::string> &input)
{
    Json::Value request;
    Json::Value params;
    std::string method;

    method.append(input[2]);
    request["Id"] = input[1];
    request["wallet_name"] = input[0];
    // omitting the first character of command: '.'
    request["Method"] = method.erase(0, 1);
    request["Parameters"] = params;

    return request;
}


// any new commands for UI must be defined here




unsigned info(const std::vector<std::string> &) {
    std::cout
            << "Welcome to confidential-bank client\n"
            << "version[confidential-bank]:CB-0.0.1\n"
            << "Press 'tab' to view autocompletions\n"
            << "Type '.help' for help \n"
            << "Type '.quit' or '.exit' to exit\n\n";
    return ret::Ok;
}

unsigned get_accounts(const std::vector<std::string> &input)
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

unsigned get_confidential_address(const std::vector<std::string> &input)
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

unsigned get_balance(const std::vector<std::string> & input)
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


unsigned get_notes(const std::vector<std::string> & input)
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


unsigned get_registered_accounts(const std::vector<std::string> & input)
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


        Postman postman;
        postman.s_client_socket(Connection::get_context());
        postman.post(request);
    }catch (...)
    {
        return ret::Error;
    }
    return ret::Ok;
}

unsigned register_account(const std::vector<std::string> & input)
{
    try {

        Json::Value request = template_creator(input);
        Json::Value params;

        if(input.size() > 3) {
            params["pk"] = input[3];
        }
        if(input.size() > 4 && isInteger(input[4])) {
            params["balance"] = atof(input[4].c_str());
        }

        request["Parameters"] = params;

        Postman postman;
        postman.s_client_socket(Connection::get_context());
        postman.post(request);
    }catch (...)
    {
        return ret::Error;
    }
    return ret::Ok;
}


unsigned get_details(const std::vector<std::string> & input)
{
    try {

        Json::Value request = template_creator(input);
        Json::Value params;

        if(input.size() > 3) {
            params["pk"] = input[3];
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

unsigned transaction(const std::vector<std::string> & input)
{
    try {

        Json::Value request = template_creator(input);
        Json::Value params;

        if(input.size() > 5) {
            params["from_pk"] = input[3];
            params["to_pk"] = input[4];

        }
        if(input.size() > 4 && isInteger(input[5])) {
            params["tx_value"] = atof(input[5].c_str());
        }

        request["Parameters"] = params;

        Postman postman;
        postman.s_client_socket(Connection::get_context());
        postman.post(request);
    }catch (...)
    {
        return ret::Error;
    }
    return ret::Ok;
}

unsigned confidential_tx_mint(const std::vector<std::string> & input)
{
    try {

        Json::Value request = template_creator(input);
        Json::Value params;

        if(input.size() > 5) {
            params["from_pk_transparent"] = input[3];
            params["to_pk_transparent"] = input[4];

        }
        if(input.size() > 6 && isInteger(input[5]) && isInteger(input[6])) {
            params["tx_value_from"] = atof(input[5].c_str());
            params["tx_value_to"] = atof(input[6].c_str());
        }

        if(input.size() > 7) {
            params["dest_conf_address"] = input[7];

        }

        if(input.size() > 7) {
            params["pk_enc"] = input[8];

        }


        request["Parameters"] = params;

        Postman postman;
        postman.s_client_socket(Connection::get_context());
        postman.post(request);
    }catch (...)
    {
        return ret::Error;
    }
    return ret::Ok;
}


unsigned confidential_tx_pour(const std::vector<std::string> & input)
{
    try {

        Json::Value request = template_creator(input);
        Json::Value params;

        if(input.size() > 5) {
            params["from_pk_transparent"] = input[3];
            params["to_pk_transparent"] = input[4];

        }
        if(input.size() > 7 && isInteger(input[5]) && isInteger(input[6]) && isInteger(input[7])) {
            params["tx_value_from"] = atof(input[5].c_str());
            params["tx_value_to"] = atof(input[6].c_str());
            params["note_value_to"] = atof(input[7].c_str());
        }

        if(input.size() > 8) {
            params["dest_conf_address"] = input[8];

        }

        if(input.size() > 8) {
            params["pk_enc"] = input[9];

        }


        request["Parameters"] = params;

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

    std::cout << "created hash is: " << input[3] << std::endl;

    return ret::Ok;
}