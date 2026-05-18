// TextLoaderConfigForm.cpp
#include "TextLoaderConfigForm.h"
#include <fstream>
#include <sstream>

namespace DataEngine {

    TextLoaderConfigForm::TextLoaderConfigForm()
        : m_selectedFile(nullptr)
        , m_config(nullptr)
        , m_table(nullptr)
        , m_loader(nullptr)
    {
        InitializeComponent();
        m_config = new TextImportConfig();
    }

    TextLoaderConfigForm::~TextLoaderConfigForm()
    {
        if (m_config) delete m_config;
    }

    void TextLoaderConfigForm::InitializeComponent()
    {
        this->Text = L"Text Loader Configuration";
        this->Size = Drawing::Size(550, 500);
        this->StartPosition = FormStartPosition::CenterParent;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;

        TableLayoutPanel^ mainPanel = gcnew TableLayoutPanel();
        mainPanel->Dock = DockStyle::Fill;
        mainPanel->RowCount = 6;
        mainPanel->ColumnCount = 1;
        mainPanel->Padding = System::Windows::Forms::Padding(10);
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 40));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 40));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 40));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 30));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 100));
        mainPanel->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 50));

        // Delimiter
        Panel^ delimPanel = gcnew Panel();
        delimPanel->Dock = DockStyle::Fill;
        delimPanel->Height = 40;
        
        Label^ lblDelimiter = gcnew Label();
        lblDelimiter->Text = L"Delimiter:";
        lblDelimiter->Location = Point(0, 8);
        lblDelimiter->Size = Drawing::Size(100, 25);
        lblDelimiter->TextAlign = ContentAlignment::MiddleRight;
        
        m_txtDelimiter = gcnew TextBox();
        m_txtDelimiter->Text = L",";
        m_txtDelimiter->MaxLength = 1;
        m_txtDelimiter->Location = Point(110, 5);
        m_txtDelimiter->Size = Drawing::Size(60, 25);
        m_txtDelimiter->Font = gcnew Drawing::Font(L"Consolas", 10, FontStyle::Bold);
        m_txtDelimiter->TextAlign = HorizontalAlignment::Center;
        m_txtDelimiter->TextChanged += gcnew EventHandler(this, &TextLoaderConfigForm::OnDelimiterTextChanged);
        
        Label^ lblDelimiterHint = gcnew Label();
        lblDelimiterHint->Text = L"(one character, e.g. , ; | \\t)";
        lblDelimiterHint->Location = Point(180, 8);
        lblDelimiterHint->Size = Drawing::Size(200, 25);
        lblDelimiterHint->ForeColor = Drawing::Color::Gray;
        lblDelimiterHint->Font = gcnew Drawing::Font(L"Segoe UI", 8);
        
        delimPanel->Controls->Add(lblDelimiter);
        delimPanel->Controls->Add(m_txtDelimiter);
        delimPanel->Controls->Add(lblDelimiterHint);

        // Header
        Panel^ headerPanel = gcnew Panel();
        headerPanel->Dock = DockStyle::Fill;
        headerPanel->Height = 40;
        
        Label^ lblHeader = gcnew Label();
        lblHeader->Text = L"Has Header Row:";
        lblHeader->Location = Point(0, 8);
        lblHeader->Size = Drawing::Size(100, 25);
        lblHeader->TextAlign = ContentAlignment::MiddleRight;
        
        m_chkHeader = gcnew CheckBox();
        m_chkHeader->Text = L"Yes, first row contains column names";
        m_chkHeader->Checked = true;
        m_chkHeader->Location = Point(110, 8);
        m_chkHeader->Size = Drawing::Size(250, 25);
        m_chkHeader->CheckStateChanged += gcnew EventHandler(this, &TextLoaderConfigForm::OnDelimiterTextChanged);
        
        headerPanel->Controls->Add(lblHeader);
        headerPanel->Controls->Add(m_chkHeader);

        // Skip Lines
        Panel^ skipPanel = gcnew Panel();
        skipPanel->Dock = DockStyle::Fill;
        skipPanel->Height = 40;
        
        Label^ lblSkipLines = gcnew Label();
        lblSkipLines->Text = L"Skip Lines:";
        lblSkipLines->Location = Point(0, 8);
        lblSkipLines->Size = Drawing::Size(100, 25);
        lblSkipLines->TextAlign = ContentAlignment::MiddleRight;
        
        m_numSkipLines = gcnew NumericUpDown();
        m_numSkipLines->Minimum = 0;
        m_numSkipLines->Maximum = 100;
        m_numSkipLines->Value = 0;
        m_numSkipLines->Location = Point(110, 5);
        m_numSkipLines->Size = Drawing::Size(60, 25);
        m_numSkipLines->ValueChanged += gcnew EventHandler(this, &TextLoaderConfigForm::OnDelimiterTextChanged);
        
        Label^ lblSkipLinesHint = gcnew Label();
        lblSkipLinesHint->Text = L"(skip first N lines before reading)";
        lblSkipLinesHint->Location = Point(180, 8);
        lblSkipLinesHint->Size = Drawing::Size(200, 25);
        lblSkipLinesHint->ForeColor = Drawing::Color::Gray;
        lblSkipLinesHint->Font = gcnew Drawing::Font(L"Segoe UI", 8);
        
        skipPanel->Controls->Add(lblSkipLines);
        skipPanel->Controls->Add(m_numSkipLines);
        skipPanel->Controls->Add(lblSkipLinesHint);

        // Preview title
        Label^ lblPreviewTitle = gcnew Label();
        lblPreviewTitle->Text = L"Preview (first 10 lines):";
        lblPreviewTitle->TextAlign = ContentAlignment::MiddleLeft;
        lblPreviewTitle->Dock = DockStyle::Fill;
        lblPreviewTitle->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);

        // Preview content
        m_lblPreview = gcnew Label();
        m_lblPreview->Text = L"No file selected";
        m_lblPreview->TextAlign = ContentAlignment::TopLeft;
        m_lblPreview->Dock = DockStyle::Fill;
        m_lblPreview->BackColor = SystemColors::Info;
        m_lblPreview->BorderStyle = BorderStyle::FixedSingle;
        m_lblPreview->Font = gcnew Drawing::Font(L"Consolas", 9);
        m_lblPreview->Padding = System::Windows::Forms::Padding(5);

        // Buttons
        Panel^ buttonPanel = gcnew Panel();
        buttonPanel->Dock = DockStyle::Fill;
        buttonPanel->Height = 50;

        m_btnOk = gcnew Button();
        m_btnOk->Text = L"Apply & Continue";
        m_btnOk->Size = Drawing::Size(120, 32);
        m_btnOk->Location = Point(230, 9);
        m_btnOk->BackColor = SystemColors::Control;
        m_btnOk->UseVisualStyleBackColor = true;
        m_btnOk->Click += gcnew EventHandler(this, &TextLoaderConfigForm::OnOkClick);

        m_btnCancel = gcnew Button();
        m_btnCancel->Text = L"Cancel";
        m_btnCancel->Size = Drawing::Size(100, 32);
        m_btnCancel->Location = Point(360, 9);
        m_btnCancel->BackColor = SystemColors::Control;
        m_btnCancel->UseVisualStyleBackColor = true;
        m_btnCancel->Click += gcnew EventHandler(this, &TextLoaderConfigForm::OnCancelClick);

        buttonPanel->Controls->Add(m_btnOk);
        buttonPanel->Controls->Add(m_btnCancel);

        mainPanel->Controls->Add(delimPanel, 0, 0);
        mainPanel->Controls->Add(headerPanel, 0, 1);
        mainPanel->Controls->Add(skipPanel, 0, 2);
        mainPanel->Controls->Add(lblPreviewTitle, 0, 3);
        mainPanel->Controls->Add(m_lblPreview, 0, 4);
        mainPanel->Controls->Add(buttonPanel, 0, 5);

        this->Controls->Add(mainPanel);
    }

    void TextLoaderConfigForm::SetTable(Table* table)
    {
        m_table = table;
    }

    void TextLoaderConfigForm::SetLoader(TextLoader* loader)
    {
        m_loader = loader;
    }

    void TextLoaderConfigForm::SetConfig(const TextImportConfig& config)
    {
        if (!m_config) return;
        *m_config = config;

        // Исправленная строка: сначала символ, потом количество
        m_txtDelimiter->Text = gcnew String(config.delimiter, 1);
        m_chkHeader->Checked = config.hasHeader;
        m_numSkipLines->Value = config.skipLines;
    }

    void TextLoaderConfigForm::GetConfig(TextImportConfig& config)
    {
        if (!m_config) return;
        config = *m_config;
    }

    void TextLoaderConfigForm::SetFilePath(String^ path)
    {
        m_selectedFile = path;
        UpdatePreview();
    }

    String^ TextLoaderConfigForm::GetFilePath()
    {
        return m_selectedFile;
    }

    System::Windows::Forms::DialogResult TextLoaderConfigForm::ShowConfigDialog()
    {
        return this->ShowDialog();
    }

    void TextLoaderConfigForm::OnOkClick(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(m_selectedFile))
        {
            ShowError(L"No file selected");
            return;
        }

        String^ delimStr = m_txtDelimiter->Text;
        if (String::IsNullOrEmpty(delimStr))
        {
            m_txtDelimiter->Text = L",";
            delimStr = L",";
        }

        ApplySettings();
        ApplyToTableAndLoader();
        
        this->DialogResult = System::Windows::Forms::DialogResult::OK;
        this->Close();
    }

    void TextLoaderConfigForm::OnCancelClick(Object^ sender, EventArgs^ e)
    {
        this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
        this->Close();
    }

    void TextLoaderConfigForm::OnDelimiterTextChanged(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(m_txtDelimiter->Text))
        {
            return;
        }
        UpdatePreview();
    }

    void TextLoaderConfigForm::ApplySettings()
    {
        if (!m_config) return;

        String^ delimStr = m_txtDelimiter->Text;
        if (!String::IsNullOrEmpty(delimStr))
        {
            m_config->delimiter = delimStr[0];
        }
        else
        {
            m_config->delimiter = ',';
        }

        m_config->hasHeader = m_chkHeader->Checked;
        m_config->skipLines = (int)m_numSkipLines->Value;
        
        msclr::interop::marshal_context context;
        m_config->source = context.marshal_as<std::string>(m_selectedFile);
    }

    void TextLoaderConfigForm::ApplyToTableAndLoader()
    {
        if (!m_table || !m_loader) return;

        // Применяем конфигурацию к загрузчику
        m_loader->SetConfig(*m_config);

        // Создаем временный загрузчик для чтения первой строки данных
        MF::WFile* file = new MF::WFile();
        if (!file->open(m_config->source.c_str(), false))
        {
            delete file;
            return;
        }

        TextLoader tempLoader(*m_config);
        tempLoader.SetFlow(file);  // Не передаём владение

        if (tempLoader.BuildIndex())
        {
            tempLoader.Reset();

            List<String^>^ headers = ParseHeaders();

            DataObject firstRow;
            if (tempLoader.HasNext())
            {
                firstRow = tempLoader.ReadNext();
            }

            std::vector<ColumnDefinition> columns;
            for (int i = 0; i < headers->Count; i++)
            {
                msclr::interop::marshal_context context;
                std::string name = context.marshal_as<std::string>(headers[i]);

                Data defaultValue;
                if (i < (int)firstRow.Size() && !firstRow.Get(i).IsNull())
                {
                    defaultValue = firstRow.Get(i);
                }
                else
                {
                    defaultValue = Data(std::string(""));
                }

                columns.push_back(ColumnDefinition(name, DataType::Text, defaultValue));
            }

            HeaderDefinition header;
            header.SetColumns(columns);
            m_table->SetHeader(header);
        }

        // Удаляем файл, но tempLoader не владеет им и не будет удалять повторно
        delete file;
    }

    void TextLoaderConfigForm::UpdatePreview()
    {
        if (String::IsNullOrEmpty(m_selectedFile))
        {
            m_lblPreview->Text = L"No file selected";
            return;
        }

        msclr::interop::marshal_context context;
        std::string path = context.marshal_as<std::string>(m_selectedFile);

        std::ifstream file(path);
        if (!file.is_open())
        {
            m_lblPreview->Text = L"Cannot preview file: " + m_selectedFile;
            return;
        }

        String^ delimStr = m_txtDelimiter->Text;
        char delimiter = (String::IsNullOrEmpty(delimStr)) ? ',' : delimStr[0];

        std::string preview;
        int lineCount = 0;
        std::string line;
        int skipLines = (int)m_numSkipLines->Value;

        for (int i = 0; i < skipLines && std::getline(file, line); i++) {}

        while (std::getline(file, line) && lineCount < 10)
        {
            std::vector<std::string> fields;
            std::stringstream ss(line);
            std::string field;
            while (std::getline(ss, field, delimiter))
            {
                if (field.length() > 30)
                    field = field.substr(0, 27) + "...";
                fields.push_back(field);
            }

            for (size_t i = 0; i < fields.size(); i++)
            {
                if (i > 0) preview += " | ";
                preview += fields[i];
            }
            preview += "\n";
            lineCount++;
        }

        file.close();

        String^ previewText = gcnew String(preview.c_str());
        m_lblPreview->Text = L"File: " + Path::GetFileName(m_selectedFile) + L"\n\n" + previewText;
        
        if (lineCount == 0)
            m_lblPreview->Text += L"\n(No data after skipping lines)";
    }

    List<String^>^ TextLoaderConfigForm::ParseHeaders()
    {
        List<String^>^ headers = gcnew List<String^>();
        
        if (String::IsNullOrEmpty(m_selectedFile))
            return headers;

        msclr::interop::marshal_context context;
        std::string path = context.marshal_as<std::string>(m_selectedFile);

        std::ifstream file(path);
        if (!file.is_open())
            return headers;

        int skipLines = (int)m_numSkipLines->Value;
        std::string line;
        
        for (int i = 0; i < skipLines && std::getline(file, line); i++) {}

        if (m_chkHeader->Checked && std::getline(file, line))
        {
            String^ delimStr = m_txtDelimiter->Text;
            char delimiter = (String::IsNullOrEmpty(delimStr)) ? ',' : delimStr[0];
            
            std::vector<std::string> fields;
            std::stringstream ss(line);
            std::string field;
            while (std::getline(ss, field, delimiter))
            {
                headers->Add(gcnew String(field.c_str()));
            }
        }
        else
        {
            if (std::getline(file, line))
            {
                String^ delimStr = m_txtDelimiter->Text;
                char delimiter = (String::IsNullOrEmpty(delimStr)) ? ',' : delimStr[0];
                
                std::vector<std::string> fields;
                std::stringstream ss(line);
                std::string field;
                while (std::getline(ss, field, delimiter))
                {
                    headers->Add(L"Column_" + headers->Count);
                }
            }
        }

        file.close();
        return headers;
    }

    void TextLoaderConfigForm::ShowError(String^ message)
    {
        MessageBox::Show(message, L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }

}