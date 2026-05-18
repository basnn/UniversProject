// TextLoaderConfigForm.h
#pragma once
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>
#include "Loader.h"
#include "Table.h"
#include "Date.h"
#include "Time.h"
#include <MFP/MFWR.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;
using namespace System::Globalization;

namespace DataEngine {

    public ref class TextLoaderConfigForm : public Form
    {
    private:
        TextBox^ m_txtDelimiter;
        CheckBox^ m_chkHeader;
        NumericUpDown^ m_numSkipLines;
        Button^ m_btnOk;
        Button^ m_btnCancel;
        Label^ m_lblPreview;
        
        String^ m_selectedFile;
        TextImportConfig* m_config;
        Table* m_table;           // Сохраняем Table
        TextLoader* m_loader;     // Сохраняем TextLoader

    public:
        TextLoaderConfigForm();
        ~TextLoaderConfigForm();

        void SetConfig(const TextImportConfig& config);
        void GetConfig(TextImportConfig& config);
        void SetFilePath(String^ path);
        String^ GetFilePath();
        
        void SetTable(Table* table);
        void SetLoader(TextLoader* loader);

        System::Windows::Forms::DialogResult ShowConfigDialog();
        List<String^>^ ParseHeaders();

    private:
        void InitializeComponent();
        void OnOkClick(Object^ sender, EventArgs^ e);
        void OnCancelClick(Object^ sender, EventArgs^ e);
        void OnDelimiterTextChanged(Object^ sender, EventArgs^ e);
        void UpdatePreview();
        void ApplySettings();
        void ShowError(String^ message);
        void ApplyToTableAndLoader();  // Новый метод для применения к Table и Loader
    };

}