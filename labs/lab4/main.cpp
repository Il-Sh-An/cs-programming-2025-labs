#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdint>
#include <sstream>

using namespace std::literals;

void F1(){
    int temperature_;
    std::cout <<  "Введите температуру в градусах:\n"s;
    std::cin >> temperature_;
    if(temperature_ > 120 || temperature_ < -100){
        throw std::logic_error("Нереальная температура");
    }
    std::cout << "Кондиционер "s << ((temperature_ >= 20)?"выключен\n"s:"ключен\n"s);
}

void F2(){
    std::array<std::string,4> month{"зима","весна","лето","осень"s};
    int month_count_;
    std::cout <<  "Введите номер месяца:\n"s;
    std::cin >> month_count_;
    if(month_count_ < 1 || month_count_ > 12){
        throw std::logic_error("Это что за месяц?");
    }
    std::cout << "Это " << month.at((uint64_t)month_count_ / 4);
}

void F3(){
    static std::vector<double> dog_ages{10.5,21.0};
    dog_ages.reserve(22);
    for(int i = 1;i < 21;++i){
        dog_ages.push_back(*dog_ages.rbegin() + 4.0);
    }
    int dog_age_;
    std::cout << "Введите возраст собаки (в годах):\n";
    std::cin >> dog_age_;
    if(std::cin.peek() == '\n'){
        if(dog_age_ > 22){
            std::cout << "Ошибка: собака столько не живёт\n";
        }else if(dog_age_ < 1){
            std::cout << "Ошибка: возраст должен быть не меньше 1\n";
        }else{
            std::cout << dog_ages.at((uint64_t)dog_age_ - 1);
        }   
    }else{
        std::cout << "Ошибка в вводе: введено не число\n";
    }
}

void F4(){
    std::string number;
    std::cout << "Введите число:\n";
    std::cin >> number;

    int summer_ = 0;
    for(char ch: number){
        summer_ += (int)(ch - '0');
    }

    if(summer_ % 3 == 0 && !(stoi(number) & 1)){
        std::cout << "Число " << stoi(number) << " делится на 6\n";
    }else{
        std::cout << "Число " << stoi(number) << " не делится на 6\n";
    }
}

struct GreatPass{
    bool IsUpper = 0;
    bool IsAlpha = 0;
    bool IsDigit = 0;
    bool IsSpec = 0;

    operator bool(){
        if(!IsUpper || !IsAlpha || !IsDigit || !IsSpec){
            return false;
        }else{
            return true;
        }
    }

    std::string GetWarning(){
        std::vector<std::string> warning;
        if(!IsUpper){
            warning.push_back("заглавные буквы");
        }
        if(!IsAlpha){
            warning.push_back("строчные буквы");
        }
        if(!IsDigit){
            warning.push_back("числа");
        }
        if(!IsSpec){
            warning.push_back("специальные символы");
        }
        
        if(warning.size() == 1){
            return warning.at(0);
        }
        std::string warn;
        
        for(auto it = warning.begin(); it != warning.end();++it){
            if(it == warning.end() - 1)[[unlikely]]{
                warn += " и ";
            }else[[likely]]{
                warn += " , ";
            }
            warn += *it;
        }
        return warn;
    }
};

void F5(){
    std::string password;
    std::cout << "Введите пароль:\n";
    std::cin >> password;
    GreatPass pass_checker;

    for(char ch: password){
        if(islower(int(ch))){
            pass_checker.IsAlpha = 1;
        }
        if(isupper(int(ch))){
            pass_checker.IsUpper = 1;
        }
        if(isdigit(int(ch))){
            pass_checker.IsDigit = 1;
        }
        if(ispunct(int(ch))){
            pass_checker.IsSpec = 1;
        }
    }

    if(pass_checker){
        std::cout << "Пароль надежен\n";
    }else{
        std::cout << "Пароль ненадежный: отсутствуют " << pass_checker.GetWarning();
    }
    
}

void F6(){
    int year;
    std::cout << "Введите год:\n";
    std::cin >> year;
    if(((year % 4 == 0) && (year % 100 == 1)) || (year % 400 == 0) ){
        std::cout << year << " - Високосный год\n";
    }else{
        std::cout << year << " - Не високосный год\n";
    }
}

void F7(){
    std::string numbers;
    std::cout << "Введите три числа через пробел:\n";
    std::cin >> numbers;
    std::istringstream input_str(numbers);
    int smallint;
    input_str >> smallint;
    for(int num;input_str >> num;){
        if(num < smallint){
            smallint = num;
        }
    }
    std:: cout << smallint << "Наименьшее число\n";
}

void F8(){
    double pack_sum;
    double skidka = 0.0;
    std::cout << "Введите сумму покупки:";
    std::cin >> pack_sum;
    if(pack_sum < 1000){

    }else if(pack_sum < 5000){
        skidka = 0.05;
    }else if(pack_sum < 1000){
        skidka = 0.1;
    }else{
        skidka = 0.15;
    }

    std::cout << "\nВаша скидка: " << int(skidka * 100) << "%\n";
    std::cout << "К оплате: " << pack_sum - pack_sum * skidka;

}

void F9(){
    std::vector<std::string> time_seq{"ночь","утро","вень","вечер"};
    int64_t time;
    std::cout << "Введите час (0–23):\n";
    std::cin >> time;
    if(time > 23 || time < 0){
        std::cout << "Ошибка: неправильно введено время\n";
        return;
    }
    std::cout << "Сейчас " << time_seq.at((uint64_t)time/6);

}

void F10(){
    int number;
    std::cout << "Введите одно число:";
    std::cin >> number;
    if(std::cin.peek() != '\n'){
        if(isspace(std::cin.peek())){
            std::cout << "Введено два числа\n";
            return;
        }else{
            std::cout << "Введена буква между цифрами\n";
            return;
        } 
    }else{
        bool is_pure = true;
        if(number > 1){
            for(int i = 2; i < number; ++i){
                if(number % i == 0){
                    is_pure = 0;
                    break;
                }
            }
        }else if(number  < 1){
            std::cout << "Введено неправильное число\n";
            return;
        }

        std::cout << number << " - " << (is_pure?"простое число\n":"составное число\n");
    }
}

int main(){
    setlocale(LC_ALL,"ru_Ru.UTF-8");
    F1();
}