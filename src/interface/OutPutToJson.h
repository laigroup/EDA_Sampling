//
// Created by win on 2023/11/4.
//

#ifndef PROJECT_OUTPUTTOJSON_H
#define PROJECT_OUTPUTTOJSON_H
// CUDD
#include "../../lib/cudd-3.0.0/cplusplus/cuddObj.hh"
#include "../../lib/cudd-3.0.0/cudd/cuddInt.h"

#include "../../lib/json.hpp"
#include <iostream>
#include <fstream>
#include <string>

void toJsonFormat(Cudd& mgr, BDD& ans, const std::vector<jsons::Var>& vars, int num, unsigned int seed, const std::string& outputFile);
#endif //PROJECT_OUTPUTTOJSON_H
