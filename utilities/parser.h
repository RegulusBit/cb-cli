#ifndef H_PARSER
#define H_PARSER


#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <jsoncpp/json/json.h>

class Exception
{
public:
    Exception(std::string desc);
    void Log(void);
private:
    std::string Description;
};

class Parameters
{
public:
    Parameters(int argc, char** argv);
    void ParseParameters(void);
    void ParseMethod(void);
    void CheckSanity(void);
    void toJSON(void);

    //test method(for debugging purposes)
    void get_params(void);


private:

    int argc;
    char** argv;
    std::unordered_map<std::string, std::vector<std::string>> parameters;
    std::string Method;
    Json::Value ParametersJSON;

    //internal methods needed in class
    void help(void);
    bool IsSwitchChar(char c);

};





#endif