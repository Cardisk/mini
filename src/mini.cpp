#include "mini.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>

#define error(format, ...) fprintf(stderr, format, __VA_ARGS__), exit(EXIT_FAILURE); 

namespace {
    enum Type {
        INVALID = 0,
        SECTION,
        IDENTIFIER,
        SEPARATOR,
    };

    struct Token {
        Type type;
        std::string data;
    };

    typedef std::vector<Token> Tokens;

    std::string read_from_file(std::string file_path) {
        std::ifstream file(file_path);
        if (!file.is_open()) error("Could not open %s.\n", file_path.c_str());
        std::string src;
        file >> src;
        return src;
    }

    Tokens lex(std::string src) {
        Tokens tkns;
        // TODO: implement lexing here.
        size_t cur = 0;
        while (src[cur] != src.back()) {
            if (src[cur] == '\n') {
                // TODO: maybe handle token position?
                cur++;
                continue;
            }

            if (src[cur] == '[') {
                // TODO: handle section.
                continue;
            }

            if (src[cur] == ':' || src[cur] == '=') {
                // TODO: handle separator.
                continue;
            }

            if (src[cur] == '"') {
                // TODO: handle strings.
                continue;
            }

            // TODO: handle the token as a property.
        }
        return tkns;
    }
}
