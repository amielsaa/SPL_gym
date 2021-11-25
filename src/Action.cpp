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
    cout<<errorMsg<<endl;
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
        for(int i=0;i<customers.size();i++) {
            delete customers[i];
        }
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
    string toString = "open " + to_string(trainerId)+" ";
    for(int i=0;i<customers.size();i++){
        toString.append(customers[i]->toString());
    }
    if(getStatus()==0)
        toString.append(" Completed");
    else
        toString.append(" Error: " + getErrorMsg());
    return toString;
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

OpenTrainer *OpenTrainer::copy() {
    vector<Customer*> open_cus;
    for(int i=0;i<customers.size();i++){
        open_cus.push_back(customers[i]->copy());
    }
    OpenTrainer* open = new OpenTrainer(this->trainerId,open_cus);
    if(this->getStatus()==0)
        open->complete();
    else
        open->error(this->getErrorMsg());
    return open;

}

//--------------------ORDER------------------------
Order::Order(int id) : trainerId(id){}

void Order::act(Studio &studio) {
    if(trainerId>=studio.getNumOfTrainers())
        error("Trainer does not exist or is not open.");
    Trainer * trainer = studio.getTrainer(trainerId);
    if(!trainer || !trainer->isOpen()){
        error("Trainer does not exist or is not open.");
    } else {
        vector<Customer *> customers = trainer->getCustomers();
        vector<Workout> &workout_options = studio.getWorkoutOptions();
        if (trainer->getOrders().empty()) {
            for (int i = 0; i < customers.size(); i++) {
                trainer->order(customers[i]->getId(), customers[i]->order(workout_options), workout_options);

            }
        }
        vector<OrderPair> orders = trainer->getOrders();
        for (int i = 0; i < orders.size(); i++) {
            cout << trainer->getCustomer(orders[i].first)->getName() << " Is Doing " << orders[i].second.getName()
                 << endl;
        }
        complete();
    }
}

std::string Order::toString() const {
    string toString = "order " + to_string(trainerId) +" ";
    if(getStatus()==0)
        toString.append(" Completed");
    else
        toString.append(" Error: " + getErrorMsg());
    return toString;
}

Order *Order::copy() {
    Order* ord = new Order(trainerId);
    if(this->getStatus()==0)
        ord->complete();
    else
        ord->error(this->getErrorMsg());
    return ord;
}

//--------------------MOVE CUSTOMER------------------------
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src),dstTrainer(dst),id(customerId) {}

void MoveCustomer::act(Studio &studio) {
    if(dstTrainer>=studio.getNumOfTrainers()||srcTrainer>=studio.getNumOfTrainers())
        error("Trainer does not exist or is not open.");
    Trainer* destTrainer = studio.getTrainer(dstTrainer);
    Trainer* sourceTrainer = studio.getTrainer(srcTrainer);
    if(id>=sourceTrainer->getCustomers().size())
        error("customer does not exist");
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
                orderIds.push_back(order.second.getId());
                i--;
            }
        }
        destTrainer->order(customer->getId(),orderIds,studio.getWorkoutOptions());
        complete();
    }
    if(sourceTrainer->getCustomers().empty()){
        sourceTrainer->closeTrainer();
    }

}


std::string MoveCustomer::toString() const {
    string toString = "move " + to_string(srcTrainer) + " "+ to_string(dstTrainer) +" "+ to_string(id);
    if(getStatus()==0)
        toString.append(" Completed");
    else
        toString.append(" Error: " + getErrorMsg());
    return toString;
}

MoveCustomer *MoveCustomer::copy() {
    MoveCustomer* mov = new MoveCustomer(this->srcTrainer,this->dstTrainer,this->id);
    if(this->getStatus()==0)
        mov->complete();
    else
        mov->error(this->getErrorMsg());
    return mov;
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
    string toString = "close "+ to_string(trainerId);
    if(getStatus()==0)
        toString.append(" Completed");
    else
        toString.append(" Error: " + getErrorMsg());
    return toString;
}

