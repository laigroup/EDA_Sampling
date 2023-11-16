#include "../interface/BDDVector.h"
#include <algorithm>
using namespace std;

unsigned int sumWidth = 0;

void Reverse(string &s, int n) {
    for (int i = 0, j = n - 1; i < j; i++, j--) {
        char c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

size_t Atoi(const string& str, int radix){
    long long ans = 0;
    for (char t : str) {
        if (t >= 0 && t <= '9') ans = ans * radix + t - '0';
        else ans = ans * radix + t - 'a' + 10;
    }
    return ans;
}

void creatVarsBvc(Cudd &mgr, const vector<jsons::Var>& vars, vector<vector<BDD>>& varsBdd){
    for (const auto & var : vars){
        vector<BDD> tmp;
        tmp.reserve(var.bit_width);
        for (int i = 0; i < var.bit_width; ++ i){
            tmp.push_back(mgr.bddVar());
        }
        varsBdd.push_back(tmp);
    }
}
void getBDDVector(Cudd &mgr, jsons::BiTNode* terminalNode, const vector<vector<BDD>>& varsBdd, vector<BDD>& var){

    // 若传入值为变量，则创建BDD向量
    if (terminalNode->op == "VAR"){
        var = varsBdd[terminalNode->id];
    }

    // 若传入值为常量，则创建该常量对应的BDD
    if (terminalNode->op == "CONST"){
        string tmp_width;
        string tmp_value;
        bool flag = false;

        for (auto c : terminalNode->value){
            if (c == '\'' || c == 'h'){
                flag = true;
                continue;
            }
            if (!flag)
                tmp_width += c;
            else{
                tmp_value += c;
            }
        }
        // 获得常量位宽
        int bit_width = stoi(tmp_width);
        // 十六进制转十进制
        size_t value = Atoi(tmp_value, 16);
//        cout << "常量的十进制值：" << value << endl;
        if (value < 0){
            cout << "can not use getConBDD for negative values" << endl;
            return ;
        }
        // 获得常量对应的位向量
        for (size_t i = 0U; i < bit_width; ++ i){
            if (value & 1U){
                var.push_back(mgr.bddOne());
            }else{
                var.push_back(mgr.bddZero());
            }
            value >>= 1U;
        }
    }
}

void getFalseBvc(Cudd& mgr, vector<BDD>& var, size_t bitNum){
    for (size_t i = 0U; i < bitNum; ++ i)
        var.push_back(mgr.bddZero());
}

void getTrueBvc(Cudd& mgr, vector<BDD>& var, size_t bitNum){
    for (size_t i = 0U; i < bitNum; ++ i)
        var.push_back(mgr.bddOne());
}

void buildBvc(Cudd& mgr, vector<BDD>& var, size_t bitNum, bool isTrue){
    if (isTrue)
        getTrueBvc(mgr, var, bitNum);
    else
        getFalseBvc(mgr, var, bitNum);
}

void unifyBitWidth(Cudd& mgr, const vector<BDD> & var, vector<BDD> &res, size_t bitNum){
    buildBvc(mgr, res, bitNum, false); // 默认零扩展
    for (size_t i = 0U; i < var.size(); ++ i){
        res[i] = var[i];
    }
}

void getConstBvc(Cudd& mgr, size_t bitNum, size_t val, vector<BDD>& res){
    if (val < 0){
        throw std::logic_error("use getConstBvc for negative value");
    }
    for (size_t i = 0U; i < bitNum; ++ i){
        if (val & 1U)
            res.push_back(mgr.bddOne());
        else
            res.push_back(mgr.bddZero());
        val >>= 1U;
    }
}