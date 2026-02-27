#include "Scenarios.hpp"

#include <fstream>
#include <tuple>

namespace scenarios {

    using namespace spec_struct;

    using namespace world;
    using namespace io_func;
    using namespace json_reader;

    void Initialization(Person& person,std::ostream& os) {

        std::cout << "Привет, путник, ты попал в подземелье, тебе нужно выбраться. Попробуй.\n";
        std::cout << "Ввберите тип персонажа:\n"
        "1.Человек\n"
        "2.Дворф\n"
        "3.Эльф\n"
        "4.Гигант\n"
        "Введите одно число от 1 до 4\n";

        auto answer = CircleCall(GetAnswerNum,std::cin,std::cout,"1-4");

        person.SetCharacters(static_cast<PersonType>(answer));

        std::cout << "Вы теперь: " << PrintPersonType(person.GetType()) << '\n';

        std::cout << "Введите своё имя:\n";

        auto name = CircleCall(GetAnswerStr,std::cin,os,"!@#$%^&*()");

        person.SetName(name);

        std::cout << '\n';
        person.Print(std::cout);
        std::cout <<'\n';

        std::cout << '\n';
        std::cout << "Добро пожаловать\n"
        << "Вы очутились в мрачной комнате\n";
        std::cout << std::endl;
    }

    void CreatingWorld(World& world) {
        std::ifstream in("GameItemsEnemy.json");

        Node items_enemy;
        in >> items_enemy;

        auto& ie_map = items_enemy.AsMap();

        std::vector<Item> items;
        std::vector<Enemy> enemy;

        items.reserve(ie_map.at("items").AsArray().size());
        enemy.reserve(ie_map.at("enemies").AsArray().size());

        for(const Node& elem: ie_map.at("items").AsArray()) {
            auto& elem_ref = elem.AsMap();

            auto [name, mod_type, item_type, mod] = std::tuple{
                elem_ref.at("name").AsString(),
                elem_ref.at("mod_type").AsInt(),
                elem_ref.at("item_type").AsInt(),
                elem_ref.at("mod").AsInt()
            };

            Item item_;
            item_.SetName(std::move(name)).SetModType(static_cast<ItemModType>(mod_type)).
                                SetItemType(static_cast<ItemType>(item_type)).SetModificator(mod);

            items.emplace_back(std::move(item_));
        }

        for(const Node& elem: ie_map.at("enemies").AsArray()) {
            auto& elem_ref = elem.AsMap();

            auto [name, attack, defence, hp] = std::tuple{
                elem_ref.at("name").AsString(),
                elem_ref.at("attack").AsInt(),
                elem_ref.at("defense").AsInt(),
                elem_ref.at("hp").AsInt()
            };

            Enemy enemy_;
            enemy_.SetName(std::move(name)).SetAttack(attack).SetDefense(defence).SetHp(hp);

            enemy.emplace_back(std::move(enemy_));
        }

        world.GetItemsFromFile(std::move(items));
        world.GetEnemyFromFile(std::move(enemy));

        in.close();
    }
//Специальная функция для увеличения урона, если оружие в слоте weapon

    bool ModificateIf(Inventory& inventory,int32_t mod,std::ostream& os) {
        auto weapon_mod = inventory.GetWeapon();

        if(weapon_mod.has_value()) {
            weapon_mod->get().SetModificator(weapon_mod->get().GetMod() + mod);
            os << "Ваше оружие смертоносно\n";
            return true;
        }else {
            os << "Предмет нельзя использовать\n";
            return false;
        }
    }

    void UseItem(Person& person, int32_t id, std::ostream& os) {
        auto& inventory = person.GetModInventory();

        auto item = inventory.GetItem(id);

        if(!item){
            os << "Предмет нельзя использовать\n";
        }

        switch (item->get().GetItemType()) {
            case ItemType::Potion:
                switch(item->get().GetModType()){
                    case ItemModType::Attack:
                        if(ModificateIf(inventory,item->get().GetMod(),os)){
                            inventory.RemoveItem(id);
                        }
                        break;
                    case ItemModType::Hp:
                        person.ModHp(item->get().GetMod());
                        inventory.RemoveItem(id);
                        break;
                    case ItemModType::Agility:
                        person.ModAgility(item->get().GetMod());
                        inventory.RemoveItem(id);
                        break;
                    case ItemModType::Defense:
                        person.ModDefense(item->get().GetMod());
                        inventory.RemoveItem(id);
                        break;
                    default:
                        os << "Предмет нельзя использовать\n";
                        break;
                }
                break;
            case ItemType::Armor:
                inventory.SetArmor(id);
                break;
            case ItemType::Shield:
                inventory.SetShield(id);
                break;
            case ItemType::Weapon:
                inventory.SetWeapon(id);
                break;
            default:
                os << "Предмет нельзя использовать\n";
        }
    }

