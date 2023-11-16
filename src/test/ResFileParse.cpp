#include "ResFileParse.h"

using nlohmann::json;
using namespace std;

namespace jsons{
    void from_json(const json& j, Assignment &assignment){
        j.at("value").get_to(assignment.value);
    }
}

void resParse(vector<vector<jsons::Assignment>> &ass, size_t varNum, const string& path){
    json j;
    fstream jfile(path);
    if (!jfile.is_open()){
        cout << "Fail to open file!" << endl;
        return ;
    }
    jfile >> j;

    // 读指派列表
    cout << "打印指派列表：" << endl;
    size_t assigments_size = j["assignment_list"].size();
    cout << "assigment_list_size = " << assigments_size << endl;
    for (int i = 0; i < assigments_size; ++ i){
        vector<jsons::Assignment> tmp;
        tmp.reserve(varNum);
        for (int k = 0; k < varNum; ++ k){
            tmp.push_back(j["assignment_list"][i][k]);
        }
        ass.push_back(tmp);
        cout << "第" << i << "组指派：";
        for (const auto& as : tmp)
            cout << as.value.c_str() << " ";
        cout << endl;
        tmp.clear();
    }
}