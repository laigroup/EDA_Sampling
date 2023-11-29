#include "../interface/CalculateConstrains.h"
#include "ResFileParse.h"
#include "../interface/OutputToDot.h"
#include "../interface/OutPutToJson.h"
#include <fstream>
#include <random>
#include<ctime>

using namespace std;

vector<int> testBvc;

void ToBinary(size_t val, vector<int>& ans, size_t bitWidth){
    for (size_t i = 0U; i < bitWidth; ++ i){
        if (val & 1U){
            ans.push_back(1);
        }else{
            ans.push_back(0);
        }
        val >>= 1U;
    }
}

// count记录根到当前位置的补边个数
// 根据result文件的赋值遍历BDD
void traverseBdd(Cudd* mgr, DdNode* node, const vector<jsons::Var>& vars, int count, ofstream& testFile) {
    if (Cudd_IsComplement(node))
        count++;
//    testFile << "补边数为：" << count << endl;
    if (Cudd_IsConstant(node)) {
        if (count % 2 == 0){
            testFile << "到达终端节点，该路径可满足" << endl;
        }else{
            testFile << "到达终端节点，该路径不可满足" << endl;
        }
        return ;
    }
    unsigned int varIndex = Cudd_NodeReadIndex(node);

    DdNode* lowNode = Cudd_E(node);
    DdNode* highNode = Cudd_T(node);
    if (testBvc[varIndex] == 0) {
        traverseBdd(mgr, lowNode, vars, count, testFile);
    }else{
        traverseBdd(mgr, highNode, vars, count, testFile);
    }
}

// 获得某次指派对应的测试向量
void buildTestBvc(vector<jsons::Assignment> assignment, vector<jsons::Var> vars){
    for (size_t i = 0; i < assignment.size(); ++ i){ // 在某指派下为各变量赋值
        size_t value = Atoi(assignment[i].value, 16); // 十六进制转十进制
        vector<int> tmp;
        ToBinary(value, tmp, vars[i].bit_width); // 十进制转二进制
        testBvc.insert(testBvc.end(), tmp.begin(), tmp.end());
        tmp.clear();
    }
}

// 在所有指派下测试该BDD是否满足约束
void testConstraint(Cudd &mgr, const BDD& bdd, const vector<vector<jsons::Assignment>>& assignments, const vector<jsons::Var>& vars, ofstream& testFile){
    int assignment_num = 0;
    for (auto & assignment : assignments){ // 遍历所有指派组
        buildTestBvc(assignment, vars);
        testFile << "在第 " << assignment_num ++ << " 次指派下：" << endl;
        traverseBdd(&mgr, bdd.getNode(), vars, 0, testFile);
        testBvc.clear();
    }
}

// 获取合取后的约束结果，并在合取过程中检测单个约束是否可满足
BDD getConjunctiveRes(Cudd& mgr, const vector<jsons::BiTNode>& T, const vector<vector<BDD>>& varsBdd, const vector<jsons::Var>& vars, const string& constraintName){
    vector<BDD> res;
    BDD ans = mgr.bddOne();
    int constraint_num = 0; // 约束序号
//    ofstream testFile;
//    testFile.open("resTest/testSingleConstraint.txt", ios::app);
//    testFile << constraintName << "测试结果如下：" << endl;
    for (auto root : T){ // 合取所有约束
        res.clear();
        getConstrainBDD(mgr, &root, varsBdd, res); // 递归计算单个约束条件的BDD res
        BDD tmp = mgr.bddZero();
        for (const auto& bdd : res){ // 约束内各个变元析取
            tmp = tmp | bdd;
        }
//        // 测试单个约束是否可满足
//        testFile << "第" << constraint_num << "个约束：" << endl;
//        testConstraint(mgr, tmp, assignments, vars, testFile);

        ans = ans & tmp; // 约束间合取
        cout << "Successful conjunction: constraint " << constraint_num ++ << endl;
    }
//    testFile.close();
    return ans;
}

// 测试在该BDD中，给定指派assigment是否可满足
void isSatAssignment(Cudd& mgr, const vector<jsons::Assignment>& assignment, const vector<jsons::Var>& vars,
                     ofstream& testFile, int& assignment_num, const BDD& ans){
    buildTestBvc(assignment, vars); // 根据给定指派构建测试向量

    testFile << "在第 " << assignment_num ++ << " 次指派下：" << endl;
    traverseBdd(&mgr, ans.getNode(), vars, 0, testFile);
    testFile << endl;
    testBvc.clear();
}

