#pragma once

using namespace System;

namespace AutosalonApp {
    public ref class Client {
    public:
        String^ FullName;
        String^ Phone;
        double Budget;
        String^ ReqBrand;
        String^ ReqModel;
        String^ ReqCondition;

        Client(String^ fullName, String^ phone, double budget, String^ reqBrand, String^ reqModel, String^ reqCondition) {
            FullName = fullName;
            Phone = phone;
            Budget = budget;
            ReqBrand = reqBrand;
            ReqModel = reqModel;
            ReqCondition = reqCondition;
        }
    };
}
