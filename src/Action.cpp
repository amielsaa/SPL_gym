//
// Created by amiel on 10/11/2021.
//

#include "../include/Action.h"
#include "../include/Studio.h"


using namespace std;

BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = move(errorMsg);
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

BaseAction::~BaseAction() {
    errorMsg.clear();
    status = ActionStatus::COMPLETED;
}

//--------------------OPEN TRAINER------------------------

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id),customers(customersList) {

}

void OpenTrainer::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if(!trainer || trainer->isOpen()){
        error("Workout session does not exist or is already open.");
    } else if(trainer->getCapacity() >= customers.size()){

        trainer->openTrainer();
        for(int i =0;i<customers.size();i++){
            trainer->addCustomer(customers[i]);
        }
        complete();
    } else{
        error("Trainer capacity is lower than requested amount of customers.");
    }
}


std::string OpenTrainer::toString() const {
    return "Trainer " + to_string(trainerId);
}

//Destructor
OpenTrainer::~OpenTrainer() {
    clear();
}

//Copy Constructor
OpenTrainer::OpenTrainer(const OpenTrainer &other) : trainerId(other.trainerId) {
    copy(other.customers);
}

//Copy Assignment
OpenTrainer& OpenTrainer::operator=(const OpenTrainer &other) {
    if(this!=&other){
        clear();
        copy(other.customers);
    }
    return *this;
}

//Move Constructor
OpenTrainer::OpenTrainer(OpenTrainer &&other) : trainerId(other.trainerId) ,customers(other.customers){
    other.clear();
}

//Move Assignment
OpenTrainer& OpenTrainer::operator=(OpenTrainer &&other) {
    if(this!=&other){
        customers = move(other.customers);
    }
    return *this;
}

void OpenTrainer::clear() {
    if(!customers.empty()) {
        customers.clear();
    }
}
void OpenTrainer::copy(std::vector<Customer*> customers)  {
    this->customers = std::vector<Customer*>(customers);
}

//--------------------ORDER------------------------
Order::Order(int id) : trainerId(id){}

void Order::act(Studio &studio) {
    Trainer * trainer = studio.getTrainer(trainerId);
    if(!trainer || !trainer->isOpen()){
        error("Trainer does not exist or is not open.");
    } else{
        vector<Customer*> customers = trainer->getCustomers();
        vector<Workout>& workout_options = studio.getWorkoutOptions();
        for(int i=0;i<customers.size();i++){
            trainer->order(customers[i]->getId(),customers[i]->order(workout_options),workout_options);

        }
        vector<OrderPair> orders = trainer->getOrders();
        for(int i =0;i<orders.size();i++){
            cout<<trainer->getCustomer(orders[i].first)->getName()<<" Is Doing "<<orders[i].second.getName()<<endl;
        }
    }
}

std::string Order::toString() const {
    return "order";
}