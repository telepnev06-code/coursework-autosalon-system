#pragma once
#using <System.dll>
#using <System.Windows.Forms.dll>
#include "Car.h"
#include "Client.h"

namespace AutosalonApp {
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::IO;

    public ref class DataService {
    public:
        static List<Car^>^ Cars = gcnew List<Car^>();
        static List<Client^>^ Clients = gcnew List<Client^>();

        static event EventHandler^ OnDataChanged;

        static void NotifyDataChanged() {
            OnDataChanged(nullptr, EventArgs::Empty);
        }

        static void LoadCars(String^ path) {
            if (!File::Exists(path)) return;
            Cars->Clear();
            try {
                StreamReader^ reader = gcnew StreamReader(path, System::Text::Encoding::UTF8);
                String^ line;
                while ((line = reader->ReadLine()) != nullptr) {
                    if (String::IsNullOrWhiteSpace(line)) continue;
                    cli::array<String^>^ parts = line->Split(';');
                    if (parts->Length >= 4) {
                        String^ brand = parts[0]->Trim();
                        String^ model = parts[1]->Trim();
                        int year = 0;
                        double price = 0.0;
                        double mv = 0.0;
                        String^ cond = L"Не указано";
                        String^ specs = L"";

                        if (Int32::TryParse(parts[2]->Trim(), year) && Double::TryParse(parts[3]->Trim(), price)) {
                            if (parts->Length >= 7) {
                                Double::TryParse(parts[4]->Trim(), mv);
                                cond = parts[5]->Trim();
                                specs = parts[6]->Trim();
                            }
                            Cars->Add(gcnew Car(brand, model, year, price, mv, cond, specs));
                        }
                    }
                }
                reader->Close();
            } catch (Exception^) {}
            NotifyDataChanged();
        }

        static void SaveCars(String^ path) {
            try {
                StreamWriter^ writer = gcnew StreamWriter(path, false, System::Text::Encoding::UTF8);
                for each (Car^ car in Cars) {
                    writer->WriteLine(car->Brand + L";" + car->Model + L";" + car->Year + L";" + car->Price + L";" + car->MarketValue + L";" + car->Condition + L";" + car->Specs);
                }
                writer->Close();
            } catch (Exception^) {}
        }

        static void LoadClients(String^ path) {
            if (!File::Exists(path)) return;
            Clients->Clear();
            try {
                StreamReader^ reader = gcnew StreamReader(path, System::Text::Encoding::UTF8);
                String^ line;
                while ((line = reader->ReadLine()) != nullptr) {
                    if (String::IsNullOrWhiteSpace(line)) continue;
                    cli::array<String^>^ parts = line->Split(';');
                    if (parts->Length >= 6) {
                        String^ name = parts[0]->Trim();
                        String^ phone = parts[1]->Trim();
                        double budget = 0.0;
                        String^ rBrand = parts[3]->Trim();
                        String^ rModel = parts[4]->Trim();
                        String^ rCond = parts[5]->Trim();
                        if (Double::TryParse(parts[2]->Trim(), budget)) {
                            Clients->Add(gcnew Client(name, phone, budget, rBrand, rModel, rCond));
                        }
                    }
                }
                reader->Close();
            } catch (Exception^) {}
            NotifyDataChanged();
        }

        static void SaveClients(String^ path) {
            try {
                StreamWriter^ writer = gcnew StreamWriter(path, false, System::Text::Encoding::UTF8);
                for each (Client^ c in Clients) {
                    writer->WriteLine(c->FullName + L";" + c->Phone + L";" + c->Budget + L";" + c->ReqBrand + L";" + c->ReqModel + L";" + c->ReqCondition);
                }
                writer->Close();
            } catch (Exception^) {}
        }

        static List<Car^>^ GetMatches(Client^ client) {
            List<Car^>^ matches = gcnew List<Car^>();
            String^ cBrand = client->ReqBrand->ToLower();
            String^ cModel = client->ReqModel->ToLower();
            String^ cCond = client->ReqCondition->ToLower();
            double cBudget = client->Budget;

            for each (Car^ car in Cars) {
                String^ carBrand = car->Brand->ToLower();
                String^ carModel = car->Model->ToLower();
                String^ carCond = car->Condition->ToLower();
                
                bool matchBrand = String::IsNullOrEmpty(cBrand) || cBrand == L"любая" || cBrand == L"любое" || carBrand->Contains(cBrand);
                bool matchModel = String::IsNullOrEmpty(cModel) || cModel == L"любая" || cModel == L"любое" || carModel->Contains(cModel);
                bool matchCond = String::IsNullOrEmpty(cCond) || cCond == L"любая" || cCond == L"любое" || carCond->Contains(cCond);
                bool matchBudget = car->Price <= cBudget;

                if (matchBudget && matchBrand && matchModel && matchCond) {
                    matches->Add(car);
                }
            }
            return matches;
        }
    };
}
