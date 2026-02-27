#include "JsonReader.hpp"
#include <deque>
#include <iostream>
#include <sstream>

namespace json_reader {
// Node
    Node::Node(std::string&& str):value_(std::move(str)) {}
    Node::Node(Map&& map): value_(std::move(map)) {}
    Node::Node(Array&& array):value_(std::move(array)) {}
    Node::Node(double val): value_(val) {}
    Node::Node(int32_t val): value_(val) {}
    Node::Node(uint8_t ch): value_(ch) {}
    Node::Node(const std::string& str): value_(str) {}
    Node::Node(const Map& map): value_(map) {}
    Node::Node(const Array& array): value_(array) {}
    Node::Node(bool val):value_(val) {}

    Node::Node(const Node& other) = default;
    Node::Node(Node&& other) = default;

    Node& Node::operator= (const Node& other) {
        if(this != &other){
            Node temp_node(other);
            std::swap(value_,temp_node.value_);            
        }

        return *this;
    }
    Node& Node::operator= (Node&& other) {
        if(this != &other) {
            value_ = std::move(other.value_);
        }
        return *this;
    }

    bool Node::IsArray() const{
        return std::holds_alternative<Array>(value_);
    }
    bool Node::IsMap() const{
        return std::holds_alternative<Map>(value_);
    }
    bool Node::IsString() const {
        return std::holds_alternative<std::string>(value_);
    }        
    bool Node::IsDouble() const {
        return std::holds_alternative<double>(value_);
    }
    bool Node::IsInt() const {
        return std::holds_alternative<int32_t>(value_);
    }
    bool Node::IsChar() const {
        return std::holds_alternative<uint8_t>(value_);
    }
    bool Node::IsBool() const {
        return std::holds_alternative<bool>(value_);
    }
    bool Node::IsValid() const {
        return !std::holds_alternative<std::monostate>(value_);
    }
    bool Node::IsContainer() const {
        return std::holds_alternative<Map>(value_)
                || std::holds_alternative<Array>(value_);
    }

    const std::string& Node::AsString() const {
        return std::get<std::string>(value_);
    }
    const Node::Map& Node::AsMap() const {
        return std::get<Map>(value_);
    }
    const Node::Array& Node::AsArray() const {
        return std::get<Array>(value_);
    }
    double Node::AsDouble() const {
        return std::get<double>(value_);
    }
    int32_t Node::AsInt() const {
        return std::get<int32_t>(value_);
    }
    uint8_t Node::AsChar() const {
        return std::get<uint8_t>(value_);
    }
    bool Node::AsBool() const {
        return std::get<bool>(value_);
    }

// functions borders

    uint8_t GetBorder(uint8_t sym) {
        switch(sym) {
            case '{':
                return '}';
            case '}':
                return '{';
            case '[':
                return ']';
            case ']':
                return '[';
            default :
                return sym;
        }
    }

    bool IsContainerBorder(uint8_t symbol) {
        switch (symbol) {
            case '[':
                return true;
            case ']':
                return true;
            case '{':
                return true;
            case '}':
                return true;
            default:
                return false;
        }
    }

    std::string_view FindEnding(std::string_view view, uint8_t sym) {
        uint64_t ending_num = 0;
        uint8_t ending_sym = GetBorder(sym);

        int32_t counter = 0;
        for(uint64_t id = 0; id < view.size(); ++id) {
            if(view.at(id) == ending_sym && counter == 1){
                ending_num = id;
                break;
            } else if (view.at(id) == sym) {
                ++counter;
                continue;
            } else if ( view.at(id) == ending_sym) {
                --counter;
                continue;
            }else {
                continue;
            }

        }
        return view.substr(0,ending_num + 1);
    }

    void RemoveSpaces(std::string_view& view) {
        if(view.size() == 0) {
            return;
        }

        auto str_beg = view.find_first_not_of(" \n\t\r\v\f");
        auto str_end = view.find_last_not_of(" \n\t\r\v\f");

        view.remove_suffix(view.size() - str_end - 1);
        view.remove_prefix(str_beg);
        
    }

