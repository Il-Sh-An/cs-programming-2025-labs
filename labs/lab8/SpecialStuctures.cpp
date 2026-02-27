#include <algorithm>
#include <utility>
#include <fstream>

#include "SpecialStructures.hpp"


using namespace json_reader;
using namespace std::string_literals;
using namespace spec_struct;

uint64_t HashStr(std::string str){
    uint64_t temp = 0;
    for(char ch: str){
        temp += static_cast<uint64_t>(ch);
    }
    return temp *= str.size();
}

namespace spec_struct{
    BadInput::BadInput(std::string&& str):exeption(std::forward<std::string>(str)){}

    BadParse::BadParse(std::string&& str): BadInput(std::forward<std::string>(str)){}

    std::string BadInput::name(){
        return exeption;
    }

    BadChoise::BadChoise(std::string&& str): BadInput(std::forward<std::string>(str)){}

    FullInventory::FullInventory(std::string&& str):BadInput(std::forward<std::string>(str)){}

    Exit::Exit(std::string&& str):BadInput(std::forward<std::string>(str)){}

    std::istream& operator>>(std::istream& is,PersonType& type){
        int8_t type_;
        is >> type_;
        type = static_cast<PersonType>(type_);
        return is;
    }

    std::string PrintPersonType(PersonType type){
        std::string name;
        switch(type){
            case PersonType::none:
                name = "None";
                break;
            case PersonType::Human:
                name = "Человек";
                break;
            case PersonType::Elf:
                name = "Эльф";
                break;
            case PersonType::Dworf:
                name = "Гном";
                break;
            case PersonType::Giant:
                name = "Гигант";
                break;
            default:
                break;
        }
        return name;
    }
// Person
    const std::string& Person::GetName()const {
        return name_;
    }
    PersonType Person::GetType()const {
        return type_;
    }
    int32_t Person::GetExp()const {
        return exp_;
    }
    int32_t Person::GetAttack()const {
        return attack_;
    }
    int32_t Person::GetDefense()const {
        return defense_;
    }
    int32_t Person::GetAgility()const {
        return agility_;
    }
    int32_t Person::GetHp()const {
        return hp_;
    }
    int32_t Person::GetLevel()const {
        return level_;
    }
    int32_t& Person::GetPointers() {
        return pointers_;
    }

    Inventory& Person::GetModInventory(){
        return *inventory_;
    }

    int32_t Person::GetNeedExpForLvl() {
        return 100 + 100 * level_;
    }

    Person& Person::SetName(std::string name) {
        name_ = std::move(name);
        return *this;
    }
    Person& Person::SetHeight(int32_t height) {
        height_ = height;
        return *this;
    }
    Person& Person::SetWeight(int32_t weight) {
        weight_ = weight;
        return *this;
    }
    Person& Person::SetType(PersonType type) {
        type_ = type;
        return *this;
    }
    Person& Person::SetAttack(int32_t attack) {
        attack_ = attack;
        return *this;
    }
    Person& Person::SetDefense(int32_t def) {
        defense_ = def;
        return *this;
    }
    Person& Person::SetAgility(int32_t agil) {
        agility_ = agil;
        return *this;
    }
    Person& Person::SetHp(int32_t hp) {
        hp_ = hp;
        return *this;
    }
    Person& Person::SetMaxHp(int32_t max_hp) {
        max_hp_ = max_hp;
        return *this;
    }
    Person& Person::SetInventory(Inventory& inv) {
        inventory_ = &inv;
        return *this;
    }

