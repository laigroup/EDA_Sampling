//
// Created by win on 2023/11/5.
//

#ifndef PROJECT_UTIL_H
#define PROJECT_UTIL_H

// CUDD
#include "../../lib/cudd-3.0.0/cplusplus/cuddObj.hh"
#include "../../lib/cudd-3.0.0/cudd/cuddInt.h"

// 获取变量对应的BDD向量
#include "../interface/BDDVector.h"

// 输入解析
#include "../interface/InputFileParse.h"

void utilAdd(Cudd &mgr, std::vector<BDD> const& lhs, std::vector<BDD> const& rhs, std::vector<BDD>& res);
void utilSub(Cudd &mgr, std::vector<BDD> const& lhs, std::vector<BDD> const& rhs, std::vector<BDD>& res);
void utilShl(Cudd& mgr, std::vector<BDD>& bvc, unsigned int offset);
void utilLogNeg(Cudd &mgr, const std::vector<BDD>& lhs, std::vector<BDD> &res);
void utilLogOr(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD> &res);
void utilEqu(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res);
void utilGreaterThan(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res);
void utilLthOrEqu(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res);
std::string addBigNum(std::string str1, std::string str2);
std::string divBigNum(std::string str1, std::string str2);
bool checkZero(const std::string &str);


#endif //PROJECT_UTIL_H
