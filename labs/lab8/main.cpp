#include "Scenarios.hpp"


#include <iostream>
#include <windows.h>
#include <fstream>

using namespace std::string_literals;
using namespace spec_struct;
using namespace world;
using namespace io_func;
using namespace scenarios;

int main(){
    setlocale(LC_ALL,"ru-Ru.cp1251");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Person my_person;

    Inventory my_inventory;
    my_person.SetInventory(my_inventory);

    World my_world(8,8);
    
    std::pair<int32_t,int32_t> my_position{0,0};
    int32_t world_level = 1;

    std::ifstream saving_input("game_save.json");

    if(saving_input.is_open()) {
        std::cout << "Хотите загрузить прошлое сохранение?\n"
        << "1 Да\n"
        << "2 Нет\n";

        auto answer = CircleCall(GetAnswerNum,std::cin,std::cout,"1-2");

        switch (answer) {
            case 1:
                scenarios::DownLoad(saving_input,my_world,my_inventory,my_person,my_position,world_level);
                break;
            case 2:
                break;
        }
    }

    Initialization(my_person);
    CreatingWorld(my_world);

    bool is_gaming = true;
    while(is_gaming){

        switch(my_world.GetRoomType(my_position.first,my_position.second)){
            case world::RoomType::enemy:
                WriteBorder();

                EnemyBattle(my_world,my_person,is_gaming,my_position,world_level);

                break;
            case world::RoomType::empty:
                WriteBorder();

                EmptyRoom(my_world,my_person,my_position,world_level,is_gaming);

                break;
            case world::RoomType::treasure:
                WriteBorder();

                TreasureRoom(my_person,my_world,my_position);

                break;
            case world::RoomType::exit:
                WriteBorder();

                ExitRoom(world_level,my_position,is_gaming,my_world);

                break;
            default:
                break;
        }
            
    }
}