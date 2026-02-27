#include "IOFunctions.hpp"

namespace io_func{

    bool IsNum(std::string_view str) {
        for(const auto& ch: str){
            if(!isdigit(ch)){
                return false;
            }
        }
        return true;
    }

    int32_t ToNum(std::string_view str) {
        return stoi(std::string(str));
    }

    int32_t ToNum(const std::string& str) {
        return stoi(str);
    }

    std::expected<std::deque<std::string_view>,std::string>
    Parse(std::string_view line, std::string_view sep = "[]{},._-") {
        if(line.empty()){
            return std::unexpected("Пустая строчка");
        }
        if(line.find_first_not_of(sep) == std::string::npos){
            return std::unexpected("Строчка заполнена не тем");
        }

        std::deque<std::string_view> container;

        while(!line.empty()){
            auto str_beg = std::min(line.size(),line.find_first_not_of(sep));
            auto str_end = std::min(line.size(),line.find_first_of(sep,str_beg));
            if(str_beg < str_end){
               container.push_back(line.substr(str_beg,str_end - str_beg));
            }
            line.remove_prefix(str_end);
            
        }
        return container;
    }
    
    std::expected<int32_t,std::string>
    GetAnswerNum(std::istream& is,std::string_view rule) {
        std::string line;
        getline(is,line);

        auto parsed_rule = Parse(rule,"-");
        int32_t low = ToNum(parsed_rule->at(0));
        int32_t high = ToNum(parsed_rule->at(1));

        if(!parsed_rule){
            return std::unexpected("Ошибка в парсинге правила");
        }

        auto parsed_line = Parse(line);

        if(!parsed_line){
            return std::unexpected(parsed_line.error());
        }

        if(parsed_line->size() > 1){
            return std::unexpected("Принято не одно лишь только число");
        }
        
        if(!IsNum(parsed_line->at(0))){
            return std::unexpected("Это не число");
        }
        int32_t num = ToNum(parsed_line->at(0));

        if(!(num >= low && num <= high)){
            return std::unexpected("Число не подходит под диапазон");
        }
        
        return num;
    }

    std::expected<std::deque<int32_t>,std::string>
    GetAnswerNums(std::istream& is,std::string_view rule){
        std::string line;
        getline(is,line);

        auto parsed_rule = Parse(rule,"-");
        int32_t sum = ToNum(parsed_rule->at(0));
        int32_t count = ToNum(parsed_rule->at(1));

        auto parsed_line = Parse(line);

        if(!parsed_line.has_value()) {
            
        }

        if(static_cast<int32_t>(parsed_line->size()) != count){
            return std::unexpected("Количество чисел должно быть равно " + std::to_string(count));
        }

        int32_t elem_sum = 0;
        for(const auto& str: parsed_line.value()){
            elem_sum += ToNum(str);
        }

        if(elem_sum > sum){
            return std::unexpected("Сумма больше чем нужно" + std::to_string(sum));
        }
        std::deque<int32_t> temp_;
        
        for(const auto& str: parsed_line.value()){
            temp_.push_back(ToNum(str));
        }
        
        return temp_; 
    }

    std::expected<std::string,std::string>
    GetAnswerStr(std::istream& is, std:: string_view rule){
        std::string line;
        getline(is,line);

        if(line.find_first_of(rule) != std::string::npos){
            return std::unexpected("Обычные буквы, пожалуйста");
        }

        return line;
    }

    void WriteBorder() {
        std:: cout << "\n###################################\n";
    }
    
}