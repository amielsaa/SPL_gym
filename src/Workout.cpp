//
// Created by amiel on 11/11/2021.
//

#include "../include/Workout.h"

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type)
                : id(w_id),name(w_name),price(w_price),type(w_type) {}

Workout &Workout::operator=(const Workout &other) {
    return *this;
}


int Workout::getId() const {
    return id;
}

std::string Workout::getName() const {
    return name;
}

int Workout::getPrice() const {
    return price;
}

WorkoutType Workout::getType() const {
    return type;
}

std::string Workout::getStringType() {
    if(type == 0)
        return "ANAEROBIC";
    else if(type==1)
        return "MIXED";
    else if(type==2)
        return "CARDIO";
    else
        return  "";
}

