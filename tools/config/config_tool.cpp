#include <cstdio>
#include <cstring>
#include <string>

#include "generate_cpp.h"
#include "generate_docs.h"
#include "schema_model.h"

namespace {
    void Print_Usage_() {
        std::fprintf(stderr,
            "usage:\n"
            "  config-tool validate <schema.json5>\n"
            "  config-tool generate <schema.json5> <output-directory>\n"
            "  config-tool check <schema.json5> <committed-directory> <temporary-directory>\n"
            "  config-tool docs <schema.json5> <output.html>\n");
    }
}

int main(int argc, char** argv) {
    const bool validate = argc == 3 && std::strcmp(argv[1], "validate") == 0;
    const bool generate = argc == 4 && std::strcmp(argv[1], "generate") == 0;
    const bool check = argc == 5 && std::strcmp(argv[1], "check") == 0;
    const bool docs = argc == 4 && std::strcmp(argv[1], "docs") == 0;
    if (!validate && !generate && !check && !docs) {
        Print_Usage_();
        return 2;
    }

    config::tool::s_schema schema;
    std::string diagnostic;
    if (!config::tool::Load_And_Validate_Schema(argv[2], &schema, &diagnostic)) {
        std::fprintf(stderr, "%s\n", diagnostic.c_str());
        return 1;
    }

    bool success = true;
    if (generate) {
        success = config::tool::Generate_Cpp(&schema, argv[3], &diagnostic);
    } else if (check) {
        success = config::tool::Check_Generated_Cpp(&schema, argv[3], argv[4], &diagnostic);
    } else if (docs) {
        success = config::tool::Generate_Documentation(&schema, argv[3], &diagnostic);
    }

    if (!success) {
        std::fprintf(stderr, "%s\n", diagnostic.c_str());
        return 1;
    }
    return 0;
}