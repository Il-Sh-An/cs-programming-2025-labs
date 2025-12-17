#include <algorithm>
#include <clocale>
#include <iostream>
#include <type_traits>
#include <iterator>
#include <memory>
#include <ranges>
#include <cstdint>
#include <cstdlib>
#include <tuple>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>


using namespace std::literals;

void F1(){
    std::list<int32_t> i_list {1,2,3,4,5,6,7,8,9,10};
    for(auto & i: i_list){
        if (i == 3){
            i = 30;
        }
    }
}
/*
Для выполнения задания создадим список чисел i_list, 
затем пройдемся по списку циклом for-each, 
используя автоматический вывод типа 
и при равенстве элемента 3, меняем его на 30.
*/

void F2(){
    std::list<int32_t> i_list {1,2,3,4,5};
    for(auto & i: i_list){
        i *= i;
    }
}

/*
Для выполнения задания создадим список i_list из любых чисел
и пройдем по нему циклом for-each, при этом умножая каждый элемент на себя.
*/

void F3(){
    std::list<int32_t> i_list {1,23,56,83,1,5};
    auto answer = [&i_list](){
        int32_t big_int = 0;
        int32_t count_size = 0;
        for(const auto& i: i_list){
            ++count_size;
            if(i > big_int){
                big_int = i;
            }
        }
        return big_int / count_size;
    }();
    std::cout << answer;
}

/*
Для выполения задания создаем список чисел i_list а потом...
Мы создаем две переменные: big_int и count_size, затем проходимся циклом по всем элементам в списке
при этом конкатенируем count_size и если переменная больше big_int, присваиваем значение этой переменной
big_int. После полного прохода по элементам i_list присваиваем перменной answer значение big_int деленное
на count_size. Для инициализации answer используем lamda функцию.
*/
class Value{
public:
    template<typename T>
    Value(T value):str(std::move(value)){}

    Value(double value):num1(value){}
    Value(int value):num2(value){}
    Value(char value):num3(value){}

    Value(const Value& other){
        str = other.str;
        num1 = other.num1;
        num2 = other.num2;
        num3 = other.num3;
    }

    Value(Value&& other){
        str = std::move(other.str);
        num1 = std::move(other.num1);
        num2 = std::move(other.num2);
        num3 = std::move(other.num3);
    }

    Value& operator= (const Value& other){
        Value temp(other);
        Swap(temp);
        return *this;
    }

    Value& operator= (Value&& other){
        Value temp(std::move(other));
        Swap(temp);
        return *this;
    }

    bool IsStr()const{return str.has_value();}
    bool IsDouble()const{return num1.has_value();}
    bool IsInt()const{return num2.has_value();}
    bool IsChar()const{return num3.has_value();}
    bool IsScalar()const{return !IsChar() && !IsStr();}

    const std::string& AsStr()const{return *str;}
    const double& AsDouble()const{return *num1;}
    const int& AsInt()const{return *num2;}
    const char& AsChar()const{return *num3;}
    double AsScalar()const{
        if(IsInt()){
            return *num2;
        }else{
            return *num1;
        }
    }

    bool operator< (const Value& other)const{
        if(other.IsScalar() && other.IsScalar()){
            return AsScalar() < other.AsScalar();
        }
        return false;
    }

    Value& Swap(Value& other){
        std::swap(str,other.str);
        std::swap(num1,other.num1);
        std::swap(num2,other.num2);
        std::swap(num3,other.num3);
        return *this;
    }

private:
    std::optional<std::string> str;
    std::optional<double> num1;
    std::optional<int> num2;
    std::optional<char> num3;
};

std::ostream& operator<<(std::ostream& os, const Value& val){
    if(val.IsStr()){
        os << val.AsStr();
    }else if(val.IsDouble()){
        os << val.AsDouble();
    }else if(val.IsInt()){
        os << val.AsInt();
    }else{
        os << val.AsChar();
    }
    return os;
}

void F4(){
    std::vector<Value> my_vector{1,2.3,3.2,5,1,4};

    bool IsOnlyScalar = true;
    for(const auto& val: my_vector){
        if(!val.IsScalar()){
            IsOnlyScalar = false;
            break;
        }
    }

    if(IsOnlyScalar && my_vector.size() != 1){
        bool is_swapped = true;
        while(is_swapped){
            is_swapped = false;
            for(size_t i = 1; i != my_vector.size();++i){
                if(my_vector.at(i) < my_vector.at(i-1)){
                    my_vector[i].Swap(my_vector[i-1]);
                    is_swapped = true;
                }
            }
        }
    }

    for(const auto& val: my_vector){
        std::cout << val << ' ';
    }
}

/*
Для выполнения работы создадим класс Value в котором будем хранить 4 переменных через std::optional, определяем функции, которые проверяют
содержится ли в этих переменных какие-то числа или строки или символ, и методы, которые будут возвращать ссылки на хранящееся значение, затем 
создадим вектор с Value и заполним его значениями. После этото проходимся циклом по всем значениям вектора  my_vector и, если метод IsScalar 
возвращает для всех значений true, сортируем векрот пузырьковой сортировкой, затем выводим значения в поток.
*/

struct Stats{
    std::string name;
    int32_t cost;
};

struct Comparator{
    bool operator()(const auto& lhs, const auto& rhs){
        return lhs.cost < rhs.cost;
    }
};

