#include "parser.h"

Parameters::Parameters(int argc, char** argv)
{
    this->argc = argc;
    this->argv = argv;
}

bool Parameters::IsSwitchChar(char c)
{
    return c == '-';
}

void Parameters::toJSON()
{
    ParametersJSON["Method"] = Method;
    Json::Value temp;
    for(auto kv : parameters)
    {
        Json::Value array;
            for(int i=0; i < kv.second.size() ; i++)
            {
                array.append(kv.second[i]);
            }
        temp[kv.first] = array;
    }
    ParametersJSON["Parameters"]= temp;
    Json::StyledWriter styledWriter;
    std::cout << styledWriter.write(ParametersJSON);

}

void Parameters::parseParameters() {

    parameters.clear();

    for (int i = 1; i < argc; i++) {
        std::string str(argv[i]);
        std::string strValue;
        size_t is_index = str.find('=');
        if (is_index != std::string::npos) {
            strValue = str.substr(is_index + 1);
            str = str.substr(0, is_index);

        }
        if (str[0] != '-')
            continue;

        // Interpret --foo as -foo.
        // If both --foo and -foo are set, the last takes effect.
        if (str.length() > 1 && str[1] == '-')
            str = str.substr(1);

        parameters[str].push_back(strValue);
    }

}

void Parameters::parseMethod()
{
    char argc_temp = argc;
    char** argv_temp = argv;
        // Skip switches
        while(argc_temp > 1 && IsSwitchChar(argv_temp[1][0]))
        {
            argc_temp--;
            argv_temp++;
        }

        Method = "";
        // Method
        if (argc_temp < 2) {
            checkSanity();
            throw Exception("Too few Parameters!\n");
        }else
            Method = argv_temp[1];

}

void Parameters::help()
{
    std::string strUsage=std::string("This is a experimental user interface to work with confidential-bank client.");

    strUsage += std::string("\n\n") + "Usage:" + "\n" +
               "  bnb-cli [options] <command> [params]  " + "Send command to BONAB" + "\n" +
               "  bnb-cli [options] help                " + "List commands" + "\n" +
               "  bnb-cli [options] help <command>      " + "Get help for a command" + "\n" + "\n" +
               "List of command:" + "\n" +
               "  bnb-cli <createAccount> [Description]" + "\n";

    logger::log(strUsage);
}

void Parameters::checkSanity()
{
    if(argc<2 || parameters.count("-?") || parameters.count("-h") || parameters.count("-help") || parameters.count("-version"))
    {
        help();
        exit(EXIT_SUCCESS);
    }

}

Json::Value Parameters::getJson()
{
    return ParametersJSON;
}

Exception::Exception(std::string desc)
{
    Description = desc;
}

void Exception::Log()
{
    std::cout << Description << std::endl;
}