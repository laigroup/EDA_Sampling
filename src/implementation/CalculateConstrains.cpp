#include <OutputToDot.h>
#include "../interface/CalculateConstrains.h"
using namespace std;

// 后序遍历进行运算
// T为树形约束的根节点，vars为变量列表，res为存储结果的BDD
void getConstrainBDD(Cudd &mgr, jsons::BiTNode* T, const vector<vector<BDD>>& varsBdd, vector<BDD> &res){
    if (T == nullptr) return ;
    else if (T->op == "VAR" || T->op == "CONST"){
        getBDDVector(mgr, T, varsBdd, res);
    }else {
        vector<BDD> lhs;
        vector<BDD> rhs;
        getConstrainBDD(mgr, T->lhs, varsBdd, lhs); // 计算左侧子式
        getConstrainBDD(mgr, T->rhs, varsBdd, rhs); // 计算右侧子式
        if (T->op == "LOG_AND")
            And(mgr, lhs, rhs, res, T);
        else if (T->op == "LOG_OR")
            Or(mgr, lhs, rhs, res, T);
        else if (T->op == "LOG_NEG")
            Neg(mgr, lhs, res, T);
        else if (T->op == "IMPLY")
            Imply(mgr, lhs, rhs, res, T);
        else if (T->op == "ADD")
            bvc_Add(mgr, lhs, rhs, res, T);
        else if (T->op == "SUB")
            bvc_Sub(mgr, lhs, rhs, res, T);
        else if (T->op == "MINUS")
            bvc_Minus(mgr, lhs, res, T);
        else if (T->op == "MUL")
            bvc_Mul(mgr, lhs, rhs, res, T);
        else if (T->op == "DIV")
            bvc_Div(mgr, lhs, rhs, res, T);
        else if (T->op == "BIT_AND")
            bvc_And(mgr, lhs, rhs, res, T);
        else if (T->op == "BIT_OR")
            bvc_Or(mgr, lhs, rhs, res, T);
        else if (T->op == "BIT_XOR")
            bvc_Xor(mgr, lhs, rhs, res, T);
        else if (T->op == "BIT_NEG")
            bvc_Neg(mgr, lhs, res, T);
        else if (T->op == "RSHIFT")
            bvc_RShift(mgr, lhs, rhs, res, T);
        else if (T->op == "LSHIFT")
            bvc_LShift(mgr, lhs, rhs, res, T);
        else if (T->op == "EQ")
            Equ(mgr, lhs, rhs, res, T);
        else if (T->op == "NEQ")
            Nequ(mgr, lhs, rhs, res, T);
        else if (T->op == "LT")
            LessThan(mgr, lhs, rhs, res, T);
        else if (T->op == "GT")
            GreaterThan(mgr, lhs, rhs, res, T);
        else if (T->op == "LTE")
            LthOrEqu(mgr, lhs, rhs, res, T);
        else if (T->op == "GTE")
            GthOrEqu(mgr, lhs, rhs, res, T);
    }
}