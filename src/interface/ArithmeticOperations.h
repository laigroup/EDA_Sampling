#ifndef PROJECT_ARITHMETICOPERATIONS_H
#define PROJECT_ARITHMETICOPERATIONS_H

// CUDD
#include "../../lib/cudd-3.0.0/cplusplus/cuddObj.hh"
#include "../../lib/cudd-3.0.0/cudd/cuddInt.h"

// 输入解析
#include "../interface/InputFileParse.h"

#include "../interface/Util.h"
// 引入移位操作
#include "../interface/BitOperations.h"
// 引入关系操作
#include "../interface/RelationalOperations.h"

// 加法操作
void bvc_Add(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 减法操作
void bvc_Sub(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 乘法操作
void bvc_Mul(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD> &rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 负号
void bvc_Minus(Cudd &mgr, const std::vector<BDD>& lhs, std::vector<BDD>& res, jsons::BiTNode* T);

void bvc_Div(Cudd &mgr, std::vector<BDD> const&  lhs, std::vector<BDD> const& rhs, std::vector<BDD>& res, jsons::BiTNode* T);

// 获取变量对应的BDD向量
#include "../interface/BDDVector.h"

#include <algorithm>
#endif //PROJECT_ARITHMETICOPERATIONS_H
