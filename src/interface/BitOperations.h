#ifndef PROJECT_BITOPERATIONS_H
#define PROJECT_BITOPERATIONS_H
// CUDD
#include "../../lib/cudd-3.0.0/cplusplus/cuddObj.hh"
#include "../../lib/cudd-3.0.0/cudd/cuddInt.h"

// 输入解析
#include "../interface/InputFileParse.h"

// 获取变量对应的BDD向量
#include "../interface/BDDVector.h"

// 按位与
void bvc_And(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 按位或
void bvc_Or(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 按位异或
void bvc_Xor(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 按位取反
void bvc_Neg(Cudd &mgr, const std::vector<BDD>& lhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 逻辑右移操作
void bvc_RShift(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
// 逻辑左移操作
void bvc_LShift(Cudd &mgr, const std::vector<BDD>& lhs, const std::vector<BDD>& rhs, std::vector<BDD>& res, jsons::BiTNode* T);
#endif //PROJECT_BITOPERATIONS_H
