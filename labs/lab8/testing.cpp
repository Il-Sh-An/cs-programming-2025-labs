#include "testing.hpp"

#include <cassert>
#include <fstream>

using namespace spec_struct;
using namespace world;

namespace testing {
    void TestInventory() {
        Inventory inventory;
        assert(inventory.size() == 0);

        auto shield = inventory.GetShield();

        assert(!shield.has_value());

        Item item;
        item.SetName("shield").SetItemType(ItemType::Shield);

        inventory.AddItem(std::move(item));
        shield = inventory.GetShield();

        assert(!shield.has_value());
        assert(inventory.size() == 1);

        inventory.SetShield(0);

        assert(shield.has_value());
        assert(inventory.size() == 0);

        
    }
    void TestWorld() {

        World world(4,4);

        assert(world.GetRoomType(0,0) != RoomType::empty);
        assert(world.GetRoomType(3,3) != RoomType::exit);

        assert(!world.IsAvaliableRoom(4,4));
        assert(!world.IsAvaliableRoom(0,3));

        auto items = world.GetItems(0,0);
        auto enemy = world.GetEnemy(0,0);

        assert(!items.has_value());
        assert(!enemy.has_value());
    }
}