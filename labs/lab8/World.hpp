#pragma once

#include <vector>
#include <cstdint>
#include <utility>
#include <unordered_map>

#include "SpecialStructures.hpp"

namespace world {
    using namespace spec_struct;

    enum class RoomType: int8_t {none,exit,enemy,empty,treasure};

    std::ostream& operator<< (std::ostream&,RoomType);

    class World {
    public:

        World(int32_t,int32_t);

        RoomType GetRoomType(int32_t,int32_t) const;

        std::expected<std::reference_wrapper<Enemy>,std::string>
        GetEnemy(int32_t,int32_t);

        std::expected<std::reference_wrapper<std::vector<Item>>,std::string>
        GetItems(int32_t,int32_t);
        
        void ChangeRoomType(int32_t,int32_t,RoomType);
        void ChangeRooms();

        World& AddItemToRoom(int32_t, int32_t,Item&&);

        World& GetItemsFromFile(std::vector<Item>&& );
        World& GetEnemyFromFile(std::vector<Enemy>&& );

        bool HaveItems(int32_t,int32_t)const;
        bool IsAvaliableRoom(int32_t,int32_t)const;

        void Upload(json_reader::Node);
        json_reader::Node DownLoad() const;
    private:
        int32_t GetNumber(int32_t,int32_t)const;
        std::pair<int32_t,int32_t> size_;
        std::vector<RoomType> world_map_;
        std::unordered_map<int32_t,Enemy> enemy_map_;
        std::unordered_map<int32_t,std::vector<Item>> item_map_;
    };

}