#pragma once
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#include "DataService.h"
#include "RoundedButton.h"
#include "DashboardView.h"
#include "CarManagerView.h"
#include "ClientManagerView.h"
#include "MatchingView.h"

namespace AutosalonApp {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class MyForm : public Form {
    private:
        Panel^ sidebarPanel;
        Panel^ contentPanel;
        Panel^ headerPanel;
        Label^ headerTitleLabel;

        RoundedButton^ navDashBtn;
        RoundedButton^ navCarsBtn;
        RoundedButton^ navClientsBtn;
        RoundedButton^ navMatchBtn;

        DashboardView^ dashboardView;
        CarManagerView^ carView;
        ClientManagerView^ clientView;
        MatchingView^ matchView;

        Image^ cachedBg;

    public:
        MyForm() {
            this->Text = L"Система управления автосалоном";
            this->Size = System::Drawing::Size(1100, 750);
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Regular);
            this->BackColor = Color::FromArgb(240, 240, 240);
            this->DoubleBuffered = true;
            this->Load += gcnew EventHandler(this, &MyForm::OnFormLoad);

            if (System::IO::File::Exists(L"app_icon.png")) {
                try {
                    Bitmap^ bmp = gcnew Bitmap(L"app_icon.png");
                    this->Icon = System::Drawing::Icon::FromHandle(bmp->GetHicon());
                } catch(Exception^) {}
            }

            if (System::IO::File::Exists(L"bg_showroom.png")) {
                try {
                    cachedBg = Image::FromFile(L"bg_showroom.png");
                } catch(Exception^) {}
            }

            sidebarPanel = gcnew Panel();
            sidebarPanel->Dock = DockStyle::Left;
            sidebarPanel->Width = 200;
            sidebarPanel->BackColor = Color::FromArgb(30, 30, 30);

            headerPanel = gcnew Panel();
            headerPanel->Dock = DockStyle::Top;
            headerPanel->Height = 50;
            headerPanel->BackColor = Color::White;
            
            headerTitleLabel = gcnew Label();
            headerTitleLabel->Font = gcnew System::Drawing::Font(L"Segoe UI", 16, FontStyle::Bold);
            headerTitleLabel->ForeColor = Color::FromArgb(40, 40, 40);
            headerTitleLabel->Location = Point(20, 10);
            headerTitleLabel->AutoSize = true;
            headerPanel->Controls->Add(headerTitleLabel);

            contentPanel = gcnew Panel();
            contentPanel->Dock = DockStyle::Fill;
            contentPanel->BackColor = Color::Transparent;
            contentPanel->Paint += gcnew PaintEventHandler(this, &MyForm::OnContentPaint);

            navDashBtn = CreateNavButton(L"Главная", L"icon_dash.png", 50);
            navCarsBtn = CreateNavButton(L"Автомобили", L"icon_car.png", 100);
            navClientsBtn = CreateNavButton(L"Клиенты", L"icon_user.png", 150);
            navMatchBtn = CreateNavButton(L"Подбор", L"icon_search.png", 200);

            navDashBtn->Click += gcnew EventHandler(this, &MyForm::OnNav);
            navCarsBtn->Click += gcnew EventHandler(this, &MyForm::OnNav);
            navClientsBtn->Click += gcnew EventHandler(this, &MyForm::OnNav);
            navMatchBtn->Click += gcnew EventHandler(this, &MyForm::OnNav);

            sidebarPanel->Controls->Add(navDashBtn);
            sidebarPanel->Controls->Add(navCarsBtn);
            sidebarPanel->Controls->Add(navClientsBtn);
            sidebarPanel->Controls->Add(navMatchBtn);

            this->Controls->Add(contentPanel);
            this->Controls->Add(headerPanel);
            this->Controls->Add(sidebarPanel);

            dashboardView = gcnew DashboardView();
            carView = gcnew CarManagerView();
            clientView = gcnew ClientManagerView();
            matchView = gcnew MatchingView();

            contentPanel->Controls->Add(dashboardView);
            contentPanel->Controls->Add(carView);
            contentPanel->Controls->Add(clientView);
            contentPanel->Controls->Add(matchView);

            ShowView(dashboardView, L"");
        }

    private:
        void OnContentPaint(Object^ sender, PaintEventArgs^ e) {
            if (cachedBg != nullptr) {
                e->Graphics->DrawImage(cachedBg, contentPanel->ClientRectangle);
                SolidBrush^ overlay = gcnew SolidBrush(Color::FromArgb(130, 255, 255, 255));
                e->Graphics->FillRectangle(overlay, contentPanel->ClientRectangle);
            } else {
                e->Graphics->Clear(Color::FromArgb(249, 250, 251));
            }
        }

        void OnFormLoad(Object^ sender, EventArgs^ e) {
            DataService::LoadCars(L"cars.txt");
            DataService::LoadClients(L"clients.txt");
        }

        RoundedButton^ CreateNavButton(String^ text, String^ iconPath, int yPos) {
            RoundedButton^ btn = gcnew RoundedButton();
            btn->Text = text;
            btn->Location = Point(10, yPos);
            btn->Size = System::Drawing::Size(180, 45);
            btn->DefaultColor = Color::FromArgb(40, 40, 40);
            btn->HoverColor = Color::FromArgb(21, 93, 252);
            btn->BackColor = btn->DefaultColor;
            
            if (System::IO::File::Exists(iconPath)) {
                try {
                    btn->Image = Image::FromFile(iconPath);
                } catch(Exception^) {}
            }
            return btn;
        }

        void ShowView(UserControl^ view, String^ title) {
            dashboardView->Visible = false;
            carView->Visible = false;
            clientView->Visible = false;
            matchView->Visible = false;
            view->Visible = true;
            view->BringToFront();
            
            if (view == dashboardView) {
                headerPanel->Visible = false;
            } else {
                headerPanel->Visible = true;
                headerTitleLabel->Text = title;
            }

            if (view == matchView) matchView->RefreshClients();
        }

        void OnNav(Object^ sender, EventArgs^ e) {
            if (sender == navDashBtn) ShowView(dashboardView, L"");
            if (sender == navCarsBtn) ShowView(carView, L"Управление автомобилями");
            if (sender == navClientsBtn) ShowView(clientView, L"Управление клиентами");
            if (sender == navMatchBtn) ShowView(matchView, L"Подбор автомобилей");
        }
    };
}
