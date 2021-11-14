//
// Created by amiel on 09/11/2021.
//
#include <algorithm>
#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id):name(c_name), id(c_id) {}
std::string Customer ::getName() const {
    return  name;
}
int Customer::getId() const {
    return  id;

}
//-------------------------------------------------------------------------------SweatyCustomer-----------------------------------------------
SweatyCustomer::SweatyCustomer(std::string name, int id):Customer (name, id) {}
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> swt_order;
    for (int i = 0; i < workout_options.size(); ++i) {
        if(workout_options[i].getType()==2)
            swt_order.push_back(workout_options[i].getId());
    }
    return swt_order;
}
std::string SweatyCustomer::toString() const {
    std::string str = "swt "+ getName();
}
//-------------------------------------------------------------------------------cheapCostumer-----------------------------------------------
CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id) {}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options)
{
    std::vector<int> chp_order;
    int cheapest =workout_options[0].getPrice();
    int workout_id=0;
    for (int i = 1; i < workout_options.size(); ++i) {
        if(workout_options[i].getPrice()<cheapest)
        {
            workout_id=workout_options[i].getId();
            cheapest = workout_options[i].getPrice();
        }
    }
    chp_order.push_back(workout_id);
    return chp_order;
}
std::string CheapCustomer::toString() const
{
    std::string str = "chp "+ getName();
}
//-------------------------------------------------------------------------------HeavyMuscleCustomer-----------------------------------------------
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name,id) {}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options)
{
    std::vector<int> mcl_order;
    //sorting workout_options vector by each workout price
    struct price_check{
        bool operator() (Workout w1, Workout w2) { return (w1.getPrice()>w2.getPrice());}
    }workout_price_check;
    //sorting
    std::sort(workout_options[0],workout_options[workout_options.size()-1], workout_price_check);
    //insert  workout id to int vector
    for (int i = 0; i < workout_options.size(); ++i) {
        if(workout_options[i].getType()==0)
            mcl_order.push_back(workout_options[i].getId());
    }
    return  mcl_order;
}
std::string HeavyMuscleCustomer::toString() const
{
    std::string str = "mcl "+ getName();
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name,id){}
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {


    
}

