#pragma once
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#include "Car.h"
#include "Client.h"

namespace AutosalonApp {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class MyForm : public Form {
    private:
        TabControl^ tabControl;
        TabPage^ tabCars;
        TabPage^ tabClients;
        TabPage^ tabMatching;

        // --- Вкладка Автомобили ---
        DataGridView^ carDataGridView;
        Label^ brandLabel; TextBox^ brandTextBox;
        Label^ modelLabel; TextBox^ modelTextBox;
        Label^ yearLabel; TextBox^ yearTextBox;
        Label^ priceLabel; TextBox^ priceTextBox;
        Label^ mvLabel; TextBox^ mvTextBox;
        Label^ conditionLabel; TextBox^ conditionTextBox;
        Label^ specsLabel; TextBox^ specsTextBox;
        Button^ addCarButton; Button^ deleteCarButton;
        Button^ saveCarsButton; Button^ loadCarsButton;

        // --- Вкладка Клиенты (Заготовка) ---
        DataGridView^ clientDataGridView;
        Label^ nameLabel; TextBox^ nameTextBox;
        Label^ phoneLabel; TextBox^ phoneTextBox;
        Label^ budgetLabel; TextBox^ budgetTextBox;
        Label^ reqBrandLabel; TextBox^ reqBrandTextBox;
        Label^ reqModelLabel; TextBox^ reqModelTextBox;
        Label^ reqConditionLabel; TextBox^ reqConditionTextBox;
        Button^ addClientButton; Button^ deleteClientButton;
        Button^ saveClientsButton; Button^ loadClientsButton;

        // --- Вкладка Подбор (Заготовка) ---
        Label^ clientSelectLabel;
        ComboBox^ clientComboBox;
        Button^ refreshClientsButton;
        Button^ matchButton;
        Button^ orderButton;
        DataGridView^ matchingDataGridView;

    public:
        MyForm() {
            this->Text = L"Система управления автосалоном";
            this->Size = System::Drawing::Size(800, 600);

            tabControl = gcnew TabControl();
            tabControl->Dock = DockStyle::Fill;

            tabCars = gcnew TabPage(L"Автомобили");
            tabClients = gcnew TabPage(L"Клиенты");
            tabMatching = gcnew TabPage(L"Подбор и Логистика");

            tabControl->TabPages->Add(tabCars);
            tabControl->TabPages->Add(tabClients);
            tabControl->TabPages->Add(tabMatching);

            this->Controls->Add(tabControl);

            InitializeCarsTab();
            InitializeClientsTab();
            InitializeMatchingTab();
        }

