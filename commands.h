//
// Created by reza on 4/19/18.
//

#ifndef REPLXX_COMMANDS_H
#define REPLXX_COMMANDS_H

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <replxx.hxx>
#include "utilities/request.h"
#include "cpp-readline/src/Console.hpp"
#include "iostream"


using Replxx = replxx::Replxx;
namespace cr = CppReadline;
using ret = cr::Console::ReturnCode;


//command signatures must be defined here , or they will be ignored
unsigned info(const std::vector<std::string> &);
unsigned get_accounts(const std::vector<std::string> & input);
unsigned get_notes(const std::vector<std::string> & input);
unsigned purge_accounts(const std::vector<std::string> & input);
unsigned add_account(const std::vector<std::string> & input);
unsigned transaction(const std::vector<std::string> & input);
unsigned confidential_tx_mint(const std::vector<std::string> & input);
unsigned confidential_tx_pour(const std::vector<std::string> & input);
unsigned get_registered_accounts(const std::vector<std::string> & input);
unsigned get_balance(const std::vector<std::string> & input);
unsigned get_confidential_address(const std::vector<std::string> &input);


// test method, for debugging purposes
unsigned test(const std::vector<std::string> & input);

#endif //REPLXX_COMMANDS_H
