//
// Created by win on 2023/9/22.
//

#ifndef PROJECT_LOGICOPERATIONS_H
#define PROJECT_LOGICOPERATIONS_H
// CUDD
#include "../../lib/cudd-3.0.0/cplusplus/cuddObj.hh"
#include "../../lib/cudd-3.0.0/cudd/cuddInt.h"

// 输入解析
#include "../interface/InputFileParse.h"

// 获取变量对应的BDD向量
#include "../interface/BDDVector.h"

#include <string>

// 逻辑与运算
void And(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD> &res, jsons::BiTNode* T);

// 逻辑或运算
void Or(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD> &res, jsons::BiTNode* T);

// 逻辑取反
void Neg(Cudd &mgr, const std::vector<BDD>& lhs, std::vector<BDD> &res, jsons::BiTNode* T);

// 逻辑蕴含
void Imply(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD> &res, jsons::BiTNode* T);
#endif //PROJECT_LOGICOPERATIONS_H
