//
// Created by win on 2023/11/4.
//

#ifndef PROJECT_SAMPLING_H
#define PROJECT_SAMPLING_H

// CUDD
#include "../../lib/cudd-3.0.0/cplusplus/cuddObj.hh"
#include "../../lib/cudd-3.0.0/cudd/cuddInt.h"

#include "../interface/InputFileParse.h"
#include "../interface/BDDVector.h"

#include <random>
#include <fstream>
#include <ostream>
#include <gmp-6.3.0/gmpxx.h>

class NodeLabel{
private:
    unsigned int varIdx;
    DdNode * left;
    DdNode * right;

public:
    NodeLabel(unsigned int idx, DdNode* l, DdNode* r) : varIdx(idx), left(l), right(r){}

    unsigned int getVarIdx() const {
        return varIdx;
    }

    DdNode* getLeftNode() const {
        return left;
    }

    DdNode* getRightNode() const {
        return right;
    }


    bool operator < (const NodeLabel& node) const {
        if (this->varIdx < node.varIdx){
            return true;
        }else if (this->varIdx == node.varIdx){
            if (this->left < node.left){
                return true;
            }else if (this->left == node.left){
                if (this->right < node.right){
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }

    bool operator == (const NodeLabel& label) const {
        return (this->varIdx == label.varIdx) && (this->left == label.left)
        && (this->right == label.right);
    }
};

class hash_fun_set{
public:
    unsigned int operator() (const NodeLabel& label) const {
        return label.getVarIdx();
    }
};

class hash_fun_map{
public:
    size_t operator() (const NodeLabel& label) const {
        using std::hash;
        return ((hash<unsigned int>()(label.getVarIdx())
               ^ (hash<DdNode*>()(label.getLeftNode()) << 1)) >> 1)
                       ^ (hash<DdNode*>()(label.getRightNode()) << 1);
    }
};

class mycmp{
public:
    bool operator() (const NodeLabel& label1, const NodeLabel& label2) const {
        return (label1.getVarIdx() == label2.getVarIdx()) && (label1.getLeftNode() == label2.getLeftNode())
        && (label1.getRightNode() == label2.getRightNode());
    }
};

extern std::vector<int> satPath; // 采样之后各个变量的取值

mpz_class countPath(Cudd* mgr, DdNode* node, bool toOne);

// 单次采样
void simpling(Cudd* mgr, DdNode* node, std::mt19937 &gen, std::uniform_real_distribution<> &dis, int count);


#endif //PROJECT_SAMPLING_H
