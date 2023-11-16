#include <../interface/BitOperations.h>

using namespace std;

/*
 * 位运算的结果为多位的位向量
 * 当位向量全零时，判定当前运算结果为假
 * 否则，判定当前运算结果为真
 * */
// 按位与
void bvc_And(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;
    size_t res_bitNum = T->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    buildBvc(mgr, res, res_bitNum, false);

    for (size_t i = 0; i < res_bitNum; i ++)
        res[i] = lExp[i] & rExp[i];
}

// 按位或
void bvc_Or(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;
    size_t res_bitNum = T->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    buildBvc(mgr, res, res_bitNum, false);

    for (size_t i = 0; i < res_bitNum; i ++){
        res[i] = lExp[i] | rExp[i];
    }
}

// 按位异或
void bvc_Xor(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;
    size_t res_bitNum = T->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    buildBvc(mgr, res, res_bitNum, false);

    for (size_t i = 0; i < res_bitNum; i ++){
        res[i] = (lExp[i] ^ rExp[i]);
    }
}

// 按位取反
void bvc_Neg(Cudd &mgr, const vector<BDD>& lhs, vector<BDD>& res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t res_bitNum = T->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    buildBvc(mgr, res, res_bitNum, false);

    for (int i = 0; i < res_bitNum; ++ i){
        res[i] = ~lExp[i];
    }
}

// 逻辑右移操作
void bvc_RShift(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;
    size_t res_bitNum = T->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    buildBvc(mgr, res, res_bitNum, false);

    if (rhs[0].IsVar()){
        BDD tmp1, rEquN;
        for (size_t i = 0U; i <= lExp.size(); ++ i){

            // 获取常量i对应的BDD
            vector<BDD> val;
            getConstBvc(mgr, rExp.size(), i, val);

            vector<BDD> tmp;
            utilEqu(mgr, rExp, val, tmp);
            rEquN = tmp[0];

            for (size_t j = 0U; j < lExp.size(); ++ j){
                if (j + i < lExp.size())
                    tmp1 = rEquN & lExp[j + i];
                else
                    tmp1 = rEquN & mgr.bddZero();
                res[j] = res[j] | tmp1;
            }
        }

        vector<BDD> val;
        getConstBvc(mgr, rExp.size(), lExp.size(), val);

        vector<BDD> tmp;
        utilGreaterThan(mgr, rExp, val, tmp);
        rEquN = tmp[0];

        tmp1 = rEquN & mgr.bddZero();

        for (size_t i = 0U; i < lExp.size(); ++ i){
            res[i] = res[i] | tmp1;
        }
    }else{
        unsigned int pos = 0;
        unsigned int tmp = 1;
        for (auto & rh : rExp){ // 计算偏移量
            if (!rh.IsZero()){
                pos += tmp;
            }
            tmp *= 2;
        }

        size_t maxNum = lExp.size() - pos;
        for (size_t i = 0U; i < maxNum; i ++){
            res[i] = lExp[i + pos];
        }
    }

}

// 逻辑左移操作
void bvc_LShift(Cudd &mgr, const vector<BDD>& lhs, const vector<BDD>& rhs, vector<BDD>& res, jsons::BiTNode* T){
    size_t lhs_bitNum = T->lhs->bit_width;
    size_t rhs_bitNum = T->rhs->bit_width;
    size_t res_bitNum = T->bit_width;

    vector<BDD> lExp;
    unifyBitWidth(mgr, lhs, lExp, lhs_bitNum);

    vector<BDD> rExp;
    unifyBitWidth(mgr, rhs, rExp, rhs_bitNum);

    buildBvc(mgr, res, res_bitNum, false);

    if (rhs[0].IsVar()){
        for (size_t i = 0U; i<= lExp.size(); ++ i){
            // 获取常量i对应的BDD
            vector<BDD> val;
            getConstBvc(mgr, rExp.size(), i, val);

            vector<BDD> tmp;
            utilEqu(mgr, rExp, val, tmp);
            BDD rEquN = tmp[0];

            for (size_t j = 0U; j < lExp.size(); ++ j){
                if (j >= i){
                    res[j] |= rEquN & lExp[j - i];
                }else{
                    res[j] |= rEquN & mgr.bddZero();
                }
            }
        }

        vector<BDD> val;
        getConstBvc(mgr, rExp.size(), lExp.size(), val);

        vector<BDD> tmp;
        utilGreaterThan(mgr, rExp, val, tmp);
        BDD rEquN = tmp[0];

        for (size_t i = 0U; i < lExp.size(); ++ i){
            res[i] |= (rEquN & mgr.bddZero());
        }
    }else{
        unsigned int pos = 0;
        unsigned int tmp = 1;
        for (auto & rh : rExp){ // 计算偏移量
            if (!rh.IsZero()){
                pos += tmp;
            }
            tmp *= 2;
        }

        // size_t minPos = (lhs.size() < pos) ? lhs.size() : pos;
        for (size_t i = pos; i < lExp.size(); ++ i){
            res[i] = lExp[i - pos];
        }
    }
}

