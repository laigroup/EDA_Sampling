#include <OutputToDot.h>
#include "../interface/LogicOperations.h"

using namespace std;
/*
 * 逻辑运算的结果为单比特向量
 * 当比特为零时，结果为假
 * 否则，结果为真
 * */
// 逻辑与
void And(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD> &res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    vector<BDD> tmp1;
    vector<BDD> tmp2;
    // 检测var1是否为真
    for (size_t i = 0; i < lExp.size(); i ++){
        if (i != 0)
            tmp1.push_back(tmp1[i-1] | lExp[i]);
        else
            tmp1.push_back(lExp[i]);
    }

    // 检测var2是否为真
    for (size_t i = 0; i < rExp.size(); i ++){
        if (i != 0)
            tmp2.push_back(tmp2[i-1] | rExp[i]);
        else
            tmp2.push_back(rExp[i]);
    }
    res.push_back(tmp1[lExp.size()-1] * tmp2[rExp.size()-1]);
}

// 逻辑或
void Or(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD> &res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    vector<BDD> tmp1;
    vector<BDD> tmp2;
    // 检测var1是否为真
    for (size_t i = 0; i < lExp.size(); i ++){
        if (i != 0)
            tmp1.push_back(tmp1[i-1] | lExp[i]);
        else
            tmp1.push_back(lExp[i]);
    }

    // 检测var2是否为真
    for (size_t i = 0; i < rhs.size(); i ++){
        if (i != 0)
            tmp2.push_back(tmp2[i-1] | rExp[i]);
        else
            tmp2.push_back(rExp[i]);
    }

    res.push_back(tmp1[lExp.size()-1] | tmp2[rExp.size()-1]);
}

// 逻辑取反
// 这里需要注意，一定是先算出取反前的逻辑值，再取反
void Neg(Cudd &mgr, const vector<BDD>& lhs, vector<BDD> &res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    BDD tmp = mgr.bddZero();
    for (const auto& bdd : lExp){
        tmp = tmp | bdd;
    }
    res.push_back(~tmp);
}

// 逻辑蕴含
void Imply(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD> &res, jsons::BiTNode* T){
    vector<BDD> neg_lhs;
    utilLogNeg(mgr, lhs, neg_lhs);
    utilLogOr(mgr, neg_lhs, rhs, res);
}