    void PutDownItem(Person& person, World& world, int32_t id, std::pair<int32_t,int32_t>& pos) {
        auto& inventory = person.GetModInventory();
        auto& item = inventory.GetItem(id)->get();
        world.AddItemToRoom(pos.first,pos.second,std::move(item));
        inventory.RemoveItem(id);
    }

    void PickUpItem(Person& person, World& world, std::pair<int32_t,int32_t>& pos) {
        auto& inventory = person.GetModInventory();

        auto items_or = world.GetItems(pos.first,pos.second);
        if(!items_or) {
            std::cout << items_or.error() << '\n';
            return;
        }
        auto& items = items_or.value().get();

        bool is_exit = false;
        while(!is_exit){
            if(items.empty()){
                return;
            }

            std::cout <<'\n';

            int32_t counter = 0;
            for(const auto& i: items){
                std::cout << ++counter << ' ' << i.GetName() << '\n';
            }
            std::cout << '\n' << counter + 1 << ' ' << "Выйти\n";

            std::cout << "Введите номер предмета:\n";

            auto answer = CircleCall(GetAnswerNum,std::cin,std::cout,"1-" + std::to_string(counter + 1));
            WriteBorder();

            if(answer == counter + 1){
                is_exit = true;
                return;
            }

            Item temp = items.at(static_cast<uint64_t>(answer-1));

            if(inventory.HasSpace()){
                
                inventory.AddItem(std::move(temp));
                items.erase(items.begin() + answer - 1);
            }else{
                inventory.Print(std::cout);        
                std::cout << "Нет места, укажите номер предмета, чтобы его выкинуть:\n";

                auto answer2 = CircleCall(GetAnswerNum,std::cin,std::cout,"1-" + std::to_string(inventory.size()));

                items.push_back(inventory.GetItem(answer2)->get());
                inventory.RemoveItem(answer2);
                inventory.AddItem(std::move(temp));
                
            }
        }
        
    }

    void UseInventory(Person& person, World& world,std::pair<int32_t,int32_t>& pos) {
        auto & inventory = person.GetModInventory();

        bool exit = false;
        while(!exit){
            std::cout << "Выберите предмет для использования:\n";
            inventory.Print(std::cout);
            uint64_t inv_size = inventory.size();
            std::cout << inv_size + 1 << " " << "Выйти\n";
            std::cout << "Введите номер:\n";

            auto num = CircleCall(
                GetAnswerNum,std::cin, std::cout, "1-" + std::to_string(inv_size + 1)
            );

            if(static_cast<uint64_t>(num) == inv_size + 1){
                exit = true;
                continue;
            }

            std::cout << "Что делаем?\n"
                << "1 Использовать\n"
                << "2 Выкинуть\n";
            
            auto command = CircleCall(GetAnswerNum,std::cin,std::cout,"1-2");

            switch(command){
                case(1):
                    UseItem(person, num - 1,std::cout);
                    break;
                case(2):
                    PutDownItem(person,world,num - 1,pos);
                    break;
            }

        }
    }

    std::string GetHint(world::RoomType type) {
        int32_t p = std::rand() % 10;
        std::string temp;
        if(p > 5){
            switch(type){
                case world::RoomType::enemy:
                    temp = "Впереди пахнет кровью";
                    break;
                case world::RoomType::treasure:
                    temp = "Мм... Звук монет";
                    break;
                case world::RoomType::empty:
                    temp =  "Ничего";
                    break;
                case world::RoomType::exit:
                    temp =  "Выход";
                    break;
                default:
                    temp = "Не знаю";
                    break;
            }
        }else{
            temp = "???";
        }
        return temp;
    }

