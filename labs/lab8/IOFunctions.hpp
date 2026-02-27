#pragma once

#include <string>
#include <string_view>
#include <deque>
#include <functional>
#include <expected>

#include "SpecialStructures.hpp"

namespace io_func{

    bool IsNum(std::string_view );

    int32_t ToNum(std::string_view );

    int32_t ToNum(const std::string& );

    std::expected<std::deque<std::string_view>,std::string>
    Parse(std::string_view, std::string_view);

    std::expected<int32_t,std::string>
    GetAnswerNum(std::istream&,std::string_view );

    std::expected<std::deque<int32_t>,std::string>
    GetAnswerNums(std::istream&,std::string_view);

    std::expected<std::string,std::string>
    GetAnswerStr(std::istream& , std:: string_view );

    template<typename Func>
    auto CircleCall(Func function,std::istream& is,std::ostream& os, std::string_view rule) {
        while(true) {
            auto answer = function(is,rule);
            if(answer) {
                return answer.value();
            }else {
                os << answer.error() << '\n';
                continue;
            }
        }
    }
    
    void WriteBorder();
}