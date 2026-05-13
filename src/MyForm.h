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

    public ref class RoundedButton : public Button {
    public:
        Color DefaultColor = Color::FromArgb(21, 93, 252);
        Color HoverColor = Color::FromArgb(40, 120, 255);
        Color TextColor = Color::White;

        RoundedButton() {
            this->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->FlatAppearance->BorderSize = 0;
            this->BackColor = DefaultColor;
            this->ForeColor = TextColor;
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            this->Cursor = Cursors::Hand;
        }
    protected:
        virtual void OnMouseEnter(EventArgs^ e) override {
            this->BackColor = HoverColor;
            Button::OnMouseEnter(e);
        }
        virtual void OnMouseLeave(EventArgs^ e) override {
            this->BackColor = DefaultColor;
            Button::OnMouseLeave(e);
        }
        virtual void OnPaint(PaintEventArgs^ e) override {
            System::Drawing::Drawing2D::GraphicsPath^ path = gcnew System::Drawing::Drawing2D::GraphicsPath();
            int radius = 8;
            path->AddArc(0, 0, radius * 2, radius * 2, 180, 90);
            path->AddArc(this->Width - radius * 2, 0, radius * 2, radius * 2, 270, 90);
            path->AddArc(this->Width - radius * 2, this->Height - radius * 2, radius * 2, radius * 2, 0, 90);
            path->AddArc(0, this->Height - radius * 2, radius * 2, radius * 2, 90, 90);
            path->CloseFigure();
            this->Region = gcnew System::Drawing::Region(path);
            e->Graphics->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;
            SolidBrush^ brush = gcnew SolidBrush(this->BackColor);
            e->Graphics->FillPath(brush, path);
            
            if (this->Image != nullptr) {
                e->Graphics->DrawImage(this->Image, 10, (this->Height - 24) / 2, 24, 24);
                Rectangle textRect = Rectangle(44, 0, this->Width - 44, this->Height);
                TextRenderer::DrawText(e->Graphics, this->Text, this->Font, textRect, this->ForeColor, TextFormatFlags::Left | TextFormatFlags::VerticalCenter);
            } else {
                TextRenderer::DrawText(e->Graphics, this->Text, this->Font, this->ClientRectangle, this->ForeColor, TextFormatFlags::HorizontalCenter | TextFormatFlags::VerticalCenter);
            }
        }
    };

    public ref class MyForm : public Form {
    private:
        Panel^ sidebarPanel;
        Panel^ contentPanel;
        RoundedButton^ navCarsBtn;
        RoundedButton^ navClientsBtn;
        RoundedButton^ navMatchBtn;

        Panel^ pnlCars;
        Panel^ pnlClients;
        Panel^ pnlMatch;

        DataGridView^ carDataGridView;
        Label^ brandLabel; ComboBox^ brandComboBox;
        Label^ modelLabel; ComboBox^ modelComboBox;
        Label^ yearLabel; TextBox^ yearTextBox;
        Label^ priceLabel; TextBox^ priceTextBox;
        Label^ mvLabel; TextBox^ mvTextBox;
        Label^ conditionLabel; ComboBox^ conditionComboBox;
        Label^ specsLabel; TextBox^ specsTextBox;
        RoundedButton^ addCarButton; RoundedButton^ deleteCarButton;
        RoundedButton^ saveCarsButton; RoundedButton^ loadCarsButton;

        DataGridView^ clientDataGridView;
        Label^ nameLabel; TextBox^ nameTextBox;
        Label^ phoneLabel; TextBox^ phoneTextBox;
        Label^ budgetLabel; TextBox^ budgetTextBox;
        Label^ reqBrandLabel; ComboBox^ reqBrandComboBox;
        Label^ reqModelLabel; ComboBox^ reqModelComboBox;
        Label^ reqConditionLabel; ComboBox^ reqConditionComboBox;
        RoundedButton^ addClientButton; RoundedButton^ deleteClientButton;
        RoundedButton^ saveClientsButton; RoundedButton^ loadClientsButton;

        Label^ clientSelectLabel;
        ComboBox^ clientComboBox;
        Label^ clientRequestLabel;
        RoundedButton^ refreshClientsButton;
        RoundedButton^ matchButton;
        DataGridView^ matchingDataGridView;

    public:
        MyForm() {
            this->Text = L"Система управления автосалоном";
            this->Size = System::Drawing::Size(1000, 700);
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Regular);
            this->BackColor = Color::FromArgb(240, 240, 240);
            if (System::IO::File::Exists(L"bg.png")) {
                this->BackgroundImage = Image::FromFile(L"bg.png");
                this->BackgroundImageLayout = ImageLayout::Stretch;
            }

            sidebarPanel = gcnew Panel();
            sidebarPanel->Dock = DockStyle::Left;
            sidebarPanel->Width = 200;
            sidebarPanel->BackColor = Color::FromArgb(30, 30, 30);

            contentPanel = gcnew Panel();
            contentPanel->Dock = DockStyle::Fill;
            contentPanel->BackColor = Color::Transparent;

            navCarsBtn = CreateNavButton(L"Автомобили", L"car.png", 50);
            navCarsBtn->Click += gcnew EventHandler(this, &MyForm::OnNavCarsClick);
            
            navClientsBtn = CreateNavButton(L"Клиенты", L"user.png", 100);
            navClientsBtn->Click += gcnew EventHandler(this, &MyForm::OnNavClientsClick);
            
            navMatchBtn = CreateNavButton(L"Подбор", L"search.png", 150);
            navMatchBtn->Click += gcnew EventHandler(this, &MyForm::OnNavMatchClick);

            sidebarPanel->Controls->Add(navCarsBtn);
            sidebarPanel->Controls->Add(navClientsBtn);
            sidebarPanel->Controls->Add(navMatchBtn);

            this->Controls->Add(contentPanel);
            this->Controls->Add(sidebarPanel);

            InitializeCarsTab();
            InitializeClientsTab();
            InitializeMatchingTab();

            ShowPanel(pnlCars);
        }

    private:
        RoundedButton^ CreateNavButton(String^ text, String^ iconPath, int yPos) {
            RoundedButton^ btn = gcnew RoundedButton();
            btn->Text = text;
            btn->Location = Point(10, yPos);
            btn->Size = System::Drawing::Size(180, 45);
            btn->DefaultColor = Color::FromArgb(40, 40, 40);
            btn->HoverColor = Color::FromArgb(21, 93, 252);
            btn->BackColor = btn->DefaultColor;
            if (System::IO::File::Exists(iconPath)) {
                btn->Image = Image::FromFile(iconPath);
            }
            return btn;
        }

        void ShowPanel(Panel^ pnl) {
            pnlCars->Visible = false;
            pnlClients->Visible = false;
            pnlMatch->Visible = false;
            pnl->Visible = true;
            pnl->BringToFront();
        }

        void OnNavCarsClick(Object^ sender, EventArgs^ e) { ShowPanel(pnlCars); }
        void OnNavClientsClick(Object^ sender, EventArgs^ e) { ShowPanel(pnlClients); }
        void OnNavMatchClick(Object^ sender, EventArgs^ e) { ShowPanel(pnlMatch); }

        void StyleDataGridView(DataGridView^ dgv) {
            dgv->EnableHeadersVisualStyles = false;
            dgv->BorderStyle = BorderStyle::None;
            dgv->BackgroundColor = Color::White;
            dgv->CellBorderStyle = DataGridViewCellBorderStyle::SingleHorizontal;
            dgv->DefaultCellStyle->SelectionBackColor = Color::FromArgb(21, 93, 252);
            dgv->DefaultCellStyle->SelectionForeColor = Color::White;
            dgv->DefaultCellStyle->BackColor = Color::White;
            dgv->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(249, 249, 249);
            dgv->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(230, 230, 230);
            dgv->ColumnHeadersDefaultCellStyle->ForeColor = Color::Black;
            dgv->ColumnHeadersDefaultCellStyle->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            dgv->ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle::None;
            dgv->RowHeadersVisible = false;
            dgv->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        }

        Panel^ CreateCardPanel(int x, int y, int w, int h) {
            Panel^ p = gcnew Panel();
            p->Location = Point(x, y);
            p->Size = System::Drawing::Size(w, h);
            p->BackColor = Color::White;
            return p;
        }

        void InitializeCarsTab() {
            pnlCars = gcnew Panel();
            pnlCars->Dock = DockStyle::Fill;
            pnlCars->Visible = false;
            pnlCars->BackColor = Color::Transparent;

            Panel^ gridCard = CreateCardPanel(20, 20, 750, 400);
            carDataGridView = gcnew DataGridView();
            carDataGridView->Location = Point(10, 10);
            carDataGridView->Size = System::Drawing::Size(730, 380);
            carDataGridView->ColumnCount = 7;
            carDataGridView->Columns[0]->Name = L"Марка";
            carDataGridView->Columns[1]->Name = L"Модель";
            carDataGridView->Columns[2]->Name = L"Год";
            carDataGridView->Columns[3]->Name = L"Цена";
            carDataGridView->Columns[4]->Name = L"Рын. стоимость";
            carDataGridView->Columns[5]->Name = L"Состояние";
            carDataGridView->Columns[6]->Name = L"Характеристики";
            StyleDataGridView(carDataGridView);
            gridCard->Controls->Add(carDataGridView);

            Panel^ inputCard = CreateCardPanel(20, 440, 750, 200);
            brandLabel = gcnew Label(); brandLabel->Text = L"Марка:"; brandLabel->Location = Point(10, 15); brandLabel->AutoSize = true;
            brandComboBox = gcnew ComboBox(); brandComboBox->Location = Point(10, 40); brandComboBox->Width = 100;
            brandComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Toyota", L"BMW", L"Mercedes-Benz", L"Audi", L"Ford", L"Lada", L"Volkswagen", L"Kia", L"Hyundai", L"Renault", L"Nissan", L"Chevrolet" });
            brandComboBox->SelectedIndexChanged += gcnew EventHandler(this, &MyForm::OnBrandSelectedIndexChanged);

            modelLabel = gcnew Label(); modelLabel->Text = L"Модель:"; modelLabel->Location = Point(120, 15); modelLabel->AutoSize = true;
            modelComboBox = gcnew ComboBox(); modelComboBox->Location = Point(120, 40); modelComboBox->Width = 100;

            yearLabel = gcnew Label(); yearLabel->Text = L"Год:"; yearLabel->Location = Point(230, 15); yearLabel->AutoSize = true;
            yearTextBox = gcnew TextBox(); yearTextBox->Location = Point(230, 40); yearTextBox->Width = 60;

            priceLabel = gcnew Label(); priceLabel->Text = L"Цена:"; priceLabel->Location = Point(300, 15); priceLabel->AutoSize = true;
            priceTextBox = gcnew TextBox(); priceTextBox->Location = Point(300, 40); priceTextBox->Width = 70;

            mvLabel = gcnew Label(); mvLabel->Text = L"Рын. стоим.:"; mvLabel->Location = Point(380, 15); mvLabel->AutoSize = true;
            mvTextBox = gcnew TextBox(); mvTextBox->Location = Point(380, 40); mvTextBox->Width = 80;

            conditionLabel = gcnew Label(); conditionLabel->Text = L"Состояние:"; conditionLabel->Location = Point(470, 15); conditionLabel->AutoSize = true;
            conditionComboBox = gcnew ComboBox(); conditionComboBox->Location = Point(470, 40); conditionComboBox->Width = 100;
            conditionComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Новое", L"Отличное", L"Хорошее", L"Удовлетворительное", L"Требует ремонта" });

            specsLabel = gcnew Label(); specsLabel->Text = L"Характеристики:"; specsLabel->Location = Point(580, 15); specsLabel->AutoSize = true;
            specsTextBox = gcnew TextBox(); specsTextBox->Location = Point(580, 40); specsTextBox->Width = 150;

            addCarButton = gcnew RoundedButton(); addCarButton->Text = L"Добавить"; addCarButton->Location = Point(10, 80); addCarButton->Size = System::Drawing::Size(100, 35); addCarButton->Click += gcnew EventHandler(this, &MyForm::OnAddCarClick);
            deleteCarButton = gcnew RoundedButton(); deleteCarButton->Text = L"Удалить"; deleteCarButton->Location = Point(120, 80); deleteCarButton->Size = System::Drawing::Size(100, 35); deleteCarButton->DefaultColor = Color::FromArgb(220, 53, 69); deleteCarButton->HoverColor = Color::FromArgb(200, 35, 51); deleteCarButton->BackColor = deleteCarButton->DefaultColor; deleteCarButton->Click += gcnew EventHandler(this, &MyForm::OnDeleteCarClick);
            saveCarsButton = gcnew RoundedButton(); saveCarsButton->Text = L"Сохранить БД"; saveCarsButton->Location = Point(230, 80); saveCarsButton->Size = System::Drawing::Size(120, 35); saveCarsButton->DefaultColor = Color::FromArgb(40, 167, 69); saveCarsButton->HoverColor = Color::FromArgb(33, 136, 56); saveCarsButton->BackColor = saveCarsButton->DefaultColor; saveCarsButton->Click += gcnew EventHandler(this, &MyForm::OnSaveCarsClick);
            loadCarsButton = gcnew RoundedButton(); loadCarsButton->Text = L"Загрузить БД"; loadCarsButton->Location = Point(360, 80); loadCarsButton->Size = System::Drawing::Size(120, 35); loadCarsButton->DefaultColor = Color::FromArgb(23, 162, 184); loadCarsButton->HoverColor = Color::FromArgb(19, 132, 150); loadCarsButton->BackColor = loadCarsButton->DefaultColor; loadCarsButton->Click += gcnew EventHandler(this, &MyForm::OnLoadCarsClick);

            inputCard->Controls->Add(brandLabel); inputCard->Controls->Add(brandComboBox);
            inputCard->Controls->Add(modelLabel); inputCard->Controls->Add(modelComboBox);
            inputCard->Controls->Add(yearLabel); inputCard->Controls->Add(yearTextBox);
            inputCard->Controls->Add(priceLabel); inputCard->Controls->Add(priceTextBox);
            inputCard->Controls->Add(mvLabel); inputCard->Controls->Add(mvTextBox);
            inputCard->Controls->Add(conditionLabel); inputCard->Controls->Add(conditionComboBox);
            inputCard->Controls->Add(specsLabel); inputCard->Controls->Add(specsTextBox);
            inputCard->Controls->Add(addCarButton); inputCard->Controls->Add(deleteCarButton);
            inputCard->Controls->Add(saveCarsButton); inputCard->Controls->Add(loadCarsButton);

            pnlCars->Controls->Add(gridCard);
            pnlCars->Controls->Add(inputCard);
            contentPanel->Controls->Add(pnlCars);
        }

        void InitializeClientsTab() {
            pnlClients = gcnew Panel();
            pnlClients->Dock = DockStyle::Fill;
            pnlClients->Visible = false;
            pnlClients->BackColor = Color::Transparent;

            Panel^ gridCard = CreateCardPanel(20, 20, 750, 400);
            clientDataGridView = gcnew DataGridView();
            clientDataGridView->Location = Point(10, 10);
            clientDataGridView->Size = System::Drawing::Size(730, 380);
            clientDataGridView->ColumnCount = 6;
            clientDataGridView->Columns[0]->Name = L"ФИО";
            clientDataGridView->Columns[1]->Name = L"Телефон";
            clientDataGridView->Columns[2]->Name = L"Бюджет";
            clientDataGridView->Columns[3]->Name = L"Желаемая Марка";
            clientDataGridView->Columns[4]->Name = L"Желаемая Модель";
            clientDataGridView->Columns[5]->Name = L"Предпочт. состояние";
            StyleDataGridView(clientDataGridView);
            gridCard->Controls->Add(clientDataGridView);

            Panel^ inputCard = CreateCardPanel(20, 440, 750, 200);
            nameLabel = gcnew Label(); nameLabel->Text = L"ФИО:"; nameLabel->Location = Point(10, 15); nameLabel->AutoSize = true;
            nameTextBox = gcnew TextBox(); nameTextBox->Location = Point(10, 40); nameTextBox->Width = 120;

            phoneLabel = gcnew Label(); phoneLabel->Text = L"Телефон:"; phoneLabel->Location = Point(140, 15); phoneLabel->AutoSize = true;
            phoneTextBox = gcnew TextBox(); phoneTextBox->Location = Point(140, 40); phoneTextBox->Width = 100;

            budgetLabel = gcnew Label(); budgetLabel->Text = L"Бюджет:"; budgetLabel->Location = Point(250, 15); budgetLabel->AutoSize = true;
            budgetTextBox = gcnew TextBox(); budgetTextBox->Location = Point(250, 40); budgetTextBox->Width = 80;

            reqBrandLabel = gcnew Label(); reqBrandLabel->Text = L"Жел. Марка:"; reqBrandLabel->Location = Point(340, 15); reqBrandLabel->AutoSize = true;
            reqBrandComboBox = gcnew ComboBox(); reqBrandComboBox->Location = Point(340, 40); reqBrandComboBox->Width = 100;
            reqBrandComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Любая", L"Toyota", L"BMW", L"Mercedes-Benz", L"Audi", L"Ford", L"Lada", L"Volkswagen", L"Kia", L"Hyundai", L"Renault", L"Nissan", L"Chevrolet" });
            reqBrandComboBox->SelectedIndexChanged += gcnew EventHandler(this, &MyForm::OnReqBrandSelectedIndexChanged);

            reqModelLabel = gcnew Label(); reqModelLabel->Text = L"Жел. Модель:"; reqModelLabel->Location = Point(450, 15); reqModelLabel->AutoSize = true;
            reqModelComboBox = gcnew ComboBox(); reqModelComboBox->Location = Point(450, 40); reqModelComboBox->Width = 100;

            reqConditionLabel = gcnew Label(); reqConditionLabel->Text = L"Предпочт. состояние:"; reqConditionLabel->Location = Point(560, 15); reqConditionLabel->AutoSize = true;
            reqConditionComboBox = gcnew ComboBox(); reqConditionComboBox->Location = Point(560, 40); reqConditionComboBox->Width = 100;
            reqConditionComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Любое", L"Новое", L"Отличное", L"Хорошее", L"Удовлетворительное", L"Требует ремонта" });

            addClientButton = gcnew RoundedButton(); addClientButton->Text = L"Добавить"; addClientButton->Location = Point(10, 80); addClientButton->Size = System::Drawing::Size(100, 35); addClientButton->Click += gcnew EventHandler(this, &MyForm::OnAddClientClick);
            deleteClientButton = gcnew RoundedButton(); deleteClientButton->Text = L"Удалить"; deleteClientButton->Location = Point(120, 80); deleteClientButton->Size = System::Drawing::Size(100, 35); deleteClientButton->DefaultColor = Color::FromArgb(220, 53, 69); deleteClientButton->HoverColor = Color::FromArgb(200, 35, 51); deleteClientButton->BackColor = deleteClientButton->DefaultColor; deleteClientButton->Click += gcnew EventHandler(this, &MyForm::OnDeleteClientClick);
            saveClientsButton = gcnew RoundedButton(); saveClientsButton->Text = L"Сохранить БД"; saveClientsButton->Location = Point(230, 80); saveClientsButton->Size = System::Drawing::Size(120, 35); saveClientsButton->DefaultColor = Color::FromArgb(40, 167, 69); saveClientsButton->HoverColor = Color::FromArgb(33, 136, 56); saveClientsButton->BackColor = saveClientsButton->DefaultColor; saveClientsButton->Click += gcnew EventHandler(this, &MyForm::OnSaveClientsClick);
            loadClientsButton = gcnew RoundedButton(); loadClientsButton->Text = L"Загрузить БД"; loadClientsButton->Location = Point(360, 80); loadClientsButton->Size = System::Drawing::Size(120, 35); loadClientsButton->DefaultColor = Color::FromArgb(23, 162, 184); loadClientsButton->HoverColor = Color::FromArgb(19, 132, 150); loadClientsButton->BackColor = loadClientsButton->DefaultColor; loadClientsButton->Click += gcnew EventHandler(this, &MyForm::OnLoadClientsClick);

            inputCard->Controls->Add(nameLabel); inputCard->Controls->Add(nameTextBox);
            inputCard->Controls->Add(phoneLabel); inputCard->Controls->Add(phoneTextBox);
            inputCard->Controls->Add(budgetLabel); inputCard->Controls->Add(budgetTextBox);
            inputCard->Controls->Add(reqBrandLabel); inputCard->Controls->Add(reqBrandComboBox);
            inputCard->Controls->Add(reqModelLabel); inputCard->Controls->Add(reqModelComboBox);
            inputCard->Controls->Add(reqConditionLabel); inputCard->Controls->Add(reqConditionComboBox);
            inputCard->Controls->Add(addClientButton); inputCard->Controls->Add(deleteClientButton);
            inputCard->Controls->Add(saveClientsButton); inputCard->Controls->Add(loadClientsButton);

            pnlClients->Controls->Add(gridCard);
            pnlClients->Controls->Add(inputCard);
            contentPanel->Controls->Add(pnlClients);
        }

        void InitializeMatchingTab() {
            pnlMatch = gcnew Panel();
            pnlMatch->Dock = DockStyle::Fill;
            pnlMatch->Visible = false;
            pnlMatch->BackColor = Color::Transparent;

            Panel^ topCard = CreateCardPanel(20, 20, 750, 100);
            
            clientSelectLabel = gcnew Label(); clientSelectLabel->Text = L"Выберите клиента:"; clientSelectLabel->Location = Point(10, 20); clientSelectLabel->AutoSize = true;
            clientComboBox = gcnew ComboBox(); clientComboBox->Location = Point(160, 17); clientComboBox->Width = 200;
            clientComboBox->DropDownStyle = ComboBoxStyle::DropDownList;
            clientComboBox->SelectedIndexChanged += gcnew EventHandler(this, &MyForm::OnClientSelectionChanged);

            refreshClientsButton = gcnew RoundedButton(); refreshClientsButton->Text = L"Обновить"; refreshClientsButton->Location = Point(380, 12); refreshClientsButton->Size = System::Drawing::Size(100, 32); refreshClientsButton->DefaultColor = Color::FromArgb(108, 117, 125); refreshClientsButton->HoverColor = Color::FromArgb(90, 98, 104); refreshClientsButton->BackColor = refreshClientsButton->DefaultColor; refreshClientsButton->Click += gcnew EventHandler(this, &MyForm::OnRefreshClientsClick);

            clientRequestLabel = gcnew Label(); clientRequestLabel->Location = Point(10, 60); clientRequestLabel->AutoSize = true;
            clientRequestLabel->Text = L"Заявка: -";
            clientRequestLabel->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);

            matchButton = gcnew RoundedButton(); matchButton->Text = L"Найти подходящие авто"; matchButton->Location = Point(550, 50); matchButton->Size = System::Drawing::Size(180, 35); matchButton->Click += gcnew EventHandler(this, &MyForm::OnMatchClick);

            topCard->Controls->Add(clientSelectLabel);
            topCard->Controls->Add(clientComboBox);
            topCard->Controls->Add(refreshClientsButton);
            topCard->Controls->Add(clientRequestLabel);
            topCard->Controls->Add(matchButton);

            Panel^ gridCard = CreateCardPanel(20, 140, 750, 480);
            matchingDataGridView = gcnew DataGridView();
            matchingDataGridView->Location = Point(10, 10);
            matchingDataGridView->Size = System::Drawing::Size(730, 460);
            matchingDataGridView->ColumnCount = 7;
            matchingDataGridView->Columns[0]->Name = L"Марка";
            matchingDataGridView->Columns[1]->Name = L"Модель";
            matchingDataGridView->Columns[2]->Name = L"Год";
            matchingDataGridView->Columns[3]->Name = L"Цена";
            matchingDataGridView->Columns[4]->Name = L"Рын. стоимость";
            matchingDataGridView->Columns[5]->Name = L"Состояние";
            matchingDataGridView->Columns[6]->Name = L"Характеристики";
            StyleDataGridView(matchingDataGridView);
            gridCard->Controls->Add(matchingDataGridView);

            pnlMatch->Controls->Add(topCard);
            pnlMatch->Controls->Add(gridCard);
            contentPanel->Controls->Add(pnlMatch);
        }

        void OnBrandSelectedIndexChanged(Object^ sender, EventArgs^ e) {
            modelComboBox->Items->Clear();
            String^ brand = brandComboBox->Text;
            if (brand == L"Toyota") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Camry", L"Corolla", L"RAV4" });
            else if (brand == L"BMW") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"X5", L"3 Series" });
            else if (brand == L"Mercedes-Benz") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"E-Class", L"C-Class" });
            else if (brand == L"Audi") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"A4" });
            else if (brand == L"Ford") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Focus" });
            else if (brand == L"Lada") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Vesta", L"Granta" });
            else if (brand == L"Volkswagen") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Polo" });
            else if (brand == L"Kia") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Rio" });
            else if (brand == L"Hyundai") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Solaris" });
            else if (brand == L"Renault") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Logan", L"Duster" });
            else if (brand == L"Nissan") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Qashqai", L"X-Trail", L"Almera" });
            else if (brand == L"Chevrolet") modelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Cruze", L"Tahoe", L"Aveo" });

            if (modelComboBox->Items->Count > 0) {
                modelComboBox->SelectedIndex = 0;
            }
        }
        void OnReqBrandSelectedIndexChanged(Object^ sender, EventArgs^ e) {
            reqModelComboBox->Items->Clear();
            String^ brand = reqBrandComboBox->Text;
            if (brand == L"Любая") {
                reqModelComboBox->Items->Add(L"Любая");
            }
            else {
                reqModelComboBox->Items->Add(L"Любая");
                if (brand == L"Toyota") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Camry", L"Corolla", L"RAV4" });
                else if (brand == L"BMW") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"X5", L"3 Series" });
                else if (brand == L"Mercedes-Benz") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"E-Class", L"C-Class" });
                else if (brand == L"Audi") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"A4" });
                else if (brand == L"Ford") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Focus" });
                else if (brand == L"Lada") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Vesta", L"Granta" });
                else if (brand == L"Volkswagen") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Polo" });
                else if (brand == L"Kia") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Rio" });
                else if (brand == L"Hyundai") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Solaris" });
                else if (brand == L"Renault") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Logan", L"Duster" });
                else if (brand == L"Nissan") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Qashqai", L"X-Trail", L"Almera" });
                else if (brand == L"Chevrolet") reqModelComboBox->Items->AddRange(gcnew cli::array<Object^> { L"Cruze", L"Tahoe", L"Aveo" });
            }
            if (reqModelComboBox->Items->Count > 0) {
                reqModelComboBox->SelectedIndex = 0;
            }
        }
        void OnAddCarClick(Object^ sender, EventArgs^ e) {
            String^ brand = brandComboBox->Text->Trim();
            String^ model = modelComboBox->Text->Trim();
            String^ yearStr = yearTextBox->Text->Trim();
            String^ priceStr = priceTextBox->Text->Trim();
            String^ mvStr = mvTextBox->Text->Trim();
            String^ condition = conditionComboBox->Text->Trim();
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

            brandComboBox->Text = L"";
            modelComboBox->Text = L"";
            yearTextBox->Clear();
            priceTextBox->Clear();
            mvTextBox->Clear();
            conditionComboBox->Text = L"";
            specsTextBox->Clear();
        }
        void OnDeleteCarClick(Object^ sender, EventArgs^ e) {
            if (carDataGridView->SelectedRows->Count > 0) {
                for (int i = carDataGridView->SelectedRows->Count - 1; i >= 0; i--) {
                    if (!carDataGridView->SelectedRows[i]->IsNewRow) {
                        carDataGridView->Rows->Remove(carDataGridView->SelectedRows[i]);
                    }
                }
            }
            else if (carDataGridView->CurrentRow != nullptr && !carDataGridView->CurrentRow->IsNewRow) {
                carDataGridView->Rows->Remove(carDataGridView->CurrentRow);
            }
            else {
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
                        }
                        else {
                            MessageBox::Show(L"Ошибка формата чисел в строке: " + line, L"Ошибка чтения", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                        }
                    }
                    else {
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
        void OnAddClientClick(Object^ sender, EventArgs^ e) {
            String^ name = nameTextBox->Text->Trim();
            String^ phone = phoneTextBox->Text->Trim();
            String^ budgetStr = budgetTextBox->Text->Trim();
            String^ rBrand = reqBrandComboBox->Text->Trim();
            String^ rModel = reqModelComboBox->Text->Trim();
            String^ rCond = reqConditionComboBox->Text->Trim();

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
            reqBrandComboBox->Text = L"";
            reqModelComboBox->Text = L"";
            reqConditionComboBox->Text = L"";
        }
        void OnDeleteClientClick(Object^ sender, EventArgs^ e) {
            if (clientDataGridView->SelectedRows->Count > 0) {
                for (int i = clientDataGridView->SelectedRows->Count - 1; i >= 0; i--) {
                    if (!clientDataGridView->SelectedRows[i]->IsNewRow) {
                        clientDataGridView->Rows->Remove(clientDataGridView->SelectedRows[i]);
                    }
                }
            }
            else if (clientDataGridView->CurrentRow != nullptr && !clientDataGridView->CurrentRow->IsNewRow) {
                clientDataGridView->Rows->Remove(clientDataGridView->CurrentRow);
            }
            else {
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
            }
            else {
                MessageBox::Show(L"Список клиентов пуст. Перейдите на вкладку 'Клиенты' и добавьте данные.", L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }
        void OnClientSelectionChanged(Object^ sender, EventArgs^ e) {
            if (clientComboBox->SelectedIndex == -1) {
                clientRequestLabel->Text = L"Заявка: -";
                return;
            }
            String^ selectedClientName = clientComboBox->SelectedItem->ToString();
            for (int i = 0; i < clientDataGridView->Rows->Count; i++) {
                if (!clientDataGridView->Rows[i]->IsNewRow) {
                    if (Convert::ToString(clientDataGridView->Rows[i]->Cells[0]->Value) == selectedClientName) {
                        String^ cBudget = Convert::ToString(clientDataGridView->Rows[i]->Cells[2]->Value);
                        String^ cBrand = Convert::ToString(clientDataGridView->Rows[i]->Cells[3]->Value);
                        String^ cModel = Convert::ToString(clientDataGridView->Rows[i]->Cells[4]->Value);
                        clientRequestLabel->Text = L"Заявка: " + cBrand + L" " + cModel + L" до " + cBudget + L" у.е.";
                        break;
                    }
                }
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

                    bool matchBrand = String::IsNullOrEmpty(cBrand) || cBrand == L"любая" || cBrand == L"любое" || carBrand->Contains(cBrand);
                    bool matchModel = String::IsNullOrEmpty(cModel) || cModel == L"любая" || cModel == L"любое" || carModel->Contains(cModel);
                    bool matchCond = String::IsNullOrEmpty(cCond) || cCond == L"любая" || cCond == L"любое" || carCond->Contains(cCond);
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
    };
}