    void ChooseRoom(world::World& world, std::pair<int32_t,int32_t>& pos) {
        std::vector<std::pair<int32_t,int32_t>> points_temp{
            {pos.first + 1,pos.second}, {pos.first - 1,pos.second},
            {pos.first,pos.second + 1}, {pos.first,pos.second - 1}
        };

        std::vector<std::pair<int32_t,int32_t>> points;
        points.reserve(points_temp.size());

        for(auto it = points_temp.begin(); it != points_temp.end(); ++it){
            if(world.IsAvaliableRoom(it->first,it->second)){
                points.push_back(*it);
            }
        }

        int32_t counter = 0;
        for(const auto& [first,second]: points){
            std::cout << ++counter << ' ' << GetHint(world.GetRoomType(first,second)) << '\n';
        }

        std::cout << "Введите число с номером комнаты:\n";

        auto answer = CircleCall(GetAnswerNum,std::cin,std::cout,"1-" + std::to_string(counter));

        pos = points.at(static_cast<uint64_t>(answer-1));
    }

    void DistributeExp(Person& hero) {
        auto& points = hero.GetPointers();

        if(points == 0) {
            std::cout << "У вас не хватает опыта\n";
            return;
        }
        hero.Print(std::cout);
        std::cout << "\n\n";

        std::cout << "У вас " << points << " очков\n";
        std::cout << "Введите три цифры в сумме должны быть не болеше " << points;

        auto answer = CircleCall(GetAnswerNums,std::cin,std::cout,"3 " + std::to_string(points));

        points = points - answer.at(0) - answer.at(1) - answer.at(2);

        hero.ModAttack(answer.at(0));
        hero.ModDefense(answer.at(1));
        hero.ModAgility(answer.at(2));

        std::cout << "Вы повысили характеристики\n";
        
    }
// Доп функции для атаки
    void EnemyAttack(Person& hero,Enemy& enemy) {
        hero.ModHp(-(std::max((int32_t)0,enemy.GetAttack() - 
        (hero.GetDefense() + hero.GetModInventory().GetDefenseMod()))));
    }

    void HeroAttack(Person& hero,Enemy& enemy) {
        int32_t attack = hero.GetAttack();

        if(hero.GetModInventory().GetAttackMod() != 0){
            attack = hero.GetModInventory().GetAttackMod();
        }

        enemy.ModificateHp(-(std::max(0, attack - enemy.GetDefense())));
    }

    void PrintCharacters(Person& hero,Enemy& enemy) {
        WriteBorder();
        enemy.Print(std::cout);
        std::cout << '\n';

        int32_t attack = hero.GetAttack();
        if(hero.GetModInventory().GetAttackMod() != 0){
            attack = hero.GetModInventory().GetAttackMod();
        }
        WriteBorder();
        std::cout << "Имя " << hero.GetName() << ", Здоровье: " << hero.GetHp() << '\n'
            << "Атака: " <<  attack << " , "
             << "Защита : " << hero.GetDefense() + hero.GetModInventory().GetDefenseMod() << '\n';
        WriteBorder();
    }

    void EnemyTurn(Person& person,Enemy& enemy) {
        PrintCharacters(person,enemy);
        std::cout << '\n'
        << "На вас нападает " + enemy.GetName() << '\n'
        << "Что будете делать:\n"
        << "1 Попытаться уклониться \n"
        << "2 Попытаться парировать\n"
        << "Введите 1 или 2\n";


        auto answer = CircleCall(GetAnswerNum,std::cin,std::cout,"1-2");

        switch(answer) {
            case 1:
                if(std::rand() % 50 + person.GetAgility() > 25) {
                    std::cout << "Вы уклонились\n";
                    break;
                }else {
                    EnemyAttack(person,enemy);
                    std::cout << "Вы теряете здоровье\n";
                    break;
                }   
            case 2:
                if(std::rand() % 100 + person.GetAgility()  > 75) {
                    HeroAttack(person,enemy);
                    std::cout << "Вы спарировали\n";
                    break;
                }else {
                    EnemyAttack(person,enemy);
                    std::cout << "Вы теряете здоровье\n";
                    break;
                }  
        }
    }

    void HeroTurn(World& world,Person& person,Enemy& enemy,std::pair<int32_t,int32_t>& pos) {
        PrintCharacters(person,enemy);

        std::cout << '\n'
        << "Вы нападаете!" << '\n'
        << "Что будете делать:\n"
        << "1 Ударить\n"
        << "2 Использовать инвентарь\n"
        << "Введите 1 или 2:\n";

        auto answer = CircleCall(GetAnswerNum,std::cin,std::cout,"1-2");
        WriteBorder();

        switch(answer) {
            case 1:
                if(std::rand() % 50 + person.GetAgility() > 25) {
                    HeroAttack(person,enemy);
                    std::cout << "Вы успешено бьёте\n";
                    break;
                }else {
                    std::cout << "Вы промахнулись\n";
                    break;
                }   
            case 2:
                UseInventory(person,world,pos);
                break;
        }
    }