    private:
        void InitializeCarsTab() {
            carDataGridView = gcnew DataGridView();
            carDataGridView->Location = Point(10, 10);
            carDataGridView->Size = System::Drawing::Size(740, 380);
            carDataGridView->ColumnCount = 7;
            carDataGridView->Columns[0]->Name = L"Марка";
            carDataGridView->Columns[1]->Name = L"Модель";
            carDataGridView->Columns[2]->Name = L"Год";
            carDataGridView->Columns[3]->Name = L"Цена";
            carDataGridView->Columns[4]->Name = L"Рын. стоимость";
            carDataGridView->Columns[5]->Name = L"Состояние";
            carDataGridView->Columns[6]->Name = L"Характеристики";

            // Марка
            brandLabel = gcnew Label(); brandLabel->Text = L"Марка:"; brandLabel->Location = Point(10, 400); brandLabel->AutoSize = true;
            brandTextBox = gcnew TextBox(); brandTextBox->Location = Point(10, 420); brandTextBox->Width = 100;

            // Модель
            modelLabel = gcnew Label(); modelLabel->Text = L"Модель:"; modelLabel->Location = Point(120, 400); modelLabel->AutoSize = true;
            modelTextBox = gcnew TextBox(); modelTextBox->Location = Point(120, 420); modelTextBox->Width = 100;

            // Год
            yearLabel = gcnew Label(); yearLabel->Text = L"Год:"; yearLabel->Location = Point(230, 400); yearLabel->AutoSize = true;
            yearTextBox = gcnew TextBox(); yearTextBox->Location = Point(230, 420); yearTextBox->Width = 60;

            // Цена
            priceLabel = gcnew Label(); priceLabel->Text = L"Цена:"; priceLabel->Location = Point(300, 400); priceLabel->AutoSize = true;
            priceTextBox = gcnew TextBox(); priceTextBox->Location = Point(300, 420); priceTextBox->Width = 70;

            // Рын. стоимость
            mvLabel = gcnew Label(); mvLabel->Text = L"Рын. стоим.:"; mvLabel->Location = Point(380, 400); mvLabel->AutoSize = true;
            mvTextBox = gcnew TextBox(); mvTextBox->Location = Point(380, 420); mvTextBox->Width = 80;

            // Состояние
            conditionLabel = gcnew Label(); conditionLabel->Text = L"Состояние:"; conditionLabel->Location = Point(470, 400); conditionLabel->AutoSize = true;
            conditionTextBox = gcnew TextBox(); conditionTextBox->Location = Point(470, 420); conditionTextBox->Width = 100;

            // Спецификации
            specsLabel = gcnew Label(); specsLabel->Text = L"Характеристики:"; specsLabel->Location = Point(580, 400); specsLabel->AutoSize = true;
            specsTextBox = gcnew TextBox(); specsTextBox->Location = Point(580, 420); specsTextBox->Width = 150;

            // Кнопки
            addCarButton = gcnew Button(); addCarButton->Text = L"Добавить"; addCarButton->Location = Point(10, 460); addCarButton->Size = System::Drawing::Size(100, 35); addCarButton->Click += gcnew EventHandler(this, &MyForm::OnAddCarClick);
            deleteCarButton = gcnew Button(); deleteCarButton->Text = L"Удалить"; deleteCarButton->Location = Point(120, 460); deleteCarButton->Size = System::Drawing::Size(100, 35); deleteCarButton->Click += gcnew EventHandler(this, &MyForm::OnDeleteCarClick);
            saveCarsButton = gcnew Button(); saveCarsButton->Text = L"Сохранить БД"; saveCarsButton->Location = Point(230, 460); saveCarsButton->Size = System::Drawing::Size(120, 35); saveCarsButton->Click += gcnew EventHandler(this, &MyForm::OnSaveCarsClick);
            loadCarsButton = gcnew Button(); loadCarsButton->Text = L"Загрузить БД"; loadCarsButton->Location = Point(360, 460); loadCarsButton->Size = System::Drawing::Size(120, 35); loadCarsButton->Click += gcnew EventHandler(this, &MyForm::OnLoadCarsClick);

            tabCars->Controls->Add(carDataGridView);
            tabCars->Controls->Add(brandLabel); tabCars->Controls->Add(brandTextBox);
            tabCars->Controls->Add(modelLabel); tabCars->Controls->Add(modelTextBox);
            tabCars->Controls->Add(yearLabel); tabCars->Controls->Add(yearTextBox);
            tabCars->Controls->Add(priceLabel); tabCars->Controls->Add(priceTextBox);
            tabCars->Controls->Add(mvLabel); tabCars->Controls->Add(mvTextBox);
            tabCars->Controls->Add(conditionLabel); tabCars->Controls->Add(conditionTextBox);
            tabCars->Controls->Add(specsLabel); tabCars->Controls->Add(specsTextBox);
            tabCars->Controls->Add(addCarButton); tabCars->Controls->Add(deleteCarButton);
            tabCars->Controls->Add(saveCarsButton); tabCars->Controls->Add(loadCarsButton);
        }

