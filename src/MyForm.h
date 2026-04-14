#pragma once
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#include "Car.h"

namespace AutosalonApp {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class MyForm : public Form {
    private:
        Label^ systemLabel;
        DataGridView^ carDataGridView;
        Button^ addTestCarButton;

    public:
        MyForm() {
            this->Text = L"Автосалон";
            this->Size = System::Drawing::Size(600, 400);

            systemLabel = gcnew Label();
            systemLabel->Text = L"Система управления автосалоном";
            systemLabel->Location = Point(10, 10);
            systemLabel->AutoSize = true;

            carDataGridView = gcnew DataGridView();
            carDataGridView->Location = Point(10, 40);
            carDataGridView->Size = System::Drawing::Size(560, 250);
            carDataGridView->ColumnCount = 4;
            carDataGridView->Columns[0]->Name = L"Марка";
            carDataGridView->Columns[1]->Name = L"Модель";
            carDataGridView->Columns[2]->Name = L"Год";
            carDataGridView->Columns[3]->Name = L"Цена";

            addTestCarButton = gcnew Button();
            addTestCarButton->Text = L"Добавить тестовое авто";
            addTestCarButton->Location = Point(10, 300);
            addTestCarButton->Size = System::Drawing::Size(200, 40);
            addTestCarButton->Click += gcnew EventHandler(this, &MyForm::OnAddTestCarClick);

            this->Controls->Add(systemLabel);
            this->Controls->Add(carDataGridView);
            this->Controls->Add(addTestCarButton);
        }

    private:
        void OnAddTestCarClick(Object^ sender, EventArgs^ e) {
            Car^ testCar = gcnew Car(L"Toyota", L"Camry", 2022, 25000.0, L"Отличное", L"2.5L, Автомат");
            carDataGridView->Rows->Add(testCar->Brand, testCar->Model, testCar->Year, testCar->Price);
        }
    };
}
