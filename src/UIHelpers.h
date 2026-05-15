#pragma once
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

namespace AutosalonApp {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Drawing::Drawing2D;

    public ref class UIHelpers {
    public:
        static void StyleGrid(DataGridView^ dgv) {
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
            
            dgv->DefaultCellStyle->WrapMode = DataGridViewTriState::True;
            dgv->AutoSizeRowsMode = DataGridViewAutoSizeRowsMode::AllCells;
            dgv->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgv->MultiSelect = false;
            dgv->ReadOnly = true;
        }
        
        static Panel^ CreateCard() {
            Panel^ p = gcnew Panel();
            p->BackColor = Color::White;
            p->Padding = System::Windows::Forms::Padding(15);
            p->Margin = System::Windows::Forms::Padding(10);
            return p;
        }
    };

    public ref class GlassCard : public Panel {
    private:
        SolidBrush^ overlayBrush;
        Pen^ borderPen;
        GraphicsPath^ path;
    public:
        GlassCard() {
            this->BackColor = Color::Transparent;
            this->Padding = System::Windows::Forms::Padding(20);
            overlayBrush = gcnew SolidBrush(Color::FromArgb(160, 255, 255, 255));
            borderPen = gcnew Pen(Color::FromArgb(150, 255, 255, 255), 1.0f);
        }
    protected:
        virtual void OnResize(EventArgs^ e) override {
            Panel::OnResize(e);
            if (this->Width > 0 && this->Height > 0) {
                path = gcnew GraphicsPath();
                int radius = 15;
                path->AddArc(0, 0, radius * 2, radius * 2, 180, 90);
                path->AddArc(this->Width - radius * 2, 0, radius * 2, radius * 2, 270, 90);
                path->AddArc(this->Width - radius * 2, this->Height - radius * 2, radius * 2, radius * 2, 0, 90);
                path->AddArc(0, this->Height - radius * 2, radius * 2, radius * 2, 90, 90);
                path->CloseFigure();
            }
        }
        
        virtual void OnPaint(PaintEventArgs^ e) override {
            if (path == nullptr) return;
            e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
            e->Graphics->FillPath(overlayBrush, path);
            e->Graphics->DrawPath(borderPen, path);
        }
    };
}
