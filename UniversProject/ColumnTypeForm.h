//// ColumnTypeForm.h - добавить объявление OnShown
//#pragma once
//#include <msclr\marshal.h>
//#include <msclr\marshal_cppstd.h>
//#include "Loader.h"
//#include <fstream>
//#include <sstream>
//#include <vector>
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
//    public ref class ColumnTypeForm : public Form
//    {
//    private:
//        DataGridView^ m_dataGridView;
//        Button^ m_btnOk;
//        Button^ m_btnCancel;
//        TextImportConfig* m_config;
//        String^ m_filePath;
//        char m_delimiter;
//        int m_skipLines;
//        bool m_hasHeader;
//        
//        std::vector<std::vector<std::string>>* m_sampleData;
//        List<String^>^ m_headers;
//
//    public:
//        ColumnTypeForm();
//        ~ColumnTypeForm();
//
//        void SetHeaders(List<String^>^ headers);
//        void SetConfig(TextImportConfig* config);
//        void GetConfig(TextImportConfig* config);
//        void SetFilePath(String^ path);
//        void SetDelimiter(char delimiter);
//        void SetSkipLines(int skipLines);
//        void SetHasHeader(bool hasHeader);
//
//    protected:
//        void OnShown(EventArgs^ e) override;
//
//    private:
//        void InitializeComponent();
//        void OnOkClick(Object^ sender, EventArgs^ e);
//        void OnCancelClick(Object^ sender, EventArgs^ e);
//        void OnDataTypeChanged(Object^ sender, DataGridViewCellEventArgs^ e);
//        void LoadSampleData();
//        void UpdatePreviewRow(int rowIndex);
//        String^ FormatValue(const std::string& value, DataType type);
//        String^ GetOriginalValue(int row, int col);
//    };
//
//}