#include "TextLoaderConfigForm.h"
#include "Loader.h"
#include "Date.h"
#include "Time.h"
#include"TestForm.h"
#include <MFP/MFWR.h>
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;
using namespace DataEngine;


[STAThread]
int  WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Инициализация парсеров даты и времени
    DataEngine::Date::InitializeDefaultParser();
    DataEngine::Time::InitializeDefaultParser();

    DataEngine::TestForm^ form = gcnew DataEngine::TestForm();
    Application::Run(form);

    return 0;
}




