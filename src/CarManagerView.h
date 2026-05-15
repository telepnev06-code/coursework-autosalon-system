#pragma once
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>
#include "DataService.h"
#include "RoundedButton.h"
#include "UIHelpers.h"

namespace AutosalonApp {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class CarManagerView : public UserControl {
    private:
        TableLayoutPanel^ mainLayout;
        DataGridView^ grid;
        FlowLayoutPanel^ inputLayout;

        TextBox^ brandTextBox;
        TextBox^ modelTextBox;
        TextBox^ yearTextBox;
        TextBox^ priceTextBox;
        TextBox^ mvTextBox;
        TextBox^ conditionTextBox;
        TextBox^ specsTextBox;

        RoundedButton^ addBtn;
        RoundedButton^ delBtn;

        int selectedIndex = -1;

        Panel^ WrapInput(String^ labelText, Control^ inputControl) {
            Panel^ p = gcnew Panel();
            p->Size = System::Drawing::Size(190, 60);
            p->Margin = System::Windows::Forms::Padding(5);
            Label^ lbl = gcnew Label();
            lbl->Text = labelText;
            lbl->AutoSize = true;
            lbl->Location = Point(0,0);
            inputControl->Location = Point(0, 22);
            inputControl->Width = 180;
            p->Controls->Add(lbl);
            p->Controls->Add(inputControl);
            return p;
        }

        void ClearInputs() {
            brandTextBox->Text = L""; modelTextBox->Text = L""; yearTextBox->Text = L"";
            priceTextBox->Text = L""; mvTextBox->Text = L""; conditionTextBox->Text = L""; specsTextBox->Text = L"";
            addBtn->Text = L"Добавить";
            selectedIndex = -1;
            grid->ClearSelection();
        }

