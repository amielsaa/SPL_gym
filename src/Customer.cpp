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
Customer::~Customer(){}
Customer::Customer(const Customer & other) :name(other.getName()),id(other.getId()) {}
//Customer* Customer::copy(){}

//Customer &Customer::operator=(const Customer &other) {
//    if(this!=&other)
//    {
//
//    }
//}
//-------------------------------------------------------------------------------SweatyCustomer-----------------------------------------------
SweatyCustomer::SweatyCustomer(std::string name, int id):Customer (name, id) {}
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> swt_order;
    for (unsigned int i = 0; i < workout_options.size(); ++i) {
        if(workout_options[i].getType()==2)
            swt_order.push_back(workout_options[i].getId());
    }
    return swt_order;
}
std::string SweatyCustomer::toString() const {
    std::string str = getName()+",swt ";
    return str;
}
Customer* SweatyCustomer::copy(){
    SweatyCustomer* newCustomer =new  SweatyCustomer(getName(), getId());
    return newCustomer;
}


//-------------------------------------------------------------------------------cheapCostumer-----------------------------------------------
CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id) {}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options)
{
    std::vector<int> chp_order;
    int cheapest =workout_options[0].getPrice();
    int workout_id=0;
    for (unsigned int i = 1; i < workout_options.size(); ++i) {
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
    std::string str = getName()+",chp ";
    return str;
}
Customer* CheapCustomer::copy(){
    CheapCustomer* newCustomer =new  CheapCustomer(getName(), getId());
    return newCustomer;
}
////-------------------------------------------------------------------------------HeavyMuscleCustomer-----------------------------------------------
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name,id) {}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options)
{
    struct P_copy {
        int wo_id;
        int wo_price;
        int wo_type;
        P_copy(int id, int price, int type) {
            this -> wo_id = id;
            this -> wo_price = price;
            this ->wo_type = type;
        }
    };
    std::vector<int> mcl_order;
    std::vector<P_copy> partial_copy;
    //initialize partial copy
    for (unsigned int i = 0; i < workout_options.size(); ++i) {
        partial_copy.push_back(P_copy(workout_options[i].getId(),workout_options[i].getPrice(), workout_options[i].getType()));
    }

    std::sort(partial_copy.begin(),partial_copy.end(),[](const P_copy& lhs, const P_copy& rhs) {
        return lhs.wo_price < rhs.wo_price;
    });
    //insert  workout id to int vector
    for (unsigned int i = 0; i < partial_copy.size(); ++i) {
        if(partial_copy[i].wo_type==0)
            mcl_order.push_back(partial_copy[i].wo_id);
    }
    return  mcl_order;
}
//bool comparator(const Workout& lhs, const Workout& rhs) {
//    return lhs.getPrice() < rhs.getPrice();
//}
std::string HeavyMuscleCustomer::toString() const
{
    std::string str =getName() +",mcl ";
    return str;
}
Customer* HeavyMuscleCustomer::copy() {
    HeavyMuscleCustomer *newCustomer = new HeavyMuscleCustomer(getName(), getId());
    return newCustomer;
}
//-------------------------------------------------------------------------------FullBodyCustomer-----------------------------------------------
FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name,id){}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> fbd_order;
    int cheapest_cardio = workout_options[0].getPrice();
    int cardio_id=0;
    int cheapest_anaerobic = workout_options[0].getPrice();
    int anaerobic_id=0;
    int m_expensive_mixtype=workout_options[0].getPrice();
    int mixtype_id =0;
    for (unsigned int i = 1; i < workout_options.size(); ++i) {
        if(workout_options[i].getType()==0 && workout_options[i].getPrice()<cheapest_anaerobic){
            cheapest_anaerobic = workout_options[i].getPrice();
            anaerobic_id = workout_options[i].getId();
        }
        else if (workout_options[i].getType()==2 && workout_options[i].getPrice()<cheapest_cardio){
            cheapest_cardio = workout_options[i].getPrice();
            cardio_id =workout_options[i].getId();
        }
        else if(workout_options[i].getType()==1 && workout_options[i].getPrice()>m_expensive_mixtype){
            m_expensive_mixtype = workout_options[i].getPrice();
            mixtype_id = workout_options[i].getId();
        }
    }
    fbd_order.push_back(cardio_id);
    fbd_order.push_back(mixtype_id);
    fbd_order.push_back(anaerobic_id);
    return fbd_order;
}
std::string FullBodyCustomer::toString() const {
    std::string str =  getName()+",fbd ";
    return str;
}
Customer* FullBodyCustomer::copy() {
    FullBodyCustomer *newCustomer = new FullBodyCustomer(getName(), getId());
    return newCustomer;
}