//
// Created by win on 2023/10/26.
//

#ifndef PROJECT_RESFILEPARSE_H
#define PROJECT_RESFILEPARSE_H

#include "../../lib/json.hpp"
#include <iostream>
#include <fstream>
#include <string>

namespace jsons{
    class Assignment{
    public:
        std::string value;
        Assignment() = default;
    };
}

void resParse(std::vector<std::vector<jsons::Assignment>> &ass, size_t varNum, const std::string& path);
#endif //PROJECT_RESFILEPARSE_H
