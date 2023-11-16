//
// Created by win on 2023/9/22.
//
#ifndef PROJECT_INPUTFILEPARSE_H
#define PROJECT_INPUTFILEPARSE_H

#include "../../lib/json.hpp"
#include <iostream>
#include <fstream>
#include <string>

namespace jsons{
    class Var{
    public:
        int id;
        std::string name;
        int isSign;
        int bit_width;
    };

    class BiTNode{
    public:
        std::string op;
        int id;
        std::string value;
        BiTNode* lhs;
        BiTNode* rhs;
        int bit_width;
        BiTNode() = default;
    };
}

// 自底向上计算各个子表达式的位宽
void bottomUp(jsons::BiTNode* T, const std::vector<jsons::Var> &vars);

// 自顶向下统一位宽
void topDown(jsons::BiTNode* T, const std::vector<jsons::Var> &vars);

// 中序输出约束树并计算当前约束中的最大位宽
void printConstrainTree(jsons::BiTNode* T, const std::vector<jsons::Var> &vars);
// 解析并输出json文件
void inputParse(std::vector<jsons::Var> &vars, std::vector<jsons::BiTNode> &T, const std::string& path);

#endif //PROJECT_INPUTFILEPARSE_H