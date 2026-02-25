#pragma once
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>
#include <Windows.h>


namespace CSVAnalyzer {

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

    public ref struct Book
    {
        String^ Name;
        String^ Author;
        double UserRating;
        int Reviews;
        double Price;
        int Year;
        String^ Genre;
    };

    public ref struct ChartClickRegion
    {
        String^ Type;
        String^ Value;
        System::Drawing::Rectangle Bounds;
        Object^ Data;
        bool IsHighlighted;
    };

    ref class AuthorComparer
    {
    public:
        static int Compare(KeyValuePair<String^, int> a, KeyValuePair<String^, int> b)
        {
            return b.Value.CompareTo(a.Value);
        }
    };

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
            this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
            this->comboBoxGenre->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBoxGenre_SelectedIndexChanged);
            this->comboBoxMetric->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBoxMetric_SelectedIndexChanged);

            this->dataGridViewAllBooks->KeyDown += gcnew KeyEventHandler(this, &MyForm::dataGridViewAllBooks_KeyDown);
            this->dataGridViewAllBooks->MouseClick += gcnew MouseEventHandler(this, &MyForm::dataGridViewAllBooks_MouseClick);
            this->dataGridViewAllBooks->SelectionChanged += gcnew EventHandler(this, &MyForm::dataGridViewAllBooks_SelectionChanged);

            this->pictureBoxChart1->MouseClick += gcnew MouseEventHandler(this, &MyForm::pictureBoxChart_MouseClick);
            this->pictureBoxChart2->MouseClick += gcnew MouseEventHandler(this, &MyForm::pictureBoxChart_MouseClick);
            this->pictureBoxChart3->MouseClick += gcnew MouseEventHandler(this, &MyForm::pictureBoxChart_MouseClick);
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::ComboBox^ comboBoxGenre;
        System::Windows::Forms::ComboBox^ comboBoxMetric;
        System::Windows::Forms::Label^ labelMetric;
        System::Windows::Forms::Label^ labelGenre;
        System::Windows::Forms::Button^ buttonAnalyze;
        System::Windows::Forms::Button^ buttonLoadFile;
        System::Windows::Forms::Button^ buttonClearFilter;
        System::Windows::Forms::DataGridView^ dataGridViewAllBooks;
        System::Windows::Forms::PictureBox^ pictureBoxChart1;
        System::Windows::Forms::PictureBox^ pictureBoxChart2;
        System::Windows::Forms::PictureBox^ pictureBoxChart3;
        System::Windows::Forms::StatusStrip^ statusStrip;
        System::Windows::Forms::ToolStripStatusLabel^ statusLabel;
        System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel;
        System::Windows::Forms::SplitContainer^ splitContainer;
        System::Windows::Forms::Panel^ panelControls;
        System::Windows::Forms::Panel^ panelChart3Controls;
        System::Windows::Forms::ContextMenuStrip^ contextMenu;
        System::Windows::Forms::ToolStripMenuItem^ copyMenuItem;
        System::ComponentModel::Container^ components;

        List<Book^>^ books;                    // Все книги из CSV
        List<Book^>^ currentDisplayBooks;      // Текущие отображаемые книги (отсортированы по рейтингу)
        List<Book^>^ baseFilteredBooks;        // Книги, отфильтрованные по жанру (без сортировки)
        Dictionary<int, int>^ bookIndexMap;    // Маппинг: позиция в таблице -> индекс в baseFilteredBooks

        String^ currentFileName;
        String^ currentSelectedGenre;
        String^ currentSelectedMetric;
        bool isDataLoaded;
        bool isResizing;
        bool isProgrammaticSelection;

        List<ChartClickRegion^>^ chart1Regions;
        List<ChartClickRegion^>^ chart2Regions;
        List<ChartClickRegion^>^ chart3Regions;
        String^ activeFilter;
        String^ activeFilterValue;
        int activeChart;

        List<String^>^ highlightedAuthors;
        List<int>^ highlightedYears;
        List<String^>^ highlightedPriceRanges;
        List<String^>^ highlightedRatingRanges;

        Book^ selectedBook;
        int selectedBookIndex;  // Индекс выбранной книги в currentDisplayBooks

        static int CompareBooksByRating(Book^ a, Book^ b)
        {
            if (a->UserRating > b->UserRating) return -1;
            if (a->UserRating < b->UserRating) return 1;
            return 0;
        }

        void InitializeComponent(void)
        {
            this->comboBoxGenre = gcnew System::Windows::Forms::ComboBox();
            this->comboBoxMetric = gcnew System::Windows::Forms::ComboBox();
            this->labelMetric = gcnew System::Windows::Forms::Label();
            this->labelGenre = gcnew System::Windows::Forms::Label();
            this->buttonAnalyze = gcnew System::Windows::Forms::Button();
            this->buttonLoadFile = gcnew System::Windows::Forms::Button();
            this->buttonClearFilter = gcnew System::Windows::Forms::Button();
            this->dataGridViewAllBooks = gcnew System::Windows::Forms::DataGridView();
            this->pictureBoxChart1 = gcnew System::Windows::Forms::PictureBox();
            this->pictureBoxChart2 = gcnew System::Windows::Forms::PictureBox();
            this->pictureBoxChart3 = gcnew System::Windows::Forms::PictureBox();
            this->statusStrip = gcnew System::Windows::Forms::StatusStrip();
            this->statusLabel = gcnew System::Windows::Forms::ToolStripStatusLabel();
            this->tableLayoutPanel = gcnew System::Windows::Forms::TableLayoutPanel();
            this->splitContainer = gcnew System::Windows::Forms::SplitContainer();
            this->panelControls = gcnew System::Windows::Forms::Panel();
            this->panelChart3Controls = gcnew System::Windows::Forms::Panel();
            this->contextMenu = gcnew System::Windows::Forms::ContextMenuStrip();
            this->copyMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem();

            this->books = gcnew List<Book^>();
            this->currentDisplayBooks = gcnew List<Book^>();
            this->baseFilteredBooks = gcnew List<Book^>();
            this->bookIndexMap = gcnew Dictionary<int, int>();
            this->chart1Regions = gcnew List<ChartClickRegion^>();
            this->chart2Regions = gcnew List<ChartClickRegion^>();
            this->chart3Regions = gcnew List<ChartClickRegion^>();
            this->highlightedAuthors = gcnew List<String^>();
            this->highlightedYears = gcnew List<int>();
            this->highlightedPriceRanges = gcnew List<String^>();
            this->highlightedRatingRanges = gcnew List<String^>();

            this->currentFileName = nullptr;
            this->currentSelectedGenre = "Fiction";
            this->currentSelectedMetric = "Author";
            this->isDataLoaded = false;
            this->isResizing = false;
            this->isProgrammaticSelection = false;
            this->activeFilter = "";
            this->activeFilterValue = "";
            this->activeChart = 0;
            this->selectedBook = nullptr;
            this->selectedBookIndex = -1;

            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewAllBooks))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxChart1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxChart2))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxChart3))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer))->BeginInit();

            this->splitContainer->SuspendLayout();
            this->statusStrip->SuspendLayout();
            this->tableLayoutPanel->SuspendLayout();
            this->panelControls->SuspendLayout();
            this->contextMenu->SuspendLayout();
            this->SuspendLayout();

            System::Drawing::Font^ defaultFont = gcnew System::Drawing::Font("Segoe UI", 9, System::Drawing::FontStyle::Regular);

            this->Font = defaultFont;

            this->copyMenuItem->Text = L"Копировать";
            this->copyMenuItem->Font = defaultFont;
            this->copyMenuItem->Click += gcnew System::EventHandler(this, &MyForm::copyMenuItem_Click);
            this->contextMenu->Items->Add(this->copyMenuItem);

            this->panelControls->Controls->Add(this->labelGenre);
            this->panelControls->Controls->Add(this->comboBoxGenre);
            this->panelControls->Controls->Add(this->buttonLoadFile);
            this->panelControls->Controls->Add(this->buttonAnalyze);
            this->panelControls->Controls->Add(this->buttonClearFilter);
            this->panelControls->Dock = System::Windows::Forms::DockStyle::Top;
            this->panelControls->Height = 50;
            this->panelControls->Name = L"panelControls";
            this->panelControls->Font = defaultFont;

            this->labelGenre->Location = System::Drawing::Point(12, 15);
            this->labelGenre->Name = L"labelGenre";
            this->labelGenre->Size = System::Drawing::Size(50, 20);
            this->labelGenre->Text = L"Жанр:";
            this->labelGenre->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
            this->labelGenre->Font = defaultFont;

            this->comboBoxGenre->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            array<String^>^ genres = gcnew array<String^>{ "Художественная", "Нехудожественная" };
            this->comboBoxGenre->Items->AddRange(genres);
            this->comboBoxGenre->Location = System::Drawing::Point(70, 12);
            this->comboBoxGenre->Name = L"comboBoxGenre";
            this->comboBoxGenre->Size = System::Drawing::Size(140, 24);
            this->comboBoxGenre->TabIndex = 0;
            this->comboBoxGenre->SelectedIndex = 0;
            this->comboBoxGenre->Enabled = false;
            this->comboBoxGenre->Font = defaultFont;

            this->buttonLoadFile->Location = System::Drawing::Point(220, 12);
            this->buttonLoadFile->Name = L"buttonLoadFile";
            this->buttonLoadFile->Size = System::Drawing::Size(120, 23);
            this->buttonLoadFile->TabIndex = 1;
            this->buttonLoadFile->Text = L"Загрузить CSV...";
            this->buttonLoadFile->UseVisualStyleBackColor = true;
            this->buttonLoadFile->Click += gcnew System::EventHandler(this, &MyForm::buttonLoadFile_Click);
            this->buttonLoadFile->Font = defaultFont;

            this->buttonAnalyze->Location = System::Drawing::Point(350, 12);
            this->buttonAnalyze->Name = L"buttonAnalyze";
            this->buttonAnalyze->Size = System::Drawing::Size(100, 23);
            this->buttonAnalyze->TabIndex = 2;
            this->buttonAnalyze->Text = L"Анализ";
            this->buttonAnalyze->UseVisualStyleBackColor = true;
            this->buttonAnalyze->Click += gcnew System::EventHandler(this, &MyForm::buttonAnalyze_Click);
            this->buttonAnalyze->Enabled = false;
            this->buttonAnalyze->Font = defaultFont;

            this->buttonClearFilter->Location = System::Drawing::Point(460, 12);
            this->buttonClearFilter->Name = L"buttonClearFilter";
            this->buttonClearFilter->Size = System::Drawing::Size(120, 23);
            this->buttonClearFilter->TabIndex = 4;
            this->buttonClearFilter->Text = L"Сбросить фильтр";
            this->buttonClearFilter->UseVisualStyleBackColor = true;
            this->buttonClearFilter->Click += gcnew System::EventHandler(this, &MyForm::buttonClearFilter_Click);
            this->buttonClearFilter->Enabled = false;
            this->buttonClearFilter->Font = defaultFont;

            this->panelChart3Controls->Controls->Add(this->labelMetric);
            this->panelChart3Controls->Controls->Add(this->comboBoxMetric);
            this->panelChart3Controls->BackColor = System::Drawing::Color::Gainsboro;
            this->panelChart3Controls->Height = 30;
            this->panelChart3Controls->Dock = System::Windows::Forms::DockStyle::Bottom;
            this->panelChart3Controls->Font = defaultFont;

            this->labelMetric->Location = System::Drawing::Point(5, 5);
            this->labelMetric->Name = L"labelMetric";
            this->labelMetric->Size = System::Drawing::Size(80, 20);
            this->labelMetric->Text = L"Показатель:";
            this->labelMetric->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            this->labelMetric->Font = defaultFont;

            this->comboBoxMetric->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            array<String^>^ metrics = gcnew array<String^>{
                "По авторам",
                "По цене",
                "По рейтингу"
            };
            this->comboBoxMetric->Items->AddRange(metrics);
            this->comboBoxMetric->Location = System::Drawing::Point(90, 3);
            this->comboBoxMetric->Name = L"comboBoxMetric";
            this->comboBoxMetric->Size = System::Drawing::Size(120, 24);
            this->comboBoxMetric->TabIndex = 3;
            this->comboBoxMetric->SelectedIndex = 0;
            this->comboBoxMetric->Enabled = false;
            this->comboBoxMetric->Font = defaultFont;

            this->splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer->Location = System::Drawing::Point(0, 50);
            this->splitContainer->Name = L"splitContainer";
            this->splitContainer->Size = System::Drawing::Size(1200, 650);
            this->splitContainer->SplitterDistance = 450;
            this->splitContainer->TabIndex = 4;

            this->splitContainer->Panel1->Controls->Add(this->dataGridViewAllBooks);
            this->dataGridViewAllBooks->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridViewAllBooks->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridViewAllBooks->Name = L"dataGridViewAllBooks";
            this->dataGridViewAllBooks->RowHeadersWidth = 51;
            this->dataGridViewAllBooks->AllowUserToAddRows = false;
            this->dataGridViewAllBooks->AllowUserToDeleteRows = false;
            this->dataGridViewAllBooks->ReadOnly = true;
            this->dataGridViewAllBooks->ContextMenuStrip = this->contextMenu;
            this->dataGridViewAllBooks->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            this->dataGridViewAllBooks->MultiSelect = false;
            this->dataGridViewAllBooks->ClipboardCopyMode = DataGridViewClipboardCopyMode::EnableAlwaysIncludeHeaderText;
            this->dataGridViewAllBooks->Font = defaultFont;
            this->dataGridViewAllBooks->BackgroundColor = System::Drawing::Color::White;
            this->dataGridViewAllBooks->BorderStyle = BorderStyle::FixedSingle;
            this->dataGridViewAllBooks->GridColor = System::Drawing::Color::FromArgb(240, 240, 240);
            this->dataGridViewAllBooks->DefaultCellStyle->SelectionBackColor = System::Drawing::Color::FromArgb(210, 230, 250);
            this->dataGridViewAllBooks->DefaultCellStyle->SelectionForeColor = System::Drawing::Color::Black;

            this->dataGridViewAllBooks->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            this->dataGridViewAllBooks->ColumnCount = 6;

            this->dataGridViewAllBooks->Columns[0]->Name = "№";
            this->dataGridViewAllBooks->Columns[0]->MinimumWidth = 40;
            this->dataGridViewAllBooks->Columns[0]->FillWeight = 5;
            this->dataGridViewAllBooks->Columns[0]->SortMode = DataGridViewColumnSortMode::NotSortable;
            this->dataGridViewAllBooks->Columns[0]->DefaultCellStyle->Alignment = DataGridViewContentAlignment::MiddleRight;

            this->dataGridViewAllBooks->Columns[1]->Name = "Название";
            this->dataGridViewAllBooks->Columns[1]->MinimumWidth = 150;
            this->dataGridViewAllBooks->Columns[1]->FillWeight = 40;
            this->dataGridViewAllBooks->Columns[1]->SortMode = DataGridViewColumnSortMode::NotSortable;

            this->dataGridViewAllBooks->Columns[2]->Name = "Автор";
            this->dataGridViewAllBooks->Columns[2]->MinimumWidth = 120;
            this->dataGridViewAllBooks->Columns[2]->FillWeight = 25;
            this->dataGridViewAllBooks->Columns[2]->SortMode = DataGridViewColumnSortMode::NotSortable;

            this->dataGridViewAllBooks->Columns[3]->Name = "Рейтинг";
            this->dataGridViewAllBooks->Columns[3]->MinimumWidth = 60;
            this->dataGridViewAllBooks->Columns[3]->FillWeight = 10;
            this->dataGridViewAllBooks->Columns[3]->SortMode = DataGridViewColumnSortMode::NotSortable;
            this->dataGridViewAllBooks->Columns[3]->DefaultCellStyle->Alignment = DataGridViewContentAlignment::MiddleRight;

            this->dataGridViewAllBooks->Columns[4]->Name = "Цена, $";
            this->dataGridViewAllBooks->Columns[4]->MinimumWidth = 70;
            this->dataGridViewAllBooks->Columns[4]->FillWeight = 10;
            this->dataGridViewAllBooks->Columns[4]->SortMode = DataGridViewColumnSortMode::NotSortable;
            this->dataGridViewAllBooks->Columns[4]->DefaultCellStyle->Alignment = DataGridViewContentAlignment::MiddleRight;

            this->dataGridViewAllBooks->Columns[5]->Name = "Год";
            this->dataGridViewAllBooks->Columns[5]->MinimumWidth = 50;
            this->dataGridViewAllBooks->Columns[5]->FillWeight = 10;
            this->dataGridViewAllBooks->Columns[5]->SortMode = DataGridViewColumnSortMode::NotSortable;
            this->dataGridViewAllBooks->Columns[5]->DefaultCellStyle->Alignment = DataGridViewContentAlignment::MiddleRight;

            this->dataGridViewAllBooks->AllowUserToResizeColumns = true;

            this->splitContainer->Panel2->Controls->Add(this->tableLayoutPanel);

            this->tableLayoutPanel->ColumnCount = 1;
            this->tableLayoutPanel->RowCount = 3;
            this->tableLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tableLayoutPanel->Name = L"tableLayoutPanel";
            this->tableLayoutPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 33.33f));
            this->tableLayoutPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 33.33f));
            this->tableLayoutPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 33.33f));

            this->pictureBoxChart1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxChart1->Name = L"pictureBoxChart1";
            this->pictureBoxChart1->TabStop = false;
            this->pictureBoxChart1->BackColor = System::Drawing::Color::White;
            this->pictureBoxChart1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pictureBoxChart1->Cursor = Cursors::Hand;
            this->pictureBoxChart1->Tag = 1;

            this->pictureBoxChart2->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxChart2->Name = L"pictureBoxChart2";
            this->pictureBoxChart2->TabStop = false;
            this->pictureBoxChart2->BackColor = System::Drawing::Color::White;
            this->pictureBoxChart2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pictureBoxChart2->Cursor = Cursors::Hand;
            this->pictureBoxChart2->Tag = 2;

            this->pictureBoxChart3->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxChart3->Name = L"pictureBoxChart3";
            this->pictureBoxChart3->TabStop = false;
            this->pictureBoxChart3->BackColor = System::Drawing::Color::White;
            this->pictureBoxChart3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pictureBoxChart3->Cursor = Cursors::Hand;
            this->pictureBoxChart3->Tag = 3;

            this->tableLayoutPanel->Controls->Add(this->pictureBoxChart1, 0, 0);
            this->tableLayoutPanel->Controls->Add(this->pictureBoxChart2, 0, 1);

            System::Windows::Forms::Panel^ panelChart3 = gcnew System::Windows::Forms::Panel();
            panelChart3->Dock = System::Windows::Forms::DockStyle::Fill;
            panelChart3->Controls->Add(this->pictureBoxChart3);
            panelChart3->Controls->Add(this->panelChart3Controls);
            this->tableLayoutPanel->Controls->Add(panelChart3, 0, 2);

            this->statusStrip->Items->AddRange(gcnew cli::array<System::Windows::Forms::ToolStripItem^>(1) { this->statusLabel });
            this->statusStrip->Location = System::Drawing::Point(0, 700);
            this->statusStrip->Name = L"statusStrip";
            this->statusStrip->Size = System::Drawing::Size(1200, 22);
            this->statusStrip->TabIndex = 5;
            this->statusStrip->Text = L"statusStrip";
            this->statusStrip->Font = defaultFont;

            this->statusLabel->Name = L"statusLabel";
            this->statusLabel->Size = System::Drawing::Size(1185, 17);
            this->statusLabel->Text = L"Готов. Нажмите 'Загрузить CSV...' для выбора файла.";
            this->statusLabel->Font = defaultFont;

            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1200, 722);
            this->Controls->Add(this->splitContainer);
            this->Controls->Add(this->panelControls);
            this->Controls->Add(this->statusStrip);
            this->Name = L"MyForm";
            this->Text = L"Анализатор CSV - Книжные бестселлеры Amazon 2009-2019";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->MinimumSize = System::Drawing::Size(800, 600);

            

            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewAllBooks))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxChart1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxChart2))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxChart3))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer))->EndInit();

            this->splitContainer->ResumeLayout();
            this->statusStrip->ResumeLayout();
            this->statusStrip->PerformLayout();
            this->tableLayoutPanel->ResumeLayout();
            this->panelControls->ResumeLayout();
            this->contextMenu->ResumeLayout();
            this->ResumeLayout();
            this->PerformLayout();
        }

        // Обработчик для копирования через контекстное меню
        System::Void copyMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
        {
            CopySelectedCells();
        }

        // Обработчик для копирования через Ctrl+C
        System::Void dataGridViewAllBooks_KeyDown(System::Object^ sender, KeyEventArgs^ e)
        {
            if (e->Control && e->KeyCode == Keys::C)
            {
                CopySelectedCells();
                e->Handled = true;
            }
        }

        // Обработчик для контекстного меню по правому клику
        System::Void dataGridViewAllBooks_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
        {
            if (e->Button == System::Windows::Forms::MouseButtons::Right)
            {
                DataGridView::HitTestInfo^ hit = dataGridViewAllBooks->HitTest(e->X, e->Y);
                if (hit->Type == DataGridViewHitTestType::Cell)
                {
                    int rowIndex = hit->RowIndex;
                    if (rowIndex >= 0 && rowIndex < dataGridViewAllBooks->Rows->Count)
                    {
                        if (dataGridViewAllBooks->Rows[rowIndex]->Selected)
                        {
                            // Если строка уже выделена, снимаем выделение
                            dataGridViewAllBooks->ClearSelection();
                            selectedBook = nullptr;
                            selectedBookIndex = -1;
                            ClearHighlights();
                            RefreshCurrentView();
                        }
                        else
                        {
                            // Выделяем новую строку
                            dataGridViewAllBooks->ClearSelection();
                            dataGridViewAllBooks->Rows[rowIndex]->Selected = true;
                        }
                    }
                }
            }
        }

        // Обработчик изменения выделения в таблице
        System::Void dataGridViewAllBooks_SelectionChanged(System::Object^ sender, EventArgs^ e)
        {
            if (!isDataLoaded || isResizing || isProgrammaticSelection) return;

            // Очищаем все подсветки
            ClearHighlights();
            selectedBook = nullptr;
            selectedBookIndex = -1;

            if (dataGridViewAllBooks->SelectedRows->Count > 0)
            {
                DataGridViewRow^ selectedRow = dataGridViewAllBooks->SelectedRows[0];
                int rowIndex = selectedRow->Index;

                if (rowIndex >= 0 && rowIndex < currentDisplayBooks->Count)
                {
                    selectedBook = currentDisplayBooks[rowIndex];
                    selectedBookIndex = rowIndex;

                    // Подсвечиваем автора
                    if (!highlightedAuthors->Contains(selectedBook->Author))
                        highlightedAuthors->Add(selectedBook->Author);

                    // Подсвечиваем год
                    if (!highlightedYears->Contains(selectedBook->Year))
                        highlightedYears->Add(selectedBook->Year);

                    // Определяем ценовой диапазон
                    String^ priceRange = GetPriceRange(selectedBook->Price);
                    if (!highlightedPriceRanges->Contains(priceRange))
                        highlightedPriceRanges->Add(priceRange);

                    // Определяем диапазон рейтинга
                    String^ ratingRange = GetRatingRange(selectedBook->UserRating);
                    if (!highlightedRatingRanges->Contains(ratingRange))
                        highlightedRatingRanges->Add(ratingRange);

                    statusLabel->Text = "Выбрана книга: " + selectedBook->Name + " (" + selectedBook->Year + ", $" + selectedBook->Price.ToString("F2") + ")";
                }
            }

            // Перерисовываем графики с подсветкой
            if (isDataLoaded)
            {
                List<Book^>^ filtered = FilterBooksByGenre(currentSelectedGenre);
                DrawCharts(filtered);
            }
        }

        // Очистка всех подсветок
        void ClearHighlights()
        {
            highlightedAuthors->Clear();
            highlightedYears->Clear();
            highlightedPriceRanges->Clear();
            highlightedRatingRanges->Clear();
        }

        // Получение ценового диапазона (в долларах для фильтрации)
        String^ GetPriceRange(double price)
        {
            if (price < 10) return "0-10$";
            if (price < 20) return "10-20$";
            if (price < 30) return "20-30$";
            if (price < 40) return "30-40$";
            return "40$+";
        }

        // Получение диапазона рейтинга
        String^ GetRatingRange(double rating)
        {
            if (rating < 1) return "0-1";
            if (rating < 2) return "1-2";
            if (rating < 3) return "2-3";
            if (rating < 4) return "3-4";
            return "4-5";
        }

        // Обработчик клика по графикам
        System::Void pictureBoxChart_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
        {
            PictureBox^ chart = safe_cast<PictureBox^>(sender);
            int chartNumber = safe_cast<int>(chart->Tag);

            List<ChartClickRegion^>^ regions;
            if (chartNumber == 1)
                regions = chart1Regions;
            else if (chartNumber == 2)
                regions = chart2Regions;
            else
                regions = chart3Regions;

            for each(ChartClickRegion ^ region in regions)
            {
                if (region->Bounds.Contains(e->Location))
                {
                    // Сбрасываем выделение книги при применении фильтра
                    if (selectedBook != nullptr)
                    {
                        isProgrammaticSelection = true;
                        dataGridViewAllBooks->ClearSelection();
                        isProgrammaticSelection = false;
                        selectedBook = nullptr;
                        selectedBookIndex = -1;
                        ClearHighlights();
                    }

                    if (activeChart == chartNumber && activeFilter == region->Type && activeFilterValue == region->Value)
                    {
                        ClearFilter();
                    }
                    else
                    {
                        ApplyFilter(chartNumber, region);
                    }
                    break;
                }
            }
        }

        // Применение фильтра
        void ApplyFilter(int chartNumber, ChartClickRegion^ region)
        {
            activeChart = chartNumber;
            activeFilter = region->Type;
            activeFilterValue = region->Value;

            HighlightActiveChart(chartNumber);
            UpdateFilteredView();

            statusLabel->Text = "Фильтр: " + GetFilterDescription();
            buttonClearFilter->Enabled = true;
        }

        // Сброс фильтра
        System::Void buttonClearFilter_Click(System::Object^ sender, System::EventArgs^ e)
        {
            ClearFilter();
        }

        void ClearFilter()
        {
            activeChart = 0;
            activeFilter = "";
            activeFilterValue = "";

            ResetChartHighlights();
            RefreshCurrentView();

            statusLabel->Text = "Фильтр сброшен";
            buttonClearFilter->Enabled = false;
        }

        // Подсветка активного графика
        void HighlightActiveChart(int chartNumber)
        {
            ResetChartHighlights();

            PictureBox^ activeChartBox;
            if (chartNumber == 1)
                activeChartBox = pictureBoxChart1;
            else if (chartNumber == 2)
                activeChartBox = pictureBoxChart2;
            else
                activeChartBox = pictureBoxChart3;

            activeChartBox->BorderStyle = BorderStyle::Fixed3D;
        }

        // Сброс подсветки графиков
        void ResetChartHighlights()
        {
            pictureBoxChart1->BorderStyle = BorderStyle::FixedSingle;
            pictureBoxChart2->BorderStyle = BorderStyle::FixedSingle;
            pictureBoxChart3->BorderStyle = BorderStyle::FixedSingle;
        }

        // Получение описания фильтра
        String^ GetFilterDescription()
        {
            if (activeFilter == "Year")
                return "Год: " + activeFilterValue;
            else if (activeFilter == "Author")
                return "Автор: " + activeFilterValue;
            else if (activeFilter == "PriceRange")
                return "Цена: " + activeFilterValue;
            else if (activeFilter == "RatingRange")
                return "Рейтинг: " + activeFilterValue;
            else
                return "";
        }

        // Обновление отфильтрованного представления
        void UpdateFilteredView()
        {
            if (String::IsNullOrEmpty(currentSelectedGenre)) return;

            List<Book^>^ baseFiltered = FilterBooksByGenre(currentSelectedGenre);

            // Сохраняем базовый список без сортировки
            baseFilteredBooks->Clear();
            for each(Book ^ book in baseFiltered)
            {
                baseFilteredBooks->Add(book);
            }

            currentDisplayBooks->Clear();
            bookIndexMap->Clear();

            if (activeFilter == "Year")
            {
                int year = Int32::Parse(activeFilterValue);
                for each(Book ^ book in baseFiltered)
                {
                    if (book->Year == year)
                        currentDisplayBooks->Add(book);
                }
            }
            else if (activeFilter == "Author")
            {
                for each(Book ^ book in baseFiltered)
                {
                    if (book->Author == activeFilterValue)
                        currentDisplayBooks->Add(book);
                }
            }
            else if (activeFilter == "PriceRange")
            {
                for each(Book ^ book in baseFiltered)
                {
                    if (activeFilterValue == "0-10$" && book->Price < 10)
                        currentDisplayBooks->Add(book);
                    else if (activeFilterValue == "10-20$" && book->Price >= 10 && book->Price < 20)
                        currentDisplayBooks->Add(book);
                    else if (activeFilterValue == "20-30$" && book->Price >= 20 && book->Price < 30)
                        currentDisplayBooks->Add(book);
                    else if (activeFilterValue == "30-40$" && book->Price >= 30 && book->Price < 40)
                        currentDisplayBooks->Add(book);
                    else if (activeFilterValue == "40$+" && book->Price >= 40)
                        currentDisplayBooks->Add(book);
                }
            }
            else if (activeFilter == "RatingRange")
            {
                for each(Book ^ book in baseFiltered)
                {
                    if (activeFilterValue == "0-1" && book->UserRating < 1)
                        currentDisplayBooks->Add(book);
                    else if (activeFilterValue == "1-2" && book->UserRating >= 1 && book->UserRating < 2)
                        currentDisplayBooks->Add(book);
                    else if (activeFilterValue == "2-3" && book->UserRating >= 2 && book->UserRating < 3)
                        currentDisplayBooks->Add(book);
                    else if (activeFilterValue == "3-4" && book->UserRating >= 3 && book->UserRating < 4)
                        currentDisplayBooks->Add(book);
                    else if (activeFilterValue == "4-5" && book->UserRating >= 4)
                        currentDisplayBooks->Add(book);
                }
            }
            else
            {
                for each(Book ^ book in baseFiltered)
                {
                    currentDisplayBooks->Add(book);
                }
            }

            if (currentDisplayBooks->Count > 0)
            {
                // Сортируем по рейтингу для отображения
                array<Book^>^ sorted = currentDisplayBooks->ToArray();
                Array::Sort(sorted, gcnew Comparison<Book^>(&MyForm::CompareBooksByRating));

                // Очищаем и заполняем таблицу
                dataGridViewAllBooks->Rows->Clear();
                currentDisplayBooks->Clear();

                for (int i = 0; i < sorted->Length; i++)
                {
                    Book^ book = sorted[i];
                    currentDisplayBooks->Add(book);

                    String^ ratingStr = book->UserRating.ToString("F1", CultureInfo::InvariantCulture);

                    dataGridViewAllBooks->Rows->Add(
                        (i + 1).ToString(),
                        book->Name,
                        book->Author,
                        ratingStr,
                        "$" + book->Price.ToString("F2"),
                        book->Year.ToString()
                    );
                }

                dataGridViewAllBooks->AutoResizeColumns(DataGridViewAutoSizeColumnsMode::DisplayedCells);

                if (!String::IsNullOrEmpty(activeFilter))
                {
                    statusLabel->Text = "Показано " + currentDisplayBooks->Count + " книг. " + GetFilterDescription();
                }

                // Восстанавливаем выделение, если была выбрана книга
                if (selectedBookIndex >= 0 && selectedBookIndex < currentDisplayBooks->Count &&
                    currentDisplayBooks[selectedBookIndex] == selectedBook)
                {
                    isProgrammaticSelection = true;
                    dataGridViewAllBooks->Rows[selectedBookIndex]->Selected = true;
                    isProgrammaticSelection = false;
                }
                else
                {
                    selectedBook = nullptr;
                    selectedBookIndex = -1;
                }
            }
            else
            {
                dataGridViewAllBooks->Rows->Clear();
                if (!String::IsNullOrEmpty(activeFilter))
                {
                    statusLabel->Text = "Нет книг, соответствующих фильтру: " + GetFilterDescription();
                }
            }
        }

        // Метод копирования выделенных ячеек
        void CopySelectedCells()
        {
            if (dataGridViewAllBooks->SelectedRows->Count > 0 || dataGridViewAllBooks->SelectedCells->Count > 0)
            {
                try
                {
                    DataObject^ data = dataGridViewAllBooks->GetClipboardContent();
                    if (data != nullptr)
                    {
                        Clipboard::SetDataObject(data);
                        statusLabel->Text = "Данные скопированы в буфер обмена";
                    }
                }
                catch (Exception^ ex)
                {
                    MessageBox::Show("Ошибка при копировании: " + ex->Message, "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
            else
            {
                statusLabel->Text = "Нет выделенных строк для копирования";
            }
        }

        System::Void MyForm_Resize(System::Object^ sender, System::EventArgs^ e)
        {
            if (isDataLoaded && books->Count > 0)
            {
                isResizing = true;

                String^ savedFilter = activeFilter;
                String^ savedFilterValue = activeFilterValue;
                int savedActiveChart = activeChart;

                List<String^>^ savedAuthors = gcnew List<String^>();
                for each(String ^ author in highlightedAuthors)
                    savedAuthors->Add(author);

                List<int>^ savedYears = gcnew List<int>();
                for each(int year in highlightedYears)
                    savedYears->Add(year);

                List<String^>^ savedPriceRanges = gcnew List<String^>();
                for each(String ^ range in highlightedPriceRanges)
                    savedPriceRanges->Add(range);

                List<String^>^ savedRatingRanges = gcnew List<String^>();
                for each(String ^ range in highlightedRatingRanges)
                    savedRatingRanges->Add(range);

                Book^ savedSelectedBook = selectedBook;
                int savedSelectedIndex = selectedBookIndex;

                List<Book^>^ filtered = FilterBooksByGenre(currentSelectedGenre);
                DrawCharts(filtered);

                activeFilter = savedFilter;
                activeFilterValue = savedFilterValue;
                activeChart = savedActiveChart;

                highlightedAuthors->Clear();
                for each(String ^ author in savedAuthors)
                    highlightedAuthors->Add(author);

                highlightedYears->Clear();
                for each(int year in savedYears)
                    highlightedYears->Add(year);

                highlightedPriceRanges->Clear();
                for each(String ^ range in savedPriceRanges)
                    highlightedPriceRanges->Add(range);

                highlightedRatingRanges->Clear();
                for each(String ^ range in savedRatingRanges)
                    highlightedRatingRanges->Add(range);

                selectedBook = savedSelectedBook;
                selectedBookIndex = savedSelectedIndex;

                if (!String::IsNullOrEmpty(activeFilter))
                {
                    UpdateFilteredView();
                    HighlightActiveChart(activeChart);
                }
                else
                {
                    RefreshCurrentView();
                }

                isResizing = false;
            }
        }

        System::Void comboBoxGenre_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
        {
            if (isDataLoaded && comboBoxGenre->SelectedItem != nullptr)
            {
                String^ selectedGenre = comboBoxGenre->SelectedItem->ToString();
                if (selectedGenre == "Художественная")
                    currentSelectedGenre = "Fiction";
                else if (selectedGenre == "Нехудожественная")
                    currentSelectedGenre = "Non Fiction";

                ClearFilter();
                ClearHighlights();
                selectedBook = nullptr;
                selectedBookIndex = -1;
                dataGridViewAllBooks->ClearSelection();
                RefreshCurrentView();
            }
        }

        System::Void comboBoxMetric_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
        {
            if (isDataLoaded && comboBoxMetric->SelectedItem != nullptr)
            {
                String^ selectedMetric = comboBoxMetric->SelectedItem->ToString();

                if (selectedMetric == "По авторам")
                    currentSelectedMetric = "Author";
                else if (selectedMetric == "По цене")
                    currentSelectedMetric = "Price";
                else if (selectedMetric == "По рейтингу")
                    currentSelectedMetric = "Rating";

                if (isDataLoaded)
                {
                    List<Book^>^ filtered = FilterBooksByGenre(currentSelectedGenre);

                    if (pictureBoxChart3->Image != nullptr)
                    {
                        delete pictureBoxChart3->Image;
                        pictureBoxChart3->Image = nullptr;
                    }

                    chart3Regions->Clear();
                    DrawDistributionChart(filtered);
                }
            }
        }

        void RefreshCurrentView()
        {
            if (String::IsNullOrEmpty(currentSelectedGenre)) return;

            List<Book^>^ filtered = FilterBooksByGenre(currentSelectedGenre);

            // Сохраняем базовый список без сортировки
            baseFilteredBooks->Clear();
            for each(Book ^ book in filtered)
            {
                baseFilteredBooks->Add(book);
            }

            // Сортируем по рейтингу для отображения
            array<Book^>^ sorted = filtered->ToArray();
            Array::Sort(sorted, gcnew Comparison<Book^>(&MyForm::CompareBooksByRating));

            isProgrammaticSelection = true;

            dataGridViewAllBooks->Rows->Clear();
            currentDisplayBooks->Clear();

            for (int i = 0; i < sorted->Length; i++)
            {
                Book^ book = sorted[i];
                currentDisplayBooks->Add(book);

                String^ ratingStr = book->UserRating.ToString("F1", CultureInfo::InvariantCulture);

                dataGridViewAllBooks->Rows->Add(
                    (i + 1).ToString(),
                    book->Name,
                    book->Author,
                    ratingStr,
                    "$" + book->Price.ToString("F2"),
                    book->Year.ToString()
                );
            }

            dataGridViewAllBooks->AutoResizeColumns(DataGridViewAutoSizeColumnsMode::DisplayedCells);

            // Восстанавливаем выделение, если была выбрана книга
            if (selectedBookIndex >= 0 && selectedBookIndex < currentDisplayBooks->Count &&
                currentDisplayBooks[selectedBookIndex] == selectedBook)
            {
                dataGridViewAllBooks->Rows[selectedBookIndex]->Selected = true;
            }
            else
            {
                selectedBook = nullptr;
                selectedBookIndex = -1;
            }

            isProgrammaticSelection = false;

            DrawCharts(filtered);

            statusLabel->Text = "Показано " + filtered->Count + " книг в жанре \"" +
                (currentSelectedGenre == "Fiction" ? "Художественная" : "Нехудожественная") + "\"";
        }

        List<Book^>^ FilterBooksByGenre(String^ genre)
        {
            List<Book^>^ filtered = gcnew List<Book^>();
            for each(Book ^ book in books)
            {
                if (book->Genre == genre)
                {
                    filtered->Add(book);
                }
            }
            return filtered;
        }

        System::Void buttonLoadFile_Click(System::Object^ sender, System::EventArgs^ e)
        {
            System::Windows::Forms::OpenFileDialog^ openFileDialog = gcnew System::Windows::Forms::OpenFileDialog();
            openFileDialog->Title = "Выберите CSV файл";
            openFileDialog->Filter = "CSV файлы (*.csv)|*.csv|Все файлы (*.*)|*.*";
            openFileDialog->FilterIndex = 1;
            openFileDialog->RestoreDirectory = true;

            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
            {
                currentFileName = openFileDialog->FileName;
                LoadCSV(currentFileName);
            }
        }

        void LoadCSV(String^ filename)
        {
            try {
                if (!File::Exists(filename))
                {
                    System::Windows::Forms::MessageBox::Show("Файл не найден: " + filename, "Ошибка",
                        System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
                    return;
                }

                books->Clear();
                currentDisplayBooks->Clear();
                baseFilteredBooks->Clear();
                bookIndexMap->Clear();
                dataGridViewAllBooks->Rows->Clear();
                dataGridViewAllBooks->ClearSelection();
                selectedBook = nullptr;
                selectedBookIndex = -1;

                if (pictureBoxChart1->Image != nullptr)
                {
                    delete pictureBoxChart1->Image;
                    pictureBoxChart1->Image = nullptr;
                }
                if (pictureBoxChart2->Image != nullptr)
                {
                    delete pictureBoxChart2->Image;
                    pictureBoxChart2->Image = nullptr;
                }
                if (pictureBoxChart3->Image != nullptr)
                {
                    delete pictureBoxChart3->Image;
                    pictureBoxChart3->Image = nullptr;
                }

                chart1Regions->Clear();
                chart2Regions->Clear();
                chart3Regions->Clear();
                ClearHighlights();

                GC::Collect();
                GC::WaitForPendingFinalizers();

                buttonAnalyze->Enabled = false;
                comboBoxGenre->Enabled = false;
                comboBoxMetric->Enabled = false;
                buttonClearFilter->Enabled = false;
                isDataLoaded = false;

                statusLabel->Text = "Загрузка файла: " + Path::GetFileName(filename) + "...";
                System::Windows::Forms::Application::DoEvents();

                array<String^>^ lines = File::ReadAllLines(filename);

                CultureInfo^ culture = gcnew CultureInfo("en-US");

                int validBooks = 0;
                for (int i = 1; i < lines->Length; i++)
                {
                    String^ line = lines[i];
                    if (String::IsNullOrWhiteSpace(line))
                        continue;

                    Book^ book = ParseCSVLine(line, culture);
                    if (book != nullptr)
                    {
                        books->Add(book);
                        validBooks++;
                    }

                    if (i % 100 == 0)
                    {
                        statusLabel->Text = "Загрузка: загружено " + validBooks + " книг...";
                        System::Windows::Forms::Application::DoEvents();
                    }
                }

                this->Text = "Анализатор CSV - Книжные бестселлеры Amazon 2009-2019 [" + Path::GetFileName(filename) + "]";
                statusLabel->Text = "Загружено " + books->Count + " книг из файла " + Path::GetFileName(filename);

                if (books->Count > 0)
                {
                    comboBoxGenre->Enabled = true;
                    comboBoxMetric->Enabled = true;
                    buttonAnalyze->Enabled = true;
                    isDataLoaded = true;

                    RefreshCurrentView();
                }
                else
                {
                    System::Windows::Forms::MessageBox::Show("В файле не найдено валидных книг.", "Предупреждение",
                        System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Warning);
                }
            }
            catch (Exception^ ex) {
                System::Windows::Forms::MessageBox::Show("Ошибка загрузки CSV: " + ex->Message, "Ошибка",
                    System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
                statusLabel->Text = "Ошибка загрузки файла";
            }
        }

        Book^ ParseCSVLine(String^ line, CultureInfo^ culture)
        {
            try {
                List<String^>^ fields = gcnew List<String^>();
                bool inQuotes = false;
                String^ currentField = "";

                for (int i = 0; i < line->Length; i++)
                {
                    wchar_t c = line[i];

                    if (c == '"')
                    {
                        inQuotes = !inQuotes;
                    }
                    else if (c == ',' && !inQuotes)
                    {
                        fields->Add(currentField);
                        currentField = "";
                    }
                    else
                    {
                        currentField += c;
                    }
                }

                fields->Add(currentField);

                if (fields->Count >= 7)
                {
                    Book^ book = gcnew Book();

                    book->Name = fields[0]->Trim('"');
                    book->Author = fields[1]->Trim('"');

                    String^ ratingStr = fields[2]->Trim();
                    double rating;
                    if (Double::TryParse(ratingStr, NumberStyles::Any, culture, rating))
                    {
                        book->UserRating = rating;
                    }
                    else
                    {
                        if (!Double::TryParse(ratingStr, NumberStyles::Any, CultureInfo::InvariantCulture, rating))
                        {
                            book->UserRating = 0.0;
                        }
                        else
                        {
                            book->UserRating = rating;
                        }
                    }

                    String^ reviewsStr = fields[3]->Trim();
                    int reviews;
                    if (Int32::TryParse(reviewsStr, reviews))
                        book->Reviews = reviews;
                    else
                        book->Reviews = 0;

                    String^ priceStr = fields[4]->Trim();
                    double price;
                    if (Double::TryParse(priceStr, NumberStyles::Any, culture, price))
                    {
                        book->Price = price;
                    }
                    else
                    {
                        if (!Double::TryParse(priceStr, NumberStyles::Any, CultureInfo::InvariantCulture, price))
                        {
                            book->Price = 0.0;
                        }
                        else
                        {
                            book->Price = price;
                        }
                    }

                    String^ yearStr = fields[5]->Trim();
                    int year;
                    if (Int32::TryParse(yearStr, year))
                        book->Year = year;
                    else
                        book->Year = 0;

                    book->Genre = fields[6]->Trim('"');

                    return book;
                }
            }
            catch (Exception^) {
            }

            return nullptr;
        }

        System::Void buttonAnalyze_Click(System::Object^ sender, System::EventArgs^ e)
        {
            ClearFilter();
            ClearHighlights();
            selectedBook = nullptr;
            selectedBookIndex = -1;
            dataGridViewAllBooks->ClearSelection();
            RefreshCurrentView();
        }

        void DrawCharts(List<Book^>^ filtered)
        {
            DrawRatingChart(filtered);
            DrawReviewsChart(filtered);
            DrawDistributionChart(filtered);
        }

        void DrawRatingChart(List<Book^>^ filtered)
        {
            if (pictureBoxChart1->Width <= 0 || pictureBoxChart1->Height <= 0)
                return;

            chart1Regions->Clear();

            Dictionary<int, double>^ yearRatingSum = gcnew Dictionary<int, double>();
            Dictionary<int, int>^ yearRatingCount = gcnew Dictionary<int, int>();

            for each(Book ^ book in filtered)
            {
                if (!yearRatingSum->ContainsKey(book->Year))
                {
                    yearRatingSum->Add(book->Year, 0.0);
                    yearRatingCount->Add(book->Year, 0);
                }

                yearRatingSum[book->Year] += book->UserRating;
                yearRatingCount[book->Year]++;
            }

            System::Drawing::Bitmap^ bmp = gcnew System::Drawing::Bitmap(pictureBoxChart1->Width, pictureBoxChart1->Height);
            System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bmp);
            g->Clear(System::Drawing::Color::White);
            g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

            DrawLineChartWithRegions(g, bmp->Width, bmp->Height, yearRatingSum, yearRatingCount,
                "Средний рейтинг по годам", System::Drawing::Color::Blue, 0, 5, chart1Regions);

            if (pictureBoxChart1->Image != nullptr)
                delete pictureBoxChart1->Image;

            pictureBoxChart1->Image = bmp;
            delete g;
        }

        void DrawReviewsChart(List<Book^>^ filtered)
        {
            if (pictureBoxChart2->Width <= 0 || pictureBoxChart2->Height <= 0)
                return;

            chart2Regions->Clear();

            Dictionary<int, int>^ yearReviews = gcnew Dictionary<int, int>();

            for each(Book ^ book in filtered)
            {
                if (!yearReviews->ContainsKey(book->Year))
                {
                    yearReviews->Add(book->Year, 0);
                }

                yearReviews[book->Year] += book->Reviews;
            }

            System::Drawing::Bitmap^ bmp = gcnew System::Drawing::Bitmap(pictureBoxChart2->Width, pictureBoxChart2->Height);
            System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bmp);
            g->Clear(System::Drawing::Color::White);
            g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

            DrawBarChartWithRegions(g, bmp->Width, bmp->Height, yearReviews,
                "Общее количество отзывов по годам", System::Drawing::Color::Orange, chart2Regions);

            if (pictureBoxChart2->Image != nullptr)
                delete pictureBoxChart2->Image;

            pictureBoxChart2->Image = bmp;
            delete g;
        }

        void DrawDistributionChart(List<Book^>^ filtered)
        {
            if (pictureBoxChart3->Width <= 0 || pictureBoxChart3->Height <= 0)
                return;

            chart3Regions->Clear();

            System::Drawing::Bitmap^ bmp = gcnew System::Drawing::Bitmap(pictureBoxChart3->Width, pictureBoxChart3->Height);
            System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bmp);
            g->Clear(System::Drawing::Color::White);
            g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

            String^ title = "";

            if (currentSelectedMetric == "Author")
            {
                title = "Топ-10 авторов по количеству книг";
                DrawAuthorDistributionWithRegions(g, bmp->Width, bmp->Height, filtered, title, chart3Regions);
            }
            else if (currentSelectedMetric == "Price")
            {
                title = "Распределение по цене ($)";
                DrawPriceDistributionWithRegions(g, bmp->Width, bmp->Height, filtered, title, chart3Regions);
            }
            else if (currentSelectedMetric == "Rating")
            {
                title = "Распределение по рейтингу";
                DrawRatingDistributionWithRegions(g, bmp->Width, bmp->Height, filtered, title, chart3Regions);
            }

            if (pictureBoxChart3->Image != nullptr)
                delete pictureBoxChart3->Image;

            pictureBoxChart3->Image = bmp;
            delete g;
        }

        void DrawAuthorDistributionWithRegions(System::Drawing::Graphics^ g, int width, int height,
            List<Book^>^ filtered, String^ title, List<ChartClickRegion^>^ regions)
        {
            Dictionary<String^, int>^ authorCount = gcnew Dictionary<String^, int>();

            for each(Book ^ book in filtered)
            {
                if (!authorCount->ContainsKey(book->Author))
                {
                    authorCount->Add(book->Author, 0);
                }
                authorCount[book->Author]++;
            }

            array<KeyValuePair<String^, int>>^ authors = gcnew array<KeyValuePair<String^, int>>(authorCount->Count);
            int index = 0;
            for each(KeyValuePair<String^, int> pair in authorCount)
            {
                authors[index++] = pair;
            }

            Array::Sort(authors, gcnew Comparison<KeyValuePair<String^, int>>(&AuthorComparer::Compare));

            int count = Math::Min(10, authors->Length);

            DrawBarChartWithLabelsAndRegions(g, width, height, authors, count, title,
                System::Drawing::Color::Purple, "Количество книг", "Author", regions);
        }

        void DrawPriceDistributionWithRegions(System::Drawing::Graphics^ g, int width, int height,
            List<Book^>^ filtered, String^ title, List<ChartClickRegion^>^ regions)
        {
            Dictionary<String^, int>^ priceRanges = gcnew Dictionary<String^, int>();
            priceRanges->Add("0-10$", 0);
            priceRanges->Add("10-20$", 0);
            priceRanges->Add("20-30$", 0);
            priceRanges->Add("30-40$", 0);
            priceRanges->Add("40$+", 0);

            for each(Book ^ book in filtered)
            {
                if (book->Price < 10)
                    priceRanges["0-10$"]++;
                else if (book->Price < 20)
                    priceRanges["10-20$"]++;
                else if (book->Price < 30)
                    priceRanges["20-30$"]++;
                else if (book->Price < 40)
                    priceRanges["30-40$"]++;
                else
                    priceRanges["40$+"]++;
            }

            array<KeyValuePair<String^, int>>^ ranges = gcnew array<KeyValuePair<String^, int>>(priceRanges->Count);
            int index = 0;
            for each(KeyValuePair<String^, int> pair in priceRanges)
            {
                ranges[index++] = pair;
            }

            DrawBarChartWithLabelsAndRegions(g, width, height, ranges, ranges->Length, title,
                System::Drawing::Color::Teal, "Количество книг", "PriceRange", regions);
        }

        void DrawRatingDistributionWithRegions(System::Drawing::Graphics^ g, int width, int height,
            List<Book^>^ filtered, String^ title, List<ChartClickRegion^>^ regions)
        {
            Dictionary<String^, int>^ ratingRanges = gcnew Dictionary<String^, int>();
            ratingRanges->Add("0-1", 0);
            ratingRanges->Add("1-2", 0);
            ratingRanges->Add("2-3", 0);
            ratingRanges->Add("3-4", 0);
            ratingRanges->Add("4-5", 0);

            for each(Book ^ book in filtered)
            {
                if (book->UserRating < 1)
                    ratingRanges["0-1"]++;
                else if (book->UserRating < 2)
                    ratingRanges["1-2"]++;
                else if (book->UserRating < 3)
                    ratingRanges["2-3"]++;
                else if (book->UserRating < 4)
                    ratingRanges["3-4"]++;
                else
                    ratingRanges["4-5"]++;
            }

            array<KeyValuePair<String^, int>>^ ranges = gcnew array<KeyValuePair<String^, int>>(ratingRanges->Count);
            int index = 0;
            for each(KeyValuePair<String^, int> pair in ratingRanges)
            {
                ranges[index++] = pair;
            }

            DrawBarChartWithLabelsAndRegions(g, width, height, ranges, ranges->Length, title,
                System::Drawing::Color::DarkOrange, "Количество книг", "RatingRange", regions);
        }

        void DrawLineChartWithRegions(System::Drawing::Graphics^ g, int width, int height,
            Dictionary<int, double>^ sum, Dictionary<int, int>^ count,
            String^ title, System::Drawing::Color color,
            double minY, double maxY, List<ChartClickRegion^>^ regions)
        {
            System::Drawing::Pen^ blackPen = gcnew System::Drawing::Pen(System::Drawing::Color::Black, 1);
            int margin = 50;
            int chartWidth = width - 2 * margin;
            int chartHeight = height - 2 * margin - 30;

            g->DrawLine(blackPen, margin, margin + 20, margin, height - margin);
            g->DrawLine(blackPen, margin, height - margin, width - margin, height - margin);

            System::Drawing::Font^ titleFont = gcnew System::Drawing::Font("Segoe UI", 9, System::Drawing::FontStyle::Bold);
            g->DrawString(title, titleFont, System::Drawing::Brushes::Black,
                width / 2 - 100, 5);

            int minYear = Int32::MaxValue;
            int maxYear = Int32::MinValue;
            for each(int year in sum->Keys)
            {
                if (year < minYear) minYear = year;
                if (year > maxYear) maxYear = year;
            }

            if (minYear > maxYear)
            {
                delete blackPen;
                delete titleFont;
                return;
            }

            double yearRange = maxYear - minYear;
            if (yearRange == 0) yearRange = 1;

            double maxValue = 0;
            for each(int year in sum->Keys)
            {
                double avgValue = sum[year] / count[year];
                if (avgValue > maxValue) maxValue = avgValue;
            }

            if (maxY > 0)
                maxValue = maxY;

            if (maxValue == 0) maxValue = 1;

            System::Drawing::Font^ axisFont = gcnew System::Drawing::Font("Segoe UI", 7);
            for (int i = 0; i <= 5; i++)
            {
                int y = height - margin - (int)((double)i / 5 * chartHeight);
                double value = (double)i / 5 * maxValue;
                g->DrawString(value.ToString("F1"), axisFont, System::Drawing::Brushes::Black, margin - 40, y - 8);
            }

            System::Drawing::Pen^ gridPen = gcnew System::Drawing::Pen(System::Drawing::Color::LightGray, 1);
            gridPen->DashStyle = System::Drawing::Drawing2D::DashStyle::Dash;

            for (int i = 0; i <= 5; i++)
            {
                int y = height - margin - (int)((double)i / 5 * chartHeight);
                g->DrawLine(gridPen, margin, y, width - margin, y);
            }

            array<int>^ years = gcnew array<int>(sum->Keys->Count);
            sum->Keys->CopyTo(years, 0);
            Array::Sort(years);

            System::Drawing::Pen^ chartPen = gcnew System::Drawing::Pen(color, 2);
            int prevX = -1, prevY = -1;

            int pointRadius = 12;

            for each(int year in years)
            {
                double avgValue = sum[year] / count[year];

                int x = margin + (int)((year - minYear) / yearRange * chartWidth);
                int y = height - margin - (int)((avgValue / maxValue) * chartHeight);

                x = Math::Max(margin, Math::Min(width - margin, x));
                y = Math::Max(margin + 20, Math::Min(height - margin, y));

                Color pointColor = color;
                if (highlightedYears->Contains(year))
                {
                    pointColor = Color::Red;
                }

                g->FillEllipse(gcnew System::Drawing::SolidBrush(pointColor), x - pointRadius / 2, y - pointRadius / 2, pointRadius, pointRadius);

                ChartClickRegion^ region = gcnew ChartClickRegion();
                region->Type = "Year";
                region->Value = year.ToString();
                region->Bounds = System::Drawing::Rectangle(x - pointRadius, y - pointRadius, pointRadius * 2, pointRadius * 2);
                regions->Add(region);

                if (prevX != -1 && prevY != -1)
                {
                    g->DrawLine(chartPen, prevX, prevY, x, y);
                }

                prevX = x;
                prevY = y;

                if (years->Length < 10 || (year % 2 == 0))
                {
                    System::Drawing::Font^ yearFont = gcnew System::Drawing::Font("Segoe UI", 6);
                    g->DrawString(year.ToString(), yearFont, System::Drawing::Brushes::Black, x - 10, height - margin + 5);
                    delete yearFont;
                }
            }

            delete blackPen;
            delete gridPen;
            delete chartPen;
            delete titleFont;
            delete axisFont;
        }

        void DrawBarChartWithRegions(System::Drawing::Graphics^ g, int width, int height,
            Dictionary<int, int>^ data, String^ title,
            System::Drawing::Color color, List<ChartClickRegion^>^ regions)
        {
            System::Drawing::Pen^ blackPen = gcnew System::Drawing::Pen(System::Drawing::Color::Black, 1);
            int margin = 50;
            int chartWidth = width - 2 * margin;
            int chartHeight = height - 2 * margin - 30;

            g->DrawLine(blackPen, margin, margin + 20, margin, height - margin);
            g->DrawLine(blackPen, margin, height - margin, width - margin, height - margin);

            System::Drawing::Font^ titleFont = gcnew System::Drawing::Font("Segoe UI", 9, System::Drawing::FontStyle::Bold);
            g->DrawString(title, titleFont, System::Drawing::Brushes::Black,
                width / 2 - 100, 5);

            int minYear = Int32::MaxValue;
            int maxYear = Int32::MinValue;
            int maxValue = 0;

            for each(int year in data->Keys)
            {
                if (year < minYear) minYear = year;
                if (year > maxYear) maxYear = year;
                if (data[year] > maxValue) maxValue = data[year];
            }

            if (minYear > maxYear)
            {
                delete blackPen;
                delete titleFont;
                return;
            }

            double yearRange = maxYear - minYear;
            if (yearRange == 0) yearRange = 1;

            if (maxValue == 0) maxValue = 1;

            System::Drawing::Font^ axisFont = gcnew System::Drawing::Font("Segoe UI", 7);
            for (int i = 0; i <= 5; i++)
            {
                int y = height - margin - (int)((double)i / 5 * chartHeight);
                int value = (int)((double)i / 5 * maxValue / 1000);
                g->DrawString(value.ToString() + "K", axisFont, System::Drawing::Brushes::Black, margin - 45, y - 8);
            }

            System::Drawing::Pen^ gridPen = gcnew System::Drawing::Pen(System::Drawing::Color::LightGray, 1);
            gridPen->DashStyle = System::Drawing::Drawing2D::DashStyle::Dash;

            for (int i = 0; i <= 5; i++)
            {
                int y = height - margin - (int)((double)i / 5 * chartHeight);
                g->DrawLine(gridPen, margin, y, width - margin, y);
            }

            array<int>^ years = gcnew array<int>(data->Keys->Count);
            data->Keys->CopyTo(years, 0);
            Array::Sort(years);

            int barWidth = 20;

            for each(int year in years)
            {
                int x = margin + (int)((year - minYear) / yearRange * chartWidth) - barWidth / 2;
                int barHeight = (int)((double)data[year] / maxValue * chartHeight);

                x = Math::Max(margin, Math::Min(width - margin - barWidth, x));

                if (barHeight > 0)
                {
                    Color barColor = color;
                    if (highlightedYears->Contains(year))
                    {
                        barColor = Color::Red;
                    }

                    System::Drawing::Rectangle barRect = System::Drawing::Rectangle(
                        x, height - margin - barHeight, barWidth, barHeight);

                    System::Drawing::Drawing2D::LinearGradientBrush^ brush =
                        gcnew System::Drawing::Drawing2D::LinearGradientBrush(
                            barRect, barColor, DarkenColor(barColor), 90.0f);

                    g->FillRectangle(brush, barRect);
                    g->DrawRectangle(System::Drawing::Pens::DarkGray, barRect);
                    delete brush;

                    System::Drawing::Rectangle clickRect = System::Drawing::Rectangle(
                        x - 5,
                        height - margin - barHeight - 5,
                        barWidth + 10,
                        barHeight + 10);
                    ChartClickRegion^ region = gcnew ChartClickRegion();
                    region->Type = "Year";
                    region->Value = year.ToString();
                    region->Bounds = clickRect;
                    regions->Add(region);
                }

                if (years->Length < 10 || (year % 2 == 0))
                {
                    System::Drawing::Font^ yearFont = gcnew System::Drawing::Font("Segoe UI", 6);
                    g->DrawString(year.ToString(), yearFont, System::Drawing::Brushes::Black, x, height - margin + 5);
                    delete yearFont;
                }
            }

            delete blackPen;
            delete gridPen;
            delete titleFont;
            delete axisFont;
        }

        void DrawBarChartWithLabelsAndRegions(System::Drawing::Graphics^ g, int width, int height,
            array<KeyValuePair<String^, int>>^ data, int count,
            String^ title, System::Drawing::Color color,
            String^ yAxisLabel, String^ regionType,
            List<ChartClickRegion^>^ regions)
        {
            int margin = 70;
            int chartWidth = width - 2 * margin;
            int chartHeight = height - 2 * margin - 40;

            System::Drawing::Pen^ blackPen = gcnew System::Drawing::Pen(System::Drawing::Color::Black, 1);
            g->DrawLine(blackPen, margin, margin + 20, margin, height - margin);
            g->DrawLine(blackPen, margin, height - margin, width - margin, height - margin);

            System::Drawing::Font^ titleFont = gcnew System::Drawing::Font("Segoe UI", 9, System::Drawing::FontStyle::Bold);
            g->DrawString(title, titleFont, System::Drawing::Brushes::Black,
                width / 2 - 100, 5);

            int maxValue = 0;
            for (int i = 0; i < count; i++)
            {
                if (data[i].Value > maxValue)
                    maxValue = data[i].Value;
            }
            if (maxValue == 0) maxValue = 1;

            System::Drawing::Pen^ gridPen = gcnew System::Drawing::Pen(System::Drawing::Color::LightGray, 1);
            gridPen->DashStyle = System::Drawing::Drawing2D::DashStyle::Dash;

            System::Drawing::Font^ axisFont = gcnew System::Drawing::Font("Segoe UI", 7);

            for (int i = 0; i <= 5; i++)
            {
                int y = height - margin - (int)((double)i / 5 * chartHeight);
                g->DrawLine(gridPen, margin, y, width - margin, y);

                int value = (int)((double)i / 5 * maxValue);
                g->DrawString(value.ToString(), axisFont, System::Drawing::Brushes::Black, margin - 40, y - 8);
            }

            int barWidth = (chartWidth - 20) / count;
            if (barWidth > 50) barWidth = 50;

            for (int i = 0; i < count; i++)
            {
                int x = margin + 20 + i * (barWidth + 5);
                int barHeight = (int)((double)data[i].Value / maxValue * chartHeight);

                Color barColor = color;

                if (regionType == "Author" && highlightedAuthors->Contains(data[i].Key))
                {
                    barColor = Color::Red;
                }
                else if (regionType == "PriceRange" && highlightedPriceRanges->Contains(data[i].Key))
                {
                    barColor = Color::Red;
                }
                else if (regionType == "RatingRange" && highlightedRatingRanges->Contains(data[i].Key))
                {
                    barColor = Color::Red;
                }

                if (barHeight > 0)
                {
                    System::Drawing::Rectangle barRect = System::Drawing::Rectangle(
                        x, height - margin - barHeight, barWidth, barHeight);

                    System::Drawing::Drawing2D::LinearGradientBrush^ brush =
                        gcnew System::Drawing::Drawing2D::LinearGradientBrush(
                            barRect, barColor, DarkenColor(barColor), 90.0f);

                    g->FillRectangle(brush, barRect);
                    g->DrawRectangle(System::Drawing::Pens::DarkGray, barRect);
                    delete brush;

                    System::Drawing::Rectangle clickRect = System::Drawing::Rectangle(
                        x - 5,
                        height - margin - barHeight - 5,
                        barWidth + 10,
                        barHeight + 10);
                    ChartClickRegion^ region = gcnew ChartClickRegion();
                    region->Type = regionType;
                    region->Value = data[i].Key;
                    region->Bounds = clickRect;
                    regions->Add(region);

                    if (i < 3 || data[i].Value > maxValue * 0.7)
                    {
                        g->DrawString(data[i].Value.ToString(), axisFont, System::Drawing::Brushes::Black,
                            x, height - margin - barHeight - 20);
                    }
                }

                String^ label = data[i].Key;
                if (label->Length > 10)
                    label = label->Substring(0, 8) + "...";

                g->DrawString(label, axisFont, System::Drawing::Brushes::Black, x, height - margin + 5);
            }

            delete blackPen;
            delete gridPen;
            delete titleFont;
            delete axisFont;
        }

        System::Drawing::Color DarkenColor(System::Drawing::Color color)
        {
            return System::Drawing::Color::FromArgb(
                Math::Max(0, (int)(color.R * 0.7)),
                Math::Max(0, (int)(color.G * 0.7)),
                Math::Max(0, (int)(color.B * 0.7))
            );
        }
    };
}