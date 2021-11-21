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
        //delete allocated customer objects due to 'open' failure
//        for(int i=0;i<customers.size();i++) {
//            delete customers[i];
//        }
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
        complete();
    }
}

std::string Order::toString() const {
    return "order";
}

//--------------------MOVE CUSTOMER------------------------
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src),dstTrainer(dst),id(customerId) {}

void MoveCustomer::act(Studio &studio) {

    Trainer* destTrainer = studio.getTrainer(dstTrainer);
    Trainer* sourceTrainer = studio.getTrainer(srcTrainer);
    Customer* customer = sourceTrainer->getCustomer(id);
    if(!destTrainer || !sourceTrainer || !destTrainer->isOpen() | !sourceTrainer->isOpen()
        | !sourceTrainer->getCustomer(customer->getId()) | destTrainer->getCapacity() == destTrainer->getCustomers().size()) {
        error("Cannot move customer");
    } else{
        //move customer
        sourceTrainer->removeCustomer(id);
        destTrainer->addCustomer(customer);
        //move orders
        vector<int> orderIds;
        for(int i=0;i<sourceTrainer->getOrders().size();i++) {
            if(sourceTrainer->getOrders()[i].first == id) {
                OrderPair order(sourceTrainer->getOrders()[i].first,sourceTrainer->getOrders()[i].second);
                sourceTrainer->getOrders().erase(sourceTrainer->getOrders().begin() + i);
                orderIds.push_back(sourceTrainer->getOrders()[i].second.getId());
                i--;
            }
        }
        destTrainer->order(customer->getId(),orderIds,studio.getWorkoutOptions());
        complete();
    }
    if(sourceTrainer->getCustomers().size()==0){
        sourceTrainer->closeTrainer();
    }

}


std::string MoveCustomer::toString() const {
    return "Move customer";
}

//--------------------CLOSE------------------------

Close::Close(int id) : trainerId(id) {}

void Close::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    if(!trainer || !trainer->isOpen()){
        error("Trainer does not exist or is not open");
    } else{
        int salary = trainer->getSalary();
        trainer->closeTrainer();
        cout<<"Trainer "<<trainerId<<" closed. Salary "<<salary<<"NIS"<<endl;
        complete();
    }
}

std::string Close::toString() const {
    return "close";
}

//--------------------CloseAll------------------------
CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for(int i =0;i<studio.getNumOfTrainers();i++) {
        Trainer* trainer = studio.getTrainer(i);
        if(trainer->isOpen()) {
            int salary = trainer->getSalary();
            trainer->closeTrainer();
            cout<<"Trainer "<<i<<" closed. Salary "<<salary<<"NIS"<<endl;
        }
    }
    complete();
}

std::string CloseAll::toString() const {
    return "closeall";
}

//--------------------PrintTrainerStatus------------------------
PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    if(!trainer->isOpen())
        error("Trainer session should be open.");
    else{
        cout<<"Trainer "<<trainerId<<" status: open"<<endl;
        cout<<"Customers: "<<endl;
        for(int i=0;i<trainer->getCustomers().size();i++){
            cout<<trainer->getCustomers()[i]->getId()<<" "<<trainer->getCustomers()[i]->getName()<<endl;
        }
        for(int i=0;i<trainer->getOrders().size();i++) {
            cout<< trainer->getOrders()[i].second.getName()<<" "<< trainer->getOrders()[i].second.getPrice()<<" "<<trainer->getOrders()[i].first<<endl;
        }
        cout<<"Current trainer salary:  "<<trainer->getSalary()<<"NIS"<<endl;
        complete();
    }
}

std::string PrintTrainerStatus::toString() const {
    return "status";
}