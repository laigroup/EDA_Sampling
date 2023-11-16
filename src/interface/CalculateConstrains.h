//
// Created by win on 2023/9/27.
//

#ifndef PROJECT_CALCULATECONSTRAINS_H
#define PROJECT_CALCULATECONSTRAINS_H
// CUDD
#include "../../lib/cudd-3.0.0/cplusplus/cuddObj.hh"
#include "../../lib/cudd-3.0.0/cudd/cuddInt.h"

// 输入解析
#include "../interface/InputFileParse.h"

// 变量操作
#include "../interface/ArithmeticOperations.h"
#include "../interface/BitOperations.h"
#include "../interface/LogicOperations.h"
#include "../interface/RelationalOperations.h"

// 后序遍历计算约束树
// T为树形约束的根节点，vars为变量列表，res为存储结果的BDD
void getConstrainBDD(Cudd &mgr, jsons::BiTNode* T, const std::vector<std::vector<BDD>>& varsBdd, std::vector<BDD> &res);
#endif //PROJECT_CALCULATECONSTRAINS_H
