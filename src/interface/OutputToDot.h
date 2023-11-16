//
// Created by win on 2023/9/26.
//

#ifndef PROJECT_OUTPUTTODOT_H
#define PROJECT_OUTPUTTODOT_H
// CUDD
#include "../../lib/cudd-3.0.0/cplusplus/cuddObj.hh"
#include "../../lib/cudd-3.0.0/cudd/cuddInt.h"

#include "../interface/InputFileParse.h"

void writeDotFile(Cudd& mgr, const BDD& bdd, const std::string& dotFileDir);
#endif //PROJECT_OUTPUTTODOT_H