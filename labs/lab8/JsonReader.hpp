#pragma once

#include <variant>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <string>
#include <expected>
#include <functional>

namespace json_reader {

    class Node {
    public:
        using Map = std::unordered_map<std::string,Node>;
        using Array = std::vector<Node>;
        using ValueType = std::variant<std::monostate,Array,Map,std::string,double,int32_t,uint8_t,bool>;
        
        Node() = default;
        Node(std::string&&);
        Node(Map&&);
        Node(Array&&);
        Node(const std::string&);
        Node(const Map&);
        Node(const Array&);
        Node(double);
        Node(int32_t);
        Node(uint8_t);
        Node(bool);
        Node(const Node&);
        Node(Node&&);

        Node& operator= (const Node&);
        Node& operator= (Node&&);

        bool IsArray() const;
        bool IsMap() const;
        bool IsString() const;        
        bool IsDouble() const;
        bool IsInt() const;
        bool IsChar() const;
        bool IsBool() const;
        bool IsValid() const;
        bool IsContainer() const;

        const std::string& AsString() const;
        const Map& AsMap() const;
        const Array& AsArray() const;
        double AsDouble() const;
        int32_t AsInt() const;
        uint8_t AsChar() const;
        bool AsBool() const;

    private:
        ValueType value_;
    };
    
    struct ParseExeption {
        ParseExeption(std::string&&);
        std::string name();
        std::string line;
    };

    std::ostream& operator<< (std::ostream&,const Node&);
    std::istream& operator>> (std::istream&,Node&);

}