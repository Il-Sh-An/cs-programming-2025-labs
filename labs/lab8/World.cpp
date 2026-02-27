#include "World.hpp"

#include <algorithm>
#include <random>
#include <string>

using namespace json_reader;

namespace world{
    std::ostream& operator<< (std::ostream& os,RoomType type) {
        os << static_cast<int8_t>(type);
        return os;
    }

    void World::ChangeRooms() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(world_map_.begin() + 1, world_map_.end() - 1,g);
    }

    World::World(int32_t a ,int32_t b) {
        world_map_.reserve(static_cast<uint64_t>(a*b));
        int32_t all_elem = a * b;
        int32_t per_25 = all_elem * 25 / 100;
        int32_t per_75 = all_elem  - per_25;
        int32_t per_75_2 = per_75 / 2;

        for(int32_t i = 0; i < per_25 - 1; ++ i){
            world_map_.push_back(RoomType::empty);
        }
        for(int32_t i = 0; i < per_75_2; ++i){
            world_map_.push_back(RoomType::enemy);
            world_map_.push_back(RoomType::treasure);
        }
        
        world_map_.push_back(RoomType::exit);
        
        size_.first = a;
        size_.second = b;

        ChangeRooms();
    }

    RoomType World::GetRoomType(int32_t a,int32_t b) const {
        if(IsAvaliableRoom(a,b)){
            return world_map_.at(static_cast<uint64_t>(a* size_.first + b));
        }else{
            return RoomType::none;
        }
    }

    std::expected<std::reference_wrapper<Enemy>,std::string>
    World::GetEnemy(int32_t a,int32_t b) {
        if(!IsAvaliableRoom(a,b)){
            throw BadChoise("Нет такой комнаты");
        }
        if(enemy_map_.contains(GetNumber(a,b))){
            return enemy_map_.at(GetNumber(a,b));
        }else{
            return std::unexpected("Никого нет");
        }
    }

    std::expected<std::reference_wrapper<std::vector<Item>>,std::string>
    World::GetItems(int32_t a,int32_t b) {
        if(!IsAvaliableRoom(a,b)){
            return std::unexpected("Такой комнаты нет");
        }
        if(item_map_.contains(GetNumber(a,b))) {
            return item_map_.at(GetNumber(a,b));
        }else {
            return std::unexpected("Ничего нет");
        }
    }

    int32_t World::GetNumber(int32_t a,int32_t b)const{
        return a * size_.second + b;
    }

    void World::ChangeRoomType(int32_t a,int32_t b,RoomType type){
        auto& room = world_map_.at(static_cast<uint64_t>(GetNumber(a,b)));
        if(room != type){
            room = type;
        }
    }

    World& World::GetItemsFromFile(std::vector<Item>&& items_){

        for(uint64_t i = 0; i != world_map_.size(); ++i){
            if(world_map_.at(i) == RoomType::treasure){
                std::vector<Item> room_items_;
                for(int32_t a = 0; a < 3; ++a){
                    room_items_.push_back(items_[static_cast<uint64_t>(std::rand()) % items_.size()]);
                }

                item_map_[static_cast<int32_t>(i)] = std::move(room_items_);
            }
        }
        return *this;
    }

    World& World::GetEnemyFromFile(std::vector<Enemy>&& enemies_){

        for(uint64_t i = 0; i != world_map_.size(); ++i){
            if(world_map_.at(i) == RoomType::enemy){
                uint64_t id = static_cast<uint64_t>(std::rand()) % enemies_.size();

                enemy_map_[static_cast<int32_t>(i)] = enemies_[id];
            }
        }
        return *this;
    }

    bool World::HaveItems(int32_t a,int32_t b)const{
        return item_map_.contains(GetNumber(a,b));
    }

    bool World::IsAvaliableRoom(int32_t a,int32_t b)const{
        return a < size_.first && a >= 0 && b >= 0 && b < size_.second;
    }

    void World::Upload(Node node) {
        auto& settings = node.AsMap();

        auto& size = settings.at("size").AsArray();

        size_ = {size.at(0).AsInt(),size.at(1).AsInt()};

        auto& world_map = settings.at("world_map").AsArray();

        for(uint64_t id = 0; id != world_map.size(); ++id){
            world_map_.at(id) = static_cast<RoomType>(world_map.at(id).AsInt());
        }

        auto& enemy_map = settings.at("enemy_map").AsMap();
        enemy_map_.clear();

        for(const auto& [key,enemy]: enemy_map) {
            Enemy enemy_temp;
            enemy_temp.Upload(enemy);

            enemy_map_.emplace(stoi(key),std::move(enemy_temp));
        }

        auto& item_map = settings.at("item_map").AsMap();
        item_map_.clear();

        for(const auto& [key,items]: item_map) {
            std::vector<Item> items_temp;
            items_temp.reserve(items.AsArray().size());

            for(const auto& i: items.AsArray()) {
                Item item_temp;
                item_temp.Upload(i);
                items_temp.emplace_back(std::move(item_temp));
            }

            item_map_.emplace(stoi(key),std::move(items_temp));
        }

    }
    Node World::DownLoad() const {
        Node::Map map_;

        Node::Array size;
        size.emplace_back(size_.first);
        size.emplace_back(size_.second);

        map_.emplace("size",std::move(size));

        Node::Array world_map;
        world_map.reserve(world_map_.size());

        for(const auto& num: world_map_) {
            world_map.push_back(Node(static_cast<int32_t>(num)));
        }

        map_.emplace("world_map",std::move(world_map));

        Node::Map enemy_map;

        for(const auto& [num,enemy]: enemy_map_) {
            enemy_map.emplace(std::to_string(num),enemy.DownLoad());
        }

        map_.emplace("enemy_map",std::move(enemy_map));

        Node::Map item_map;

        for(const auto& [key,elems] : item_map_){
            Node::Array arr;

            for(const auto& elem: elems) {
                arr.emplace_back(elem.DownLoad());
            }

            item_map.emplace(std::to_string(key),std::move(arr));
        }

        map_.emplace("item_map",std::move(item_map));

        return Node(std::move(map_));
    }

    World& World::AddItemToRoom(int32_t a, int32_t b,Item&& item) {
        item_map_[GetNumber(a,b)].push_back(std::move(item));
        return *this;
    }
    
}