    void MakeCharacters(const Node& node, Person&  person) {

        auto& person_characters = node.AsMap();

        auto& height = person_characters.at("height").AsMap();
        auto& attack = person_characters.at("attack").AsMap();
        auto& defense = person_characters.at("defense").AsMap();
        auto& agility = person_characters.at("agility").AsMap();
        auto& hp = person_characters.at("hp").AsMap();


        person.SetHeight(height.at("min").AsInt() + std::rand() % (height.at("max").AsInt() - height.at("min").AsInt())).
        SetWeight(height.at("max").AsInt() - 100 + (std::rand() % 10 - 5)).
        SetAttack(attack.at("min").AsInt() + std::rand() % (attack.at("max").AsInt() - attack.at("min").AsInt())).
        SetDefense(defense.at("min").AsInt() + std::rand() % (defense.at("max").AsInt() - defense.at("min").AsInt())).
        SetAgility(agility.at("min").AsInt() + std::rand() % (agility.at("max").AsInt() - agility.at("min").AsInt())).
        SetHp(hp.at("min").AsInt() + std::rand() % (hp.at("max").AsInt() - hp.at("min").AsInt())).
        SetMaxHp(person.GetHp());
    }

    Person& Person::SetCharacters(PersonType type) {
        std::ifstream in("PersonCharacters.json");

        Node input_file;
        in >> input_file;

        in.close();

        switch (type){
            case PersonType::Human:
                MakeCharacters(input_file.AsMap().at("Human"),*this);

                type_ = PersonType::Human;
                break;
            case PersonType::Dworf:
                MakeCharacters(input_file.AsMap().at("Dworf"),*this);

                type_ = PersonType::Dworf;
                break;
            case PersonType::Elf:
                MakeCharacters(input_file.AsMap().at("Elf"),*this);

                type_ = PersonType::Elf;
                break;
            case PersonType::Giant:
                MakeCharacters(input_file.AsMap().at("Giant"),*this);

                type_ = PersonType::Giant;
                break;
            default:
                break;
        }

        return *this;
    }
    
    Person& Person::ModExp(int32_t exp) {
        exp_ += exp;
        if(exp_ >= GetNeedExpForLvl()){
            ++level_;
            SetMaxHp(max_hp_ + 5);
            exp_ -= GetNeedExpForLvl();
            pointers_ += 4;
        }
        return *this;
    }
    Person& Person::ModAttack(int32_t attack) {
        attack_ += attack;
        return *this;
    }
    Person& Person::ModDefense(int32_t def) {
        defense_ += def;
        return *this;
    }
    Person& Person::ModAgility(int32_t agil) {
        agility_ = agil;
        return *this;
    }
    Person& Person::ModHp(int32_t hp) {
        if(hp < 0) {
            hp_ = std::max(0,hp_ + hp);
        }else {
            hp_ = std::min(hp_ + hp, max_hp_);
        }
        
        return *this;
    }

    void Person::Print(std::ostream& os)const {
        os << "Имя : "s << name_ << " , " << "Раса : "s << PrintPersonType(type_) << '\n';
        os << "Атака : "s << attack_ << " , " << "Здоровье : "s << hp_ << '\n';
        os << "Защита : "s << defense_ << " , " << "Уровень : "s << level_ << '\n';
        os << "Ловкость : "s << agility_ << " , " << "Опыт : "s << exp_ << '\n';
    }

    void Person::Upload(json_reader::Node node) {
        auto& map_ = node.AsMap();
        name_ = map_.at("name").AsString();
        attack_ = map_.at("attack").AsInt();
        defense_ = map_.at("defense").AsInt();
        agility_ = map_.at("agility").AsInt();
        type_ = static_cast<spec_struct::PersonType>(map_.at("person_type").AsInt());
        exp_ = map_.at("agility").AsInt();
        hp_ = map_.at("hp").AsInt();
        level_ = map_.at("level").AsInt();
        max_hp_ = map_.at("max_hp").AsInt();
        height_ = map_.at("height").AsInt();
        weight_ = map_.at("weight").AsInt();
    }
    json_reader::Node Person::DownLoad() const {
        Node::Map map_;

        map_.emplace("name",Node(name_));
        map_.emplace("attack",Node(attack_));
        map_.emplace("defense",Node(defense_));
        map_.emplace("agility",Node(agility_));
        map_.emplace("person_type",Node(static_cast<int32_t>(type_)));
        map_.emplace("exp",Node(exp_));
        map_.emplace("hp",Node(hp_));
        map_.emplace("level",Node(level_));
        map_.emplace("max_hp",Node(max_hp_));
        map_.emplace("height",Node(height_));
        map_.emplace("weight",Node(weight_));

        return Node(std::move(map_));
    }

// Функции Enemy

