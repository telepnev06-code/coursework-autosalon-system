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
        
        // Поля ввода
        Label^ brandLabel;
        TextBox^ brandTextBox;
        
        Label^ modelLabel;
        TextBox^ modelTextBox;
        
        Label^ yearLabel;
        TextBox^ yearTextBox;
        
        Label^ priceLabel;
        TextBox^ priceTextBox;

        // Кнопки
        Button^ addButton;
        Button^ deleteButton;

    public:
        MyForm() {
            this->Text = L"Автосалон";
            this->Size = System::Drawing::Size(600, 450);

            systemLabel = gcnew Label();
            systemLabel->Text = L"Система управления автосалоном";
            systemLabel->Location = Point(10, 10);
            systemLabel->AutoSize = true;

            carDataGridView = gcnew DataGridView();
            carDataGridView->Location = Point(10, 40);
            carDataGridView->Size = System::Drawing::Size(560, 240);
            carDataGridView->ColumnCount = 4;
            carDataGridView->Columns[0]->Name = L"Марка";
            carDataGridView->Columns[1]->Name = L"Модель";
            carDataGridView->Columns[2]->Name = L"Год";
            carDataGridView->Columns[3]->Name = L"Цена";

            // Марка
            brandLabel = gcnew Label();
            brandLabel->Text = L"Марка:";
            brandLabel->Location = Point(10, 290);
            brandLabel->AutoSize = true;
            
            brandTextBox = gcnew TextBox();
            brandTextBox->Location = Point(10, 310);
            brandTextBox->Width = 120;

            // Модель
            modelLabel = gcnew Label();
            modelLabel->Text = L"Модель:";
            modelLabel->Location = Point(140, 290);
            modelLabel->AutoSize = true;

            modelTextBox = gcnew TextBox();
            modelTextBox->Location = Point(140, 310);
            modelTextBox->Width = 120;

            // Год
            yearLabel = gcnew Label();
            yearLabel->Text = L"Год:";
            yearLabel->Location = Point(270, 290);
            yearLabel->AutoSize = true;

            yearTextBox = gcnew TextBox();
            yearTextBox->Location = Point(270, 310);
            yearTextBox->Width = 80;

            // Цена
            priceLabel = gcnew Label();
            priceLabel->Text = L"Цена:";
            priceLabel->Location = Point(360, 290);
            priceLabel->AutoSize = true;

            priceTextBox = gcnew TextBox();
            priceTextBox->Location = Point(360, 310);
            priceTextBox->Width = 80;

            // Кнопка Добавить
            addButton = gcnew Button();
            addButton->Text = L"Добавить";
            addButton->Location = Point(10, 350);
            addButton->Size = System::Drawing::Size(120, 35);
            addButton->Click += gcnew EventHandler(this, &MyForm::OnActionClick);

            // Кнопка Удалить
            deleteButton = gcnew Button();
            deleteButton->Text = L"Удалить";
            deleteButton->Location = Point(140, 350);
            deleteButton->Size = System::Drawing::Size(120, 35);
            deleteButton->Click += gcnew EventHandler(this, &MyForm::OnActionClick);

            // Добавляем все элементы на форму
            this->Controls->Add(systemLabel);
            this->Controls->Add(carDataGridView);
            
            this->Controls->Add(brandLabel);
            this->Controls->Add(brandTextBox);
            this->Controls->Add(modelLabel);
            this->Controls->Add(modelTextBox);
            this->Controls->Add(yearLabel);
            this->Controls->Add(yearTextBox);
            this->Controls->Add(priceLabel);
            this->Controls->Add(priceTextBox);
            
            this->Controls->Add(addButton);
            this->Controls->Add(deleteButton);
        }

    private:
        void OnActionClick(Object^ sender, EventArgs^ e) {
            MessageBox::Show(L"Кнопка нажата!", L"Действие", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    };
}
