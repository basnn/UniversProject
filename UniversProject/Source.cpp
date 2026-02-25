#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int  WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    CSVAnalyzer::MyForm form;
    Application::Run(% form);
    return 0;
}