// 测试给定指派集合是否全部可满足
void isResSat(Cudd &mgr, const vector<jsons::Var>& vars, const vector<jsons::BiTNode>& T,
              const vector<vector<BDD>>& varsBdd, const vector<vector<jsons::Assignment>>& assignments, const string& constraintName){

    // 获取约束合取后的BDD
    BDD ans = getConjunctiveRes(mgr, T, varsBdd, vars, constraintName);

    // 测试对最终得到的BDD ans，所有指派是否都可满足
    int assignment_num = 0;
    ofstream testFile;
    testFile.open("testResConstraint.txt", ios::trunc);
    testFile << constraintName << "测试结果如下：" << endl;
    for (auto & assignment : assignments){ // 遍历所有指派组
        isSatAssignment(mgr, assignment, vars, testFile, assignment_num, ans);
    }
    testFile.close();
}

//模板函数：将string类型变量转换为常用的数值类型（此方法具有普遍适用性）
template <class Type> Type stringToNum(const string& str) {
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;/
}

void print_usage(const char* name)
{
    cout<<"\nUsage: " << name << " [options]" << std::endl
        << "Options:" << std::endl
        << "  -input,   -i   constraint file path \n"
        << "  -output,  -o   result file path \n"
        << "  -seed,    -s   seed for generating random numbers \n"
        << "  -count,   -c   number of samples \n"
        << "  -help,    -h   print this help info \n"
        << "\n"
        << std::endl;
}

int main(int argc, char* argv[]){
    clock_t start = clock();

    string constraintFile; // 约束文件
    string outputFile; // 采样结果文件

    size_t seed = 0U;
    int sampleCnt = 1000;

    if (argc<2) // 未输入可选项，则输出帮助信息
    {
        print_usage(argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i ++) // 对可选项做判断
    {
        std::string t_arg = std::string(argv[i]);
        if (t_arg == "-h" || t_arg == "--help")
        {
            print_usage(argv[0]);
            return 0;
        }
        else if (t_arg == "-seed" || t_arg == "-s")
        {
            if( (i+1) < argc) // 读入参数未越界
            {
                seed = stringToNum<unsigned int>(argv[++i]); // 向后读一个参数
            }
            else
            {
                printf("Option -seed requires one argument \n");
                print_usage(argv[0]);
            }
        }
        else if (t_arg == "-input" || t_arg == "-i")
        {
            if( (i+1) < argc) // 读入参数未越界
            {
                constraintFile = argv[++i]; // 向后读一个参数
            }
            else
            {
                printf("Option -i requires one argument \n");
                print_usage(argv[0]);
            }
        }
        else if (t_arg == "-output" || t_arg == "-o")
        {
            if( (i+1) < argc) // 读入参数未越界
            {
                outputFile = argv[++i]; // 向后读一个参数
            }
            else
            {
                printf("Option -o requires one argument \n");
                print_usage(argv[0]);
            }
        }
        else if (t_arg == "-count" || t_arg == "-c")
        {
            if( (i+1) < argc) // 读入参数未越界
            {
                sampleCnt = stringToNum<int>(argv[++i]); // 向后读一个参数
            }
            else
            {
                printf("Option -count requires one argument \n");
                print_usage(argv[0]);
            }
        }
        else
        {
            printf("Option %s is not available \n", argv[i] );
        }
    }

    Cudd mgr;

    vector<jsons::Var> vars; // 变量列表
    vector<jsons::BiTNode> T; // 约束树
    inputParse(vars, T, constraintFile); // 读取约束文件构建约束树和变量数组

    // 创建所有变量对应的BDD向量
    vector<vector<BDD>> varsBdd;
    creatVarsBvc(mgr, vars, varsBdd);

//    // 解析给定的指派文件
//    vector<vector<jsons::Assignment>> assignments;
//    resParse(assignments, vars.size(), outputFile);

    // 计算变量的总位宽
    for (const auto& var : vars){
        sumWidth += var.bit_width;
    }
//    cout << "sumWidth:" << sumWidth << endl;

    // 选择最优排序策略 TODO
    Cudd_AutodynEnable(mgr.getManager(), CUDD_REORDER_SIFT);

//    // 测试文件中的赋值对最终得到的BDD是否可满足
//    cout << "测试文件：" << constraintFile << endl;
//    isResSat(mgr, vars, T, varsBdd, assignments, constraintFile);
//    cout << "------------------------------------测试完毕---------------------------------------------" << endl;

    // 统计各节点的可满足路径数，及各个节点走左分支的概率
    BDD ans = getConjunctiveRes(mgr, T, varsBdd, vars, constraintFile);
    cout << endl;

//    writeDotFile(mgr, ans, "");

    // 将采样结果输出到json文件
    toJsonFormat(mgr, ans, vars, sampleCnt, seed, outputFile);
    clock_t end = clock();

    ofstream logFile;
    logFile.open("./log_file.txt", ios::app);
    logFile << "constraint file: " << constraintFile << endl;
    logFile << "node num: " << Cudd_DagSize(ans.getNode()) << endl;
    logFile << "paths to nonZero num: " << Cudd_CountPathsToNonZero(ans.getNode()) << endl;
    logFile << "running time: " << (double)(end-start)/CLOCKS_PER_SEC << endl;
    logFile << endl;
    logFile.close();

    return 0;
}



