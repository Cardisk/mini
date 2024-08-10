#include "mini.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>

using namespace mini;

#define error(format, ...) do { fprintf(stderr, format, __VA_ARGS__), exit(EXIT_FAILURE); } while (0)

#define UNREACHABLE error("Technically reaching this branch is impossible.", NULL)

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
        size_t cur = 0;
        while (cur < src.size() && src[cur] != src.back()) {
            if (src[cur] == '\n' || src[cur] == ' ') {
                // TODO: maybe handle token position?
                cur++;
                continue;
            }

            if (src[cur] == '[') {
                // eating '['.
                size_t start = ++cur;
                // eating the identidier.
                while (cur < src.size() && src[cur] != ']') cur++;

                Token t = {
                    .type = Type::SECTION,
                    .data = src.substr(start, cur - start),
                };
                
                // eating ']'.
                cur++;
                tkns.emplace_back(t);
                continue;
            }

            if (src[cur] == ':' || src[cur] == '=') {
                // eating the separator.
                size_t start = cur++;

                Token t = {
                    .type = Type::SEPARATOR,
                    .data = src.substr(start, cur - start),
                };

                tkns.emplace_back(t);
                continue;
            }

            if (src[cur] == '"') {
                // eating '"'.
                size_t start = ++cur;
                // eating the identidier.
                while (cur < src.size() && src[cur] != '"') cur++;

                Token t = {
                    .type = Type::IDENTIFIER,
                    .data = src.substr(start, cur - start),
                };
                
                // eating '"'.
                cur++;
                tkns.emplace_back(t);
                continue;
            }

            size_t start = cur++;

            while (cur < src.size() && 
                    src[cur] != ' ' && 
                    src[cur] != ':' && 
                    src[cur] != '=') cur++;

            Token t = {
                .type = Type::IDENTIFIER,
                .data = src.substr(start, cur - start),
            };

            tkns.emplace_back(t);
        }
        return tkns;
    }
}

bool Section::add_section(std::string name) {
    return false;
}

Section &Section::get_section(std::string name) {
}

bool Section::add_prop(std::string name, std::string val) {
    return false;
}

std::string &Section::get_prop(std::string name) {
}

Object read(std::string at) {
    if (!at.ends_with(".ini"))
        error("Invalid file extension of '%s', expected '.ini'.", at.c_str());

    std::string src = read_from_file(at);
    Tokens tkns = lex(src);

    Object obj { 
        .file_path = at,
        .global = Section(),
    };

    Section &sec = obj.global;

    size_t cur = 0;
    while (cur < tkns.size()) {
        switch (tkns[cur].type) {
        case Type::SECTION: {
            std::string name = tkns[cur].data;
            if (!sec.add_section(name)) 
                error("Cannot insert '%s' section.", name.c_str());

            sec = sec.get_section(name);
            // eating the section declaration.
            cur++;
        } break;
        case Type::IDENTIFIER: {
            std::string name = tkns[cur].data;
            // eating property name.
            cur++;
            if (cur < tkns.size() && !(tkns[cur].type == Type::SEPARATOR))
                error("Missing separator for property '%s'.", name.c_str());
            // eating the separator.
            cur++;
            if (cur >= tkns.size()) 
                error("Unfinished property definition.", NULL);

            std::string value = tkns[cur].data;
            if (!sec.add_prop(name, value)) 
                error("Cannot insert property '%s'.", name.c_str());
            // eating property value.
            cur++;
        } break;
        case Type::SEPARATOR:
        case Type::INVALID:
            UNREACHABLE;
        }
    }

    return obj;
}

bool write(Object &obj, char separator) {
    return false;
}
