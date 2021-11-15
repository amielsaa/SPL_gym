//
// Created by amiel on 13/11/2021.
//

#include "../include/Trainer.h"

using namespace std;

Trainer::Trainer(int t_capacity) : capacity(t_capacity) {

}

bool Trainer::isOpen() {
    return open;
}

int Trainer::getSalary() {
    int summedSalary = 0;
    for(int i =0;i<orderList.size();i++){
        summedSalary += orderList[i].second.getPrice();
    }
    return summedSalary;
}

void Trainer::closeTrainer() {
    open = false;
}

void Trainer::openTrainer() {
    open = true;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids,
                    const std::vector<Workout> &workout_options) {
    for(int i =0;i<workout_ids.size();i++){
        OrderPair p(customer_id,workout_options[workout_ids[i]]);
        orderList.push_back(p);
    }
}

vector<OrderPair>& Trainer::getOrders() {
    return orderList;
}

vector<Customer*>& Trainer::getCustomers() {
    return customersList;
}

int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}



Customer* Trainer::getCustomer(int id) {
    return customersList[getPosById(id)];
}

void Trainer::removeCustomer(int id) {
    customersList.erase(customersList.begin()+ getPosById(id));
}

//return position of a customer in customersList vector
int Trainer::getPosById(int id) {
    int ret;
    bool found = false;
    for(int i=0;i<customersList.size() & !found;i++){
        if(customersList[i]->getId() == id){
            ret = i;
            found = true;
        }
    }
    return ret;
}


//Destructor
Trainer::~Trainer() {
    clear();
}
//Copy Constructor
Trainer::Trainer(const Trainer &other) {
    copy(other.capacity,other.open,other.customersList,other.orderList);
}
//Move Constructor
Trainer::Trainer(Trainer &&other) : capacity(other.capacity),open(other.open),customersList(other.customersList),orderList(other.orderList) {
    other.clear();
}

////Copy Assignment
//Trainer& Trainer::operator=(const Trainer &other) {
//
//}
//
////Move Assignment
//Trainer& Trainer::operator=(Trainer &&other) {
//
//}

void Trainer::copy(int other_capacity, bool other_open, vector<Customer *> other_customersList, vector<OrderPair> other_orderList) {
    customersList = vector<Customer*>(other_customersList);
    orderList=vector<OrderPair>(other_orderList);
    capacity=other_capacity;
    open=other_open;
}

void Trainer::clear() {
    if(!customersList.empty())
        customersList.clear();
    if(!orderList.empty())
        orderList.clear();
    capacity=0;
    open = false;
}