    public:
        CarManagerView() {
            this->Dock = DockStyle::Fill;
            this->BackColor = Color::Transparent;
            
            mainLayout = gcnew TableLayoutPanel();
            mainLayout->Dock = DockStyle::Fill;
            mainLayout->RowCount = 2;
            mainLayout->ColumnCount = 1;
            mainLayout->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 70));
            mainLayout->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 30));

            Panel^ gridCard = UIHelpers::CreateCard();
            gridCard->Dock = DockStyle::Fill;
            grid = gcnew DataGridView();
            grid->Dock = DockStyle::Fill;
            
            grid->ColumnCount = 7;
            grid->Columns[0]->Name = L"Марка"; grid->Columns[0]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            grid->Columns[1]->Name = L"Модель"; grid->Columns[1]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            grid->Columns[2]->Name = L"Год"; grid->Columns[2]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            grid->Columns[3]->Name = L"Цена"; grid->Columns[3]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            grid->Columns[4]->Name = L"Рыночная стоимость"; grid->Columns[4]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            grid->Columns[5]->Name = L"Состояние"; grid->Columns[5]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            grid->Columns[6]->Name = L"Характеристики"; grid->Columns[6]->AutoSizeMode = DataGridViewAutoSizeColumnMode::Fill;
            grid->Columns[6]->MinimumWidth = 200;
            UIHelpers::StyleGrid(grid);
            grid->SelectionChanged += gcnew EventHandler(this, &CarManagerView::OnGridSelect);
            gridCard->Controls->Add(grid);

            Panel^ inputCard = UIHelpers::CreateCard();
            inputCard->Dock = DockStyle::Fill;
            inputLayout = gcnew FlowLayoutPanel();
            inputLayout->Dock = DockStyle::Fill;
            inputLayout->AutoScroll = true;

            brandTextBox = gcnew TextBox(); modelTextBox = gcnew TextBox(); yearTextBox = gcnew TextBox();
            priceTextBox = gcnew TextBox(); mvTextBox = gcnew TextBox(); conditionTextBox = gcnew TextBox(); specsTextBox = gcnew TextBox();

            inputLayout->Controls->Add(WrapInput(L"Марка:", brandTextBox));
            inputLayout->Controls->Add(WrapInput(L"Модель:", modelTextBox));
            inputLayout->Controls->Add(WrapInput(L"Год:", yearTextBox));
            inputLayout->Controls->Add(WrapInput(L"Цена:", priceTextBox));
            inputLayout->Controls->Add(WrapInput(L"Рыночная стоимость:", mvTextBox));
            inputLayout->Controls->Add(WrapInput(L"Состояние:", conditionTextBox));
            inputLayout->Controls->Add(WrapInput(L"Характеристики:", specsTextBox));

            FlowLayoutPanel^ btnLayout = gcnew FlowLayoutPanel();
            btnLayout->Width = 600; btnLayout->Height = 50;
            addBtn = gcnew RoundedButton(); addBtn->Text = L"Добавить"; addBtn->Size = System::Drawing::Size(150, 35); addBtn->Click += gcnew EventHandler(this, &CarManagerView::OnAdd);
            delBtn = gcnew RoundedButton(); delBtn->Text = L"Удалить"; delBtn->Size = System::Drawing::Size(100, 35); delBtn->DefaultColor = Color::FromArgb(220, 53, 69); delBtn->Click += gcnew EventHandler(this, &CarManagerView::OnDel);
            
            btnLayout->Controls->Add(addBtn); btnLayout->Controls->Add(delBtn);
            inputLayout->Controls->Add(btnLayout);
            inputCard->Controls->Add(inputLayout);

            mainLayout->Controls->Add(gridCard, 0, 0);
            mainLayout->Controls->Add(inputCard, 0, 1);
            this->Controls->Add(mainLayout);

            DataService::OnDataChanged += gcnew EventHandler(this, &CarManagerView::OnDataChanged);
        }

        void RefreshGrid() {
            grid->Rows->Clear();
            for each (Car^ c in DataService::Cars) {
                grid->Rows->Add(c->Brand, c->Model, c->Year, c->Price, c->MarketValue, c->Condition, c->Specs);
            }
            ClearInputs();
        }

    private:
        void OnDataChanged(Object^ sender, EventArgs^ e) { RefreshGrid(); }
        
        void OnGridSelect(Object^ sender, EventArgs^ e) {
            if (grid->SelectedRows->Count > 0) {
                selectedIndex = grid->SelectedRows[0]->Index;
                if (selectedIndex >= 0 && selectedIndex < DataService::Cars->Count) {
                    Car^ c = DataService::Cars[selectedIndex];
                    brandTextBox->Text = c->Brand;
                    modelTextBox->Text = c->Model;
                    yearTextBox->Text = c->Year.ToString();
                    priceTextBox->Text = c->Price.ToString();
                    mvTextBox->Text = c->MarketValue.ToString();
                    conditionTextBox->Text = c->Condition;
                    specsTextBox->Text = c->Specs;
                    addBtn->Text = L"Изменить";
                }
            }
        }

        void OnAdd(Object^ sender, EventArgs^ e) {
            int year = 0; double price = 0, mv = 0;
            if (!Int32::TryParse(yearTextBox->Text, year) || !Double::TryParse(priceTextBox->Text, price) || !Double::TryParse(mvTextBox->Text, mv)) return;
            
            Car^ newCar = gcnew Car(brandTextBox->Text, modelTextBox->Text, year, price, mv, conditionTextBox->Text, specsTextBox->Text);
            
            if (selectedIndex >= 0 && selectedIndex < DataService::Cars->Count && addBtn->Text == L"Изменить") {
                DataService::Cars[selectedIndex] = newCar;
            } else {
                DataService::Cars->Add(newCar);
            }
            DataService::SaveCars(L"cars.txt");
            DataService::NotifyDataChanged();
        }

        void OnDel(Object^ sender, EventArgs^ e) {
            if (grid->CurrentRow != nullptr && grid->CurrentRow->Index < DataService::Cars->Count) {
                DataService::Cars->RemoveAt(grid->CurrentRow->Index);
                DataService::SaveCars(L"cars.txt");
                DataService::NotifyDataChanged();
            }
        }
    };
}
