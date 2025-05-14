#include "Form1.h"
#include "pch.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    LibraryApp::Form1^ form = gcnew LibraryApp::Form1();

    Application::Run(form);

    return 0;
}