    void RemoveFirstSpaces(std::string_view& view) {
        auto str_beg = view.find_first_not_of(" \n\t\r\v\f");

        view.remove_prefix(str_beg);
    }

    std::string_view RemoveBorders(std::string_view view) {
        return {view.begin() + 1,view.end() - 1};
    }
 
    Node ReadString(std::string_view view) {

        if(view.empty()) {
            return Node{};
        }

        return Node(std::string(view));
    }

    bool IsDigits(std::string_view view) {
        if(view.empty()) {
            return false;
        }

        for(const auto ch : view) {
            if(!isdigit(ch)) {
                return false;
            }
        }
        return true;
    }

    bool ReadDigits(std::string_view& view) {
        uint64_t id = 0;
        for(; id != view.size(); ++id) {
            if(!isdigit(view.at(id))) {
                
                return false;
            }
        }
        view.remove_prefix(id);
        return true;
    }

    bool IsDouble(std::string_view view) {
        if(!ReadDigits(view)) {
            return false;
        }
        
        if(view.front() != '.'){
            return false;
        }
        view.remove_prefix(1);

        if(!ReadDigits(view)) {
            return false;
        }

        if(view.empty()) {
            return true;
        }

        if(view.front() != '+' || view.front() != '-') {
            return false;
        }

        view.remove_prefix(1);

        if(view.front() != 'E' || view.front() != 'e') {
            return false;
        }

        view.remove_prefix(1);

        if(!ReadDigits(view)) {
            return false;
        }else {
            return true;
        }
    }

    Node ReadNum(std::string_view view) {

        if(IsDouble(view)) {
            return Node(stod(std::string(view)));
        }else if(IsDigits(view)){
            return Node(stoi(std::string(view)));
        }else {
            throw ParseExeption("Ошибка в прочтении числа Json");
        }
    }

    Node ReadChar(std::string_view view) {
        if(view.size() != 1) {
            throw ParseExeption("Ошибка в прочтении символа Json");
        }

        if(!isalpha(view.front())) {
            throw ParseExeption("Json exeption reading symbol");
        }

        return Node(view.front());
    }

    Node ReadFalse(std::string_view view) {
        if(view  != "false" && view != "False") {
            throw ParseExeption("Json exeption reading false");
        }
        return Node(false);
    }

    Node ReadTrue(std::string_view view) {
        if(view  != "true" && view != "True") {
            throw ParseExeption("Json exeption reading true");
        }
        return Node(true);
    }

// functions reading nodes   

    Node ReadNode(std::string_view view) {
        RemoveSpaces(view);

        switch(view.front()) {
            case '\"':
                return ReadString(RemoveBorders(view));
            case '\'':
                return ReadChar(RemoveBorders(view));
            case 't':
                return ReadTrue(view);
            case 'f':
                return ReadFalse(view);
            default:
                if(isdigit(view.front())) {
                    return ReadNum(view);
                }else {
                    throw ParseExeption("Json exeption reading symbol");
                }
        }
    }


    Node ReadContainer(std::string_view view);

    Node ReadSmth(std::string_view& view) {
        Node node;

        RemoveFirstSpaces(view);

        uint64_t separator;

        if(IsContainerBorder(static_cast<uint8_t>(view.front()))) {
            std::string_view container = FindEnding(view,static_cast<uint8_t>(view.front()));

            separator = std::min(view.size(),view.find(',',container.size()));

            node = ReadContainer(container);
        }else {
            separator = std::min(view.size(),view.find(','));

            node = ReadNode(view.substr(0,separator));
        }

        if(separator < view.size()) {
            ++separator;
        }

        view.remove_prefix(separator);

        return node;

    }

