// FilterInfoForm.cpp - добавить реализацию GetOperatorFromName

#include "FilterInfoForm.h"

namespace DataEngine {

    FilterInfoForm::FilterInfoForm()
        : m_filteredTable(nullptr)
        , m_result(nullptr)
        , m_hasResult(false)
    {
        InitializeComponent();
        LoadOperationsFromGlobalList();
    }

    FilterInfoForm::~FilterInfoForm()
    {
    }

    void FilterInfoForm::LoadOperationsFromGlobalList()
    {
        m_cboOperation->Items->Clear();
        
        std::vector<FilterOperation*>& operations = FilterOperation::GetOperations();
        
        for (auto* op : operations)
        {
            if (op)
            {
                String^ name = gcnew String(op->GetName().c_str());
                if (!m_cboOperation->Items->Contains(name))
                {
                    m_cboOperation->Items->Add(name);
                }
            }
        }
        
        if (m_cboOperation->Items->Count > 0)
            m_cboOperation->SelectedIndex = 0;
    }

    void FilterInfoForm::InitializeComponent()
    {
        this->Text = L"Add Filter Condition";
        this->Size = Drawing::Size(650, 450);
        this->StartPosition = FormStartPosition::CenterParent;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;

        TableLayoutPanel^ mainPanel = gcnew TableLayoutPanel();
        mainPanel->Dock = DockStyle::Fill;
        mainPanel->RowCount = 10;
        mainPanel->ColumnCount = 2;
        mainPanel->Padding = System::Windows::Forms::Padding(15);
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 35));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 30));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 25));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 35));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 30));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 25));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 35));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 30));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 25));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 100));
        mainPanel->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Absolute, 110));
        mainPanel->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100));

        // Column selection
        Label^ lblColumn = gcnew Label();
        lblColumn->Text = L"Column:";
        lblColumn->TextAlign = ContentAlignment::MiddleRight;
        lblColumn->Dock = DockStyle::Fill;
        lblColumn->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);

        m_cboColumn = gcnew ComboBox();
        m_cboColumn->DropDownStyle = ComboBoxStyle::DropDownList;
        m_cboColumn->Dock = DockStyle::Fill;
        m_cboColumn->SelectedIndexChanged += gcnew EventHandler(this, &FilterInfoForm::OnColumnSelectedIndexChanged);

        m_lblColumnHint = gcnew Label();
        m_lblColumnHint->Text = L"";
        m_lblColumnHint->ForeColor = Drawing::Color::Gray;
        m_lblColumnHint->Font = gcnew Drawing::Font(L"Segoe UI", 8);
        m_lblColumnHint->Dock = DockStyle::Fill;
        mainPanel->SetColumnSpan(m_lblColumnHint, 2);

        // Operation selection
        Label^ lblOperation = gcnew Label();
        lblOperation->Text = L"Operator:";
        lblOperation->TextAlign = ContentAlignment::MiddleRight;
        lblOperation->Dock = DockStyle::Fill;
        lblOperation->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);

        m_cboOperation = gcnew ComboBox();
        m_cboOperation->DropDownStyle = ComboBoxStyle::DropDownList;
        m_cboOperation->Dock = DockStyle::Fill;
        m_cboOperation->SelectedIndexChanged += gcnew EventHandler(this, &FilterInfoForm::OnOperationSelectedIndexChanged);

        m_lblOperationHint = gcnew Label();
        m_lblOperationHint->Text = L"";
        m_lblOperationHint->ForeColor = Drawing::Color::Gray;
        m_lblOperationHint->Font = gcnew Drawing::Font(L"Segoe UI", 8);
        m_lblOperationHint->Dock = DockStyle::Fill;
        mainPanel->SetColumnSpan(m_lblOperationHint, 2);

        // Value input
        Label^ lblValue = gcnew Label();
        lblValue->Text = L"Value:";
        lblValue->TextAlign = ContentAlignment::MiddleRight;
        lblValue->Dock = DockStyle::Fill;
        lblValue->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);

        m_txtValue = gcnew TextBox();
        m_txtValue->Dock = DockStyle::Fill;
        m_txtValue->TextChanged += gcnew EventHandler(this, &FilterInfoForm::OnValueTextChanged);

        m_lblValueHint = gcnew Label();
        m_lblValueHint->Text = L"Enter value for comparison (examples: text, 123, 45.67, 2024-01-01)";
        m_lblValueHint->ForeColor = Drawing::Color::Gray;
        m_lblValueHint->Font = gcnew Drawing::Font(L"Segoe UI", 8);
        m_lblValueHint->Dock = DockStyle::Fill;
        mainPanel->SetColumnSpan(m_lblValueHint, 2);

        // Buttons
        Panel^ buttonPanel = gcnew Panel();
        buttonPanel->Dock = DockStyle::Fill;
        buttonPanel->Height = 50;

        m_btnOk = gcnew Button();
        m_btnOk->Text = L"Apply Filter";
        m_btnOk->Size = Drawing::Size(110, 32);
        m_btnOk->Location = Point(250, 9);
        m_btnOk->Enabled = false;
        m_btnOk->Click += gcnew EventHandler(this, &FilterInfoForm::OnOkClick);

        m_btnCancel = gcnew Button();
        m_btnCancel->Text = L"Cancel";
        m_btnCancel->Size = Drawing::Size(100, 32);
        m_btnCancel->Location = Point(370, 9);
        m_btnCancel->Click += gcnew EventHandler(this, &FilterInfoForm::OnCancelClick);

        buttonPanel->Controls->Add(m_btnOk);
        buttonPanel->Controls->Add(m_btnCancel);

        mainPanel->Controls->Add(lblColumn, 0, 0);
        mainPanel->Controls->Add(m_cboColumn, 1, 0);
        mainPanel->Controls->Add(m_lblColumnHint, 0, 1);
        mainPanel->Controls->Add(lblOperation, 0, 2);
        mainPanel->Controls->Add(m_cboOperation, 1, 2);
        mainPanel->Controls->Add(m_lblOperationHint, 0, 3);
        mainPanel->Controls->Add(lblValue, 0, 4);
        mainPanel->Controls->Add(m_txtValue, 1, 4);
        mainPanel->Controls->Add(m_lblValueHint, 0, 5);
        mainPanel->Controls->Add(buttonPanel, 1, 6);

        this->Controls->Add(mainPanel);
    }

    void FilterInfoForm::SetFilteredTable(FilteredTable* table)
    {
        m_filteredTable = table;
        
        if (m_filteredTable)
        {
            const HeaderDefinition& header = m_filteredTable->GetHeader();
            const std::vector<ColumnDefinition>& columns = header.GetColumns();
            
            m_cboColumn->Items->Clear();
            
            for (size_t i = 0; i < columns.size(); i++)
            {
                String^ item = gcnew String(columns[i].GetName().c_str());
                m_cboColumn->Items->Add(item);
            }
            
            if (m_cboColumn->Items->Count > 0)
                m_cboColumn->SelectedIndex = 0;
        }
    }

    bool FilterInfoForm::GetFilterInfo(FilterInfo& outInfo)
    {
        if (!m_hasResult)
            return false;
        
        outInfo = std::move(*m_result);
        delete m_result;
        m_result = nullptr;
        return true;
    }

    void FilterInfoForm::OnColumnSelectedIndexChanged(Object^ sender, EventArgs^ e)
    {
        UpdateColumnHint();
        UpdateOkButtonState();
    }

    void FilterInfoForm::OnOperationSelectedIndexChanged(Object^ sender, EventArgs^ e)
    {
        UpdateOperationHint();
        UpdateOkButtonState();
        
        String^ selectedOp = safe_cast<String^>(m_cboOperation->SelectedItem);
        
        bool needValue = true;
        if (selectedOp == L"IsNull" || selectedOp == L"IsNotNull")
        {
            needValue = false;
        }
        
        m_txtValue->Enabled = needValue;
        m_lblValueHint->Text = needValue ? 
            L"Enter value for comparison (examples: text, 123, 45.67, 2024-01-01)" : 
            L"No value needed for this operator";
        
        if (!needValue)
            m_txtValue->Text = L"";
    }

    void FilterInfoForm::OnValueTextChanged(Object^ sender, EventArgs^ e)
    {
        UpdateOkButtonState();
    }

    void FilterInfoForm::UpdateColumnHint()
    {
        if (!m_filteredTable || m_cboColumn->SelectedIndex < 0)
        {
            m_lblColumnHint->Text = L"";
            return;
        }
        
        int colIndex = m_cboColumn->SelectedIndex;
        const HeaderDefinition& header = m_filteredTable->GetHeader();
        const std::vector<ColumnDefinition>& columns = header.GetColumns();
        
        if (colIndex < (int)columns.size())
        {
            const ColumnDefinition& col = columns[colIndex];
            String^ typeName = gcnew String(Data::GetTypeName(col.GetType()));
            String^ defaultValue = gcnew String(col.GetDefaultValue().ToString().c_str());
            
            m_lblColumnHint->Text = String::Format(L"Type: {0} | Default value: {1}", typeName, defaultValue);
            
            if (String::IsNullOrEmpty(m_txtValue->Text))
            {
                m_txtValue->Text = defaultValue;
            }
        }
    }

    void FilterInfoForm::UpdateOperationHint()
    {
        if (m_cboOperation->SelectedIndex < 0)
        {
            m_lblOperationHint->Text = L"";
            return;
        }
        
        String^ selectedOp = safe_cast<String^>(m_cboOperation->SelectedItem);
        
        if (selectedOp == L"ValueCompare")
            m_lblOperationHint->Text = L"Compare column value with specified value";
        else if (selectedOp == L"AND")
            m_lblOperationHint->Text = L"Logical AND between conditions";
        else if (selectedOp == L"OR")
            m_lblOperationHint->Text = L"Logical OR between conditions";
        else if (selectedOp == L"NOT")
            m_lblOperationHint->Text = L"Logical NOT (negation) of a condition";
        else if (selectedOp == L"IsNull")
            m_lblOperationHint->Text = L"Field value is null/empty";
        else if (selectedOp == L"IsNotNull")
            m_lblOperationHint->Text = L"Field value is not null/empty";
        else
            m_lblOperationHint->Text = L"";
    }

    void FilterInfoForm::UpdateOkButtonState()
    {
        if (m_cboColumn->SelectedIndex < 0)
        {
            m_btnOk->Enabled = false;
            return;
        }
        
        if (m_cboOperation->SelectedIndex < 0)
        {
            m_btnOk->Enabled = false;
            return;
        }
        
        String^ selectedOp = safe_cast<String^>(m_cboOperation->SelectedItem);
        bool needValue = (selectedOp != L"IsNull" && selectedOp != L"IsNotNull");
        
        if (needValue && String::IsNullOrEmpty(m_txtValue->Text))
        {
            m_btnOk->Enabled = false;
            return;
        }
        
        m_btnOk->Enabled = true;
    }

    void FilterInfoForm::OnOkClick(Object^ sender, EventArgs^ e)
    {
        if (!m_filteredTable || m_cboColumn->SelectedIndex < 0)
        {
            ShowError(L"Invalid column selection");
            return;
        }
        
        int colIndex = m_cboColumn->SelectedIndex;
        
        Data value;
        msclr::interop::marshal_context context;
        std::string strValue = context.marshal_as<std::string>(m_txtValue->Text);
        value = Data(strValue);
        
        auto operation = std::make_unique<ValueCompareOperation>(FilterOperator::Equal, value);
        m_result = new FilterInfo(colIndex, std::move(operation));
        m_hasResult = true;
        
        this->DialogResult = System::Windows::Forms::DialogResult::OK;
        this->Close();
    }

    void FilterInfoForm::OnCancelClick(Object^ sender, EventArgs^ e)
    {
        this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
        this->Close();
    }

    void FilterInfoForm::ShowError(String^ message)
    {
        MessageBox::Show(message, L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }

}