        void InitializeClientsTab() {
            clientDataGridView = gcnew DataGridView();
            clientDataGridView->Location = Point(10, 10);
            clientDataGridView->Size = System::Drawing::Size(740, 380);
            clientDataGridView->ColumnCount = 6;
            clientDataGridView->Columns[0]->Name = L"ФИО";
            clientDataGridView->Columns[1]->Name = L"Телефон";
            clientDataGridView->Columns[2]->Name = L"Бюджет";
            clientDataGridView->Columns[3]->Name = L"Желаемая Марка";
            clientDataGridView->Columns[4]->Name = L"Желаемая Модель";
            clientDataGridView->Columns[5]->Name = L"Желаемое Состояние";

            // ФИО
            nameLabel = gcnew Label(); nameLabel->Text = L"ФИО:"; nameLabel->Location = Point(10, 400); nameLabel->AutoSize = true;
            nameTextBox = gcnew TextBox(); nameTextBox->Location = Point(10, 420); nameTextBox->Width = 120;

            // Телефон
            phoneLabel = gcnew Label(); phoneLabel->Text = L"Телефон:"; phoneLabel->Location = Point(140, 400); phoneLabel->AutoSize = true;
            phoneTextBox = gcnew TextBox(); phoneTextBox->Location = Point(140, 420); phoneTextBox->Width = 100;

            // Бюджет
            budgetLabel = gcnew Label(); budgetLabel->Text = L"Бюджет:"; budgetLabel->Location = Point(250, 400); budgetLabel->AutoSize = true;
            budgetTextBox = gcnew TextBox(); budgetTextBox->Location = Point(250, 420); budgetTextBox->Width = 80;

            // Желаемая Марка
            reqBrandLabel = gcnew Label(); reqBrandLabel->Text = L"Жел. Марка:"; reqBrandLabel->Location = Point(340, 400); reqBrandLabel->AutoSize = true;
            reqBrandTextBox = gcnew TextBox(); reqBrandTextBox->Location = Point(340, 420); reqBrandTextBox->Width = 100;

            // Желаемая Модель
            reqModelLabel = gcnew Label(); reqModelLabel->Text = L"Жел. Модель:"; reqModelLabel->Location = Point(450, 400); reqModelLabel->AutoSize = true;
            reqModelTextBox = gcnew TextBox(); reqModelTextBox->Location = Point(450, 420); reqModelTextBox->Width = 100;

            // Желаемое состояние
            reqConditionLabel = gcnew Label(); reqConditionLabel->Text = L"Жел. Состояние:"; reqConditionLabel->Location = Point(560, 400); reqConditionLabel->AutoSize = true;
            reqConditionTextBox = gcnew TextBox(); reqConditionTextBox->Location = Point(560, 420); reqConditionTextBox->Width = 100;

            // Кнопки
            addClientButton = gcnew Button(); addClientButton->Text = L"Добавить"; addClientButton->Location = Point(10, 460); addClientButton->Size = System::Drawing::Size(100, 35); addClientButton->Click += gcnew EventHandler(this, &MyForm::OnAddClientClick);
            deleteClientButton = gcnew Button(); deleteClientButton->Text = L"Удалить"; deleteClientButton->Location = Point(120, 460); deleteClientButton->Size = System::Drawing::Size(100, 35); deleteClientButton->Click += gcnew EventHandler(this, &MyForm::OnDeleteClientClick);
            saveClientsButton = gcnew Button(); saveClientsButton->Text = L"Сохранить БД"; saveClientsButton->Location = Point(230, 460); saveClientsButton->Size = System::Drawing::Size(120, 35); saveClientsButton->Click += gcnew EventHandler(this, &MyForm::OnSaveClientsClick);
            loadClientsButton = gcnew Button(); loadClientsButton->Text = L"Загрузить БД"; loadClientsButton->Location = Point(360, 460); loadClientsButton->Size = System::Drawing::Size(120, 35); loadClientsButton->Click += gcnew EventHandler(this, &MyForm::OnLoadClientsClick);

            tabClients->Controls->Add(clientDataGridView);
            tabClients->Controls->Add(nameLabel); tabClients->Controls->Add(nameTextBox);
            tabClients->Controls->Add(phoneLabel); tabClients->Controls->Add(phoneTextBox);
            tabClients->Controls->Add(budgetLabel); tabClients->Controls->Add(budgetTextBox);
            tabClients->Controls->Add(reqBrandLabel); tabClients->Controls->Add(reqBrandTextBox);
            tabClients->Controls->Add(reqModelLabel); tabClients->Controls->Add(reqModelTextBox);
            tabClients->Controls->Add(reqConditionLabel); tabClients->Controls->Add(reqConditionTextBox);
            tabClients->Controls->Add(addClientButton); tabClients->Controls->Add(deleteClientButton);
            tabClients->Controls->Add(saveClientsButton); tabClients->Controls->Add(loadClientsButton);
        }

