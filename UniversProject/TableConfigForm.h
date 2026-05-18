// TableConfigForm.h
#pragma once
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>
#include "Table.h"
#include "HeaderDefinition.h"
#include "ColumnDefinition.h"

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

    public ref class TableConfigForm : public Form
    {
    private:
        DataGridView^ m_dataGridView;
        Button^ m_btnAddColumn;
        Button^ m_btnRemoveColumn;
        Button^ m_btnMoveUp;
        Button^ m_btnMoveDown;
        Button^ m_btnOk;
        Button^ m_btnCancel;
        Button^ m_btnReset;
        
        Table* m_table;
        
        ref struct ColumnInfo
        {
            String^ Name;
            DataType Type;
            String^ DefaultValue;
            String^ ConvertedValue;
        };
        
        List<ColumnInfo^>^ m_tempColumns;

    public:
        TableConfigForm();
        ~TableConfigForm();

        void SetTable(Table* table);

    private:
        void InitializeComponent();
        void OnAddColumnClick(Object^ sender, EventArgs^ e);
        void OnRemoveColumnClick(Object^ sender, EventArgs^ e);
        void OnMoveUpClick(Object^ sender, EventArgs^ e);
        void OnMoveDownClick(Object^ sender, EventArgs^ e);
        void OnResetClick(Object^ sender, EventArgs^ e);
        void OnOkClick(Object^ sender, EventArgs^ e);
        void OnCancelClick(Object^ sender, EventArgs^ e);
        void OnCellValueChanged(Object^ sender, DataGridViewCellEventArgs^ e);
        void OnCurrentCellDirtyStateChanged(Object^ sender, EventArgs^ e);
        void OnSelectionChanged(Object^ sender, EventArgs^ e);
        
        void RefreshDataGridView();
        void LoadFromTable();
        void ApplyToTable();
        void ShowError(String^ message);
        void UpdateConvertedValue(int rowIndex);
        
        DataType GetDataTypeFromString(String^ typeStr);
        String^ GetStringFromDataType(DataType type);
        String^ ConvertValueByType(String^ value, DataType newType);
    };

}