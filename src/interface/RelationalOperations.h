//
// Created by win on 2023/10/1.
//

#ifndef PROJECT_RELATIONALOPERATIONS_H
#define PROJECT_RELATIONALOPERATIONS_H
// CUDD
#include "../../lib/cudd-3.0.0/cplusplus/cuddObj.hh"
#include "../../lib/cudd-3.0.0/cudd/cuddInt.h"

// 输入解析
#include "../interface/InputFileParse.h"

// 获取变量对应的BDD向量
#include "../interface/BDDVector.h"

// 相等关系
void Equ(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 不等关系
void Nequ(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 小于关系
void LessThan(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 大于关系
void GreaterThan(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 小于等于关系
void LthOrEqu(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 大于等于关系
void GthOrEqu(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);

#endif //PROJECT_RELATIONALOPERATIONS_H