        void InitializeMatchingTab() {
            clientSelectLabel = gcnew Label(); clientSelectLabel->Text = L"Выберите клиента:"; clientSelectLabel->Location = Point(10, 15); clientSelectLabel->AutoSize = true;
            
            clientComboBox = gcnew ComboBox(); clientComboBox->Location = Point(130, 12); clientComboBox->Width = 200;
            clientComboBox->DropDownStyle = ComboBoxStyle::DropDownList;
            
            refreshClientsButton = gcnew Button(); refreshClientsButton->Text = L"Обновить список"; refreshClientsButton->Location = Point(340, 10); refreshClientsButton->Size = System::Drawing::Size(120, 25); refreshClientsButton->Click += gcnew EventHandler(this, &MyForm::OnRefreshClientsClick);
            
            matchButton = gcnew Button(); matchButton->Text = L"Найти подходящие авто"; matchButton->Location = Point(10, 50); matchButton->Size = System::Drawing::Size(160, 35); matchButton->Click += gcnew EventHandler(this, &MyForm::OnMatchClick);
            
            orderButton = gcnew Button(); orderButton->Text = L"Сформировать заявку"; orderButton->Location = Point(180, 50); orderButton->Size = System::Drawing::Size(160, 35); orderButton->Click += gcnew EventHandler(this, &MyForm::OnOrderClick);
            
            matchingDataGridView = gcnew DataGridView();
            matchingDataGridView->Location = Point(10, 100);
            matchingDataGridView->Size = System::Drawing::Size(740, 290);
            matchingDataGridView->ColumnCount = 7;
            matchingDataGridView->Columns[0]->Name = L"Марка";
            matchingDataGridView->Columns[1]->Name = L"Модель";
            matchingDataGridView->Columns[2]->Name = L"Год";
            matchingDataGridView->Columns[3]->Name = L"Цена";
            matchingDataGridView->Columns[4]->Name = L"Рын. стоимость";
            matchingDataGridView->Columns[5]->Name = L"Состояние";
            matchingDataGridView->Columns[6]->Name = L"Характеристики";

            tabMatching->Controls->Add(clientSelectLabel);
            tabMatching->Controls->Add(clientComboBox);
            tabMatching->Controls->Add(refreshClientsButton);
            tabMatching->Controls->Add(matchButton);
            tabMatching->Controls->Add(orderButton);
            tabMatching->Controls->Add(matchingDataGridView);
        }

