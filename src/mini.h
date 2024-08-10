#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>

namespace mini {
    class Section;

    typedef std::unordered_map<std::string, std::string> Props;
    typedef std::unordered_map<std::string, Section> Sections;

    class Section {
    public:
        explicit Section(std::string name = "global"): name(std::move(name)) {}

        std::string &get_name() { return this->name; }
        void set_name(std::string name) { this->name = name; }

        bool add_prop(std::string name, std::string val);
        std::string &get_prop(std::string name);
        
        bool add_section(std::string name);
        Section &get_section(std::string name);
    private:
        std::string name;
        Props props;
        Sections sections;
    };

    struct Object {
        std::string file_path;
        Section global;
    };

    Object read(std::string at);
    bool write(Object &obj, char separator);
}