Close *Close::copy() {
    Close* clos = new Close(this->trainerId);
    if(this->getStatus()==0)
        clos->complete();
    else
        clos->error(this->getErrorMsg());
    return clos;
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

//--------------------PrintWorkoutOptions------------------------

PrintWorkoutOptions::PrintWorkoutOptions() {}

void PrintWorkoutOptions::act(Studio &studio) {
    for(int i=0;i<studio.getWorkoutOptions().size();i++){
        cout<<studio.getWorkoutOptions()[i].getName()<<", "<<studio.getWorkoutOptions()[i].getStringType()<<", "<<studio.getWorkoutOptions()[i].getPrice()<<endl;
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    string toString = "workout_options ";
    if(getStatus()==0)
        toString.append(" Completed");
    else
        toString.append(" Error: " + getErrorMsg());
    return toString;
}

PrintWorkoutOptions *PrintWorkoutOptions::copy() {
    PrintWorkoutOptions* print = new PrintWorkoutOptions();
    if(this->getStatus()==0)
        print->complete();
    else
        print->error(this->getErrorMsg());
    return print;
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
        cout<<"Order: "<<endl;
        for(int i=0;i<trainer->getOrders().size();i++) {
            cout<< trainer->getOrders()[i].second.getName()<<" "<< trainer->getOrders()[i].second.getPrice()<<"NIS "<<trainer->getOrders()[i].first<<endl;
        }
        cout<<"Current trainer salary:  "<<trainer->getSalary()<<"NIS"<<endl;
        complete();
    }
}

std::string PrintTrainerStatus::toString() const {
    string toString = "status "+ to_string(trainerId);
    if(getStatus()==0)
        toString.append(" Completed");
    else
        toString.append(" Error: " + getErrorMsg());
    return toString;
}
PrintTrainerStatus *PrintTrainerStatus::copy() {
    PrintTrainerStatus * printTrainerStatus = new PrintTrainerStatus(this->trainerId);
    if(this->getStatus()==0)
        printTrainerStatus->complete();
    else
        printTrainerStatus->error(this->getErrorMsg());
    return printTrainerStatus;
}

//--------------------PrintActionsLog------------------------
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Studio &studio) {
    for(int i =0;i<studio.getActionsLog().size();i++){
        cout<<studio.getActionsLog()[i]->toString()<<endl;
    }for(int i =0;i<studio.getTempLog().size();i++){
        cout<<studio.getTempLog()[i]->toString()<<endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    string toString = "log ";
    if(getStatus()==0)
        toString.append(" Completed");
    else
        toString.append(" Error: " + getErrorMsg());
    return toString;
}
PrintActionsLog *PrintActionsLog::copy() {
    PrintActionsLog *printActionsLog = new PrintActionsLog();
    if(this->getStatus()==0)
        printActionsLog->complete();
    else
        printActionsLog->error(this->getErrorMsg());
    return printActionsLog;

}

//--------------------BackUpStudio------------------------
BackupStudio::BackupStudio() {}

void BackupStudio::act(Studio &studio) {
    auto* studioBackup = new Studio(studio);
//    if(backup== nullptr)
//        backup = new Studio(studio);
    if(backup!= nullptr)
        delete backup;

    backup = studioBackup;
    delete studioBackup;

}

std::string BackupStudio::toString() const {
    string toString = "backup ";
    if(getStatus()==0)
        toString.append(" Completed");
    else
        toString.append(" Error: " + getErrorMsg());
    return toString;
}
BackupStudio *BackupStudio::copy() {
    BackupStudio * backupStudio = new BackupStudio();
    if(this->getStatus()==0)
        backupStudio->complete();
    else
        backupStudio->error(this->getErrorMsg());
    return backupStudio;

}

//-------------------RestoreStudio-------------------------
RestoreStudio::RestoreStudio() {}

void RestoreStudio::act(Studio &studio) {
    if(backup== nullptr)
        cout<<"no backup available"<<endl;
    else{
        //delete studio;
        studio = *backup;
    }
}

std::string RestoreStudio::toString() const {
    string toString = "restore ";
    if(getStatus()==0)
        toString.append(" Completed");
    else
        toString.append(" Error: " + getErrorMsg());
    return toString;
}
RestoreStudio *RestoreStudio:: copy(){
    RestoreStudio * restoreStudio = new RestoreStudio();
    if(this->getStatus()==0)
        restoreStudio->complete();
    else
        restoreStudio->error(this->getErrorMsg());
    return restoreStudio;

}
