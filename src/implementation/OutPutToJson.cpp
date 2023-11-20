#include "../interface/InputFileParse.h"
#include "../interface/OutPutToJson.h"
#include "../interface/OutputToDot.h"
#include "../interface/Sampling.h"
#include <fstream>
#include <string>

using nlohmann::json;
using namespace std;

// 十进制转其他进制
string intToA(size_t n,int radix)
{
    string ans;
    do{
        size_t t = n % radix;
        if(t >= 0 && t <= 9) ans += (t + '0');
        else ans += (t - 10 + 'a');
        n /= radix;
    }while(n != 0);  //使用do{}while()以防止输入为0的情况
    reverse(ans.begin(),ans.end());
    return ans;
}


void bvcToNum(const vector<jsons::Var>& vars, vector<string>& res){
    size_t pos = 0;
    for (const auto & var : vars){
        // 二进制转十进制
        size_t j = 0, k = pos; // j用于给变量赋值，k用于遍历satPath
        size_t sum = 0, power = 1;
        while (j < var.bit_width){
            sum += satPath[k++] * power;
            power *= 2;
            j ++;
        }
        pos = k;
        // 十进制转十六进制
        res.push_back(intToA(sum, 16));
    }
}

// num为采样数
void toJsonFormat(Cudd& mgr, BDD& ans, const vector<jsons::Var>& vars, int num, unsigned int seed, const string& outputFile){
    // 计算各个节点的模型数
    mpz_class modelCnt = countPath(&mgr, ans.getNode(), false);

//    cout << "模型数：" << modelCnt << endl;
//    cout << Cudd_CountPathsToNonZero(ans.getNode()) << endl;
//    cout << "结点个数：" << Cudd_DagSize(ans.getNode()) << endl;

    // 进行采样并且将结果存入json对象
    json j;
    j["assignment_list"] = json::array();

//    std::random_device rd; // 用于获得随机数引擎的种子
    std::mt19937 gen(seed); // seed播种的标准
    std::uniform_real_distribution<> dis(0, 1);

    vector<string> res;

    for (size_t i = 0; i < num; ++ i){
        // 初始化satPath
        satPath.clear();
        satPath.resize(sumWidth);

        // 随机初始化bit取值
        for (size_t k = 0; k < sumWidth; ++ k){
            if (dis(gen) > 0.5){
                satPath[k] = 1;
            }else{
                satPath[k] = 0;
            }
        }

        // 随机采样
        simpling(&mgr, ans.getNode(), gen, dis, 0);

        // 路径向量分解为变量的16进制取值
        bvcToNum(vars, res);

        for (auto & val : res){
            json j_sub;
            j_sub["value"] = val;
            j["assignment_list"][i].push_back(j_sub);
        }
        // 重置res
        res.clear();
    }

    ofstream sampleFile;
    sampleFile.open(outputFile, ios::trunc);
    if (!sampleFile){
        cout << "outputFile is not open! " << endl;
        return ;
    }
    sampleFile << std::setw(2) << j;
    sampleFile.close();
}
