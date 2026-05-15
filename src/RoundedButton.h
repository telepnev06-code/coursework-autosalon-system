#pragma once
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

namespace AutosalonApp {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Drawing::Drawing2D;

    public ref class RoundedButton : public Button {
    private:
        bool isHovered = false;
        System::Windows::Forms::Timer^ animTimer;
        int animStep = 0;
        Color _defaultColor = Color::FromArgb(21, 93, 252);
    public:
        property Color DefaultColor {
            Color get() { return _defaultColor; }
            void set(Color value) { 
                _defaultColor = value; 
                if (!isHovered) {
                    CurrentColor = value;
                    this->BackColor = value;
                }
            }
        }
        
        Color HoverColor = Color::FromArgb(60, 140, 255);
        Color TextColor = Color::White;
        Color CurrentColor;

        RoundedButton() {
            this->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->FlatAppearance->BorderSize = 0;
            this->BackColor = _defaultColor;
            CurrentColor = _defaultColor;
            this->ForeColor = TextColor;
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            this->Cursor = Cursors::Hand;
            
            animTimer = gcnew System::Windows::Forms::Timer();
            animTimer->Interval = 15;
            animTimer->Tick += gcnew EventHandler(this, &RoundedButton::OnAnimTick);
        }
    protected:
        virtual void OnMouseEnter(EventArgs^ e) override {
            isHovered = true;
            animTimer->Start();
            Button::OnMouseEnter(e);
        }
        virtual void OnMouseLeave(EventArgs^ e) override {
            isHovered = false;
            animTimer->Start();
            Button::OnMouseLeave(e);
        }
        void OnAnimTick(Object^ sender, EventArgs^ e) {
            int step = 15;
            int r = CurrentColor.R, g = CurrentColor.G, b = CurrentColor.B;
            Color target = isHovered ? HoverColor : _defaultColor;
            bool done = true;
            
            if (r != target.R) { r += (target.R > r) ? Math::Min(step, target.R - r) : -Math::Min(step, r - target.R); done = false; }
            if (g != target.G) { g += (target.G > g) ? Math::Min(step, target.G - g) : -Math::Min(step, g - target.G); done = false; }
            if (b != target.B) { b += (target.B > b) ? Math::Min(step, target.B - b) : -Math::Min(step, b - target.B); done = false; }
            
            CurrentColor = Color::FromArgb(r, g, b);
            this->BackColor = CurrentColor;
            this->Invalidate();
            if (done) animTimer->Stop();
        }
        virtual void OnPaint(PaintEventArgs^ e) override {
            GraphicsPath^ path = gcnew GraphicsPath();
            int radius = 8;
            path->AddArc(0, 0, radius * 2, radius * 2, 180, 90);
            path->AddArc(this->Width - radius * 2, 0, radius * 2, radius * 2, 270, 90);
            path->AddArc(this->Width - radius * 2, this->Height - radius * 2, radius * 2, radius * 2, 0, 90);
            path->AddArc(0, this->Height - radius * 2, radius * 2, radius * 2, 90, 90);
            path->CloseFigure();
            this->Region = gcnew System::Drawing::Region(path);
            e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
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
}
