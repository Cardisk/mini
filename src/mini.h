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
        explicit Section(std::string name = "global", std::string path = ""): name(std::move(name)),
                                                                              path(std::move(path)) {}

        std::string &get_name() { return this->name; }
        void set_name(std::string name) { this->name = name; }
        
        std::string &get_path() { return this->path; }
        void set_path(std::string path) { this->path = path; }

        Props &get_props() { return this->props; }
        Sections &get_sections() { return this->sections; }

        bool add_prop(std::string name, std::string val);
        std::string &get_prop(std::string name);
        
        bool add_section(std::string name);
        Section &get_section(std::string name);
    private:
        std::string name;
        std::string path;
        Props props;
        Sections sections;
    };

    class Object {
    public:
        explicit Object(std::string path = ""): file_path(std::move(path)), 
                                                global(Section()) {}

        std::string &get_file_path() { return this->file_path; }
        void set_file_path(std::string file_path) { this->file_path = file_path; }

        Section &get_global() { return this->global; }

        std::string &get_prop_from_path(std::string path, char separator = '/');
        Section &get_section_from_path(std::string path = "", char separator = '/');

    private:
        std::string file_path;
        Section global;
    };

    Object read(std::string at);
    void write(Object &obj, char separator = '=');
}
