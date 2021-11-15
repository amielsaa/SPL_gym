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
    return str;
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
    return str;
}
////-------------------------------------------------------------------------------HeavyMuscleCustomer-----------------------------------------------
//HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name,id) {}
//
//std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options)
//{
//    std::vector<int> mcl_order;
//    //sorting workout_options vector by each workout price
//    struct price_check{
//        bool operator() (Workout w1, Workout w2) { return (w1.getPrice()>w2.getPrice());}
//    }workout_price_check;
//    //sorting
//    std::sort(workout_options[0],workout_options[workout_options.size()-1], workout_price_check);
//    //insert  workout id to int vector
//    for (int i = 0; i < workout_options.size(); ++i) {
//        if(workout_options[i].getType()==0)
//            mcl_order.push_back(workout_options[i].getId());
//    }
//    return  mcl_order;
//}
//std::string HeavyMuscleCustomer::toString() const
//{
//    std::string str = "mcl "+ getName();
//    return str;
//}

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
    for (int i = 1; i < workout_options.size(); ++i) {
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
    std::string str = "fbd "+ getName();
    return str;
}
