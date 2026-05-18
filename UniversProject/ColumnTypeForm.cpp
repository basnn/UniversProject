//// ColumnTypeForm.cpp - реализация OnShown
//#include "ColumnTypeForm.h"
//
//namespace DataEngine {
//
//    ColumnTypeForm::ColumnTypeForm()
//        : m_config(nullptr)
//        , m_filePath(nullptr)
//        , m_delimiter(',')
//        , m_skipLines(0)
//        , m_hasHeader(true)
//        , m_sampleData(nullptr)
//        , m_headers(gcnew List<String^>())
//    {
//        InitializeComponent();
//        m_sampleData = new std::vector<std::vector<std::string>>();
//    }
//
//    ColumnTypeForm::~ColumnTypeForm()
//    {
//        if (m_sampleData) delete m_sampleData;
//    }
//
//    void ColumnTypeForm::InitializeComponent()
//    {
//        this->Text = L"Column Type Configuration - Step 2";
//        this->Size = Drawing::Size(1000, 600);
//        this->StartPosition = FormStartPosition::CenterParent;
//        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
//        this->MaximizeBox = false;
//        this->MinimizeBox = false;
//
//        TableLayoutPanel^ mainPanel = gcnew TableLayoutPanel();
//        mainPanel->Dock = DockStyle::Fill;
//        mainPanel->RowCount = 3;
//        mainPanel->ColumnCount = 1;
//        mainPanel->Padding = System::Windows::Forms::Padding(10);
//        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 30));
//        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 100));
//        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 50));
//
//        Label^ lblTitle = gcnew Label();
//        lblTitle->Text = L"Select data type for each column (shows Original -> Converted):";
//        lblTitle->TextAlign = ContentAlignment::MiddleLeft;
//        lblTitle->Dock = DockStyle::Fill;
//        lblTitle->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
//
//        m_dataGridView = gcnew DataGridView();
//        m_dataGridView->Dock = DockStyle::Fill;
//        m_dataGridView->AllowUserToAddRows = false;
//        m_dataGridView->AllowUserToDeleteRows = false;
//        m_dataGridView->ReadOnly = false;
//        m_dataGridView->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
//        m_dataGridView->RowHeadersVisible = true;
//        m_dataGridView->RowHeadersWidth = 40;
//        m_dataGridView->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
//        
//        DataGridViewTextBoxColumn^ nameColumn = gcnew DataGridViewTextBoxColumn();
//        nameColumn->Name = L"ColumnName";
//        nameColumn->HeaderText = L"Column Name";
//        nameColumn->ReadOnly = true;
//        nameColumn->Width = 150;
//        
//        DataGridViewComboBoxColumn^ typeColumn = gcnew DataGridViewComboBoxColumn();
//        typeColumn->Name = L"DataType";
//        typeColumn->HeaderText = L"Data Type";
//        typeColumn->Items->Add(L"Text");
//        typeColumn->Items->Add(L"Integer");
//        typeColumn->Items->Add(L"UInteger");
//        typeColumn->Items->Add(L"Double");
//        typeColumn->Items->Add(L"Date");
//        typeColumn->Width = 100;
//        
//        DataGridViewTextBoxColumn^ originalColumn = gcnew DataGridViewTextBoxColumn();
//        originalColumn->Name = L"Original";
//        originalColumn->HeaderText = L"Original Value (example)";
//        originalColumn->ReadOnly = true;
//        originalColumn->Width = 200;
//        
//        DataGridViewTextBoxColumn^ convertedColumn = gcnew DataGridViewTextBoxColumn();
//        convertedColumn->Name = L"Converted";
//        convertedColumn->HeaderText = L"Converted Value (example)";
//        convertedColumn->ReadOnly = true;
//        convertedColumn->Width = 200;
//        
//        m_dataGridView->Columns->Add(nameColumn);
//        m_dataGridView->Columns->Add(typeColumn);
//        m_dataGridView->Columns->Add(originalColumn);
//        m_dataGridView->Columns->Add(convertedColumn);
//        
//        m_dataGridView->CellValueChanged += gcnew DataGridViewCellEventHandler(this, &ColumnTypeForm::OnDataTypeChanged);
//
//        Panel^ buttonPanel = gcnew Panel();
//        buttonPanel->Dock = DockStyle::Fill;
//        buttonPanel->Height = 50;
//
//        m_btnOk = gcnew Button();
//        m_btnOk->Text = L"Finish";
//        m_btnOk->Size = Drawing::Size(100, 32);
//        m_btnOk->Location = Point(400, 9);
//        m_btnOk->Click += gcnew EventHandler(this, &ColumnTypeForm::OnOkClick);
//
//        m_btnCancel = gcnew Button();
//        m_btnCancel->Text = L"Back";
//        m_btnCancel->Size = Drawing::Size(100, 32);
//        m_btnCancel->Location = Point(510, 9);
//        m_btnCancel->Click += gcnew EventHandler(this, &ColumnTypeForm::OnCancelClick);
//
//        buttonPanel->Controls->Add(m_btnOk);
//        buttonPanel->Controls->Add(m_btnCancel);
//
//        mainPanel->Controls->Add(lblTitle, 0, 0);
//        mainPanel->Controls->Add(m_dataGridView, 0, 1);
//        mainPanel->Controls->Add(buttonPanel, 0, 2);
//
//        this->Controls->Add(mainPanel);
//    }
//
//    void ColumnTypeForm::OnShown(EventArgs^ e)
//    {
//        __super::OnShown(e);
//        LoadSampleData();
//        
//        // После загрузки данных обновляем все строки
//        if (m_sampleData && !m_sampleData->empty() && m_dataGridView->Rows->Count > 0)
//        {
//            for (int i = 0; i < m_dataGridView->Rows->Count && i < m_headers->Count; i++)
//            {
//                if (i < (int)(*m_sampleData)[0].size())
//                {
//                    m_dataGridView->Rows[i]->Cells[2]->Value = GetOriginalValue(0, i);
//                    
//                    String^ typeStr = safe_cast<String^>(m_dataGridView->Rows[i]->Cells[1]->Value);
//                    DataType type = DataType::Text;
//                    
//                    if (typeStr == L"Integer")
//                        type = DataType::Integer;
//                    else if (typeStr == L"UInteger")
//                        type = DataType::UInteger;
//                    else if (typeStr == L"Double")
//                        type = DataType::Double;
//                    else if (typeStr == L"Date")
//                        type = DataType::Date;
//                    
//                    m_dataGridView->Rows[i]->Cells[3]->Value = FormatValue((*m_sampleData)[0][i], type);
//                }
//            }
//        }
//    }
//
//    void ColumnTypeForm::SetHeaders(List<String^>^ headers)
//    {
//        if (!headers) return;
//        
//        m_headers->Clear();
//        for each (String^ h in headers)
//        {
//            m_headers->Add(h);
//        }
//        
//        m_dataGridView->Rows->Clear();
//        
//        for (int i = 0; i < m_headers->Count; i++)
//        {
//            DataGridViewRow^ row = gcnew DataGridViewRow();
//            row->CreateCells(m_dataGridView);
//            row->Cells[0]->Value = m_headers[i];
//            row->Cells[1]->Value = L"Text";
//            row->Cells[2]->Value = L"(waiting for data)";
//            row->Cells[3]->Value = L"(waiting for data)";
//            m_dataGridView->Rows->Add(row);
//        }
//    }
//
//    void ColumnTypeForm::SetConfig(TextImportConfig* config)
//    {
//        m_config = config;
//    }
//
//    void ColumnTypeForm::GetConfig(TextImportConfig* config)
//    {
//        if (!config) return;
//        
//        config->columnTypes.clear();
//        
//        for (int i = 0; i < m_dataGridView->Rows->Count; i++)
//        {
//            String^ typeStr = safe_cast<String^>(m_dataGridView->Rows[i]->Cells[1]->Value);
//            
//            if (typeStr == L"Integer")
//                config->columnTypes.push_back(DataType::Integer);
//            else if (typeStr == L"UInteger")
//                config->columnTypes.push_back(DataType::UInteger);
//            else if (typeStr == L"Double")
//                config->columnTypes.push_back(DataType::Double);
//            else if (typeStr == L"Date")
//                config->columnTypes.push_back(DataType::Date);
//            else
//                config->columnTypes.push_back(DataType::Text);
//        }
//    }
//
//    void ColumnTypeForm::SetFilePath(String^ path)
//    {
//        m_filePath = path;
//    }
//
//    void ColumnTypeForm::SetDelimiter(char delimiter)
//    {
//        m_delimiter = delimiter;
//    }
//
//    void ColumnTypeForm::SetSkipLines(int skipLines)
//    {
//        m_skipLines = skipLines;
//    }
//
//    void ColumnTypeForm::SetHasHeader(bool hasHeader)
//    {
//        m_hasHeader = hasHeader;
//    }
//
//    void ColumnTypeForm::OnOkClick(Object^ sender, EventArgs^ e)
//    {
//        this->DialogResult = System::Windows::Forms::DialogResult::OK;
//        this->Close();
//    }
//
//    void ColumnTypeForm::OnCancelClick(Object^ sender, EventArgs^ e)
//    {
//        this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
//        this->Close();
//    }
//
//    void ColumnTypeForm::OnDataTypeChanged(Object^ sender, DataGridViewCellEventArgs^ e)
//    {
//        if (e->RowIndex >= 0 && e->ColumnIndex == 1)
//        {
//            String^ typeStr = safe_cast<String^>(m_dataGridView->Rows[e->RowIndex]->Cells[1]->Value);
//            DataType type = DataType::Text;
//            
//            if (typeStr == L"Integer")
//                type = DataType::Integer;
//            else if (typeStr == L"UInteger")
//                type = DataType::UInteger;
//            else if (typeStr == L"Double")
//                type = DataType::Double;
//            else if (typeStr == L"Date")
//                type = DataType::Date;
//            
//            if (m_sampleData && !m_sampleData->empty() && e->RowIndex < (int)(*m_sampleData)[0].size())
//            {
//                m_dataGridView->Rows[e->RowIndex]->Cells[3]->Value = FormatValue((*m_sampleData)[0][e->RowIndex], type);
//            }
//        }
//    }
//
//    void ColumnTypeForm::LoadSampleData()
//    {
//        if (!m_sampleData) return;
//        
//        m_sampleData->clear();
//        
//        if (String::IsNullOrEmpty(m_filePath))
//            return;
//        
//        msclr::interop::marshal_context context;
//        std::string path = context.marshal_as<std::string>(m_filePath);
//        
//        std::ifstream file(path);
//        if (!file.is_open())
//            return;
//        
//        std::string line;
//        
//        for (int i = 0; i < m_skipLines && std::getline(file, line); i++) {}
//        
//        if (m_hasHeader)
//        {
//            std::getline(file, line);
//        }
//        
//        int rowCount = 0;
//        while (std::getline(file, line) && rowCount < 2)
//        {
//            std::vector<std::string> fields;
//            std::stringstream ss(line);
//            std::string field;
//            while (std::getline(ss, field, m_delimiter))
//            {
//                fields.push_back(field);
//            }
//            m_sampleData->push_back(fields);
//            rowCount++;
//        }
//        
//        file.close();
//    }
//
//    void ColumnTypeForm::UpdatePreviewRow(int rowIndex)
//    {
//        // Не используется, оставлено для совместимости
//    }
//
//    String^ ColumnTypeForm::FormatValue(const std::string& value, DataType type)
//    {
//        std::string trimmed = value;
//        size_t start = trimmed.find_first_not_of(" \t\r\n");
//        size_t end = trimmed.find_last_not_of(" \t\r\n");
//        if (start == std::string::npos) return L"(empty)";
//        trimmed = trimmed.substr(start, end - start + 1);
//        
//        try
//        {
//            switch (type)
//            {
//            case DataType::Integer:
//                {
//                    long long val = std::stoll(trimmed);
//                    return gcnew String(std::to_string(val).c_str());
//                }
//            case DataType::UInteger:
//                {
//                    unsigned long long val = std::stoull(trimmed);
//                    return gcnew String(std::to_string(val).c_str());
//                }
//            case DataType::Double:
//                {
//                    double val = std::stod(trimmed);
//                    char buf[64];
//                    sprintf_s(buf, "%.2f", val);
//                    return gcnew String(buf);
//                }
//            case DataType::Date:
//                return gcnew String(trimmed.c_str());
//            default:
//                return gcnew String(trimmed.c_str());
//            }
//        }
//        catch (...)
//        {
//            return L"(invalid)";
//        }
//    }
//
//    String^ ColumnTypeForm::GetOriginalValue(int row, int col)
//    {
//        if (!m_sampleData || m_sampleData->empty()) return L"";
//        if (row >= (int)m_sampleData->size()) return L"";
//        if (col >= (int)(*m_sampleData)[row].size()) return L"";
//        
//        return gcnew String((*m_sampleData)[row][col].c_str());
//    }
//
//}