//
// Created by win on 2023/9/27.
//

#ifndef PROJECT_BDDVECTOR_H
#define PROJECT_BDDVECTOR_H
// CUDD
#include "../../lib/cudd-3.0.0/cplusplus/cuddObj.hh"
#include "../../lib/cudd-3.0.0/cudd/cuddInt.h"

// 输入解析
#include "../interface/InputFileParse.h"

// 标记变量是否已经被创建
#include "../interface/CalculateConstrains.h"

// 记录所有变量总的位宽
extern unsigned int sumWidth;

// 创建所有向量对应的位向量
void creatVarsBvc(Cudd &mgr, const std::vector<jsons::Var>& vars, std::vector<std::vector<BDD>>& varsBdd);

// 获取给定约束中变量和常量的BDD向量
void getBDDVector(Cudd &mgr, jsons::BiTNode* terminalNode, const std::vector<std::vector<BDD>>& varsBdd, std::vector<BDD>& var);

size_t Atoi(const std::string& str, int radix); // 其他进制(大于10)转十进制

// 对向量进行位扩展，一般用于位宽对齐
/*
 * 其中，
 * var为要扩展的向量
 * res为扩展后的向量
 * bitNum为宽展后的位宽
 * con为扩展的符号
 * */
void unifyBitWidth(Cudd& mgr, const std::vector<BDD>& var, std::vector<BDD> &res, size_t bitNum);

// 创建全零或全一的位向量
void buildBvc(Cudd& mgr, std::vector<BDD>& var, size_t bitNum, bool isTrue);

// 获取数值val对应的BDD向量
void getConstBvc(Cudd& mgr, size_t bitNum, size_t val, std::vector<BDD>& res);
#endif //PROJECT_BDDVECTOR_H
