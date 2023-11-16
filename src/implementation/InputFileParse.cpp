#include "../interface/InputFileParse.h"
#include "../interface/BDDVector.h"
#include <vector>
using nlohmann::json;
using namespace::std;

namespace jsons{

    // 转换变量列表
    void from_json(const json& j, Var &var){
        j.at("id").get_to(var.id);
        j.at("name").get_to(var.name);
        j.at("signed").get_to(var.isSign);
        j.at("bit_width").get_to(var.bit_width);
    }

    // 先根转换约束列表
    void from_json(const json& j, BiTNode &node){
        j.at("op").get_to(node.op);
        if (node.op == "VAR"){ // 变量结点，递归边界 终端节点
            j.at("id").get_to(node.id);
            node.value = "";
            node.lhs = nullptr;
            node.rhs = nullptr;
        }else if (node.op == "CONST"){ // 常量结点，递归边界 终端节点
            j.at("value").get_to(node.value);
            node.id = -1;
            node.lhs = nullptr;
            node.rhs = nullptr;
        }else if (node.op == "LOG_NEG" || node.op == "BIT_NEG" || node.op == "MINUS"){
            // 逻辑取反，按位取反，负号，一元运算符，构造单分支结点的左孩子结点
            node.id = -1;
            node.value = "";
            node.rhs = nullptr;
            auto left_node = new BiTNode();
            from_json(j["lhs_expression"], *left_node);
            node.lhs = left_node;
        }else{ // 二元运算符，递归构造非终端节点的左右孩子结点
            node.id = -1;
            node.value = "";
            auto left_node = new BiTNode();
            auto right_node = new BiTNode();
            from_json(j["lhs_expression"], *left_node);
//            left_node = j["lhs_expression"];
//            j["lhs_expression"].get_to(left_node);
            node.lhs = left_node;
            from_json(j["rhs_expression"], *right_node);
//            right_node = j["rhs_expression"];
//            j["rhs_expression"].get_to(right_node);
            node.rhs = right_node;
        }
    }
}

// 自底向上计算子表达式结果的位宽
void bottomUp(jsons::BiTNode* T, const vector<jsons::Var> &vars){
    if (T->op == "ADD" || T->op == "SUB" || T->op == "MUL" || T->op == "DIV" || T->op == "BIT_AND"||
    T->op == "BIT_OR" || T->op == "BIT_XOR"){
        bottomUp(T->lhs, vars); // 计算左表达式位宽
        bottomUp(T->rhs, vars); // 计算右表达式位宽
        if(T->lhs->bit_width >= T->rhs->bit_width)
            T->bit_width = T->lhs->bit_width;
        else
            T->bit_width = T->rhs->bit_width;
    }
    else if (T->op == "EQ" || T->op == "NEQ" || T->op == "LT" || T->op == "LTE" || T->op == "GT" ||
    T->op == "GTE" || T->op == "LOG_AND" || T->op == "LOG_OR" || T->op == "IMPLY"){
        bottomUp(T->lhs, vars);
        bottomUp(T->rhs, vars);
        T->bit_width = 1;
    }
    else if (T->op == "LSHIFT" || T->op == "RSHIFT"){
        bottomUp(T->lhs, vars);
        bottomUp(T->rhs, vars);
        T->bit_width = T->lhs->bit_width;
    }
    else if (T->op == "LOG_NEG"){
        bottomUp(T->lhs, vars);
        T->bit_width = 1;
    }
    else if (T->op == "BIT_NEG" || T->op == "MINUS"){
        bottomUp(T->lhs, vars);
        T->bit_width = T->lhs->bit_width;
    }
    else if (T->op == "VAR"){
        T->bit_width = vars[T->id].bit_width;
    }
    else if(T->op == "CONST"){
        string tmp_width;
        for (auto c : T->value){
            if (c == '\'' || c == 'h'){
                break;
            }
            tmp_width += c;
        }
        // 获得常量位宽
        T->bit_width = stoi(tmp_width);
    }else{
        cout << "The OP is not yet supported" << endl;
    }
}

