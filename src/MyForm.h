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
        Button^ btnSave;
        Button^ btnLoad;

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
            addButton->Click += gcnew EventHandler(this, &MyForm::OnAddClick);

            // Кнопка Удалить
            deleteButton = gcnew Button();
            deleteButton->Text = L"Удалить";
            deleteButton->Location = Point(140, 350);
            deleteButton->Size = System::Drawing::Size(120, 35);
            deleteButton->Click += gcnew EventHandler(this, &MyForm::OnDeleteClick);

            // Кнопка Сохранить базу
            btnSave = gcnew Button();
            btnSave->Text = L"Сохранить базу";
            btnSave->Location = Point(270, 350);
            btnSave->Size = System::Drawing::Size(140, 35);
            btnSave->Click += gcnew EventHandler(this, &MyForm::OnSaveClick);

            // Кнопка Загрузить базу
            btnLoad = gcnew Button();
            btnLoad->Text = L"Загрузить базу";
            btnLoad->Location = Point(420, 350);
            btnLoad->Size = System::Drawing::Size(140, 35);
            btnLoad->Click += gcnew EventHandler(this, &MyForm::OnLoadClick);

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
            this->Controls->Add(btnSave);
            this->Controls->Add(btnLoad);
        }

    private:
        void OnAddClick(Object^ sender, EventArgs^ e) {
            String^ brand = brandTextBox->Text->Trim();
            String^ model = modelTextBox->Text->Trim();
            String^ yearStr = yearTextBox->Text->Trim();
            String^ priceStr = priceTextBox->Text->Trim();

            if (String::IsNullOrEmpty(brand) || String::IsNullOrEmpty(model) ||
                String::IsNullOrEmpty(yearStr) || String::IsNullOrEmpty(priceStr)) {
                MessageBox::Show(L"Пожалуйста, заполните все поля.", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            int year = 0;
            double price = 0.0;

            if (!Int32::TryParse(yearStr, year) || year < 1886) {
                MessageBox::Show(L"Пожалуйста, введите корректный год.", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            if (!Double::TryParse(priceStr, price) || price < 0) {
                MessageBox::Show(L"Пожалуйста, введите корректную цену.", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            Car^ newCar = gcnew Car(brand, model, year, price, L"Новая", L"");
            carDataGridView->Rows->Add(newCar->Brand, newCar->Model, newCar->Year, newCar->Price);

            brandTextBox->Clear();
            modelTextBox->Clear();
            yearTextBox->Clear();
            priceTextBox->Clear();
        }

        void OnDeleteClick(Object^ sender, EventArgs^ e) {
            if (carDataGridView->SelectedRows->Count > 0) {
                for (int i = carDataGridView->SelectedRows->Count - 1; i >= 0; i--) {
                    if (!carDataGridView->SelectedRows[i]->IsNewRow) {
                        carDataGridView->Rows->Remove(carDataGridView->SelectedRows[i]);
                    }
                }
            } else if (carDataGridView->CurrentRow != nullptr && !carDataGridView->CurrentRow->IsNewRow) {
                carDataGridView->Rows->Remove(carDataGridView->CurrentRow);
            } else {
                MessageBox::Show(L"Пожалуйста, выберите строку для удаления.", L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }
        void SaveToFile(String^ path) {
            try {
                System::IO::StreamWriter^ writer = gcnew System::IO::StreamWriter(path, false, System::Text::Encoding::UTF8);
                for (int i = 0; i < carDataGridView->Rows->Count; i++) {
                    if (!carDataGridView->Rows[i]->IsNewRow) {
                        String^ brand = Convert::ToString(carDataGridView->Rows[i]->Cells[0]->Value);
                        String^ model = Convert::ToString(carDataGridView->Rows[i]->Cells[1]->Value);
                        String^ year = Convert::ToString(carDataGridView->Rows[i]->Cells[2]->Value);
                        String^ price = Convert::ToString(carDataGridView->Rows[i]->Cells[3]->Value);
                        
                        writer->WriteLine(brand + L";" + model + L";" + year + L";" + price);
                    }
                }
                writer->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Ошибка при сохранении файла: " + ex->Message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void OnSaveClick(Object^ sender, EventArgs^ e) {
            SaveToFile(L"cars.txt");
            MessageBox::Show(L"База успешно сохранена в файл cars.txt!", L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        void LoadFromFile(String^ path) {
            try {
                if (!System::IO::File::Exists(path)) {
                    MessageBox::Show(L"Файл базы данных не найден. Сначала сохраните данные.", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    return;
                }

                System::IO::StreamReader^ reader = gcnew System::IO::StreamReader(path, System::Text::Encoding::UTF8);
                carDataGridView->Rows->Clear();

                String^ line;
                while ((line = reader->ReadLine()) != nullptr) {
                    if (String::IsNullOrWhiteSpace(line)) continue;

                    cli::array<String^>^ parts = line->Split(';');
                    if (parts->Length >= 4) {
                        String^ brand = parts[0]->Trim();
                        String^ model = parts[1]->Trim();
                        int year = 0;
                        double price = 0.0;

                        if (Int32::TryParse(parts[2]->Trim(), year) && Double::TryParse(parts[3]->Trim(), price)) {
                            Car^ loadedCar = gcnew Car(brand, model, year, price, L"Не указано", L"");
                            carDataGridView->Rows->Add(loadedCar->Brand, loadedCar->Model, loadedCar->Year, loadedCar->Price);
                        } else {
                            MessageBox::Show(L"Ошибка формата чисел в строке: " + line, L"Ошибка чтения", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                        }
                    } else {
                        MessageBox::Show(L"Поврежденная строка в базе: " + line, L"Ошибка чтения", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    }
                }
                reader->Close();
                MessageBox::Show(L"База успешно загружена!", L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Произошла ошибка при загрузке: " + ex->Message, L"Критическая ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void OnLoadClick(Object^ sender, EventArgs^ e) {
            LoadFromFile(L"cars.txt");
        }
    };
}
