//// TestForm.cpp
//#include "TestForm.h"
//
//namespace DataEngine {
//
//    TestForm::TestForm()
//        : m_table(nullptr)
//        , m_loader(nullptr)
//        , m_config(nullptr)
//        , m_headers(gcnew List<String^>())
//        , m_currentFilePath(nullptr)
//    {
//        InitializeComponent();
//        m_config = new TextImportConfig();
//        m_loaderConfigForm = gcnew TextLoaderConfigForm();
//        m_tableConfigForm = gcnew TableConfigForm();
//    }
//
//    TestForm::~TestForm()
//    {
//        if (m_table) delete m_table;
//        if (m_loader) delete m_loader;
//        if (m_config) delete m_config;
//    }
//
//    void TestForm::InitializeComponent()
//    {
//        this->Text = L"DataEngine Test Form - Data Viewer";
//        this->Size = Drawing::Size(1100, 750);
//        this->StartPosition = FormStartPosition::CenterScreen;
//        this->MinimumSize = Drawing::Size(800, 500);
//
//        m_openFileDialog = gcnew OpenFileDialog();
//        m_openFileDialog->Title = L"Select text file";
//        m_openFileDialog->Filter = L"Text/CSV files (*.txt;*.csv)|*.txt;*.csv|All files (*.*)|*.*";
//
//        TableLayoutPanel^ mainPanel = gcnew TableLayoutPanel();
//        mainPanel->Dock = DockStyle::Fill;
//        mainPanel->RowCount = 2;
//        mainPanel->ColumnCount = 1;
//        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 50));
//        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 100));
//
//        Panel^ controlPanel = gcnew Panel();
//        controlPanel->Dock = DockStyle::Fill;
//
//        m_btnOpen = gcnew Button();
//        m_btnOpen->Text = L"Open File";
//        m_btnOpen->Location = Point(10, 10);
//        m_btnOpen->Size = Drawing::Size(100, 30);
//        m_btnOpen->Click += gcnew EventHandler(this, &TestForm::OnOpenFile);
//
//        controlPanel->Controls->Add(m_btnOpen);
//
//        m_dataGridView = gcnew DataGridView();
//        m_dataGridView->Dock = DockStyle::Fill;
//        m_dataGridView->AllowUserToAddRows = false;
//        m_dataGridView->AllowUserToDeleteRows = false;
//        m_dataGridView->ReadOnly = true;
//        m_dataGridView->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
//        m_dataGridView->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::AllCells;
//        m_dataGridView->VirtualMode = false;
//
//        m_statusStrip = gcnew StatusStrip();
//        m_statusLabel = gcnew ToolStripStatusLabel();
//        m_statusLabel->Text = L"Click 'Open File' to start";
//        m_statusStrip->Items->Add(m_statusLabel);
//
//        mainPanel->Controls->Add(controlPanel, 0, 0);
//        mainPanel->Controls->Add(m_dataGridView, 0, 1);
//
//        this->Controls->Add(mainPanel);
//        this->Controls->Add(m_statusStrip);
//    }
//
//    void TestForm::OnOpenFile(Object^ sender, EventArgs^ e)
//    {
//        if (m_openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
//        {
//            m_currentFilePath = m_openFileDialog->FileName;
//            m_statusLabel->Text = L"Loading configuration for: " + Path::GetFileName(m_currentFilePath);
//            
//            LoadDataStructure();
//        }
//    }
//
//    void TestForm::LoadDataStructure()
//    {
//        if (m_table) delete m_table;
//        if (m_loader) delete m_loader;
//        
//        m_table = new Table();
//        m_loader = new TextLoader();
//        
//        m_loaderConfigForm->SetFilePath(m_currentFilePath);
//        m_loaderConfigForm->SetTable(m_table);
//        m_loaderConfigForm->SetLoader(m_loader);
//        
//        TextImportConfig defaultConfig;
//        if (m_currentFilePath->EndsWith(L".csv"))
//            defaultConfig.delimiter = ',';
//        else if (m_currentFilePath->EndsWith(L".tsv"))
//            defaultConfig.delimiter = '\t';
//        else
//            defaultConfig.delimiter = ',';
//        defaultConfig.hasHeader = true;
//        defaultConfig.skipLines = 0;
//        m_loaderConfigForm->SetConfig(defaultConfig);
//        
//        if (m_loaderConfigForm->ShowDialog() == System::Windows::Forms::DialogResult::OK)
//        {
//            m_statusLabel->Text = L"Loader configured. Opening table configuration...";
//            
//            m_tableConfigForm->SetTable(m_table);
//            
//            if (m_tableConfigForm->ShowDialog() == System::Windows::Forms::DialogResult::OK)
//            {
//                m_statusLabel->Text = L"Table configured. Loading data...";
//                LoadData();
//            }
//            else
//            {
//                m_statusLabel->Text = L"Table configuration cancelled.";
//            }
//        }
//        else
//        {
//            m_statusLabel->Text = L"Loader configuration cancelled.";
//        }
//    }
//
//    void TestForm::LoadData()
//    {
//        if (!m_config || String::IsNullOrEmpty(m_currentFilePath))
//        {
//            ShowError(L"Configuration error");
//            return;
//        }
//
//        msclr::interop::marshal_context context;
//        const char* path = context.marshal_as<const char*>(m_currentFilePath);
//
//        MF::WFile* file = new MF::WFile();
//        if (!file->open(path, false))
//        {
//            ShowError(L"Failed to open file");
//            delete file;
//            return;
//        }
//
//        if (m_loader) delete m_loader;
//        m_loader = new TextLoader(*m_config);
//        m_loader->SetFlow(file);
//        
//        if (!m_loader->BuildIndex())
//        {
//            ShowError(L"Failed to build index. Check configuration.");
//            delete file;
//            return;
//        }
//
//        if (!m_table)
//        {
//            m_table = new Table();
//        }
//
//        m_table->SetLoader(std::unique_ptr<Loader>(m_loader));
//        m_loader = nullptr;
//        m_table->Load();
//
//        m_headers->Clear();
//        const HeaderDefinition& header = m_table->GetHeader();
//        const std::vector<ColumnDefinition>& columns = header.GetColumns();
//        
//        for (size_t i = 0; i < columns.size(); i++)
//        {
//            m_headers->Add(gcnew String(columns[i].GetName().c_str()));
//        }
//
//        RefreshDataGridView();
//        UpdateStatus();
//    }
//
//    void TestForm::RefreshDataGridView()
//    {
//        if (!m_table) return;
//
//        m_dataGridView->Rows->Clear();
//        m_dataGridView->Columns->Clear();
//
//        DataGridViewTextBoxColumn^ indexColumn = gcnew DataGridViewTextBoxColumn();
//        indexColumn->Name = L"#";
//        indexColumn->HeaderText = L"#";
//        indexColumn->Width = 50;
//        indexColumn->ReadOnly = true;
//        m_dataGridView->Columns->Add(indexColumn);
//
//        for (int i = 0; i < m_headers->Count; i++)
//        {
//            DataGridViewTextBoxColumn^ col = gcnew DataGridViewTextBoxColumn();
//            col->Name = m_headers[i];
//            col->HeaderText = m_headers[i];
//            col->Width = 120;
//            col->ReadOnly = true;
//            m_dataGridView->Columns->Add(col);
//        }
//
//        for (size_t row = 0; row < m_table->Size(); row++)
//        {
//            DataObject obj = m_table->Get(row);
//            
//            DataGridViewRow^ gridRow = gcnew DataGridViewRow();
//            gridRow->CreateCells(m_dataGridView);
//            
//            gridRow->Cells[0]->Value = (row + 1).ToString();
//            
//            for (size_t col = 0; col < obj.Size() && col < (size_t)m_headers->Count; col++)
//            {
//                String^ value = gcnew String(obj.Get(col).ToString().c_str());
//                gridRow->Cells[col + 1]->Value = value;
//            }
//            
//            m_dataGridView->Rows->Add(gridRow);
//        }
//
//        if (m_dataGridView->Rows->Count > 0)
//            m_dataGridView->Rows[0]->Selected = true;
//    }
//
//    void TestForm::UpdateStatus()
//    {
//        if (!m_table)
//        {
//            m_statusLabel->Text = L"No data loaded";
//            return;
//        }
//
//        char buffer[256];
//        sprintf_s(buffer, "Total objects: %zu | Columns: %d",
//            m_table->Size(),
//            m_headers->Count);
//        
//        m_statusLabel->Text = gcnew String(buffer);
//    }
//
//    void TestForm::ShowError(String^ message)
//    {
//        MessageBox::Show(message, L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
//    }
//
//}