void F5(){
    std::vector<Stats> products{
        {"Булочка",32},{"Пельмень",54},{"Газировочка",15},{"Кулич",1},{"Огурец",6}
    };
    std::ranges::sort(products,Comparator{});
    std::pair<std::string,std::string> min_max = {products.begin()->name,products.rbegin()->name};
}
/*
Для выполнения задания создадим структуру Stats с именем и ценой продукта, затем определим условие Comparator при котором объект Stats меньше другого, 
если меньше значение цены. Наконец создаем вектор продуктов products и сортируем его, используя Comparator. Возьмем значение с начачала вектора и с конца,
это будут соответственно продукты с меньшей и большей ценой.
*/

void F6(){
    std::list<std::string> words;
    std::map<std::string,std::string> words_map;

    for(const std::string& str: words){
        words_map[str] = str;
    }
}
/*
Для выполнения задания имеем заготовленный список слов words, создадим карту с ключами - словом, 
значенимем - то же словом и спомощью цикла заполним одинаковыми словами.
*/
void F7(){
    std::string elem_name;
    std::map<std::string,std::string> trans_en_ru{{"cat","кошка"},{"dog","собака"},{"frog","лягушка"}};
    std::map<std::string,std::string> trans_ru_en;
    for(auto[word,trans] : trans_en_ru){
        trans_ru_en[trans] = word;
    }

    std::cin >> elem_name;
    if(trans_ru_en.contains(elem_name)){
        std::cout << trans_ru_en.at(elem_name);
    }else{
        std::cout << "Нет такого слова";
    }
}
/*
Для выполнения задания создадим карту с ключами - словами на русском, значениями - их переводом. Затем примем ввод слова с клавиатуры, проверим 
есть ли такое слово в словаре, и либо выведем слово, либо строчку "Нет такого слова".
*/

void F8(){
    std::string first_partner;
    std::unordered_map<std::string,std::set<std::string>> game_rules{
        {"Камень",{"Ящерица","Ножницы"}},
        {"Ножницы",{"Бумага","Ящерица"}},
        {"Бумага",{"Камень","Спок"}},
        {"Ящерица",{"Бумага","Спок"}},
        {"Спок",{"Камень","Ножницы"}},
    };
    std::cin >> first_partner;
    if(!game_rules.contains(first_partner)){
        std::cout << "Такого игрока нет" << std::endl;
        return;
    }
    std::string second_partner = [&](){
        uint32_t counter = 0;
        uint32_t random = static_cast<uint32_t>(std::rand()) % static_cast<uint32_t>(game_rules.size());
        std::string partner;
        for(const auto& key: std::views::keys(game_rules)){
            if(counter == random){
                partner = key;
            }else{
                ++counter;
            }
        }
        return partner;  
    }();

    if(first_partner == second_partner){
        std::cout << "Ничья\n";
    }else if(game_rules.at(first_partner).contains(second_partner)){
        std::cout << first_partner << " Побеждает " << second_partner << '\n';
    }else if(game_rules.at(second_partner).contains(first_partner)){
        std::cout << second_partner << " Побеждает " << first_partner << '\n';
    }
}
/*
Для выполнения задания создадим карту с правилами игры, где ключем будет имя игрока, который побеждает любого из списка значений.
Затем примем ввод имени с клавиатуры в first_partner и проверим, существует ли такой игрок, если нет - вывидем ошибку, если существует,то 
возмём остаток от деления функции rand на количество игроков, так мы пройдемся циклом по именам и выберем второго рандомного игрока и сохраним его 
в переменной second_partner. Наконец проверим есть ли в списках first_partner second_partner и наоборот, 
игрок в списке которого есть его партнёр выигрывает. Выводим результат.

*/

void F9(){
    std::list<std::string> words{"Коктель","Кино","Бандура","Дора"};
    std::map<char,std::list<std::string>> words_map;

    for(const std::string& str: words){
        words_map[*str.begin()].push_back(str);
    }
}
/*
Для выполнения задания cохраним список слов в words, создадим карту words_map где ключом бубет буква, 
а значением список слов и при помощи цикла for-each заполним карту, беря первую букву слова и добавляя слово в список.
*/

struct StudentMarks{
    std::string name;
    std::vector<int32_t> marks;
    int32_t AvarageScore(){
        int32_t score;
        for(const auto& m: marks){
            score += m;
        }
        return score / static_cast<int32_t>(marks.size());
    }
};

void F10(){
    std::vector<StudentMarks> Students_{{"Иван",{4,5,4}},{"Саша",{4,5,4}},{"Настя",{4,5,4}}};
    auto good_student = Students_.begin();
    for(auto it = std::next(good_student); it != Students_.end();++it){
        if(good_student->AvarageScore()< it->AvarageScore()){
            good_student = it;
        }
    }
    std::cout << good_student->name << " получила наивысший средний бал: " << good_student->AvarageScore();
}
/*
Для выполнения задания создадим структуру StudentMarks, которая будет содержать имя, оценки и функцию для вывода среднего бала AvarageScore.
Затем создаем вектор StudentMarks с именами и оценками студентов, сохраняем итератор на первый элемент вектора и проходимся циклом по следующим
и сравниваем их, наибольший по среднему балу оставляем в переменной good_student. В конце выводим имя и средний бал лучшего студента.
*/

int main(){
    setlocale(LC_ALL,"ru_Ru.UTF-8");
    F4();
}