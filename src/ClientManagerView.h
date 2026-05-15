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

    public ref class ClientManagerView : public UserControl {
    private:
        TableLayoutPanel^ mainLayout;
        DataGridView^ grid;
        FlowLayoutPanel^ inputLayout;

        TextBox^ nameTextBox;
        TextBox^ phoneTextBox;
        TextBox^ budgetTextBox;
        TextBox^ reqBrandTextBox;
        TextBox^ reqModelTextBox;
        TextBox^ reqConditionTextBox;

        RoundedButton^ addBtn;
        RoundedButton^ delBtn;

        int selectedIndex = -1;

        Panel^ WrapInput(String^ labelText, Control^ inputControl) {
            Panel^ p = gcnew Panel();
            p->Size = System::Drawing::Size(210, 60);
            p->Margin = System::Windows::Forms::Padding(5);
            Label^ lbl = gcnew Label();
            lbl->Text = labelText;
            lbl->AutoSize = true;
            lbl->Location = Point(0,0);
            inputControl->Location = Point(0, 22);
            inputControl->Width = 200;
            p->Controls->Add(lbl);
            p->Controls->Add(inputControl);
            return p;
        }

        void ClearInputs() {
            nameTextBox->Text = L""; phoneTextBox->Text = L""; budgetTextBox->Text = L"";
            reqBrandTextBox->Text = L""; reqModelTextBox->Text = L""; reqConditionTextBox->Text = L"";
            addBtn->Text = L"Добавить";
            selectedIndex = -1;
            grid->ClearSelection();
        }

    public:
        ClientManagerView() {
            this->Dock = DockStyle::Fill;
            this->BackColor = Color::Transparent;
            this->DoubleBuffered = true;
            
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
            grid->ColumnCount = 6;
            grid->Columns[0]->Name = L"ФИО"; grid->Columns[0]->AutoSizeMode = DataGridViewAutoSizeColumnMode::Fill;
            grid->Columns[1]->Name = L"Телефон"; grid->Columns[1]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            grid->Columns[2]->Name = L"Бюджет"; grid->Columns[2]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            grid->Columns[3]->Name = L"Желаемая марка"; grid->Columns[3]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            grid->Columns[4]->Name = L"Желаемая модель"; grid->Columns[4]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            grid->Columns[5]->Name = L"Предпочтительное состояние"; grid->Columns[5]->AutoSizeMode = DataGridViewAutoSizeColumnMode::AllCells;
            UIHelpers::StyleGrid(grid);
            grid->SelectionChanged += gcnew EventHandler(this, &ClientManagerView::OnGridSelect);
            gridCard->Controls->Add(grid);

            Panel^ inputCard = UIHelpers::CreateCard();
            inputCard->Dock = DockStyle::Fill;
            inputLayout = gcnew FlowLayoutPanel();
            inputLayout->Dock = DockStyle::Fill;
            inputLayout->AutoScroll = true;

            nameTextBox = gcnew TextBox(); phoneTextBox = gcnew TextBox(); budgetTextBox = gcnew TextBox();
            reqBrandTextBox = gcnew TextBox(); reqModelTextBox = gcnew TextBox(); reqConditionTextBox = gcnew TextBox();

            inputLayout->Controls->Add(WrapInput(L"ФИО:", nameTextBox));
            inputLayout->Controls->Add(WrapInput(L"Телефон:", phoneTextBox));
            inputLayout->Controls->Add(WrapInput(L"Бюджет:", budgetTextBox));
            inputLayout->Controls->Add(WrapInput(L"Желаемая марка:", reqBrandTextBox));
            inputLayout->Controls->Add(WrapInput(L"Желаемая модель:", reqModelTextBox));
            inputLayout->Controls->Add(WrapInput(L"Предпочтительное состояние:", reqConditionTextBox));

            FlowLayoutPanel^ btnLayout = gcnew FlowLayoutPanel();
            btnLayout->Width = 600; btnLayout->Height = 50;
            addBtn = gcnew RoundedButton(); addBtn->Text = L"Добавить"; addBtn->Size = System::Drawing::Size(150, 35); addBtn->Click += gcnew EventHandler(this, &ClientManagerView::OnAdd);
            delBtn = gcnew RoundedButton(); delBtn->Text = L"Удалить"; delBtn->Size = System::Drawing::Size(100, 35); delBtn->DefaultColor = Color::FromArgb(220, 53, 69); delBtn->Click += gcnew EventHandler(this, &ClientManagerView::OnDel);
            
            btnLayout->Controls->Add(addBtn); btnLayout->Controls->Add(delBtn);
            inputLayout->Controls->Add(btnLayout);
            inputCard->Controls->Add(inputLayout);

            mainLayout->Controls->Add(gridCard, 0, 0);
            mainLayout->Controls->Add(inputCard, 0, 1);
            this->Controls->Add(mainLayout);

            DataService::OnDataChanged += gcnew EventHandler(this, &ClientManagerView::OnDataChanged);
        }

        void RefreshGrid() {
            grid->Rows->Clear();
            for each (Client^ c in DataService::Clients) {
                grid->Rows->Add(c->FullName, c->Phone, c->Budget, c->ReqBrand, c->ReqModel, c->ReqCondition);
            }
            ClearInputs();
        }

    private:
        void OnDataChanged(Object^ sender, EventArgs^ e) { RefreshGrid(); }

        void OnGridSelect(Object^ sender, EventArgs^ e) {
            if (grid->SelectedRows->Count > 0) {
                selectedIndex = grid->SelectedRows[0]->Index;
                if (selectedIndex >= 0 && selectedIndex < DataService::Clients->Count) {
                    Client^ c = DataService::Clients[selectedIndex];
                    nameTextBox->Text = c->FullName;
                    phoneTextBox->Text = c->Phone;
                    budgetTextBox->Text = c->Budget.ToString();
                    reqBrandTextBox->Text = c->ReqBrand;
                    reqModelTextBox->Text = c->ReqModel;
                    reqConditionTextBox->Text = c->ReqCondition;
                    addBtn->Text = L"Изменить";
                }
            }
        }

        void OnAdd(Object^ sender, EventArgs^ e) {
            double budget = 0;
            if (!Double::TryParse(budgetTextBox->Text, budget)) return;
            
            Client^ newClient = gcnew Client(nameTextBox->Text, phoneTextBox->Text, budget, reqBrandTextBox->Text, reqModelTextBox->Text, reqConditionTextBox->Text);
            
            if (selectedIndex >= 0 && selectedIndex < DataService::Clients->Count && addBtn->Text == L"Изменить") {
                DataService::Clients[selectedIndex] = newClient;
            } else {
                DataService::Clients->Add(newClient);
            }
            DataService::SaveClients(L"clients.txt");
            DataService::NotifyDataChanged();
        }

        void OnDel(Object^ sender, EventArgs^ e) {
            if (grid->CurrentRow != nullptr && grid->CurrentRow->Index < DataService::Clients->Count) {
                DataService::Clients->RemoveAt(grid->CurrentRow->Index);
                DataService::SaveClients(L"clients.txt");
                DataService::NotifyDataChanged();
            }
        }
    };
}
