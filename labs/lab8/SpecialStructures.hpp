#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include <iostream>
#include <vector>
#include <optional>
#include <functional>

#include "JsonReader.hpp"

namespace spec_struct{
    class Inventory;
    class Person;
    class Item;

    struct BadInput{
        BadInput(std::string&&);
        std::string exeption;
        std::string name();
    };

    struct BadParse: BadInput {
        BadParse(std::string&&);
    };

    struct BadChoise: BadInput {
        BadChoise(std::string&&);
    };

    struct FullInventory: BadInput {
        FullInventory(std::string&&);
    };

    struct Exit: BadInput {
        Exit(std::string&&);
    };

    enum class PersonType: int8_t {none,Human, Dworf, Elf, Giant};

    std::istream& operator>>(std::istream&,PersonType&);

    std::string PrintPersonType(PersonType);

    class Person {

    public:
        Person() = default;
        const std::string& GetName()const;
        PersonType GetType()const;
        int32_t GetExp()const;
        int32_t GetAttack()const;
        int32_t GetDefense()const;
        int32_t GetAgility()const;
        int32_t GetHp()const;
        int32_t GetLevel()const;
        int32_t& GetPointers();

        Inventory& GetModInventory();

        Person& SetName(std::string);
        Person& SetHeight(int32_t);
        Person& SetWeight(int32_t);
        Person& SetType(PersonType);
        Person& SetAttack(int32_t);
        Person& SetDefense(int32_t);
        Person& SetAgility(int32_t);
        Person& SetHp(int32_t);
        Person& SetMaxHp(int32_t);
        Person& SetInventory(Inventory&);
        Person& SetCharacters(PersonType);
        
        Person& ModExp(int32_t);
        Person& ModAttack(int32_t);
        Person& ModDefense(int32_t);
        Person& ModAgility(int32_t);
        Person& ModHp(int32_t);

        void Print(std::ostream&)const;
        void Upload(json_reader::Node);
        json_reader::Node DownLoad() const;
        
    private:
    // ¬ычисл€ет количество опыта дл€ повышени€ уровн€
        int32_t GetNeedExpForLvl();
        
        std::string name_;
        PersonType type_;

        int32_t exp_ = 0;
        int32_t pointers_ = 0;

        int32_t attack_ = 0;
        int32_t defense_ = 0;
        int32_t agility_ = 0;

        int32_t hp_ = 0;
        int32_t level_ = 1;
        int32_t max_hp_ = 0;

        int32_t height_ = 0;
        int32_t weight_ = 0;

        Inventory* inventory_;
    };

    class Enemy {

    public:
        Enemy() = default;
    
        Enemy& SetName(std::string);
        Enemy& SetHp(int32_t);
        Enemy& SetAttack(int32_t);
        Enemy& SetDefense(int32_t);

        Enemy& ModificateHp(int32_t);
        Enemy& ModificateAttack(int32_t);
        Enemy& ModificateDefense(int32_t);
        Enemy& ModificateForLvl(int32_t);

        const std::string& GetName()const;
        int32_t GetHp()const;
        int32_t GetAttack()const;
        int32_t GetDefense()const;

        void Print(std::ostream&)const;

        void Upload(json_reader::Node);
        json_reader::Node DownLoad() const;
    private:
        std::string name_;
        int32_t attack_ = 0;
        int32_t defense_ = 0;
        int32_t hp_ = 0;

    };

    enum class ItemModType: int8_t {none, Hp, Attack, Defense, Agility};
    enum class ItemType : int8_t {none, Armor, Potion, Shield, Coin, Weapon};

    std::istream& operator>> (std::istream&,ItemModType&);
    std::ostream& operator<< (std::ostream&,ItemModType);
    std::istream& operator>> (std::istream& is,ItemType& mod);
    std::ostream& operator<< (std::ostream& os,ItemType mod);

    class Item {

    public:
        Item() = default;

        Item& SetName(std::string);
        Item& SetModType(ItemModType);
        Item& SetModificator(int32_t);
        Item& SetItemType(ItemType);

        Item& Swap(Item&);

        const std::string& GetName()const;
        ItemModType GetModType()const;
        int32_t GetMod()const;
        ItemType GetItemType()const;
        bool IsStack()const;

        bool operator== (const Item&)const;
        bool operator< (const Item&) const;

        void Upload(json_reader::Node);
        json_reader::Node DownLoad() const;
    private:
        std::string name_;
        ItemModType mod_type_;
        ItemType item_type_;
        int32_t modificator_ = 0;
        bool can_stacked_ = false;
    };

    void swap(Item&,Item&);

    class Inventory {

    public:
        Inventory() = default;

        Inventory& SetArmor(int32_t);
        Inventory& SetWeapon(int32_t);
        Inventory& SetShield(int32_t);
        Inventory& AddItem(Item&&);

        bool HasSpace() const;

        std::optional<std::reference_wrapper<Item>> GetArmor();
        std::optional<std::reference_wrapper<Item>> GetWeapon();
        std::optional<std::reference_wrapper<Item>> GetShield();
        int32_t GetAttackMod() const;
        int32_t GetDefenseMod() const;

        uint64_t size() const;

        std::optional<std::reference_wrapper<Item>> GetItem(int32_t);
        Inventory& RemoveItem(int32_t);
        
        void Print(std::ostream&)const;

        void Upload(json_reader::Node);
        json_reader::Node DownLoad() const;
    private:
        std::vector<std::pair<Item,int32_t>> inventory_;
        uint64_t max_size = 5;
        std::optional<Item> armor_;
        std::optional<Item> weapon_;
        std::optional<Item> shield_;
    };

}