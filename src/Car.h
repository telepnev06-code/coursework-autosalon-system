#pragma once

using namespace System;

namespace AutosalonApp {
    public ref class Car {
    public:
        String^ Brand;
        String^ Model;
        int Year;
        double Price;
        double MarketValue;
        String^ Condition;
        String^ Specs;

        Car(String^ brand, String^ model, int year, double price, double marketValue, String^ condition, String^ specs) {
            Brand = brand;
            Model = model;
            Year = year;
            Price = price;
            MarketValue = marketValue;
            Condition = condition;
            Specs = specs;
        }
    };
}
