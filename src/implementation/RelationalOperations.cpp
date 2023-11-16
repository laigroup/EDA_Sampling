#include "../interface/RelationalOperations.h"

using namespace std;

/*
 * 关系运算的结果为单比特向量
 * 当比特为零时，结果为假
 * 否则，结果为真
 * */

// 等于
void Equ(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    vector<BDD> tmp;
    for (size_t i = 0; i < lhs_bitNum; ++ i){
        if (i == 0){
            tmp.push_back(lExp[i].Xnor(rExp[i]));
        }else{
            tmp.push_back(tmp[i-1] & lExp[i].Xnor(rExp[i]));
        }
    }
    res.push_back(tmp[lhs_bitNum-1]);
}

// 不等
void Nequ(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    vector<BDD> tmp;
    for (size_t i = 0; i < lhs_bitNum; ++ i){
        if (i == 0){
            tmp.push_back(lExp[i] ^ rExp[i]);
        }else{
            tmp.push_back(tmp[i-1] | (lExp[i] ^ rExp[i]));
        }
    }
    res.push_back(tmp[lhs_bitNum - 1]);
}

// 小于
void LessThan(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    vector<BDD> tmp;
    for (size_t i = 0; i < lhs_bitNum; ++ i){
        if (i == 0){
            tmp.push_back(((!lExp[i]) & rExp[i]));
        }else{
            tmp.push_back(((!lExp[i]) & rExp[i]) | (lExp[i].Xnor(rExp[i]) & tmp[i-1]));
        }
    }
    res.push_back(tmp[lhs_bitNum - 1]);
}

// 大于
void GreaterThan(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T){
    LessThan(mgr, rhs, lhs, res, T);
}

// 小于等于
void LthOrEqu(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    vector<BDD> tmp;
    for (size_t i = 0; i < lhs_bitNum; ++ i){
        if (i == 0){
            tmp.push_back(((!lExp[i]) & rExp[i]) | (lExp[i].Xnor(rExp[i])));
        }else{
            tmp.push_back(((!lExp[i]) & rExp[i]) | (lExp[i].Xnor(rExp[i]) & tmp[i-1]));
        }
    }
    res.push_back(tmp[lhs_bitNum-1]);
}

// 大于等于
void GthOrEqu(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T) {
    LthOrEqu(mgr, rhs, lhs, res, T);
}