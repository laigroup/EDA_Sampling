#include "../interface/OutputToDot.h"

using namespace std;
int dotFileIndex = 0;

void writeDotFile(Cudd& mgr, const BDD& bdd, const string& dotFileDir) {
    string filePath = dotFileDir + "OutPutFiles/OutPutFile" + to_string(dotFileIndex++) + ".dot";
    FILE* file = fopen(filePath.c_str(), "wb");  // writes to binary file
    if (file == nullptr) {
        printf("Error opening file: %s\n", strerror(errno));
    }

    auto** ddNodeArray = static_cast<DdNode**>(malloc(sizeof(DdNode*)));
    ddNodeArray[0] = bdd.getNode();
    Cudd_DumpDot(mgr.getManager(), 1, ddNodeArray, nullptr, nullptr, file);
    free(ddNodeArray);

    fclose(file);
}