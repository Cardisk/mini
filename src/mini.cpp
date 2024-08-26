#include "mini.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <string>
#include <vector>

#define error(format) do { std::cerr << format, exit(1); } while(0)

#define UNREACHABLE error("Technically reaching this branch is impossible.")

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
        if (!file.is_open()) 
            error("Could not open '" << file_path << "': " << strerror(errno) << "\n");
        std::stringstream buf;
        buf << file.rdbuf();
        return buf.str();
    }

    Tokens lex(std::string src) {
        Tokens tkns;
        size_t cur = 0;
        while (cur < src.size()) {
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
                    src[cur] != '=' &&
                    src[cur] != '[') cur++;

            Token t = {
                .type = Type::IDENTIFIER,
                .data = src.substr(start, cur - start),
            };

            t.data.erase(std::remove_if(t.data.begin(), t.data.end(), ::isspace), t.data.end());

            tkns.emplace_back(t);
        }
        return tkns;
    }
}

std::vector<std::string> split_from_separator(std::string input, char separator) {
    std::vector<std::string> vec;

    std::stringstream stream(input);
    std::string str;
    while(std::getline(stream, str, separator)) 
        vec.push_back(str);

    return vec;
}

bool mini::Section::add_section(std::string name) {
    return this->sections.insert(std::make_pair(name, Section(name))).second;
}

mini::Section &mini::Section::get_section(std::string name) {
    return this->sections.at(name);
}

bool mini::Section::add_prop(std::string name, std::string val) {
    return this->props.insert(std::make_pair(name, val)).second;
}

std::string &mini::Section::get_prop(std::string name) {
    return this->props.at(name);
}

std::string &mini::Object::get_prop_from_path(std::string path, char separator) {
    auto vpath = split_from_separator(path, separator);
    if (vpath.size() < 1) 
        error("Invalid search path provided.");

    Section *sec = &this->global;
    while (vpath.size() > 1) {
        sec = &sec->get_section(vpath.at(0));
        vpath.erase(vpath.begin());
    }

    return sec->get_prop(vpath.at(0));
}

mini::Section &mini::Object::get_section_from_path(std::string path, char separator) {
    auto vpath = split_from_separator(path, separator);
    if (vpath.size() < 1) 
        return this->global;

    Section *sec = &this->global;
    while (vpath.size() > 1) {
        sec = &sec->get_section(vpath.at(0));
        vpath.erase(vpath.begin());
    }

    return sec->get_section(vpath.at(0));
}

mini::Object mini::read(std::string at) {
    if (!at.ends_with(".ini"))
        error("Invalid file extension of '" << at << "'expected '.ini'.");

    std::string src = read_from_file(at);
    Tokens tkns = lex(src);

    // TODO: remove this debug code.
    /* for (Token t : tkns) { */
    /*     std::cout << "----------" << std::endl; */
    /*     std::cout << t.type << std::endl; */
    /*     std::cout << t.data << std::endl; */
    /*     std::cout << "----------" << std::endl; */
    /* } */

    Object obj = Object(at);
    Section *sec = &obj.get_section_from_path("");

    size_t cur = 0;
    while (cur < tkns.size()) {
        switch (tkns[cur].type) {
        case Type::SECTION: {
            std::string name = tkns[cur].data;

            bool failed = false;
            if (name.starts_with(".")) {
                name.erase(0, 1);
                if (!sec->add_section(name))
                    failed = true;
            }
            else if (!obj.get_global().add_section(name)) failed = true;

            if (failed)
                error("Cannot insert '" << name << "' section.");

            sec = &sec->get_section(name);
            // eating the section declaration.
            cur++;
        } break;
        case Type::IDENTIFIER: {
            std::string name = tkns[cur].data;
            // eating property name.
            cur++;
            if (cur < tkns.size() && !(tkns[cur].type == Type::SEPARATOR))
                error("Missing separator for property '" << name << "'.");
            // eating the separator.
            cur++;
            if (cur >= tkns.size()) 
                error("Unfinished property '" << name << "' definition, EOF.\n");

            std::string value = tkns[cur].data;
            if (!sec->add_prop(name, value)) 
                error("Cannot insert property '" << name << "'.");
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

bool mini::write(mini::Object &obj, char separator) {
    return false;
}
