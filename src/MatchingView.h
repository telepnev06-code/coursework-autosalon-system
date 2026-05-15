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
    using namespace System::Collections::Generic;

    public ref class MatchingView : public UserControl {
    private:
        TableLayoutPanel^ mainLayout;
        DataGridView^ grid;
        ComboBox^ clientComboBox;
        Label^ requestLabel;
        RoundedButton^ matchBtn;

    public:
        MatchingView() {
            this->Dock = DockStyle::Fill;
            this->BackColor = Color::Transparent;
            
            mainLayout = gcnew TableLayoutPanel();
            mainLayout->Dock = DockStyle::Fill;
            mainLayout->RowCount = 2;
            mainLayout->ColumnCount = 1;
            mainLayout->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 80));
            mainLayout->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 100));

            Panel^ topCard = UIHelpers::CreateCard();
            topCard->Dock = DockStyle::Fill;
            
            Label^ lbl1 = gcnew Label(); lbl1->Text = L"Выберите клиента:"; lbl1->Location = Point(10, 15); lbl1->AutoSize = true;
            clientComboBox = gcnew ComboBox(); clientComboBox->DropDownStyle = ComboBoxStyle::DropDownList; clientComboBox->Location = Point(140, 12); clientComboBox->Width = 200;
            clientComboBox->SelectedIndexChanged += gcnew EventHandler(this, &MatchingView::OnClientSel);
            
            matchBtn = gcnew RoundedButton(); matchBtn->Text = L"Найти авто"; matchBtn->Location = Point(350, 10); matchBtn->Size = System::Drawing::Size(120, 30); matchBtn->Click += gcnew EventHandler(this, &MatchingView::OnMatch);

            requestLabel = gcnew Label(); requestLabel->Text = L"Заявка: -"; requestLabel->Location = Point(10, 45); requestLabel->AutoSize = true; requestLabel->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);

            topCard->Controls->Add(lbl1); topCard->Controls->Add(clientComboBox); topCard->Controls->Add(matchBtn); topCard->Controls->Add(requestLabel);

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
            UIHelpers::StyleGrid(grid);
            gridCard->Controls->Add(grid);

            mainLayout->Controls->Add(topCard, 0, 0);
            mainLayout->Controls->Add(gridCard, 0, 1);
            this->Controls->Add(mainLayout);

            DataService::OnDataChanged += gcnew EventHandler(this, &MatchingView::OnDataChanged);
            RefreshClients();
        }

        void RefreshClients() {
            clientComboBox->Items->Clear();
            for each (Client^ c in DataService::Clients) {
                clientComboBox->Items->Add(c->FullName);
            }
            if (clientComboBox->Items->Count > 0) clientComboBox->SelectedIndex = 0;
            else {
                requestLabel->Text = L"Заявка: -";
                grid->Rows->Clear();
            }
        }

    private:
        void OnDataChanged(Object^ sender, EventArgs^ e) {
            int prevIdx = clientComboBox->SelectedIndex;
            RefreshClients();
            if (prevIdx >= 0 && prevIdx < clientComboBox->Items->Count) {
                clientComboBox->SelectedIndex = prevIdx;
            }
        }
        void OnClientSel(Object^ sender, EventArgs^ e) {
            if (clientComboBox->SelectedIndex >= 0 && clientComboBox->SelectedIndex < DataService::Clients->Count) {
                Client^ c = DataService::Clients[clientComboBox->SelectedIndex];
                requestLabel->Text = L"Заявка: " + c->ReqBrand + L" " + c->ReqModel + L" до " + c->Budget + L" у.е.";
            }
        }
        void OnMatch(Object^ sender, EventArgs^ e) {
            if (clientComboBox->SelectedIndex >= 0 && clientComboBox->SelectedIndex < DataService::Clients->Count) {
                Client^ c = DataService::Clients[clientComboBox->SelectedIndex];
                List<Car^>^ results = DataService::GetMatches(c);
                grid->Rows->Clear();
                for each (Car^ car in results) {
                    grid->Rows->Add(car->Brand, car->Model, car->Year, car->Price, car->MarketValue, car->Condition, car->Specs);
                }
            }
        }
    };
}
