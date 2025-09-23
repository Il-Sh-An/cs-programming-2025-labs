#include <iostream>
#include <string>
#include <format>
#include <ranges>
#include <vector>

using namespace std::literals;

void f1(){
    [[maybe_unused]]bool b = true;
    [[maybe_unused]]char c = 'N';
    [[maybe_unused]]short sh = 2;
    [[maybe_unused]]int a = 1;
    [[maybe_unused]]float f = 2.f;
    [[maybe_unused]]double d = 2.;
    [[maybe_unused]]std::string n = "N"s;
}

void f2(){
    std::string name = "Ilya"s;
    int age = -1;
    std::cout << std::format("Name = {}, Age = {}",name,age);
}

void f3(){
    int a = 342;
    double b = 56.2;
    std::string s = "43";

    double n = a + b + std::stoi(s);
    std::cout << n;
}

void f4(){
    int a = 3;
    int b = 8;

    std::cout << (a+4*b)*(a-3*b) + a * a;
}

void f5(){
    double a,b;
    std::cout << "Введите длину и ширину:\n";
    std::cin >> a >> b;
    std::cout << std::format("Площадь: {:g}, Периметр: {:g}",a*b,2*a+2*b);
}

void f6(){
    std::string s = "*     *     *\n"s
                    " *   * *   *\n"s
                    "   *     *\n"s ;
    
    std::cout << s;
}

void f7(){
    int a = 7,b = 8;
    int increment_ = ++a;
    int decrement_ = --a;
    std::cout << std::format("(*) - {}, (/) - {}, (-) - {},(+) - {}, (%) - {}, (--) - {},(++) - {} \n",
    a*b,a/b,a-b,a+b,(int)a % (int)b,decrement_,increment_);
    std::cout << std::format("(<) - {},(<=) - {},(>) - {},(>=) - {},(==) - {},(!=) - {}",
    a<b,a<=b,a>b,a>=b,a==b,a!=b);
}

std::string GetAge( int age){
    if(age % 10 == 1){
        return "год";
    }else if(age % 10 >= 5){
        return "лет";
    }else{
        return "года";
    }
}

void f8(){
    std::string name = "Илья";
    int age;
    std::cin >> age;
    std::cout << std::format("Меня зовут <{}>, мне <{}> {}",name,age,GetAge(age));
}

void f9(){
    std::string str1 = "Съешь еще"s;
    std::string str2 = "этих мягких"s;
    std::string str3 = "французских булок,"s;
    std::string str4 = "да";
    std::string str5 = "выпей чаю";
    
    std::vector<std::string*> vec_str = {&str1,&str2,&str3,&str4,&str5};
    std::string empty_str;

    for(const auto& str: vec_str){
        static bool is_first = true;
        if(is_first){
            empty_str += *str;
            is_first = false;
        }
        
        empty_str += (" "s + *str);
        
    }

    std::cout << empty_str;
}

std::string operator*(const std::string& pr_str, int n){
    std::string s;
    for(int i = 0; i < n; ++i){
        s += pr_str;
    }
    return s;
}

void f10(){

    std::string pr_str = "Нет! Да! ";
    
    std::cout << pr_str * 4;
}

void f11(){
    int a,b,c;
    std::vector<int*> ptr_vec = {&a,&b,&c};
    std::string s_temp;
    std::cout << "Введите три числа, разделенные запятой"s;
    for(auto* ptr: ptr_vec){
        std::getline(std::cin,s_temp,',');
        *ptr = std::stoi(s_temp);
        s_temp.clear();
    }
    std::cout << std::format("Результат вычисления: <{}>",(a+c)/b);
}

template<typename It>
void OutPut(It begin, It end){
    while(begin != end){
        std::cout << *begin++;
    }
    std::cout << '\n';
}

void f12(){
    std::cout << "Введите строку больше 10 символов\n"s;
    std::string str;
    std::getline(std::cin,str);
    if(str.size()< 10 ){
        std::cout << "Больше!";
        return;
    }

    auto first_four_sym_ = str | std::views::take(4);
    auto last_two_sym_ = str | std::views::drop(str.size() - 2);
    auto into_elements_ = str | std::views::drop(3) | std::views::take(5);
    auto reverse_word_ = str | std::views::reverse;

    OutPut(first_four_sym_.begin(),first_four_sym_.end());
    OutPut(last_two_sym_.begin(),last_two_sym_.end());
    OutPut(into_elements_.begin(),into_elements_.end());
    OutPut(reverse_word_.begin(),reverse_word_.end());
}

int main(){
    setlocale(LC_ALL,"ru_Ru.UTF-8");
    f9();
}