    Enemy& Enemy::SetName(std::string name) {
        name_ = std::move(name);
        return *this;
    }
    Enemy& Enemy::SetHp(int32_t hp) {
        hp_ = hp;
        return *this;
    }
    Enemy& Enemy::SetAttack(int32_t atack) {
        attack_ = atack;
        return *this;
    }
    Enemy& Enemy::SetDefense(int32_t def) {
        defense_ = def;
        return *this;
    }

    Enemy& Enemy::ModificateHp(int32_t hp) {
        if(hp < 0) {
            hp_ = std::max(0,hp_ + hp);
        }else {
            hp_ = hp_ + hp;
        }
        
        return *this;
    }
    Enemy& Enemy::ModificateAttack(int32_t attack) {
        attack_ += attack;
        return *this;
    }
    Enemy& Enemy::ModificateDefense(int32_t def) {
        defense_ += def;
        return *this;
    }
    Enemy& Enemy::ModificateForLvl(int32_t lvl) {
        attack_ += (lvl - 1) * (attack_ / 2);
        hp_ += (lvl - 1) * (hp_ / 2);
        defense_ += (lvl - 1) * (defense_ / 2);

        return *this;
    }

    const std::string& Enemy::GetName()const {
        return name_;
    }
    int32_t Enemy::GetHp()const {
        return hp_;
    }
    int32_t Enemy::GetAttack()const {
        return attack_;
    }
    int32_t Enemy::GetDefense()const {
        return defense_;
    }

    void Enemy::Print(std::ostream& os)const {
        os << "Имя: " << name_ << ", Здоровье: " << hp_ << '\n';
        os << "Атака: " << attack_ << ", Защита: " << defense_ << '\n';
    }

    void Enemy::Upload(Node node) {
        auto& map_ = node.AsMap();

        name_ = map_.at("name").AsString();
        attack_ = map_.at("attack").AsInt();
        defense_ = map_.at("defense").AsInt();
        hp_ = map_.at("hp").AsInt();
    }

    Node Enemy::DownLoad() const{
        Node::Map map_;

        map_.emplace("name",Node(name_));
        map_.emplace("attack",Node(attack_));
        map_.emplace("defense",Node(defense_));
        map_.emplace("hp",Node(hp_));

        return Node(std::move(map_));
    }

// РњРµС‚РѕРґС‹ РєР»Р°СЃСЃР° Item

    std::ostream& operator<< (std::ostream& os,ItemModType mod){
        os << static_cast<int8_t>(mod) << '\n';
        return os;
    }
    std::istream& operator>> (std::istream& is,ItemModType& mod){
        int8_t m;
        is >> m;
        mod = static_cast<ItemModType>(m);
        return is;
    }

    std::ostream& operator<< (std::ostream& os,ItemType mod){
        os << static_cast<int8_t>(mod) << '\n';
        return os;
    }
    std::istream& operator>> (std::istream& is,ItemType& mod){
        int8_t m;
        is >> m;
        mod = static_cast<ItemType>(m);
        return is;
    }
    
    Item& Item::SetName(std::string name) {
        name_ = std::move(name);
        return *this;
    }
    Item& Item::SetModType(ItemModType type) {
        mod_type_ = type;
        return *this;
    }
    Item& Item::SetModificator(int32_t mod) {
        modificator_ = mod;
        return *this;
    }
    Item& Item::SetItemType(ItemType type) {
        if(type == ItemType::Coin || type == ItemType::Potion){
            can_stacked_ = true;
        }
        item_type_ = type;
        return *this;
    }

