#include <iostream>
#include <string>
#include <format>
#include <cstdint>

using namespace std::string_literals;

void F1(){
    std::string name;
    int age = 0;

    std::cout << "Введите имя и возраст\n";

    std::cin >> name >> age;

    for(int i = 0; i < 10; ++i){
        std::cout << std::format("Меня зовут {} и мне {} лет\n",name,age);
    }
}

void F2(){
    std::cout << "Введите число:\n";
    int n = 0;
    std::cin >> n;
    for(int i = 1; i < 10;++i){
        std::cout << std::format("{} * {} = {}\n",n,i,n*i);
    }
}

void F3(){
    for(int i = 1;i < 101; ++i){
        if(i % 3 == 0){
           std::cout << i - 1 << ", ";
        }  
    }
}

void F4(){
    uint64_t input = 0;
    std::cout << "Введите число:\n";
    std::cin >> input;

    uint64_t factorial = 1;
    for(uint64_t i = 1; !(i > input); ++i){
        factorial *= i;
    }
    std::cout << factorial;
}

void F5(){
    int number = 20;
    while(number >= 0){
        if(number == 20){
            std::cout << number--;
        }
        std::cout << ", " << number--; 
    }
}

void F6(){
    int f1 = 0;
    int f2 = 1;

    std::cout << "Введите число:";
    int num;
    std::cin >> num;
    std::cout << std::format("{}, {}, ",0,1);
    for(;;){
        int n = f1 + f2;
        if(n <= num){
            std:: cout << n << ", ";
            f1 = f2;
            f2 = n;
            continue;
        }else{
            break;
        }
    }
}

void F7(){
    std::string str;
    std::cout << "Введите строчку:\n";
    std::cin >> str;
    std::string out_str;
    for(char ch: str){
        static int num = 1;
        out_str += ch;
        out_str += '0' + char(num++);
    }
    std::cout << out_str;
}

void F8(){
    int a,b;
    while(true){
        std::cout << "Введите два числа через пробел: ";
        std::cin >> a >> b;
        std::cout << std::format("Сумма чисел: {}\n",uint64_t(a+b));
    }
}

int main(){
    setlocale(LC_ALL,"ru_Ru.UTF-8");
    F8();
}