        void OnAddCarClick(Object^ sender, EventArgs^ e) {
            String^ brand = brandTextBox->Text->Trim();
            String^ model = modelTextBox->Text->Trim();
            String^ yearStr = yearTextBox->Text->Trim();
            String^ priceStr = priceTextBox->Text->Trim();
            String^ mvStr = mvTextBox->Text->Trim();
            String^ condition = conditionTextBox->Text->Trim();
            String^ specs = specsTextBox->Text->Trim();

            if (String::IsNullOrEmpty(brand) || String::IsNullOrEmpty(model) ||
                String::IsNullOrEmpty(yearStr) || String::IsNullOrEmpty(priceStr) ||
                String::IsNullOrEmpty(mvStr) || String::IsNullOrEmpty(condition)) {
                MessageBox::Show(L"Пожалуйста, заполните основные поля (кроме характеристик).", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            int year = 0;
            double price = 0.0;
            double mv = 0.0;

            if (!Int32::TryParse(yearStr, year) || year < 1886) {
                MessageBox::Show(L"Пожалуйста, введите корректный год.", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            if (!Double::TryParse(priceStr, price) || price < 0) {
                MessageBox::Show(L"Пожалуйста, введите корректную цену.", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            
            if (!Double::TryParse(mvStr, mv) || mv < 0) {
                MessageBox::Show(L"Пожалуйста, введите корректную рыночную стоимость.", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            Car^ newCar = gcnew Car(brand, model, year, price, mv, condition, specs);
            carDataGridView->Rows->Add(newCar->Brand, newCar->Model, newCar->Year, newCar->Price, newCar->MarketValue, newCar->Condition, newCar->Specs);

            brandTextBox->Clear();
            modelTextBox->Clear();
            yearTextBox->Clear();
            priceTextBox->Clear();
            mvTextBox->Clear();
            conditionTextBox->Clear();
            specsTextBox->Clear();
        }

        void OnDeleteCarClick(Object^ sender, EventArgs^ e) {
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

        void SaveCarsToFile(String^ path) {
            try {
                System::IO::StreamWriter^ writer = gcnew System::IO::StreamWriter(path, false, System::Text::Encoding::UTF8);
                for (int i = 0; i < carDataGridView->Rows->Count; i++) {
                    if (!carDataGridView->Rows[i]->IsNewRow) {
                        String^ brand = Convert::ToString(carDataGridView->Rows[i]->Cells[0]->Value);
                        String^ model = Convert::ToString(carDataGridView->Rows[i]->Cells[1]->Value);
                        String^ year = Convert::ToString(carDataGridView->Rows[i]->Cells[2]->Value);
                        String^ price = Convert::ToString(carDataGridView->Rows[i]->Cells[3]->Value);
                        String^ mv = Convert::ToString(carDataGridView->Rows[i]->Cells[4]->Value);
                        String^ cond = Convert::ToString(carDataGridView->Rows[i]->Cells[5]->Value);
                        String^ specs = Convert::ToString(carDataGridView->Rows[i]->Cells[6]->Value);
                        
                        writer->WriteLine(brand + L";" + model + L";" + year + L";" + price + L";" + mv + L";" + cond + L";" + specs);
                    }
                }
                writer->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Ошибка при сохранении файла: " + ex->Message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void OnSaveCarsClick(Object^ sender, EventArgs^ e) {
            SaveCarsToFile(L"cars.txt");
            MessageBox::Show(L"База автомобилей успешно сохранена в файл cars.txt!", L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        void LoadCarsFromFile(String^ path) {
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
                        double mv = 0.0;
                        String^ cond = L"Не указано";
                        String^ specs = L"";

                        if (Int32::TryParse(parts[2]->Trim(), year) && Double::TryParse(parts[3]->Trim(), price)) {
                            if (parts->Length >= 7) {
                                Double::TryParse(parts[4]->Trim(), mv);
                                cond = parts[5]->Trim();
                                specs = parts[6]->Trim();
                            }
                            Car^ loadedCar = gcnew Car(brand, model, year, price, mv, cond, specs);
                            carDataGridView->Rows->Add(loadedCar->Brand, loadedCar->Model, loadedCar->Year, loadedCar->Price, loadedCar->MarketValue, loadedCar->Condition, loadedCar->Specs);
                        } else {
                            MessageBox::Show(L"Ошибка формата чисел в строке: " + line, L"Ошибка чтения", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                        }
                    } else {
                        MessageBox::Show(L"Поврежденная строка в базе: " + line, L"Ошибка чтения", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    }
                }
                reader->Close();
                MessageBox::Show(L"База автомобилей успешно загружена!", L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Произошла ошибка при загрузке: " + ex->Message, L"Критическая ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void OnLoadCarsClick(Object^ sender, EventArgs^ e) {
            LoadCarsFromFile(L"cars.txt");
        }

        // --- Обработчики вкладки Клиентов ---
        void OnAddClientClick(Object^ sender, EventArgs^ e) {
            String^ name = nameTextBox->Text->Trim();
            String^ phone = phoneTextBox->Text->Trim();
            String^ budgetStr = budgetTextBox->Text->Trim();
            String^ rBrand = reqBrandTextBox->Text->Trim();
            String^ rModel = reqModelTextBox->Text->Trim();
            String^ rCond = reqConditionTextBox->Text->Trim();

            if (String::IsNullOrEmpty(name) || String::IsNullOrEmpty(phone) || String::IsNullOrEmpty(budgetStr)) {
                MessageBox::Show(L"Пожалуйста, заполните ФИО, телефон и бюджет.", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            double budget = 0.0;
            if (!Double::TryParse(budgetStr, budget) || budget < 0) {
                MessageBox::Show(L"Пожалуйста, введите корректный бюджет.", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            Client^ newClient = gcnew Client(name, phone, budget, rBrand, rModel, rCond);
            clientDataGridView->Rows->Add(newClient->FullName, newClient->Phone, newClient->Budget, newClient->ReqBrand, newClient->ReqModel, newClient->ReqCondition);

            nameTextBox->Clear();
            phoneTextBox->Clear();
            budgetTextBox->Clear();
            reqBrandTextBox->Clear();
            reqModelTextBox->Clear();
            reqConditionTextBox->Clear();
        }

        void OnDeleteClientClick(Object^ sender, EventArgs^ e) {
            if (clientDataGridView->SelectedRows->Count > 0) {
                for (int i = clientDataGridView->SelectedRows->Count - 1; i >= 0; i--) {
                    if (!clientDataGridView->SelectedRows[i]->IsNewRow) {
                        clientDataGridView->Rows->Remove(clientDataGridView->SelectedRows[i]);
                    }
                }
            } else if (clientDataGridView->CurrentRow != nullptr && !clientDataGridView->CurrentRow->IsNewRow) {
                clientDataGridView->Rows->Remove(clientDataGridView->CurrentRow);
            } else {
                MessageBox::Show(L"Пожалуйста, выберите строку для удаления.", L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void SaveClientsToFile(String^ path) {
            try {
                System::IO::StreamWriter^ writer = gcnew System::IO::StreamWriter(path, false, System::Text::Encoding::UTF8);
                for (int i = 0; i < clientDataGridView->Rows->Count; i++) {
                    if (!clientDataGridView->Rows[i]->IsNewRow) {
                        String^ name = Convert::ToString(clientDataGridView->Rows[i]->Cells[0]->Value);
                        String^ phone = Convert::ToString(clientDataGridView->Rows[i]->Cells[1]->Value);
                        String^ budget = Convert::ToString(clientDataGridView->Rows[i]->Cells[2]->Value);
                        String^ rBrand = Convert::ToString(clientDataGridView->Rows[i]->Cells[3]->Value);
                        String^ rModel = Convert::ToString(clientDataGridView->Rows[i]->Cells[4]->Value);
                        String^ rCond = Convert::ToString(clientDataGridView->Rows[i]->Cells[5]->Value);
                        
                        writer->WriteLine(name + L";" + phone + L";" + budget + L";" + rBrand + L";" + rModel + L";" + rCond);
                    }
                }
                writer->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Ошибка при сохранении файла: " + ex->Message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void OnSaveClientsClick(Object^ sender, EventArgs^ e) {
            SaveClientsToFile(L"clients.txt");
            MessageBox::Show(L"База клиентов успешно сохранена в файл clients.txt!", L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        void LoadClientsFromFile(String^ path) {
            try {
                if (!System::IO::File::Exists(path)) {
                    MessageBox::Show(L"Файл базы клиентов не найден.", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    return;
                }

                System::IO::StreamReader^ reader = gcnew System::IO::StreamReader(path, System::Text::Encoding::UTF8);
                clientDataGridView->Rows->Clear();

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
                            Client^ loadedClient = gcnew Client(name, phone, budget, rBrand, rModel, rCond);
                            clientDataGridView->Rows->Add(loadedClient->FullName, loadedClient->Phone, loadedClient->Budget, loadedClient->ReqBrand, loadedClient->ReqModel, loadedClient->ReqCondition);
                        }
                    }
                }
                reader->Close();
                MessageBox::Show(L"База клиентов успешно загружена!", L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Произошла ошибка при загрузке: " + ex->Message, L"Критическая ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void OnLoadClientsClick(Object^ sender, EventArgs^ e) {
            LoadClientsFromFile(L"clients.txt");
        }

        // --- Обработчики вкладки Подбора ---
        void OnRefreshClientsClick(Object^ sender, EventArgs^ e) {
            clientComboBox->Items->Clear();
            for (int i = 0; i < clientDataGridView->Rows->Count; i++) {
                if (!clientDataGridView->Rows[i]->IsNewRow) {
                    String^ name = Convert::ToString(clientDataGridView->Rows[i]->Cells[0]->Value);
                    if (!String::IsNullOrEmpty(name)) {
                        clientComboBox->Items->Add(name);
                    }
                }
            }
            if (clientComboBox->Items->Count > 0) {
                clientComboBox->SelectedIndex = 0;
            } else {
                MessageBox::Show(L"Список клиентов пуст. Перейдите на вкладку 'Клиенты' и добавьте данные.", L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void OnMatchClick(Object^ sender, EventArgs^ e) {
            if (clientComboBox->SelectedIndex == -1) {
                MessageBox::Show(L"Пожалуйста, выберите клиента для подбора.", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            String^ selectedClientName = clientComboBox->SelectedItem->ToString();
            double cBudget = 0;
            String^ cBrand = L"";
            String^ cModel = L"";
            String^ cCond = L"";

            for (int i = 0; i < clientDataGridView->Rows->Count; i++) {
                if (!clientDataGridView->Rows[i]->IsNewRow) {
                    if (Convert::ToString(clientDataGridView->Rows[i]->Cells[0]->Value) == selectedClientName) {
                        cBudget = Convert::ToDouble(clientDataGridView->Rows[i]->Cells[2]->Value);
                        cBrand = Convert::ToString(clientDataGridView->Rows[i]->Cells[3]->Value)->ToLower();
                        cModel = Convert::ToString(clientDataGridView->Rows[i]->Cells[4]->Value)->ToLower();
                        cCond = Convert::ToString(clientDataGridView->Rows[i]->Cells[5]->Value)->ToLower();
                        break;
                    }
                }
            }

            matchingDataGridView->Rows->Clear();
            int matchCount = 0;

            for (int i = 0; i < carDataGridView->Rows->Count; i++) {
                if (!carDataGridView->Rows[i]->IsNewRow) {
                    double carPrice = Convert::ToDouble(carDataGridView->Rows[i]->Cells[3]->Value);
                    String^ carBrand = Convert::ToString(carDataGridView->Rows[i]->Cells[0]->Value)->ToLower();
                    String^ carModel = Convert::ToString(carDataGridView->Rows[i]->Cells[1]->Value)->ToLower();
                    String^ carCond = Convert::ToString(carDataGridView->Rows[i]->Cells[5]->Value)->ToLower();

                    bool matchBrand = String::IsNullOrEmpty(cBrand) || carBrand->Contains(cBrand);
                    bool matchModel = String::IsNullOrEmpty(cModel) || carModel->Contains(cModel);
                    bool matchCond = String::IsNullOrEmpty(cCond) || carCond->Contains(cCond);
                    bool matchBudget = carPrice <= cBudget;

                    if (matchBudget && matchBrand && matchModel && matchCond) {
                        matchingDataGridView->Rows->Add(
                            carDataGridView->Rows[i]->Cells[0]->Value,
                            carDataGridView->Rows[i]->Cells[1]->Value,
                            carDataGridView->Rows[i]->Cells[2]->Value,
                            carDataGridView->Rows[i]->Cells[3]->Value,
                            carDataGridView->Rows[i]->Cells[4]->Value,
                            carDataGridView->Rows[i]->Cells[5]->Value,
                            carDataGridView->Rows[i]->Cells[6]->Value
                        );
                        matchCount++;
                    }
                }
            }

            if (matchCount == 0) {
                MessageBox::Show(L"Подходящих автомобилей не найдено. Вы можете сформировать заявку поставщику.", L"Результат", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void OnOrderClick(Object^ sender, EventArgs^ e) {
            if (clientComboBox->SelectedIndex == -1) {
                MessageBox::Show(L"Сначала выберите клиента.", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            String^ selectedClientName = clientComboBox->SelectedItem->ToString();
            double cBudget = 0;
            String^ cBrand = L"";
            String^ cModel = L"";
            String^ cPhone = L"";

            for (int i = 0; i < clientDataGridView->Rows->Count; i++) {
                if (!clientDataGridView->Rows[i]->IsNewRow) {
                    if (Convert::ToString(clientDataGridView->Rows[i]->Cells[0]->Value) == selectedClientName) {
                        cPhone = Convert::ToString(clientDataGridView->Rows[i]->Cells[1]->Value);
                        cBudget = Convert::ToDouble(clientDataGridView->Rows[i]->Cells[2]->Value);
                        cBrand = Convert::ToString(clientDataGridView->Rows[i]->Cells[3]->Value);
                        cModel = Convert::ToString(clientDataGridView->Rows[i]->Cells[4]->Value);
                        break;
                    }
                }
            }

            try {
                System::IO::StreamWriter^ writer = gcnew System::IO::StreamWriter(L"orders.txt", true, System::Text::Encoding::UTF8);
                String^ date = DateTime::Now.ToString(L"dd.MM.yyyy HH:mm");
                writer->WriteLine(L"[" + date + L"] ЗАЯВКА НА ПОСТАВКУ:");
                writer->WriteLine(L"Клиент: " + selectedClientName + L" (Тел: " + cPhone + L")");
                writer->WriteLine(L"Требуется: " + cBrand + L" " + cModel + L" до " + cBudget + L" у.е.");
                writer->WriteLine(L"------------------------------------------------");
                writer->Close();

                MessageBox::Show(L"Заявка успешно сформирована и сохранена в файл orders.txt!", L"Логистика", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Ошибка при формировании заявки: " + ex->Message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
    };
}