    void EnemyBattle(World& world,
        Person& hero, bool& is_g,std::pair<int32_t,int32_t>& pos,int32_t world_lvl) {
        std::cout << "Вы входите в комнату, впереди противник\n\n";

        int32_t a = pos.first;
        int32_t b = pos.second;

        auto& enemy = world.GetEnemy(a,b).value().get();

        int32_t enemy_lvl = std::rand() % 2 + hero.GetLevel();
        enemy.ModificateForLvl(enemy_lvl + world_lvl);

        while(enemy.GetHp() > 0){
            HeroTurn(world,hero,enemy,pos);
            if(enemy.GetHp() == 0) {
                break;
            }
            EnemyTurn(hero,enemy);

            if(hero.GetHp() <= 0){
                std::cout << "Вы умерли\n";
                is_g = false;
                return;
            }
        }
        std::cout << "Враг умер! Из него что-то выпало.\n";

        hero.ModExp(enemy_lvl * 50);

        Item item;
        item.SetName("Зелье лечения").SetModType(ItemModType::Hp)
        .SetModificator(8).SetItemType(ItemType::Potion);

        world.AddItemToRoom(a,b,std::move(item));
        world.ChangeRoomType(a,b,world::RoomType::empty);
    }

    void DownLoad(std::ifstream& is,World& world,Inventory& inventory,
             Person& hero,std::pair<int32_t,int32_t>& pos, int32_t& world_level) {

        Node settings_node;
        is >> settings_node;

        auto& settings = settings_node.AsMap();

        world.Upload(settings.at("world"));
        hero.Upload(settings.at("person"));
        inventory.Upload(settings.at("inventory"));

        hero.SetInventory(inventory);

        pos = {settings.at("pos").AsArray().at(0).AsInt(),
                        settings.at("pos").AsArray().at(1).AsInt()};

        world_level = settings.at("world_level").AsInt();
    }

    void Save(World& world, Person& hero,std::pair<int32_t,int32_t> pos, int32_t world_level) {
        std::ofstream os("game_save.json");
        
        Node::Map saving_node;

        saving_node.emplace("world",world.DownLoad());
        saving_node.emplace("person",hero.DownLoad());
        saving_node.emplace("inventory",hero.GetModInventory().DownLoad());

        Node::Array size{Node(pos.first),Node(pos.second)};

        saving_node.emplace("pos",std::move(size));

        saving_node.emplace("world_level",Node(world_level));

        os << saving_node;
    }

    void EmptyRoom(World& world, Person& hero, std::pair<int32_t,int32_t>& pos, int32_t lvl,bool& is_gm) {

        std::cout << "Комната пуста, что хотите сделать?:\n";
        std::cout << "1 Использовать инвентарь\n"
            << "2 Выбрать комнату\n"
            << "3 Поискать предметы\n"
            << "4 Распределить очки\n"
            << "5 Сохранить и выйти\n";

            std::cout << "Введите нужную цифру:\n";

        auto answer = CircleCall(GetAnswerNum,std::cin,std::cout,"1-5");
        WriteBorder();
        
        switch(answer){
            case 1:
                UseInventory(hero,world,pos);
                break;
            case 2:
                ChooseRoom(world,pos);
                break;
            case 3:
                PickUpItem(hero,world,pos);
                break;
            case 4:
                DistributeExp(hero);
                break;
            case 5:
                Save(world,hero,pos,lvl);
                std::cout << "До свидания!\n";
                is_gm = false;
                break;
        }

    }

    void TreasureRoom(Person& person,World& world, std::pair<int32_t,int32_t>& pos) {
        std::cout << "Вы вошли в комнату, видите сундук и открываете его...\n";
        PickUpItem(person,world,pos);
        world.ChangeRoomType(pos.first,pos.second,world::RoomType::empty);
    }

    void ExitRoom(int32_t level,std::pair<int32_t,int32_t>& pos,bool& is_g,world::World& world) {
        if (level == 2){
            is_g = false;
            std::cout << "Вы прошли подземелье\n";
            return;
        }else{
            pos = {0,0};
            CreatingWorld(world);
        }
    }

}