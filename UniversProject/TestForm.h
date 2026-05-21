//// TestForm.h
//#pragma once
//#include <msclr\marshal.h>
//#include <msclr\marshal_cppstd.h>
//#include "Table.h"
//#include "Loader.h"
//#include "TextLoaderConfigForm.h"
//#include "TableConfigForm.h"
//#include <MFP/MFWR.h>
//
//using namespace System;
//using namespace System::ComponentModel;
//using namespace System::Collections;
//using namespace System::Windows::Forms;
//using namespace System::Data;
//using namespace System::Drawing;
//using namespace System::IO;
//using namespace System::Collections::Generic;
//using namespace System::Drawing::Drawing2D;
//using namespace System::Globalization;
//
//namespace DataEngine {
//
//    public ref class TestForm : public Form
//    {
//    private:
//        DataGridView^ m_dataGridView;
//        Button^ m_btnOpen;
//        StatusStrip^ m_statusStrip;
//        ToolStripStatusLabel^ m_statusLabel;
//        OpenFileDialog^ m_openFileDialog;
//        TextLoaderConfigForm^ m_loaderConfigForm;
//        TableConfigForm^ m_tableConfigForm;
//
//        Table* m_table;
//        TextLoader* m_loader;
//        TextImportConfig* m_config;
//
//        List<String^>^ m_headers;
//        String^ m_currentFilePath;
//
//    public:
//        TestForm();
//        ~TestForm();
//
//    private:
//        void InitializeComponent();
//        void OnOpenFile(Object^ sender, EventArgs^ e);
//        void RefreshDataGridView();
//        void UpdateStatus();
//        void ShowError(String^ message);
//        void LoadDataStructure();
//        void LoadData();
//    };
//
//}