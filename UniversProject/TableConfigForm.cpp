// TableConfigForm.cpp
#include "TableConfigForm.h"

namespace DataEngine {

    TableConfigForm::TableConfigForm()
        : m_table(nullptr)
        , m_tempColumns(gcnew List<ColumnInfo^>())
    {
        InitializeComponent();
    }

    TableConfigForm::~TableConfigForm()
    {
    }

    void TableConfigForm::InitializeComponent()
    {
        this->Text = L"Table Configuration - Header Setup";
        this->Size = Drawing::Size(950, 550);
        this->StartPosition = FormStartPosition::CenterParent;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;

        TableLayoutPanel^ mainPanel = gcnew TableLayoutPanel();
        mainPanel->Dock = DockStyle::Fill;
        mainPanel->RowCount = 3;
        mainPanel->ColumnCount = 1;
        mainPanel->Padding = System::Windows::Forms::Padding(10);
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 100));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 40));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 50));

        m_dataGridView = gcnew DataGridView();
        m_dataGridView->Dock = DockStyle::Fill;
        m_dataGridView->AllowUserToAddRows = false;
        m_dataGridView->AllowUserToDeleteRows = false;
        m_dataGridView->ReadOnly = false;
        m_dataGridView->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        m_dataGridView->RowHeadersVisible = true;
        m_dataGridView->RowHeadersWidth = 40;
        m_dataGridView->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
        m_dataGridView->MultiSelect = false;

        DataGridViewTextBoxColumn^ nameColumn = gcnew DataGridViewTextBoxColumn();
        nameColumn->Name = L"ColumnName";
        nameColumn->HeaderText = L"Column Name";
        nameColumn->Width = 150;

        DataGridViewComboBoxColumn^ typeColumn = gcnew DataGridViewComboBoxColumn();
        typeColumn->Name = L"DataType";
        typeColumn->HeaderText = L"Data Type";
        typeColumn->Items->Add(L"Text");
        typeColumn->Items->Add(L"Integer");
        typeColumn->Items->Add(L"UInteger");
        typeColumn->Items->Add(L"Double");
        typeColumn->Items->Add(L"Date");
        typeColumn->Items->Add(L"DateTime");
        typeColumn->Width = 100;

        DataGridViewTextBoxColumn^ defaultValueColumn = gcnew DataGridViewTextBoxColumn();
        defaultValueColumn->Name = L"DefaultValue";
        defaultValueColumn->HeaderText = L"Default Value";
        defaultValueColumn->Width = 150;

        DataGridViewTextBoxColumn^ convertedValueColumn = gcnew DataGridViewTextBoxColumn();
        convertedValueColumn->Name = L"ConvertedValue";
        convertedValueColumn->HeaderText = L"Converted Value (after type change)";
        convertedValueColumn->Width = 200;
        convertedValueColumn->ReadOnly = true;

        m_dataGridView->Columns->Add(nameColumn);
        m_dataGridView->Columns->Add(typeColumn);
        m_dataGridView->Columns->Add(defaultValueColumn);
        m_dataGridView->Columns->Add(convertedValueColumn);

        m_dataGridView->CurrentCellDirtyStateChanged += gcnew EventHandler(this, &TableConfigForm::OnCurrentCellDirtyStateChanged);
        m_dataGridView->CellValueChanged += gcnew DataGridViewCellEventHandler(this, &TableConfigForm::OnCellValueChanged);
        m_dataGridView->SelectionChanged += gcnew EventHandler(this, &TableConfigForm::OnSelectionChanged);

        Panel^ controlPanel = gcnew Panel();
        controlPanel->Dock = DockStyle::Fill;
        controlPanel->Height = 40;

        m_btnAddColumn = gcnew Button();
        m_btnAddColumn->Text = L"Add Column";
        m_btnAddColumn->Size = Drawing::Size(100, 30);
        m_btnAddColumn->Location = Point(10, 5);
        m_btnAddColumn->Click += gcnew EventHandler(this, &TableConfigForm::OnAddColumnClick);

        m_btnRemoveColumn = gcnew Button();
        m_btnRemoveColumn->Text = L"Remove Column";
        m_btnRemoveColumn->Size = Drawing::Size(110, 30);
        m_btnRemoveColumn->Location = Point(120, 5);
        m_btnRemoveColumn->Click += gcnew EventHandler(this, &TableConfigForm::OnRemoveColumnClick);

        m_btnMoveUp = gcnew Button();
        m_btnMoveUp->Text = L"Move Up";
        m_btnMoveUp->Size = Drawing::Size(80, 30);
        m_btnMoveUp->Location = Point(240, 5);
        m_btnMoveUp->Click += gcnew EventHandler(this, &TableConfigForm::OnMoveUpClick);

        m_btnMoveDown = gcnew Button();
        m_btnMoveDown->Text = L"Move Down";
        m_btnMoveDown->Size = Drawing::Size(90, 30);
        m_btnMoveDown->Location = Point(330, 5);
        m_btnMoveDown->Click += gcnew EventHandler(this, &TableConfigForm::OnMoveDownClick);

        m_btnReset = gcnew Button();
        m_btnReset->Text = L"Reset";
        m_btnReset->Size = Drawing::Size(80, 30);
        m_btnReset->Location = Point(500, 5);
        m_btnReset->Click += gcnew EventHandler(this, &TableConfigForm::OnResetClick);

        controlPanel->Controls->Add(m_btnAddColumn);
        controlPanel->Controls->Add(m_btnRemoveColumn);
        controlPanel->Controls->Add(m_btnMoveUp);
        controlPanel->Controls->Add(m_btnMoveDown);
        controlPanel->Controls->Add(m_btnReset);

        Panel^ buttonPanel = gcnew Panel();
        buttonPanel->Dock = DockStyle::Fill;
        buttonPanel->Height = 50;

        m_btnOk = gcnew Button();
        m_btnOk->Text = L"Apply & Close";
        m_btnOk->Size = Drawing::Size(110, 32);
        m_btnOk->Location = Point(350, 9);
        m_btnOk->Click += gcnew EventHandler(this, &TableConfigForm::OnOkClick);

        m_btnCancel = gcnew Button();
        m_btnCancel->Text = L"Cancel";
        m_btnCancel->Size = Drawing::Size(100, 32);
        m_btnCancel->Location = Point(470, 9);
        m_btnCancel->Click += gcnew EventHandler(this, &TableConfigForm::OnCancelClick);

        buttonPanel->Controls->Add(m_btnOk);
        buttonPanel->Controls->Add(m_btnCancel);

        mainPanel->Controls->Add(m_dataGridView, 0, 0);
        mainPanel->Controls->Add(controlPanel, 0, 1);
        mainPanel->Controls->Add(buttonPanel, 0, 2);

        this->Controls->Add(mainPanel);
    }

    void TableConfigForm::SetTable(Table* table)
    {
        m_table = table;
        if (m_table)
        {
            LoadFromTable();
        }
    }

    void TableConfigForm::LoadFromTable()
    {
        if (!m_table) return;

        m_tempColumns->Clear();

        const HeaderDefinition& header = m_table->GetHeader();
        const std::vector<ColumnDefinition>& columns = header.GetColumns();

        for (size_t i = 0; i < columns.size(); i++)
        {
            ColumnInfo^ col = gcnew ColumnInfo();
            col->Name = gcnew String(columns[i].GetName().c_str());
            col->Type = columns[i].GetType();

            Data defaultValue = columns[i].GetDefaultValue();
            std::string defaultStr = defaultValue.ToString();
            String^ defaultString = gcnew String(defaultStr.c_str());

            col->DefaultValue = defaultString;
            col->ConvertedValue = ConvertValueByType(defaultString, col->Type);
            m_tempColumns->Add(col);
        }

        RefreshDataGridView();
    }

    void TableConfigForm::RefreshDataGridView()
    {
        m_dataGridView->Rows->Clear();

        for (int i = 0; i < m_tempColumns->Count; i++)
        {
            DataGridViewRow^ row = gcnew DataGridViewRow();
            row->CreateCells(m_dataGridView);

            row->Cells[0]->Value = m_tempColumns[i]->Name;
            row->Cells[1]->Value = GetStringFromDataType(m_tempColumns[i]->Type);
            row->Cells[2]->Value = m_tempColumns[i]->DefaultValue;
            row->Cells[3]->Value = m_tempColumns[i]->ConvertedValue;

            m_dataGridView->Rows->Add(row);
        }
    }

    void TableConfigForm::UpdateConvertedValue(int rowIndex)
    {
        if (rowIndex < 0 || rowIndex >= m_tempColumns->Count) return;

        String^ defaultValue = m_tempColumns[rowIndex]->DefaultValue;
        DataType newType = m_tempColumns[rowIndex]->Type;

        String^ convertedValue = ConvertValueByType(defaultValue, newType);
        m_tempColumns[rowIndex]->ConvertedValue = convertedValue;

        if (rowIndex < m_dataGridView->Rows->Count)
        {
            m_dataGridView->Rows[rowIndex]->Cells[3]->Value = convertedValue;
        }
    }

    void TableConfigForm::OnAddColumnClick(Object^ sender, EventArgs^ e)
    {
        ColumnInfo^ col = gcnew ColumnInfo();
        col->Name = L"NewColumn";
        col->Type = DataType::Text;
        col->DefaultValue = L"";
        col->ConvertedValue = L"";
        m_tempColumns->Add(col);
        RefreshDataGridView();
    }

    void TableConfigForm::OnRemoveColumnClick(Object^ sender, EventArgs^ e)
    {
        if (m_dataGridView->SelectedRows->Count > 0)
        {
            int index = m_dataGridView->SelectedRows[0]->Index;
            if (index >= 0 && index < m_tempColumns->Count)
            {
                m_tempColumns->RemoveAt(index);
                RefreshDataGridView();
            }
        }
        else
        {
            ShowError(L"Please select a column to remove");
        }
    }

    void TableConfigForm::OnMoveUpClick(Object^ sender, EventArgs^ e)
    {
        if (m_dataGridView->SelectedRows->Count > 0)
        {
            int index = m_dataGridView->SelectedRows[0]->Index;
            if (index > 0)
            {
                ColumnInfo^ temp = m_tempColumns[index - 1];
                m_tempColumns[index - 1] = m_tempColumns[index];
                m_tempColumns[index] = temp;
                RefreshDataGridView();
                m_dataGridView->Rows[index - 1]->Selected = true;
            }
        }
    }

    void TableConfigForm::OnMoveDownClick(Object^ sender, EventArgs^ e)
    {
        if (m_dataGridView->SelectedRows->Count > 0)
        {
            int index = m_dataGridView->SelectedRows[0]->Index;
            if (index < m_tempColumns->Count - 1)
            {
                ColumnInfo^ temp = m_tempColumns[index + 1];
                m_tempColumns[index + 1] = m_tempColumns[index];
                m_tempColumns[index] = temp;
                RefreshDataGridView();
                m_dataGridView->Rows[index + 1]->Selected = true;
            }
        }
    }

    void TableConfigForm::OnResetClick(Object^ sender, EventArgs^ e)
    {
        if (m_table)
        {
            LoadFromTable();
        }
    }

    void TableConfigForm::OnCurrentCellDirtyStateChanged(Object^ sender, EventArgs^ e)
    {
        if (m_dataGridView->IsCurrentCellDirty)
        {
            m_dataGridView->CommitEdit(DataGridViewDataErrorContexts::Commit);
        }
    }

    void TableConfigForm::OnCellValueChanged(Object^ sender, DataGridViewCellEventArgs^ e)
    {
        if (e->RowIndex < 0 || e->RowIndex >= m_tempColumns->Count) return;

        if (e->ColumnIndex == 0)
        {
            String^ newName = safe_cast<String^>(m_dataGridView->Rows[e->RowIndex]->Cells[0]->Value);
            if (!String::IsNullOrEmpty(newName))
            {
                m_tempColumns[e->RowIndex]->Name = newName;
            }
        }

        if (e->ColumnIndex == 1)
        {
            String^ typeStr = safe_cast<String^>(m_dataGridView->Rows[e->RowIndex]->Cells[1]->Value);
            DataType newType = GetDataTypeFromString(typeStr);
            m_tempColumns[e->RowIndex]->Type = newType;
            UpdateConvertedValue(e->RowIndex);
        }

        if (e->ColumnIndex == 2)
        {
            String^ newValue = safe_cast<String^>(m_dataGridView->Rows[e->RowIndex]->Cells[2]->Value);
            m_tempColumns[e->RowIndex]->DefaultValue = newValue;
            UpdateConvertedValue(e->RowIndex);
        }
    }

    void TableConfigForm::OnSelectionChanged(Object^ sender, EventArgs^ e)
    {
        if (m_dataGridView->SelectedRows->Count > 1)
        {
            for (int i = 1; i < m_dataGridView->SelectedRows->Count; i++)
            {
                m_dataGridView->SelectedRows[i]->Selected = false;
            }
        }
    }

    void TableConfigForm::OnOkClick(Object^ sender, EventArgs^ e)
    {
        ApplyToTable();
        this->DialogResult = System::Windows::Forms::DialogResult::OK;
        this->Close();
    }

    void TableConfigForm::OnCancelClick(Object^ sender, EventArgs^ e)
    {
        this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
        this->Close();
    }

    void TableConfigForm::ApplyToTable()
    {
        if (!m_table) return;

        std::vector<ColumnDefinition> columns;

        for (int i = 0; i < m_tempColumns->Count; i++)
        {
            msclr::interop::marshal_context context;
            std::string name = context.marshal_as<std::string>(m_tempColumns[i]->Name);
            std::string defaultValueStr = context.marshal_as<std::string>(m_tempColumns[i]->ConvertedValue);

            Data defaultValue;
            switch (m_tempColumns[i]->Type)
            {
            case DataType::Integer:
                try { defaultValue = Data((long long)std::stoll(defaultValueStr)); }
                catch (...) { defaultValue = Data(); }
                break;
            case DataType::UInteger:
                try { defaultValue = Data((unsigned long long)std::stoull(defaultValueStr)); }
                catch (...) { defaultValue = Data(); }
                break;
            case DataType::Double:
                try { defaultValue = Data(std::stod(defaultValueStr)); }
                catch (...) { defaultValue = Data(); }
                break;
            case DataType::Date:
            {
                Date d;
                if (Date::TryParse(defaultValueStr, d))
                    defaultValue = Data(d);
                else
                    defaultValue = Data();
            }
            break;
            case DataType::DateTime:
            {
                Time t;
                if (Time::TryParse(defaultValueStr, t))
                    defaultValue = Data(t);
                else
                    defaultValue = Data();
            }
            break;
            default:
                defaultValue = Data(defaultValueStr);
                break;
            }

            columns.push_back(ColumnDefinition(name, m_tempColumns[i]->Type, defaultValue));
        }

        HeaderDefinition newHeader;
        newHeader.SetColumns(columns);
        m_table->SetHeader(newHeader);
    }

    String^ TableConfigForm::ConvertValueByType(String^ value, DataType newType)
    {
        if (String::IsNullOrEmpty(value))
        {
            switch (newType)
            {
            case DataType::Integer: return L"0";
            case DataType::UInteger: return L"0";
            case DataType::Double: return L"0.00";
            case DataType::Date: return L"25.12.2000";
            case DataType::DateTime: return L"25.12.2000 14:30:00";
            default: return L"";
            }
        }

        msclr::interop::marshal_context context;
        std::string strValue = context.marshal_as<std::string>(value);

        // Пробуем распарсить в зависимости от целевого типа напрямую
        switch (newType)
        {
        case DataType::Date:
        {
            Date d;
            if (Date::TryParse(strValue, d))
            {
                return gcnew String(d.ToString().c_str());
            }
            return L"25.12.2000";
        }
        case DataType::DateTime:
        {
            Time t;
            if (Time::TryParse(strValue, t))
            {
                return gcnew String(t.ToString().c_str());
            }
            return L"25.12.2000 14:30:00";
        }
        case DataType::Integer:
        {
            try
            {
                long long val = std::stoll(strValue);
                return gcnew String(std::to_string(val).c_str());
            }
            catch (...)
            {
                return L"0";
            }
        }
        case DataType::UInteger:
        {
            try
            {
                unsigned long long val = std::stoull(strValue);
                return gcnew String(std::to_string(val).c_str());
            }
            catch (...)
            {
                return L"0";
            }
        }
        case DataType::Double:
        {
            try
            {
                double val = std::stod(strValue);
                char buf[64];
                sprintf_s(buf, "%.2f", val);
                return gcnew String(buf);
            }
            catch (...)
            {
                return L"0.00";
            }
        }
        default:
            return value;
        }
    }

    DataType TableConfigForm::GetDataTypeFromString(String^ typeStr)
    {
        if (typeStr == L"Integer") return DataType::Integer;
        if (typeStr == L"UInteger") return DataType::UInteger;
        if (typeStr == L"Double") return DataType::Double;
        if (typeStr == L"Date") return DataType::Date;
        if (typeStr == L"DateTime") return DataType::DateTime;
        return DataType::Text;
    }

    String^ TableConfigForm::GetStringFromDataType(DataType type)
    {
        switch (type)
        {
        case DataType::Integer: return L"Integer";
        case DataType::UInteger: return L"UInteger";
        case DataType::Double: return L"Double";
        case DataType::Date: return L"Date";
        case DataType::DateTime: return L"DateTime";
        default: return L"Text";
        }
    }

    void TableConfigForm::ShowError(String^ message)
    {
        MessageBox::Show(message, L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }

}