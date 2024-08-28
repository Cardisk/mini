#include "mini.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <string>
#include <vector>

// TODO: unhardcode section path separator here as well.
// ini standard says that the path is separated by '.',
// but maybe it's a good idea to make it customizable.

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
                    src[cur] != '\n' && 
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

    std::vector<std::string> split_from_separator(std::string input, char separator) {
        std::vector<std::string> vec;

        std::stringstream stream(input);
        std::string str;
        while(std::getline(stream, str, separator)) 
            vec.push_back(str);

        return vec;
    }

    std::stringstream section_to_string(mini::Section &sec, char separator) {
        std::stringstream txt;
        // FIXME: unhardcode path separator.
        if (sec.get_name() != "global") {
            std::string path = sec.get_path();
            if (!path.empty()) path += ".";
            path += sec.get_name();
            txt << "[" << path << "]" << std::endl;
        }

        for (auto pair : sec.get_props()) {
            txt << pair.first;
            txt << " " << separator << " ";
            txt << pair.second << std::endl;
        }

        txt << std::endl;

        for (auto pair : sec.get_sections())
            txt << section_to_string(pair.second, separator).str();

        return txt;
    }
}

bool mini::Section::add_section(std::string name) {
    std::string path = "";
    if (this->name != "global") {
        if (!this->path.empty()) path += this->path + ".";
        path += this->name;
    }

    return this->sections.insert(std::make_pair(name, Section(name, path))).second;
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
    // by default at least one word needs to be provided.
    if (vpath.size() < 1) 
        error("Invalid search path provided.");

    // pointing to the correct section.
    Section *sec = &this->global;
    while (vpath.size() > 1) {
        sec = &sec->get_section(vpath.at(0));
        vpath.erase(vpath.begin());
    }

    // treating the last as the property name.
    return sec->get_prop(vpath.at(0));
}

mini::Section &mini::Object::get_section_from_path(std::string path, char separator) {
    auto vpath = split_from_separator(path, separator);
    // by default use the global one.
    if (vpath.size() < 1) 
        return this->global;

    // searching the section.
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
    Section *sec = &obj.get_global();

    size_t cur = 0;
    // FIXME: unhardcode section path separator.
    while (cur < tkns.size()) {
        switch (tkns[cur].type) {
        case Type::SECTION: {
            std::string name = tkns[cur].data;

            bool failed = false;
            // dynamic subsectioning.
            if (name.starts_with(".")) {
                if (sec->get_name() == "global")
                    error("Cannot have dynamic subsectioning at top level scope.");

                name.erase(0, 1);
                if (!sec->add_section(name))
                    failed = true;

                sec = &sec->get_section(name);
            }
            // subsectioning with absolute path.
            else if (name.find(".") != std::string::npos) {
                auto path = split_from_separator(name, '.');
                std::stringstream joined;
                for (auto i = 0; i < path.size() - 1; i++) {
                    if (i != 0) joined << "/";
                    if (path.at(i).empty())
                        error("Cannot provide empty section names inside '" << name << "' path.");
                    joined << path.at(i);
                }
                if (!obj.get_section_from_path(joined.str()).add_section(path.back())) failed = true;
                sec = &obj.get_section_from_path(joined.str()).get_section(path.back());
            }
            // top level section definition.
            else {
                if (!obj.get_global().add_section(name)) failed = true;
                sec = &obj.get_section_from_path(name);
            }
            if (failed)
                error("Cannot insert '" << name << "' section.");

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

void mini::write(mini::Object &obj, char separator) {
    std::ofstream ini(obj.get_file_path());
    if (!ini.is_open())
        error("Unable to open '" << obj.get_file_path() << "'.");

    std::stringstream content = section_to_string(obj.get_global(), separator);
    
    ini << content.rdbuf();
}
