#pragma once

#include "SpecialStructures.hpp"
#include "IOFunctions.hpp"
#include "JsonReader.hpp"
#include "World.hpp"

namespace scenarios {
    using spec_struct::Person;
    using world::World;
    using spec_struct::Inventory;

    void Initialization(Person& person,std::ostream& os = std::cout);

    void CreatingWorld(World& world);

    void UseItem(Person& person, int32_t id, std::ostream&);

    void PutDownItem(Person& person, World& world, int32_t id, std::pair<int32_t,int32_t>& pos);

    void PickUpItem(Person& person, World& world, std::pair<int32_t,int32_t>& pos);

    void UseInventory(Person& person, World& world,std::pair<int32_t,int32_t>& pos);

    std::string GetHint(world::RoomType type);

    void ChooseRoom(world::World& world, std::pair<int32_t,int32_t>& pos);

    void DistributeExp(Person& hero);

    void EnemyBattle(World& world,
        Person& hero, bool& is_g,std::pair<int32_t,int32_t>& pos,int32_t);

    void Save(World& world, Person& hero,std::pair<int32_t,int32_t> pos, int32_t world_level);

    void EmptyRoom(World& world, Person& hero, std::pair<int32_t,int32_t>& pos, int32_t lvl,bool& is_gm);

    void TreasureRoom(Person& person,World& world, std::pair<int32_t,int32_t>& pos);

    void ExitRoom(int32_t level,std::pair<int32_t,int32_t>& pos,bool& is_g,world::World& world);

    void DownLoad(std::ifstream& is,World& world,Inventory& inventory,Person& hero,std::pair<int32_t,int32_t>& pos, int32_t& world_level);


}