// 自顶向下统一各个表达式的位宽
void topDown(jsons::BiTNode* T, const vector<jsons::Var> &vars){
    if (T->op == "VAR" || T->op == "CONST")
        return;
    if (T->op == "ADD" || T->op == "SUB" || T->op == "MUL" || T->op == "DIV" || T->op == "BIT_AND"||
        T->op == "BIT_OR" || T->op == "BIT_XOR"){
        if (T->lhs->bit_width != T->bit_width)
            T->lhs->bit_width = T->bit_width;
        topDown(T->lhs, vars); // 统一左侧子表达式位宽
        if (T->rhs->bit_width != T->bit_width)
            T->rhs->bit_width = T->bit_width;
        topDown(T->rhs, vars); // 统一右侧子表达式位宽
    }
    else if (T->op == "EQ" || T->op == "NEQ" || T->op == "LT" || T->op == "LTE" || T->op == "GT" || T->op == "GTE"){
        if (T->lhs->bit_width < T->rhs->bit_width)
            T->lhs->bit_width = T->rhs->bit_width;
        if (T->lhs->bit_width > T->rhs->bit_width)
            T->rhs->bit_width = T->lhs->bit_width;
        topDown(T->lhs, vars);
        topDown(T->rhs, vars);
    }
    else if (T->op == "LOG_AND" || T->op == "LOG_OR" || T->op == "IMPLY"){
        topDown(T->lhs, vars);
        topDown(T->rhs, vars);
    }
    else if (T->op == "LOG_NEG"){
        topDown(T->lhs, vars);
    }
    else if (T->op == "LSHIFT" || T->op == "RSHIFT"){
        T->lhs->bit_width = T->bit_width;
        topDown(T->lhs, vars);
        topDown(T->rhs, vars);
    }else if (T->op == "BIT_NEG" || T->op == "MINUS"){
        T->lhs->bit_width = T->bit_width;
        topDown(T->lhs, vars);
    }else{
        cout << "The OP is not yet supported" << endl;
    }
}

// 输出约束
void printConstrainTree(jsons::BiTNode* T, const vector<jsons::Var> &vars, int conNum){
    if (T != nullptr){
        printConstrainTree(T->lhs, vars, conNum);
        if (T->op == "VAR"){
            printf("%d'(var_%d)", T->bit_width, T->id);
        }else if (T->op == "CONST"){
            printf("%d'(%s)", T->bit_width, T->value.c_str());
        }else if (T->op == "LOG_NEG" || T->op == "BIT_NEG" || T->op == "MINUS"){
            printf(" %d'(%s)", T->bit_width, T->op.c_str());
        }else{
            printf(" %d'(%s) ", T->bit_width, T->op.c_str());
        }
        printConstrainTree(T->rhs, vars, conNum);
    }
}

// vars存储变量列表
// T存储各个约束的根节点
void inputParse(vector<jsons::Var> &vars, vector<jsons::BiTNode> &T, const string& path){
    json j;
    fstream jfile(path);
    if (!jfile.is_open()){
        cout << "Fail to open file!" << endl;
        return ;
    }
    jfile >> j;

    // 读取变量列表
    size_t vars_size = j["variable_list"].size();
    cout << "vars_list_size = " << vars_size << endl;
    cout << "打印变量列表：" << endl;
    for (int i = 0; i < vars_size; i ++){
        vars.push_back(j["variable_list"][i]);
        printf("变量%d：id=%d, name=%s, signed=%d, bit_width=%d\n",
               i, vars[i].id, vars[i].name.c_str(), vars[i].isSign, vars[i].bit_width);
    }
    cout << endl;

    // 读取约束列表
    size_t constrains_size = j["constraint_list"].size();
    for (int i = 0; i < constrains_size; i ++ ){
        T.push_back(j["constraint_list"][i]);
    }

    // 统一表达式位宽
    for (auto& t : T){
        bottomUp(&t, vars);
        topDown(&t, vars);
    }

    // 输出约束
    cout << "打印约束列表：" << endl;
    cout << "constraint_list_size = " << constrains_size << endl;

    for (int i = 0; i < constrains_size; i++ ){
        cout << "约束" << i << ": ";
        printConstrainTree(&T[i], vars, i);
        cout << endl;
    }
    cout << endl;
}