    Item& Item::Swap(Item& item) {
        std::swap(name_,item.name_);
        std::swap(mod_type_,item.mod_type_);
        std::swap(item_type_,item.item_type_);
        std::swap(modificator_,item.modificator_);
        std::swap(can_stacked_,item.can_stacked_);
        return *this;
    }

    void swap(Item& lhs,Item& rhs) {
        lhs.Swap(rhs);
    }

    const std::string& Item::GetName()const {
        return name_;
    }
    ItemModType Item::GetModType()const {
        return mod_type_;
    }
    int32_t Item::GetMod()const {
        return modificator_;
    }
    ItemType Item::GetItemType()const {
        return item_type_;
    }
    bool Item::IsStack()const {
        return can_stacked_;
    }

    bool Item::operator== (const Item& other)const{
    return name_ == other.name_ 
        && mod_type_ == other.mod_type_
        && item_type_ == other.item_type_;
    }
    bool Item::operator< (const Item& other) const{
        return modificator_ < other.modificator_ && 
        HashStr(name_) < HashStr(other.name_);
    }

    void Item::Upload(Node node) {
        auto& map_ = node.AsMap();

        name_ = map_.at("name").AsString();
        mod_type_ = static_cast<ItemModType>(map_.at("mod_type").AsInt());
        item_type_ = static_cast<ItemType>(map_.at("item_type").AsInt());
        modificator_ = map_.at("mod").AsInt();
        can_stacked_ = map_.at("stack").AsBool();
    }

    json_reader::Node Item::DownLoad() const {
        Node::Map map_;

        map_.emplace("name",Node(name_));
        map_.emplace("mod_type",Node(static_cast<int32_t>(mod_type_)));
        map_.emplace("item_type",Node(static_cast<int32_t>(item_type_)));
        map_.emplace("mod",Node(modificator_));
        map_.emplace("stack",Node(can_stacked_));

        return Node(std::move(map_));
    }

// Функции Inventory

    Inventory& Inventory::SetArmor(int32_t id) {
        auto&[elem,num] = inventory_.at(static_cast<uint64_t>(id));

        if(elem.GetItemType() != ItemType::Armor){
            throw BadChoise("Its not armor");
        }
        if(armor_.has_value()) {
            swap(*armor_,elem);
        }else {
            armor_.emplace(elem);
            RemoveItem(id);
        }

        return *this;
    }
    Inventory& Inventory::SetWeapon(int32_t id) {
        auto&[elem,num] = inventory_.at(static_cast<uint64_t>(id));

        if(elem.GetItemType() != ItemType::Weapon){
            throw BadChoise("Its not weapon");
        }
        if(weapon_.has_value()) {
            swap(*weapon_,elem);
        }else {
            weapon_.emplace(elem);
            RemoveItem(id);
        }

        return *this;
    }
    Inventory& Inventory::SetShield(int32_t id) {
        auto&[elem,num] = inventory_.at(static_cast<uint64_t>(id));

        if(elem.GetItemType() != ItemType::Shield){
            throw BadChoise("Its not shield");
        }
        if(shield_.has_value()) {
            swap(*shield_,elem);
        }else {
            shield_.emplace(elem);
            RemoveItem(id);
        }

        return *this;
    }
    Inventory& Inventory::AddItem(Item&& item) {
        if(inventory_.size() == max_size){
            throw FullInventory("No Space");
        }
        
        auto elem = std::find_if(inventory_.begin(),inventory_.end(),[&item](const auto& element) {
                                                                    return element.first == item;
                            });
        if(elem != inventory_.end() && item.IsStack()){
            ++elem->second;
        }else{
            inventory_.emplace_back(std::move(item),1);
        }
        return *this;
    }
    
    bool Inventory::HasSpace() const {
        return inventory_.size() < max_size;
    }

