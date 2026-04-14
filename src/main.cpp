#using <System.dll>
#using <System.Windows.Forms.dll>
#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    AutosalonApp::MyForm^ form = gcnew AutosalonApp::MyForm();
    Application::Run(form);
    return 0;
}
