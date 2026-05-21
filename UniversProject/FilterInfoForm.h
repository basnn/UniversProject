// FilterInfoForm.h
#pragma once
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>
#include "FilteredTable.h"
#include "FilterDefaultOperation.h"

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

    public ref class FilterInfoForm : public Form
    {
    private:
        ComboBox^ m_cboColumn;
        ComboBox^ m_cboOperation;
        TextBox^ m_txtValue;
        Button^ m_btnOk;
        Button^ m_btnCancel;
        Label^ m_lblColumnHint;
        Label^ m_lblOperationHint;
        Label^ m_lblValueHint;
        
        FilteredTable* m_filteredTable;
        FilterInfo* m_result;
        bool m_hasResult;
        
    public:
        FilterInfoForm();
        ~FilterInfoForm();
        
        void SetFilteredTable(FilteredTable* table);
        bool GetFilterInfo(FilterInfo& outInfo);
        
    private:
        void InitializeComponent();
        void LoadOperationsFromGlobalList();
        void OnOkClick(Object^ sender, EventArgs^ e);
        void OnCancelClick(Object^ sender, EventArgs^ e);
        void OnColumnSelectedIndexChanged(Object^ sender, EventArgs^ e);
        void OnOperationSelectedIndexChanged(Object^ sender, EventArgs^ e);
        void OnValueTextChanged(Object^ sender, EventArgs^ e);
        void UpdateColumnHint();
        void UpdateOperationHint();
        void UpdateOkButtonState();
        void ShowError(String^ message);
        
     
    };

}