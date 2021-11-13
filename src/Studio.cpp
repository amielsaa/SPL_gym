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
    vector<string> fileContent = parseFile(configFilePath);

    //num of trainers
    int numOfTrainers = stoi(fileContent[0]);

    //trains
    string str = fileContent[1];
    cout<<"trainers:"<<endl;
    for(int i=0; i<str.size(); i++){
        if(str[i] != ',') {
            cout<<str[i]<<endl;

            //trainers.push_back(new Trainer(str[i-'0']));
        }
    }

    //workouts
    for(int i=2;i<fileContent.size();i++) {
        vector<string> workouts = workoutSplitter(fileContent[i]);
        cout<<workouts[0] +" " << getWorkoutTypeByString(workouts[1]) << " " + workouts[2] <<endl;
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

void Studio::start() {
    cout<<"Studio is now open!"<<endl;
    open = true;

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
    for(int i=0; i<workoutContent.size(); i++){
        if(workoutContent[i] != ','){
            temp+=workoutContent[i];
            if(i==workoutContent.size()-1)
                ret.push_back(temp);
        }
        else{
            ret.push_back(temp);
            temp="";
            while(i<workoutContent.size() && (workoutContent[i] == ',' | workoutContent[i] == ' ') ){
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
        if(strOneLine[0] != '#' & strOneLine.length()!=0){
            fileContent.push_back(strOneLine);
        }
    }
    inFile.close();
    return fileContent;
}