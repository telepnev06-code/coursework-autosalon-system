#pragma once
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>
#include "DataService.h"
#include "UIHelpers.h"

namespace AutosalonApp {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class DashboardView : public UserControl {
    private:
        TableLayoutPanel^ mainLayout;
        
        Label^ carCountLabel;
        Label^ clientCountLabel;
        Label^ fleetValueLabel;

        GlassCard^ CreateDashboardCard(String^ title, String^ subtext, Label^% valueLabel) {
            GlassCard^ p = gcnew GlassCard();
            p->Dock = DockStyle::Fill;
            p->Margin = System::Windows::Forms::Padding(20);

            Label^ lblTitle = gcnew Label();
            lblTitle->Text = title;
            lblTitle->Font = gcnew System::Drawing::Font(L"Segoe UI", 14, FontStyle::Bold);
            lblTitle->ForeColor = Color::Black;
            lblTitle->AutoSize = true;
            lblTitle->Location = Point(20, 20);
            lblTitle->BackColor = Color::Transparent;

            valueLabel = gcnew Label();
            valueLabel->Text = L"0";
            valueLabel->Font = gcnew System::Drawing::Font(L"Segoe UI", 28, FontStyle::Bold);
            valueLabel->ForeColor = Color::Black;
            valueLabel->AutoSize = true;
            valueLabel->Location = Point(20, 60);
            valueLabel->BackColor = Color::Transparent;

            Label^ lblSub = gcnew Label();
            lblSub->Text = subtext;
            lblSub->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Regular);
            lblSub->ForeColor = Color::FromArgb(60, 60, 60);
            lblSub->AutoSize = true;
            lblSub->Location = Point(20, 110);
            lblSub->BackColor = Color::Transparent;

            p->Controls->Add(lblTitle);
            p->Controls->Add(valueLabel);
            p->Controls->Add(lblSub);
            
            return p;
        }

    public:
        DashboardView() {
            this->Dock = DockStyle::Fill;
            this->BackColor = Color::Transparent;
            
            mainLayout = gcnew TableLayoutPanel();
            mainLayout->Dock = DockStyle::Fill;
            mainLayout->BackColor = Color::Transparent;
            mainLayout->RowCount = 1;
            mainLayout->ColumnCount = 3;
            mainLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 33.33f));
            mainLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 33.33f));
            mainLayout->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 33.33f));
            mainLayout->Padding = System::Windows::Forms::Padding(20, 80, 20, 80);

            mainLayout->Controls->Add(CreateDashboardCard(L"Автомобили", L"В наличии на площадке", carCountLabel), 0, 0);
            mainLayout->Controls->Add(CreateDashboardCard(L"Клиенты", L"Зарегистрировано в базе", clientCountLabel), 1, 0);
            mainLayout->Controls->Add(CreateDashboardCard(L"Автопарк", L"Общая стоимость машин", fleetValueLabel), 2, 0);

            this->Controls->Add(mainLayout);

            DataService::OnDataChanged += gcnew EventHandler(this, &DashboardView::OnDataChanged);
            UpdateStats();
        }

        void UpdateStats() {
            carCountLabel->Text = DataService::Cars->Count.ToString();
            clientCountLabel->Text = DataService::Clients->Count.ToString();
            
            double totalValue = 0;
            for each (Car^ c in DataService::Cars) {
                totalValue += c->Price;
            }
            fleetValueLabel->Text = totalValue.ToString(L"N0") + L" $";
        }

    private:
        void OnDataChanged(Object^ sender, EventArgs^ e) {
            UpdateStats();
        }
    };
}
