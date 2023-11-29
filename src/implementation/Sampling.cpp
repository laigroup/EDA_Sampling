#include "../interface/Sampling.h"
#include <unordered_set>

using nlohmann::json;
using namespace std;

unordered_map<NodeLabel, mpz_class, hash_fun_map, mycmp> sumToOnePath; // 通过节点的总可满足路径数
unordered_map<NodeLabel, mpz_class, hash_fun_map, mycmp> lowToOnePath; // 通过节点左分支的可满足路径数
unordered_map<NodeLabel, mpz_class, hash_fun_map, mycmp> highToOnePath; // 通过节点左分支的可满足路径数

unordered_map<NodeLabel, mpz_class, hash_fun_map, mycmp> sumToZeroPath;
unordered_map<NodeLabel, mpz_class, hash_fun_map, mycmp> lowToZeroPath;
unordered_map<NodeLabel, mpz_class, hash_fun_map, mycmp> highToZeroPath;

vector<int> satPath; // 采样之后各个变量的取值
unordered_set<NodeLabel, hash_fun_set, mycmp> visited;

int nodeNum = 0;

std::ostream& operator << (std::ostream& os, const NodeLabel& node){
    return os << "(" << node.getVarIdx() << "," << node.getLeftNode() << ","
    << node.getRightNode() << ")";
}

/*
node指向某BDD变量的指针
vars存储变量对象
count存储从根到当前节点的补边个数
testFile为输出文件
函数返回经过根节点的可满足的路径数
*/
mpz_class countPath(Cudd* mgr, DdNode* node, bool toOne){

    if (Cudd_IsConstant(node)){ // 到达叶子结点
        if (toOne){ // 若要计算到1的路径数
            return 1;
        }else{
            return 0;
        }
    }

    size_t varIndex = Cudd_NodeReadIndex(node); // 获取当前节点的索引
    DdNode* lowNode = Cudd_E(node);
    DdNode* highNode = Cudd_T(node);
    auto nodeLabel = NodeLabel(varIndex, lowNode, highNode);

    if (visited.find(nodeLabel) != visited.end()){
        if (toOne){
            return sumToOnePath[nodeLabel];
        }else{
            return sumToZeroPath[nodeLabel];
        }
    }

    lowToOnePath[nodeLabel] = countPath(mgr, lowNode, true);
    lowToZeroPath[nodeLabel] = countPath(mgr, lowNode, false);
    highToOnePath[nodeLabel] = countPath(mgr, highNode, true); // 右侧结点到1的路径数
    highToZeroPath[nodeLabel] = countPath(mgr, highNode, false); // 右侧结点到0的路径数

    int level = Cudd_ReadPerm(mgr->getManager(), varIndex);

    if (Cudd_IsConstant(lowNode)){ // 还原归约后隐藏的边
        if (level != sumWidth){
            unsigned int m = sumWidth - level;
//            cout << "所在层：" << level << endl;
//            cout << "变量号：" << varIndex << endl;
//            cout << "左侧隐藏变量数：" << m << endl;
            mpz_class sum = 1;
            for (int i = 0; i < m; ++ i){
                sum *= 2;
            }

            lowToOnePath[nodeLabel] = sum;
//            cout << "左侧隐藏路径数：" << sum << endl;
//            cout << endl;
        }
    }

    if (Cudd_IsConstant(highNode)){
        int level = Cudd_ReadInvPerm(mgr->getManager(), varIndex);
        if (level != sumWidth){
            unsigned int m = sumWidth - level;

//            cout << "所在层：" << level << endl;
//            cout << "变量号：" << varIndex << endl;
//            cout << "右侧隐藏变量数：" << m << endl;

            mpz_class sum = 1;
            for (int i = 0; i < m; ++ i){
                sum *= 2;
            }
            highToOnePath[nodeLabel] = sum;
//            cout << "右侧隐藏路径数：" << sum << endl;
//            cout << endl;
        }
    }

    if (Cudd_IsComplement(lowNode)){ // 左分支为补边，则令左分支取反
        sumToOnePath[nodeLabel] = lowToZeroPath[nodeLabel] + highToOnePath[nodeLabel];
        sumToZeroPath[nodeLabel] = lowToOnePath[nodeLabel] + highToZeroPath[nodeLabel];
    }else{
        sumToOnePath[nodeLabel] = lowToOnePath[nodeLabel] + highToOnePath[nodeLabel];
        sumToZeroPath[nodeLabel] = lowToZeroPath[nodeLabel] + highToZeroPath[nodeLabel];
    }

    visited.insert(nodeLabel);

//    cout << ++ nodeNum << endl;
    if (toOne){
        return sumToOnePath[nodeLabel];
    }else{
        return sumToZeroPath[nodeLabel];
    }
}

// 随机采样
void simpling(Cudd* mgr, DdNode* node, std::mt19937 &gen, std::uniform_real_distribution<> &dis, int count) {
    if (Cudd_IsComplement(node)){
        count ++;
    }
    if (Cudd_IsConstant(node))
        return ;

    unsigned int varIndex = Cudd_NodeReadIndex(node);
    DdNode* lowNode = Cudd_E(node);
    DdNode* highNode = Cudd_T(node);
    auto nodeLabel = NodeLabel(varIndex, lowNode, highNode);

    mpf_class prob;
    if (count % 2 == 0){ // 到当前节点的补边数为偶数，则向下找toOne的路径
        prob = (mpf_class)highToOnePath[nodeLabel] / (mpf_class)sumToOnePath[nodeLabel];
    }else{
        prob = (mpf_class)highToZeroPath[nodeLabel] / (mpf_class)sumToZeroPath[nodeLabel];
    }

    if (dis(gen) < prob){
        satPath[varIndex] = 1;
        simpling(mgr, highNode, gen, dis, count);
    }else{
        satPath[varIndex] = 0;
        simpling(mgr, lowNode, gen, dis, count);
    }
}

