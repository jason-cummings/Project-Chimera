#include "FbxParser.hpp"
#include "Logger.hpp"
#include "Util.hpp"
#include <string>

int main(int argc, char **argv) {
    std::string fbx_filename = "test.fbx";
    bool for_hitbox = false;
    if (argc > 1 && argv[1][0] != '-') {
        fbx_filename = std::string(argv[1]);
    }
    FbxParser p = FbxParser(fbx_filename);

    for (int i = 2; i < argc; i++) {
        std::string arg_str = std::string(argv[i]);
        if (arg_str.compare("-h") == 0) {
            for_hitbox = true;
        } else if (arg_str.compare("-d") == 0) {
            Logger::enableLog();
        }
    }

    p.init(for_hitbox);
    return 0;
}
