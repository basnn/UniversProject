#include "TestForm.h"
#include "FilterHub.h"
#include "FilteredTable.h"
#include "FilterInfoForm.h"
#include "FilterDefaultOperation.h"
#include "Loader.h"
#include "TableLoader.h"
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

    // Создаем структуру таблицы (HeaderDefinition)
    std::vector<DataEngine::ColumnDefinition> columns;
    columns.push_back(DataEngine::ColumnDefinition("Name", DataEngine::DataType::Text, DataEngine::Data(std::string("Default Name"))));
    columns.push_back(DataEngine::ColumnDefinition("Age", DataEngine::DataType::Integer, DataEngine::Data((long long)0)));

    DataEngine::HeaderDefinition header;
    header.SetColumns(columns);

    // Создаем таблицу с данными
    DataEngine::TableLoader* sourceTable = new DataEngine::TableLoader(header);
    sourceTable->SetName("Persons");

    // Добавляем тестовые элементы
    DataEngine::DataObject obj1;
    obj1.Resize(2);
    obj1.Set(0, DataEngine::Data(std::string("John Doe")));
    obj1.Set(1, DataEngine::Data((long long)30));
    sourceTable->Add(obj1);

    DataEngine::DataObject obj2;
    obj2.Resize(2);
    obj2.Set(0, DataEngine::Data(std::string("Jane Smith")));
    obj2.Set(1, DataEngine::Data((long long)25));
    sourceTable->Add(obj2);

    DataEngine::DataObject obj3;
    obj3.Resize(2);
    obj3.Set(0, DataEngine::Data(std::string("Bob Johnson")));
    obj3.Set(1, DataEngine::Data((long long)40));
    sourceTable->Add(obj3);

    System::Console::WriteLine(L"=== Source Table ===");
    System::Console::WriteLine(L"Name: {0}", gcnew System::String(sourceTable->GetName().c_str()));
    System::Console::WriteLine(L"Columns: {0}", sourceTable->GetHeader().GetColumns().size());
    System::Console::WriteLine(L"Rows: {0}", sourceTable->Size());

    for (size_t i = 0; i < sourceTable->Size(); i++)
    {
        DataEngine::DataObject obj = sourceTable->Get(i);
        System::Console::Write(L"  Row {0}: ", i);
        for (size_t j = 0; j < obj.Size(); j++)
        {
            System::Console::Write(L"[{0}] ", gcnew System::String(obj.Get(j).ToString().c_str()));
        }
        System::Console::WriteLine(L"");
    }

    // Создаем FilterHub
    DataEngine::FilterHub hub;
    hub.SetSourceTable(std::unique_ptr<DataEngine::Table>(sourceTable));

    // Создаем фильтрованное представление
    DataEngine::FilteredTable* filtered = hub.CreateFilteredTable();
    filtered->SetName("Filtered Persons");

    // Показываем форму создания фильтра
    DataEngine::FilterInfoForm^ filterForm = gcnew DataEngine::FilterInfoForm();
    filterForm->SetFilteredTable(filtered);

    if (filterForm->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        DataEngine::FilterInfo filterInfo;
        if (filterForm->GetFilterInfo(filterInfo))
        {
            filtered->AddFilter(std::move(filterInfo));

            System::Console::WriteLine(L"\n=== Filter applied ===");
            System::Console::WriteLine(L"Filter: {0}", gcnew System::String(filterInfo.GetDescription().c_str()));
            System::Console::WriteLine(L"Source table: {0}", gcnew System::String(sourceTable->GetName().c_str()));
            System::Console::WriteLine(L"Source rows: {0}", sourceTable->Size());
            System::Console::WriteLine(L"Filtered rows: {0}", filtered->Size());

            System::Console::WriteLine(L"\n=== Filtered Data ===");
            for (size_t i = 0; i < filtered->Size(); i++)
            {
                DataEngine::DataObject obj = filtered->Get(i);
                System::Console::Write(L"Row {0}: ", i);
                for (size_t j = 0; j < obj.Size(); j++)
                {
                    System::Console::Write(L"[{0}] ", gcnew System::String(obj.Get(j).ToString().c_str()));
                }
                System::Console::WriteLine(L"");
            }
        }
    }
    else
    {
        System::Console::WriteLine(L"\nFilter creation cancelled.");
    }

    delete sourceTable;
    delete filtered;

    System::Console::WriteLine(L"\nPress Enter to exit...");
    System::Console::ReadLine();

    return 0;
}






//
//Application::EnableVisualStyles();
//Application::SetCompatibleTextRenderingDefault(false);
//
//// Инициализация парсеров даты и времени
//DataEngine::Date::InitializeDefaultParser();
//DataEngine::Time::InitializeDefaultParser();
//
//DataEngine::TestForm^ form = gcnew DataEngine::TestForm();
//Application::Run(form);
//
//return 0;