#include "FbxParser.hpp"
#include "Logger.hpp"
#include "Util.hpp"
#include <string>

int main(int argc, char **argv) {
    std::string fbxFilename = "test.fbx";
    bool forHitbox = false;
    if (argc > 1 && argv[1][0] != '-') {
        fbxFilename = std::string(argv[1]);
    }
    FbxParser p = FbxParser(fbxFilename);

    for (int i = 2; i < argc; i++) {
        std::string argStr = std::string(argv[i]);
        if (argStr.compare("-h") == 0) {
            forHitbox = true;
        } else if (argStr.compare("-d") == 0) {
            Logger::enableLog();
        }
    }

    p.init(forHitbox);
    return 0;
}
