#include "file_util.h"

#include <cstdio>

namespace config::tool {

bool Read_File(const char* path, std::string* content) {
    if (path == nullptr || content == nullptr) {
        return false;
    }
    FILE* file = std::fopen(path, "rb");
    if (file == nullptr) {
        return false;
    }
    content->clear();
    char buffer[4096];
    while (true) {
        const size_t count = std::fread(buffer, 1, sizeof(buffer), file);
        content->append(buffer, count);
        if (count != sizeof(buffer)) {
            const bool success = std::ferror(file) == 0;
            std::fclose(file);
            return success;
        }
    }
}

bool Write_File_If_Different(const char* path, const std::string& content,
                             std::string* diagnostic) {
    if (path == nullptr || diagnostic == nullptr) {
        return false;
    }
    std::string existing;
    if (Read_File(path, &existing) && existing == content) {
        return true;
    }
    FILE* file = std::fopen(path, "wb");
    if (file == nullptr) {
        *diagnostic = std::string(path) + ": unable to open output";
        return false;
    }
    const bool wrote = std::fwrite(content.data(), 1, content.size(), file) == content.size();
    const bool closed = std::fclose(file) == 0;
    const bool success = wrote && closed;
    if (!success) {
        *diagnostic = std::string(path) + ": unable to write output";
    }
    return success;
}

}