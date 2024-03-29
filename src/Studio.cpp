//
// Created by amiel on 10/11/2021.
//

#include "../include/Studio.h"
#include "../include/Workout.h"
#include <fstream>
#include <iostream>
using namespace std;
Studio::Studio(const std::string &configFilePath) {
    open = false;
    customerId=0;
    vector<string> fileContent = parseFile(configFilePath);

    //trains
    string str = fileContent[1];
    for(unsigned int i=0; i<str.size(); i++){
        if(str[i] != ',') {
            trainers.push_back(new Trainer(str[i]-'0'));
        }
    }

    //workouts
    for(unsigned int i=2;i<fileContent.size();i++) {
        vector<string> workouts = workoutSplitter(fileContent[i]);
        workout_options.push_back(Workout(i-2,workouts[0],stoi(workouts[2]), getWorkoutTypeByString(workouts[1])));
    }
}

std::vector<Workout>& Studio::getWorkoutOptions() {
    return workout_options;
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer* Studio::getTrainer(int tid) {
    return trainers[tid];
}

void Studio::inputIteration() {
    string input;
    getline(cin,input);
    vector<string> data = inputSplitter(input);
    //open case
    if(data[0].compare("open")==0) {
        vector<Customer*> customers;
        for(unsigned int i=2;i<data.size()-1;i++) {
            customers.push_back(createCustomerByType(data[i], data[i + 1]));
            i++;
        }
        OpenTrainer* open = new OpenTrainer(stoi(data[1]),customers);
        open->act(*this);
        if(open->getStatus()==0)
            actionsLog.push_back(open);
        else
            delete open;
    }
        //order case
    else if(data[0].compare("order")==0) {
        Order* order = new Order(stoi(data[1]));
        order->act(*this);
        if(order->getStatus()==0)
            actionsLog.push_back(order);
        else
            delete order;
    }
        //move case
    else if(data[0].compare("move")==0) {
        MoveCustomer* move = new MoveCustomer(stoi(data[1]),stoi(data[2]),stoi(data[3]));
        move->act(*this);
        if(move->getStatus()==0)
            actionsLog.push_back(move);
        else
            delete move;
    }
        //close case
    else if(data[0].compare("close")==0) {
        Close* close = new Close(stoi(data[1]));
        close->act(*this);
        if(close->getStatus()==0)
            actionsLog.push_back(close);
        else
            delete close;
    }
        //closeall case
    else if(data[0].compare("closeall")==0){
        CloseAll* closeall  = new CloseAll();
        closeall->act(*this);
        delete closeall;
        open = false;
    }
        //workout_options case
    else if(data[0].compare("workout_options")==0){
        PrintWorkoutOptions* workout = new PrintWorkoutOptions();
        workout->act(*this);
        if(workout->getStatus()==0)
            actionsLog.push_back(workout);
        else
            delete workout;
    }
        //status case
    else if(data[0].compare("status")==0) {
        PrintTrainerStatus* status = new PrintTrainerStatus(stoi(data[1]));
        status->act(*this);
        if(status->getStatus()==0)
            actionsLog.push_back(status);
        else
            delete status;
    }
        //print actions log case
    else if(data[0].compare("log")==0) {
        PrintActionsLog* print = new PrintActionsLog();
        print->act(*this);
        if(print->getStatus()==0)
            actionsLog.push_back(print);
        else{

            delete print;
        }


    }
    else if(data[0].compare("backup")==0){
        BackupStudio *backupStudio = new BackupStudio();
        backupStudio->act(*this);
        if(backupStudio->getStatus()==0)
            actionsLog.push_back(backupStudio);
        else
            delete backupStudio;

    }
    else if(data[0].compare("restore")==0){
        RestoreStudio *restoreStudio = new RestoreStudio();
        restoreStudio->act(*this);
        if(restoreStudio->getStatus()==0)
            actionsLog.push_back(restoreStudio);
        else
            delete restoreStudio;
    }
}

void Studio::start() {
    cout<<"Studio is now open!"<<endl;
    open = true;
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}


bool Studio::isOpen() {
    return open;
}

Customer *Studio::createCustomerByType(std::string name, std::string type) {
    Customer * cus;
    if(type.compare("swt")==0)
        cus = new SweatyCustomer(name,customerId);
    else if(type.compare("chp") == 0)
        cus = new CheapCustomer(name,customerId);
    else if(type.compare("mcl") == 0)
        cus = new HeavyMuscleCustomer(name,customerId);
    else if(type.compare("fbd") == 0)
        cus = new FullBodyCustomer(name,customerId);
    customerId++;
    return cus;
}

std::vector<std::string> Studio::inputSplitter(std::string inputString) {
    vector<string> ret;
    string temp;
    for(unsigned int i =0;i<inputString.size();i++){
        if(inputString[i]!=',' && inputString[i] != ' '){
            temp+=inputString[i];
        } else if(i<inputString.size()-1 && inputString[i+1] != ' ' && inputString[i+1] != ',') {
            ret.push_back(temp);
            temp="";
        }
    }
    ret.push_back(temp);
    return ret;
}


WorkoutType Studio::getWorkoutTypeByString(string workoutString) {
    if(workoutString[0] == 'A')
        return WorkoutType::ANAEROBIC;
    else if(workoutString[0]=='M')
        return WorkoutType::MIXED;
    else
        return WorkoutType::CARDIO;
}

//takes the line from the conf file and divides it to 3 segments
vector<string> Studio::workoutSplitter(string workoutContent) {
    vector<string> ret;
    string temp;
    for(unsigned int i=0; i<workoutContent.size(); i++){
        if(workoutContent[i] != ','){
            temp+=workoutContent[i];
            if(i==workoutContent.size()-1)
                ret.push_back(temp);
        }
        else{
            ret.push_back(temp);
            temp="";
            while( ( i<workoutContent.size() ) && ( ( workoutContent[i] == ',' ) | ( workoutContent[i] == ' ' ) ) ){
                i++;
            }
            i--;
        }
    }
    return ret;
}

//parsing the conf file to a vector
vector<string> Studio::parseFile(const string& conf) {
    ifstream inFile(conf);
    string strOneLine;
    vector<string> fileContent;
    while(inFile) {
        getline(inFile, strOneLine);
        if( ( strOneLine[0] != '#'  ) &  ( strOneLine.length()!=0 ) ){
            fileContent.push_back(strOneLine);
        }
    }
    inFile.close();
    return fileContent;
}

Studio::~Studio() {
    clear();
    open  = false;
}
Studio::Studio(const Studio &other) {
    copy(other.open,other.trainers,other.workout_options,other.actionsLog,other.customerId);
}

Studio::Studio(Studio &&other) : open(other.open),trainers(other.trainers),workout_options(other.workout_options), actionsLog(other.actionsLog),customerId(other.customerId) {
    other.clear();
}

Studio &Studio::operator=(const Studio& other) {
    if(this!=&other) {
        clear();
        this->copy(other.open,other.trainers,other.workout_options,other.actionsLog,other.customerId);
    }
    return *this;
}

Studio &Studio::operator=(Studio &&other) {
    if(this!=&other) {
        clear();
        trainers = move(other.trainers);
        workout_options = move(other.workout_options);
        actionsLog = move(other.actionsLog);
        open = other.open;
        customerId=other.customerId;

        other.open = false;
        other.customerId = 0;
    }
    return *this;
}

void Studio::clear() {
    for(unsigned int i=0;i<trainers.size();i++){
        delete trainers[i];
    }
    for(unsigned int i=0;i<actionsLog.size();i++){
        delete actionsLog[i];
    }
    if(!trainers.empty())
        trainers.clear();
    if(!workout_options.empty())
        workout_options.clear();
    if(!actionsLog.empty())
        actionsLog.clear();
    //open= false;
    customerId=0;
}

void Studio::copy(bool other_open, std::vector<Trainer *> other_trainers, std::vector<Workout> other_workout_options,
                  std::vector<BaseAction *> other_actionsLog, int other_customerId) {
    open = other_open;
    for (unsigned int i = 0; i < other_trainers.size(); ++i) {
        Trainer* train = new Trainer(*other_trainers[i]);
        trainers.push_back(train);
    }
    for (unsigned int i = 0; i < other_actionsLog.size(); ++i) {
        vector<string> inp = inputSplitter(other_actionsLog[i]->toString());
        BaseAction* act = other_actionsLog[i]->copy();
        if(inp[0].compare("open")==0){
            act->setCustomers(trainers[stoi(inp[1])]->getCustomers());
        }
        actionsLog.push_back(act);
    }

    workout_options = other_workout_options;
    customerId = other_customerId;
}


