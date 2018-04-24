//
// Created by reza on 4/22/18.
//

#ifndef REPLXX_REPLXX_UTILS_H
#define REPLXX_REPLXX_UTILS_H

#include <regex>
#include <string>
#include <vector>
#include <cctype>
#include <utility>
#include <iomanip>
#include <iostream>
#include "replxx.hxx"

using Replxx = replxx::Replxx;

// prototypes
Replxx::completions_t hook_completion(std::string const& context, int index, void* user_data);
Replxx::hints_t hook_hint(std::string const& context, int index, Replxx::Color& color, void* user_data);
void hook_color(std::string const& str, Replxx::colors_t& colors, void* user_data);

#endif //REPLXX_REPLXX_UTILS_H
