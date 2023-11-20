#include <OutputToDot.h>
#include "../interface/ArithmeticOperations.h"
using namespace std;

/*
 * 算数运算的结果为多位的位向量
 * 当位向量全零时，判定当前运算结果为假
 * 否则，判定当前运算结果为真
 * */
void bvc_Add(Cudd &mgr, vector<BDD> const& lhs, vector<BDD> const& rhs, vector<BDD>& res, jsons::BiTNode* T){

    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;
    size_t res_bitNum = T->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    buildBvc(mgr, res, res_bitNum, false);

    BDD comp = mgr.bddZero();
    for (size_t i = 0; i < res_bitNum; i ++){
        res[i] = ((lExp[i] ^ rExp[i]) ^ comp);
        comp = (lExp[i] & rExp[i]) | (comp & (lExp[i] | rExp[i]));
    }
}

void bvc_Sub(Cudd &mgr, vector<BDD> const& lhs, vector<BDD> const& rhs, vector<BDD>& res, jsons::BiTNode* T){

    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;
    size_t res_bitNum = T->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    buildBvc(mgr, res, res_bitNum, false);

    BDD comp = mgr.bddZero();
    for (size_t i = 0; i < res_bitNum; i ++){
        res[i] = ((lExp[i] ^ rExp[i]) ^ comp);
        comp = (lExp[i] & rExp[i] & comp) | (~lExp[i] & (rExp[i] | comp));
    }
}

// TODO 未测试正确性
void bvc_Minus(Cudd &mgr, vector<BDD> const& lhs, vector<BDD>& res, jsons::BiTNode* T){

    size_t lhs_bitNum = T->lhs->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> tmp;
    tmp.reserve(lExp.size());
    for (const auto & lh : lExp){
        tmp.push_back(~lh);
    }
    BDD comp = mgr.bddOne();
    for (size_t i = 0U; i < lExp.size(); i ++){
        res.push_back(tmp[i] ^ comp);
        comp = comp & tmp[i];
    }
}

void bvc_Mul(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T) {

    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;
    size_t res_bitNum = T->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    buildBvc(mgr, res, res_bitNum, false);

    vector<BDD> leftShift;
    unifyBitWidth(mgr, lhs, leftShift, res_bitNum);

    bool isVar = false;
    for (const auto& rh : rhs){
        if (rh.IsVar()){
            isVar = true;
            break;
        }
    }

    if (isVar){ // 乘数为变量
        vector<BDD> added;
        for (const auto & rh : rhs){
            added.clear();
            utilAdd(mgr, res, leftShift, added); // 记录左移之后的中间和
            for (size_t j = 0U; j < res_bitNum; ++ j){
                res[j] = rh.Ite(added[j], res[j]);
            }
            for (size_t j = res_bitNum - 1U; j >= 1U; -- j){ // 左移一位
                leftShift[j] = leftShift[j - 1];
            }
            leftShift[0] = mgr.bddZero(); // 右补零
        }

    }else{ // 乘数为常量
        vector<BDD> tmpSum;
        buildBvc(mgr, tmpSum, res_bitNum, false);

        for (size_t i = 0U; i < rhs.size(); i++){
            if (!rhs[i].IsZero()){
                vector<BDD> tmp = tmpSum; // 记录中间和
                tmpSum.clear();
                utilAdd(mgr, leftShift, tmp, tmpSum);
            }
            utilShl(mgr, leftShift, 1U);
            res[i] = tmpSum[i];
        }
        for (size_t i = rhs.size(); i < res_bitNum; ++ i)
            res[i] = tmpSum[i];
    }
}

// 除数不能为0
// TODO 可能要在计算sumNum前，先扩展左右位宽
void bvc_Div(Cudd &mgr, vector<BDD> const&  lhs, vector<BDD> const& rhs, vector<BDD>& res, jsons::BiTNode* T){

    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;
    size_t res_bitNum = T->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    buildBvc(mgr, res, res_bitNum, false);

    size_t sumNum = lhs_bitNum + rhs_bitNum;

    vector<BDD> rem; // 被除数向量
    unifyBitWidth(mgr, lExp, rem, sumNum);

    vector<BDD> div; // 除数向量
    unifyBitWidth(mgr, rExp, div, sumNum);

    utilShl(mgr, div, lExp.size()); // 左移除数

    for (size_t i = 0U; i < res_bitNum + 1; ++ i){
        vector<BDD> divLteRem; // 返回值只有一位
        utilLthOrEqu(mgr, div, rem, divLteRem); // 除数小于等于被除数
        vector<BDD> remSubDiv;
        utilSub(mgr, rem, div, remSubDiv); // 余数减被除数

        for (size_t j = 0U; j < sumNum; ++ j) // 求新的余数
            rem[j] = divLteRem[0].Ite(remSubDiv[j], rem[j]);

        if (i > 0){
            res[res_bitNum - i] = divLteRem[0]; // 求商
        }

        for (size_t j = 0U; j < sumNum - 1; ++ j) // 除数右移一位
            div[j] = div[j + 1];

        div[sumNum - 1] = mgr.bddZero(); // 左补零
    }
}