    std::optional<std::reference_wrapper<Item>> Inventory::GetArmor() {
        if(armor_.has_value()){
            return {armor_.value()};
        }else {
            return std::nullopt;
        }
    }
    std::optional<std::reference_wrapper<Item>> Inventory::GetWeapon() {
        if(weapon_.has_value()){
            return {weapon_.value()};
        }else {
            return std::nullopt;
        }
    }
    std::optional<std::reference_wrapper<Item>> Inventory::GetShield() {
        if(shield_.has_value()){
            return {shield_.value()};
        }else {
            return std::nullopt;
        }
    }
    uint64_t Inventory::size() const {
        return inventory_.size();
    }
    int32_t Inventory::GetAttackMod() const {
        int32_t mod = 0;

        if(weapon_.has_value()) {
            mod += weapon_->GetMod();
        }
        return mod;
    }
    int32_t Inventory::GetDefenseMod() const {
        int32_t mod = 0;

        if(shield_.has_value()) {
            mod += shield_->GetMod();
        }

        if(armor_.has_value()) {
            mod += armor_->GetMod();
        }

        return mod;
    }

    std::optional<std::reference_wrapper<Item>> Inventory::GetItem(int32_t id){
        if(static_cast<uint64_t>(id) < inventory_.size()) {
            return inventory_.at(static_cast<uint64_t>(id)).first;
        }else {
            return std::nullopt;
        }
        
    }

    Inventory& Inventory::RemoveItem(int32_t id) {
        inventory_.erase(inventory_.cbegin() + id);
        return *this;
    }
    
    void Inventory::Print(std::ostream& os)const {
        int32_t counter = 0;
        os << " Броня : " << ((armor_.has_value())? armor_->GetName(): "Нет") << '\n';
        os << " Оружие : " << ((weapon_.has_value())? weapon_->GetName(): "Нет") << '\n';
        os << " Щит :  " << ((shield_.has_value())? shield_->GetName(): "Нет") << '\n';

        if(inventory_.empty()) {
            os << "Инвентарь пуст\n";
            return;
        }

        for(const auto&[elem,num] : inventory_) {
            os << ++counter << ' ' << elem.GetName() << ' ' << num << '\n';
        }
    }

    void Inventory::Upload(Node node) {
        inventory_.clear();

        auto& map_ = node.AsMap();

        auto& armor = map_.at("armor");
        auto& weapon = map_.at("weapon");
        auto& shield = map_.at("shield");

        auto& inventory = map_.at("inventory");

        if(armor.IsValid() && !armor.IsBool()) {

            armor_->Upload(armor);
        }

        if(weapon.IsValid() && !weapon.IsBool()) {

            weapon_->Upload(weapon);
            
        }

        if(shield.IsValid() && !shield.IsBool()){

            shield_->Upload(weapon);
            
        }

        if(inventory.IsValid()) {
            auto& inv_ref = inventory.AsArray();

            inventory_.reserve(inv_ref.size());

            for(uint64_t id = 1; id != inv_ref.size(); ++id) {
                Item temp_item;
                temp_item.Upload(inv_ref.at(id - 1));

                inventory_.emplace_back( std::move(temp_item), inv_ref.at(id).AsInt() );
            }
        }

    }
    Node Inventory::DownLoad() const {
        Node::Map map_;

        Node armor;
        Node weapon;
        Node shield;

        if(armor_) {
            armor = armor_->DownLoad();
        }
        if(weapon_) {
            weapon = weapon_->DownLoad();
        }
        if(shield_) {
            shield = shield_->DownLoad();
        }

        Node::Array inventory;
        inventory.reserve(inventory_.size() * 2);

        for(const auto& [elem,num]: inventory_) {
            Node::Map element;
            element.emplace("item",elem.DownLoad());
            element.emplace("num",Node(num));

            inventory.emplace_back(std::move(element));
        }

        map_.emplace("armor",armor.IsValid()? std::move(armor) : Node(false));
        map_.emplace("weapon",weapon.IsValid()? std::move(weapon) : Node(false));
        map_.emplace("shield",shield.IsValid()? std::move(shield) : Node(false));
        map_.emplace("inventory",std::move(inventory));

        return map_;
    }
}