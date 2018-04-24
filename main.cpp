#include <regex>
#include <iomanip>
#include <iostream>
#include "cpp-readline/src/Console.hpp"
#include "commands.cpp"
#include "replxx_utils.h"
#include "hashidsxx/hashids.h"

using Replxx = replxx::Replxx;
namespace cr = CppReadline;
using ret = cr::Console::ReturnCode;






int main(int argc, char *argv[]) {

    cr::Console c("\x1b[1;32mcb-cli\x1b[0m> ");

    // init the hashidsxx
    hashidsxx::Hashids id_generator("salt", 16, "abcdefghijklmnopqrstuvwxyz");

    // this counter is used to create unique ID's for each request
    unsigned request_counter = 1;

    // registering commands
    c.registerCommand(".info", info, "this command shows the manual for this application.");
    c.registerCommand(".get_accounts", get_accounts, "this command will return list of wallet accounts.");
    c.registerCommand(".purge_accounts", purge_accounts, "this command will delete all of wallet accounts.");
    c.registerCommand(".add_account", add_account, "this command will create new account in wallet.");
    c.registerCommand(".test", test, "this method is a dummy command for debugging purposes.");


    // words to be completed
    std::vector<std::string> examples = {".clear"};

    // highlight specific words
    // a regex string, and a color
    // the order matters, the last match will take precedence
    using cl = Replxx::Color;
    std::vector<std::pair<std::string, cl>> regex_color {
            // single chars
            {"\\`", cl::BRIGHTCYAN},
            {"\\'", cl::BRIGHTBLUE},
            {"\\\"", cl::BRIGHTBLUE},
            {"\\-", cl::BRIGHTBLUE},
            {"\\+", cl::BRIGHTBLUE},
            {"\\=", cl::BRIGHTBLUE},
            {"\\/", cl::BRIGHTBLUE},
            {"\\*", cl::BRIGHTBLUE},
            {"\\^", cl::BRIGHTBLUE},
            {"\\.", cl::BRIGHTMAGENTA},
            {"\\(", cl::BRIGHTMAGENTA},
            {"\\)", cl::BRIGHTMAGENTA},
            {"\\[", cl::BRIGHTMAGENTA},
            {"\\]", cl::BRIGHTMAGENTA},
            {"\\{", cl::BRIGHTMAGENTA},
            {"\\}", cl::BRIGHTMAGENTA},

            // color keywords
            {"color_black", cl::BLACK},
            {"color_red", cl::RED},
            {"color_green", cl::GREEN},
            {"color_brown", cl::BROWN},
            {"color_blue", cl::BLUE},
            {"color_magenta", cl::MAGENTA},
            {"color_cyan", cl::CYAN},
            {"color_lightgray", cl::LIGHTGRAY},
            {"color_gray", cl::GRAY},
            {"color_brightred", cl::BRIGHTRED},
            {"color_brightgreen", cl::BRIGHTGREEN},
            {"color_yellow", cl::YELLOW},
            {"color_brightblue", cl::BRIGHTBLUE},
            {"color_brightmagenta", cl::BRIGHTMAGENTA},
            {"color_brightcyan", cl::BRIGHTCYAN},
            {"color_white", cl::WHITE},
            {"color_normal", cl::NORMAL},

            // commands
            {"\\.help", cl::BRIGHTMAGENTA},
            {"\\.history", cl::BRIGHTMAGENTA},
            {"\\.quit", cl::BRIGHTMAGENTA},
            {"\\.exit", cl::BRIGHTMAGENTA},
            {"\\.clear", cl::BRIGHTMAGENTA},
            {"\\.prompt", cl::BRIGHTMAGENTA},

            // numbers
            {"[\\-|+]{0,1}[0-9]+", cl::YELLOW}, // integers
            {"[\\-|+]{0,1}[0-9]*\\.[0-9]+", cl::YELLOW}, // decimals
            {"[\\-|+]{0,1}[0-9]+e[\\-|+]{0,1}[0-9]+", cl::YELLOW}, // scientific notation

            // strings
            {"\".*?\"", cl::BRIGHTGREEN}, // double quotes
            {"\'.*?\'", cl::BRIGHTGREEN}, // single quotes
    };


    for(auto k: c.getRegisteredCommands()) {
        examples.push_back(k);
        regex_color.push_back({k, cl::BRIGHTMAGENTA});
    }

    // init the replxx
    Replxx rx;
    rx.install_window_change_handler();

    // the path to the history file
    std::string history_file {"./replxx_history.txt"};

    // load the history file if it exists
    rx.history_load(history_file);

    // set the max history size
    rx.set_max_history_size(12);

    // set the max input line size
    rx.set_max_line_size(128);

    // set the max number of hint rows to show
    rx.set_max_hint_rows(10);

    // set the callbacks
    rx.set_completion_callback(hook_completion, static_cast<void*>(&examples));
    rx.set_highlighter_callback(hook_color, static_cast<void*>(&regex_color));
    rx.set_hint_callback(hook_hint, static_cast<void*>(&examples));

    // display initial welcome message
    std::cout
            << "Welcome to confidential-bank client\n"
            << "version[confidential-bank]:CB-0.0.1\n"
            << "Press 'tab' to view autocompletions\n"
            << "Type '.help' for help \n"
            << "Type '.quit' or '.exit' to exit\n\n";

    // set the repl prompt
    std::string prompt {"\x1b[1;32mcb-cli\x1b[0m> "};


    int retCode;




    // main repl loop
    for (;;) {
        // display the prompt and retrieve input from the user
        char const *cinput = rx.input(prompt);
        std::string input{cinput};

        if (cinput == nullptr) {
            // reached EOF

            std::cout << "\n";
            //break;
        } else if (input.compare(0, 6, ".clear") == 0) {
            // clear the screen
            rx.clear_screen();
            rx.history_add(input);
            continue;

        } else {


            retCode = c.readLine(cinput, id_generator.encode(request_counter));
            request_counter++;
            if (retCode == ret::Quit) {
                rx.history_add(std::string{cinput});
                break;
            } else {
                if (retCode >= 1)
                    std::cout << "Error in reading commands. use '.help' command for more information." << std::endl;
                rx.history_add(std::string{cinput});
                continue;
            }

            }
        }

        //save the history
        rx.history_save(history_file);

    std::cout << "\nExiting cb-cli\n";
}
