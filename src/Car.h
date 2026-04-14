#pragma once

using namespace System;

namespace AutosalonApp {
    public ref class Car {
    public:
        String^ Brand;
        String^ Model;
        int Year;
        double Price;
        String^ Condition;
        String^ Specs;

        Car(String^ brand, String^ model, int year, double price, String^ condition, String^ specs) {
            Brand = brand;
            Model = model;
            Year = year;
            Price = price;
            Condition = condition;
            Specs = specs;
        }
    };
}
