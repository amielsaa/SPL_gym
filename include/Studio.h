#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

extern Studio* backup;
class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    //RoF
    ~Studio();
    Studio(const Studio& other);
    Studio(Studio&& other);
    Studio& operator=(const Studio& other);
    Studio& operator=(Studio&& other);

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int customerId;
    Customer* createCustomerByType(std::string name,std::string type);
    std::vector<std::string> parseFile(const std::string& conf);
    std::vector<std::string> workoutSplitter(std::string workoutContent);
    WorkoutType getWorkoutTypeByString(std::string workoutString);
    std::vector<std::string> inputSplitter(std::string inputString);
    void clear();
    void copy(bool other_open,std::vector<Trainer*> other_trainers,std::vector<Workout> other_workout_options,std::vector<BaseAction*> other_actionsLog,int other_customerId);
};

#endif