    Node ReadMap (std::string_view view) {
        Node::Map map;

        while(!view.empty()) {
            RemoveFirstSpaces(view);

            if(view.front() != '\"') {
                throw ParseExeption("Exeption of reading map");
            }

            auto key_sep = std::min(view.size(),view.find(':'));

            auto key = ReadNode(view.substr(0,key_sep));

            if(key_sep < view.size()) {
                ++key_sep;
            }

            view.remove_prefix(key_sep);

            auto node = ReadSmth(view);

            if(key.IsValid() && node.IsValid()) {
                map.emplace(std::move(key.AsString()),std::move(node));
            }
        }

        if(map.empty()) {
            return Node{};
        }else {
            return Node(std::move(map));
        }
    }

    Node ReadArray (std::string_view view) {
        Node::Array array;

        while(!view.empty()) {
            Node node;

            node = ReadSmth(view);

            if(node.IsValid()) {
                array.push_back(std::move(node));
            }
        }

        if(array.empty()) {
            return Node{};
        }else {
            return Node(std::move(array));
        }
    }   

    Node ReadContainer(std::string_view view) {
        RemoveSpaces(view);

        if(view.back() != GetBorder(static_cast<uint8_t>(view.front()))) {
            throw ParseExeption("Exeption of reading container");
        }

        switch (view.front()) {
            case '[':
                return ReadArray(RemoveBorders(view));
            case '{':
                return ReadMap(RemoveBorders(view));
            default:
                throw ParseExeption("Exeption of reading container");
        }   
    }

    Node ReadJson (std::istream& is) {
        std::stringstream input_stream;
        input_stream << is.rdbuf();

        std::string line_buffer(input_stream.str());

        return ReadContainer(line_buffer);
    }

// Printing functions Json

    void PrintNode(std::ostream& os,Node line) {
        if(line.IsString()) {
            os << "\"" << line.AsString() << "\"";
        }else if(line.IsDouble()) {
            os << line.AsDouble();
        }else if(line.IsInt()) {
            os << line.AsInt();
        }else if(line.IsChar()) {
            os << line.AsChar();
        }else if(line.IsBool()) {
            os << (line.AsBool()?"true":"false");
        }
    }

    void PrintContainer(std::ostream& os,Node cont, std::string spaces) {
        if(cont.IsMap()) {
            const auto& map_ = cont.AsMap();

            os << spaces << "{\n";

            uint32_t counter = 1;
            for(const auto& [key,node]: map_) {

                os << spaces + "\t" << '\"' << key << '\"' << ':';

                if(node.IsContainer()) {
                    os << '\n';
                    PrintContainer(os,node,spaces + '\t');
                }else {
                    PrintNode(os,node);
                }

                if(counter < map_.size()) {
                    os << ",\n";
                    ++counter;
                }else {
                    os << '\n';
                }
                
            }
            os << '\n' << spaces << "}";
        }else {
            const auto& array_ = cont.AsArray();

            os << spaces << "[\n";

            uint32_t counter = 1;
            for(const auto& node: array_) {

                if(node.IsContainer()) {
                    PrintContainer(os,node,spaces + "\t");
                }else {
                    PrintNode(os,node);
                }

                if(counter < array_.size()) {
                    os << ",\n";
                    ++counter;
                }else {
                    os << '\n';
                }
                
            }
            os << spaces << "]";
        }
    }

    void PrintJson (std::ostream& os, const Node& node) {
        if(!node.IsValid()) {
            os << "none";
        }else if(node.IsContainer()) {
            PrintContainer(os,node,"");
        }else {
            PrintNode(os,node);
        }
    }

// functions parsing exeptions

    ParseExeption::ParseExeption(std::string&& str):line(std::forward<std::string>(str)) {}

    std::string ParseExeption::name() {
        return line;
    }
// operators output

    std::istream& operator>> (std::istream& is,Node& node) {
        node = ReadJson(is);
        return is;
    }

    std::ostream& operator<< (std::ostream& os,const Node& node) {
        PrintJson(os,node);
        os << '\n';

        return os;
    }
}