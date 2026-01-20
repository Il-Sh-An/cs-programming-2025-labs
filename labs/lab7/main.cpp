#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <ranges>
#include <windows.h>
#include <clocale>

struct Object{
    std::string name;
    int danger = 0;
};

void F1(){
    std::vector<Object> scp{
        {"Containment Cell A", 4},
        {"Archive Vault", 1},
        {"Bio Lab Sector", 3},
        {"Observation Wing", 2}
    };

    std::sort(scp.begin(),scp.end(),[](const auto& lhs, const auto& rhs){
        return lhs.danger < rhs.danger;
    });
}

struct Employee{
    std::string name;
    int shift_cost = 0;
    int shifts = 0;

    int GetCost()const{
        return shift_cost * shifts;
    }

    int GetOneCost()const{
        return shift_cost;
    }

    bool operator< (const Employee& other){
        return GetCost() < other.GetCost();
    }
};

void F2(){
    std::vector<Employee> employees{
        {"Dr. Shaw",120,15},
        {"Agent Torres",90,22},
        {"Researcher Hall",150,10}
    };

    std::vector<std::pair<Employee*,int>> shifts_map;

    for(auto& empl: employees){
        shifts_map.emplace_back(&empl,empl.GetCost());
    }

    auto max_shift = std::max_element(shifts_map.begin(),shifts_map.end(),[](const auto& lhs, const auto& rhs){
        return lhs.second < rhs.second;
    });

    std::cout << max_shift->second;
}

struct Personal{
    Personal(std::string n,int c):
    name(std::move(n)),clearence(std::move(c)){}

    std::string name;
    int clearence;
};

struct Personal_New:Personal{
    Personal_New(std::string n, int c, std::string cat):
    Personal(n,c),category(std::move(cat)){}

    std::string category;
};

void F3(){
    std::vector<Personal> personality{
        {"Dr. Klein",2},
        {"Agent Brooks",4},
        {"Technician Reed",1}
    };

    std::vector<Personal_New> new_personal_base;
    for(const auto& [name,clearence]: personality){
        std::string category;
        if(clearence == 1){
            category = "Restricted";
        }else if(clearence < 4){
            category = "Confidential";
        }else{
            category = "Top Secret";
        }
        new_personal_base.emplace_back(name,clearence,category);
    }
    for(const auto& personal: new_personal_base){
        std::cout << personal.name << ' ' << personal.clearence << ' ' << personal.category << '\n';
    }
};

struct Zone{
    std::string name;
    int active_from = 0;
    int active_to = 0;
};

void F4(){
    std::vector<Zone> zones{
        {"Sector-12",8,18},
        {"Deep Storage",0,24},
        {"Research",9,17}
    };

    auto daily_zones = zones | 
            std::views::filter([](const Zone& zone){
                return zone.active_from >= 8 && zone.active_to <= 18;
            });
    for(const auto& zone: daily_zones){
        std::cout << zone.name << ' ';
    }
    std::cout << '\n';
}

auto IsHttp = [](std::string_view str)->bool{
    return str.find("http") != std::string_view::npos;
};

auto ModificateStr = [](std::string_view str){
    auto pos = str.find("http");
    str.remove_suffix(str.size() - pos);
    return std::string(str) + "[ДАННЫЕ УДАЛЕНЫ]";
};  

void F5(){
    std::vector<std::pair<std::string,std::string>> reports{
        {"Dr. Moss","Analysis completed. Reference: http://external-archive.net"},
        {"Agent Lee","Incident resolved without escalation."},
        {"Dr. Patel","Supplementary data available at https://secure-research.org"},
        {"Supervisor Kane","No anomalies detected during inspection."},
        {"Researcher Bloom","Extended observations uploaded to http://research-notes.lab"},
        {"Agent Novak","Perimeter secured. No external interference observed."},
        {"Dr. Hargreeve","Full containment log stored at https://internal-db.scp"},
        {"Technician Moore","Routine maintenance completed successfully."},
        {"Dr. Alvarez","Cross-reference materials: http://crosslink.foundation"},
        {"Security Officer Tan","Shift completed without incidents."},
        {"Analyst Wright","Statistical model published at https://analysis-hub.org"},
        {"Dr. Kowalski","Behavioral deviations documented internally."},
        {"Agent Fischer","Additional footage archived: http://video-storage.sec"},
        {"Senior Researcher Hall","All test results verified and approved."},
        {"Operations Lead Grant","Emergency protocol draft shared via https://ops-share.scp"}
    };

    auto mod_reports = reports | std::views::filter([](const auto& str_p){return IsHttp(str_p.second);})
                                       | std::views::transform([](const auto& str_p)->std::pair<std::string_view,std::string_view>
                                                                {
                                                                    return {str_p.first,ModificateStr(str_p.second)};
                                                                });

    for(const auto& report: mod_reports){
        std::cout << report.second << '\n';
    }
    std::cout << '\n';
}

void F6(){
    //Название SCP и уровень угрозы
    std::vector<std::pair<std::string,std::string>> scp_object{
        {"SCP-096","Euclid"},
        {"SCP-173","Euclid"},
        {"SCP-055","Keter"},
        {"SCP-999","Safe"},
        {"SCP-3001","Keter"}
    };

    auto not_safe_scp = scp_object | std::views::filter([](const auto& p){
                                            return p.second != "Safe";
                                        });

    for(const auto& scp: not_safe_scp){std::cout << scp.first << ' ';}
    std::cout << '\n';
}

void F7(){
    // id и количество персонала
    std::vector<std::pair<int,int>> incidents{
        {101,4},
        {102,12},
        {103,7},
        {104,20}
    };

    std::ranges::sort(incidents,[](const auto& lhs, const auto& rhs){
        return lhs.second > rhs.second;
    });

    for(const auto& id: incidents | std::views::take(3)){
        std::cout << id.first << ' ';
    }
    std::cout << '\n';
}

void F8(){
    //Название протокола и уровень критичности
    std::vector<std::pair<std::string,int>> protocols{
        {"Lockdown", 5},
        {"Evacuation", 4},
        {"Data Wipe", 3},
        {"Routine Scan", 1}
    };

    std::vector<std::string> strs;
    strs.reserve(protocols.size());
    
    for(const auto& [name,level] : protocols){
        strs.push_back(name + " - " + std::to_string(level));
    }

    for(const auto& str: strs){
        std::cout << str << '\n';
    }
}

void F9(){
    //Длительность отработанных смен
    std::vector<int> shifts = {6, 12, 8, 24, 10, 4};

    auto shifts_8_to_12 = shifts | std::views::filter([](const auto& shift){
                                                    return shift >= 8 && shift <= 12;
                                                });
    for(const auto& sh: shifts_8_to_12){
        std::cout << sh << ' ';
    }
    std::cout << '\n';
}

void F10(){
    // имя и резы псих теста
    std::vector<std::pair<std::string,int>> evaluations{
        {"Agent Cole",78},
        {"Dr. Weiss",92},
        {"Technician Moore",61},
        {"Researcher Lin",88}
    };

    auto max_evalutation = std::ranges::max_element(evaluations,[](const auto& lhs, const auto& rhs){
                                            return lhs.second < rhs.second;
                                        });
    std::cout << max_evalutation->first << ' ' << max_evalutation-> second<< '\n';
}

int main(){
    setlocale(LC_ALL,"ru-Ru.UTF-8");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    F5();
}