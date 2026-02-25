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

    // Управляемая структура для книги
    public ref struct Book
    {
        String^ Name;          // Название
        String^ Author;        // Автор
        double UserRating;     // Рейтинг
        int Reviews;           // Отзывы
        double Price;          // Цена
        int Year;              // Год
        String^ Genre;         // Жанр
    };

    // Класс для хранения информации о кликабельной области графика
    public ref struct ChartClickRegion
    {
        String^ Type;          // Тип области (Year, Author, PriceRange, RatingRange)
        String^ Value;         // Значение
        System::Drawing::Rectangle Bounds;      // Область для клика
        Object^ Data;          // Дополнительные данные
        bool IsHighlighted;    // Подсвечена ли область
    };

    // Класс для сравнения авторов (используется вместо лямбды)
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

            // Настройка копирования для DataGridView
            this->dataGridViewAllBooks->KeyDown += gcnew KeyEventHandler(this, &MyForm::dataGridViewAllBooks_KeyDown);
            this->dataGridViewAllBooks->MouseClick += gcnew MouseEventHandler(this, &MyForm::dataGridViewAllBooks_MouseClick);
            this->dataGridViewAllBooks->SelectionChanged += gcnew EventHandler(this, &MyForm::dataGridViewAllBooks_SelectionChanged);

            // Настройка кликов по графикам
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
        // Элементы управления
        System::Windows::Forms::ComboBox^ comboBoxGenre;      // Жанр
        System::Windows::Forms::ComboBox^ comboBoxMetric;     // Метрика для 3-го графика
        System::Windows::Forms::Label^ labelMetric;           // Подпись метрики
        System::Windows::Forms::Label^ labelGenre;            // Подпись жанра
        System::Windows::Forms::Button^ buttonAnalyze;        // Кнопка анализа
        System::Windows::Forms::Button^ buttonLoadFile;       // Кнопка загрузки
        System::Windows::Forms::DataGridView^ dataGridViewAllBooks; // Таблица всех книг
        System::Windows::Forms::PictureBox^ pictureBoxChart1; // График рейтинга
        System::Windows::Forms::PictureBox^ pictureBoxChart2; // График отзывов
        System::Windows::Forms::PictureBox^ pictureBoxChart3; // График распределения
        System::Windows::Forms::StatusStrip^ statusStrip;     // Строка состояния
        System::Windows::Forms::ToolStripStatusLabel^ statusLabel; // Метка состояния
        System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel; // Панель для графиков
        System::Windows::Forms::SplitContainer^ splitContainer; // Разделитель
        System::Windows::Forms::Panel^ panelControls;          // Панель управления
        System::Windows::Forms::ContextMenuStrip^ contextMenu; // Контекстное меню
        System::Windows::Forms::ToolStripMenuItem^ copyMenuItem; // Пункт меню копирования
        System::Windows::Forms::Button^ buttonClearFilter;     // Кнопка сброса фильтра
        System::Windows::Forms::Panel^ panelChart3Controls;    // Панель для элементов управления 3-го графика
        System::ComponentModel::Container^ components;

        // Данные
        List<Book^>^ books;           // Все книги
        List<Book^>^ currentDisplayBooks; // Текущие отображаемые книги
        String^ currentFileName;       // Текущий файл
        String^ currentSelectedGenre;  // Выбранный жанр
        String^ currentSelectedMetric; // Выбранная метрика для 3-го графика
        bool isDataLoaded;             // Флаг загрузки данных
        bool isResizing;               // Флаг изменения размера
        bool isProgrammaticSelection;  // Флаг программного выделения

        // Для интерактивных графиков
        List<ChartClickRegion^>^ chart1Regions;
        List<ChartClickRegion^>^ chart2Regions;
        List<ChartClickRegion^>^ chart3Regions;
        String^ activeFilter;          // Текущий активный фильтр
        String^ activeFilterValue;     // Значение текущего фильтра
        int activeChart;               // Номер активного графика (1, 2, 3) или 0 если фильтр не активен

        // Для подсветки на графиках
        List<String^>^ highlightedAuthors;
        List<int>^ highlightedYears;
        List<String^>^ highlightedPriceRanges;
        List<String^>^ highlightedRatingRanges;

        // Для информации о выбранной книге
        Book^ selectedBook;

        // Вспомогательные методы
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

            // Контекстное меню для копирования
            this->copyMenuItem->Text = L"Копировать";
            this->copyMenuItem->Click += gcnew System::EventHandler(this, &MyForm::copyMenuItem_Click);
            this->contextMenu->Items->Add(this->copyMenuItem);

            // panelControls
            this->panelControls->Controls->Add(this->labelGenre);
            this->panelControls->Controls->Add(this->comboBoxGenre);
            this->panelControls->Controls->Add(this->buttonLoadFile);
            this->panelControls->Controls->Add(this->buttonAnalyze);
            this->panelControls->Controls->Add(this->buttonClearFilter);
            this->panelControls->Dock = System::Windows::Forms::DockStyle::Top;
            this->panelControls->Height = 50;
            this->panelControls->Name = L"panelControls";

            // labelGenre
            this->labelGenre->Location = System::Drawing::Point(12, 15);
            this->labelGenre->Name = L"labelGenre";
            this->labelGenre->Size = System::Drawing::Size(50, 20);
            this->labelGenre->Text = L"Жанр:";
            this->labelGenre->TextAlign = System::Drawing::ContentAlignment::MiddleRight;

            // comboBoxGenre
            this->comboBoxGenre->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            array<String^>^ genres = gcnew array<String^> { "Художественная", "Нехудожественная" };
            this->comboBoxGenre->Items->AddRange(genres);
            this->comboBoxGenre->Location = System::Drawing::Point(70, 12);
            this->comboBoxGenre->Name = L"comboBoxGenre";
            this->comboBoxGenre->Size = System::Drawing::Size(140, 24);
            this->comboBoxGenre->TabIndex = 0;
            this->comboBoxGenre->SelectedIndex = 0;
            this->comboBoxGenre->Enabled = false;

            // buttonLoadFile
            this->buttonLoadFile->Location = System::Drawing::Point(220, 12);
            this->buttonLoadFile->Name = L"buttonLoadFile";
            this->buttonLoadFile->Size = System::Drawing::Size(120, 23);
            this->buttonLoadFile->TabIndex = 1;
            this->buttonLoadFile->Text = L"Загрузить CSV...";
            this->buttonLoadFile->UseVisualStyleBackColor = true;
            this->buttonLoadFile->Click += gcnew System::EventHandler(this, &MyForm::buttonLoadFile_Click);

            // buttonAnalyze
            this->buttonAnalyze->Location = System::Drawing::Point(350, 12);
            this->buttonAnalyze->Name = L"buttonAnalyze";
            this->buttonAnalyze->Size = System::Drawing::Size(100, 23);
            this->buttonAnalyze->TabIndex = 2;
            this->buttonAnalyze->Text = L"Анализ";
            this->buttonAnalyze->UseVisualStyleBackColor = true;
            this->buttonAnalyze->Click += gcnew System::EventHandler(this, &MyForm::buttonAnalyze_Click);
            this->buttonAnalyze->Enabled = false;

            // buttonClearFilter
            this->buttonClearFilter->Location = System::Drawing::Point(460, 12);
            this->buttonClearFilter->Name = L"buttonClearFilter";
            this->buttonClearFilter->Size = System::Drawing::Size(120, 23);
            this->buttonClearFilter->TabIndex = 4;
            this->buttonClearFilter->Text = L"Сбросить фильтр";
            this->buttonClearFilter->UseVisualStyleBackColor = true;
            this->buttonClearFilter->Click += gcnew System::EventHandler(this, &MyForm::buttonClearFilter_Click);
            this->buttonClearFilter->Enabled = false;

            // panelChart3Controls - панель для элементов управления 3-го графика
            this->panelChart3Controls->Controls->Add(this->labelMetric);
            this->panelChart3Controls->Controls->Add(this->comboBoxMetric);
            this->panelChart3Controls->BackColor = System::Drawing::Color::LightGray;
            this->panelChart3Controls->Height = 30;
            this->panelChart3Controls->Dock = System::Windows::Forms::DockStyle::Bottom;

            // labelMetric (перемещена на панель 3-го графика)
            this->labelMetric->Location = System::Drawing::Point(5, 5);
            this->labelMetric->Name = L"labelMetric";
            this->labelMetric->Size = System::Drawing::Size(100, 20);
            this->labelMetric->Text = L"Показатель:";
            this->labelMetric->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;

            // comboBoxMetric (перемещен на панель 3-го графика)
            this->comboBoxMetric->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            array<String^>^ metrics = gcnew array<String^> {
                "По авторам",
                    "По цене",
                    "По рейтингу"
            };
            this->comboBoxMetric->Items->AddRange(metrics);
            this->comboBoxMetric->Location = System::Drawing::Point(110, 3);
            this->comboBoxMetric->Name = L"comboBoxMetric";
            this->comboBoxMetric->Size = System::Drawing::Size(120, 24);
            this->comboBoxMetric->TabIndex = 3;
            this->comboBoxMetric->SelectedIndex = 0;
            this->comboBoxMetric->Enabled = false;

            // splitContainer
            this->splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer->Location = System::Drawing::Point(0, 50);
            this->splitContainer->Name = L"splitContainer";
            this->splitContainer->Size = System::Drawing::Size(1200, 650);
            this->splitContainer->SplitterDistance = 450;
            this->splitContainer->TabIndex = 4;

            // splitContainer.Panel1 - DataGridView
            this->splitContainer->Panel1->Controls->Add(this->dataGridViewAllBooks);
            this->dataGridViewAllBooks->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridViewAllBooks->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridViewAllBooks->Name = L"dataGridViewAllBooks";
            this->dataGridViewAllBooks->RowHeadersWidth = 51;
            this->dataGridViewAllBooks->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
            this->dataGridViewAllBooks->AllowUserToAddRows = false;
            this->dataGridViewAllBooks->AllowUserToDeleteRows = false;
            this->dataGridViewAllBooks->ReadOnly = true;
            this->dataGridViewAllBooks->ContextMenuStrip = this->contextMenu;
            this->dataGridViewAllBooks->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            this->dataGridViewAllBooks->MultiSelect = false;
            this->dataGridViewAllBooks->ClipboardCopyMode = DataGridViewClipboardCopyMode::EnableAlwaysIncludeHeaderText;

            // splitContainer.Panel2 - Charts
            this->splitContainer->Panel2->Controls->Add(this->tableLayoutPanel);

            // tableLayoutPanel для графиков
            this->tableLayoutPanel->ColumnCount = 1;
            this->tableLayoutPanel->RowCount = 3;
            this->tableLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tableLayoutPanel->Name = L"tableLayoutPanel";
            this->tableLayoutPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 33.33f));
            this->tableLayoutPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 33.33f));
            this->tableLayoutPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 33.33f));

            // pictureBoxChart1 - Рейтинг
            this->pictureBoxChart1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxChart1->Name = L"pictureBoxChart1";
            this->pictureBoxChart1->TabStop = false;
            this->pictureBoxChart1->BackColor = System::Drawing::Color::White;
            this->pictureBoxChart1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pictureBoxChart1->Cursor = Cursors::Hand;
            this->pictureBoxChart1->Tag = 1;

            // pictureBoxChart2 - Отзывы
            this->pictureBoxChart2->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxChart2->Name = L"pictureBoxChart2";
            this->pictureBoxChart2->TabStop = false;
            this->pictureBoxChart2->BackColor = System::Drawing::Color::White;
            this->pictureBoxChart2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pictureBoxChart2->Cursor = Cursors::Hand;
            this->pictureBoxChart2->Tag = 2;

            // pictureBoxChart3 - Распределение с панелью управления
            this->pictureBoxChart3->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxChart3->TabStop = false;
            this->pictureBoxChart3->BackColor = System::Drawing::Color::White;
            this->pictureBoxChart3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pictureBoxChart3->Cursor = Cursors::Hand;
            this->pictureBoxChart3->Tag = 3;

            // Добавляем графики в TableLayoutPanel
            this->tableLayoutPanel->Controls->Add(this->pictureBoxChart1, 0, 0);
            this->tableLayoutPanel->Controls->Add(this->pictureBoxChart2, 0, 1);

            // Для 3-го графика создаем составной элемент
            System::Windows::Forms::Panel^ panelChart3 = gcnew System::Windows::Forms::Panel();
            panelChart3->Dock = System::Windows::Forms::DockStyle::Fill;
            panelChart3->Controls->Add(this->pictureBoxChart3);
            panelChart3->Controls->Add(this->panelChart3Controls);
            this->tableLayoutPanel->Controls->Add(panelChart3, 0, 2);

            // statusStrip
            this->statusStrip->Items->AddRange(gcnew cli::array<System::Windows::Forms::ToolStripItem^>(1) { this->statusLabel });
            this->statusStrip->Location = System::Drawing::Point(0, 700);
            this->statusStrip->Name = L"statusStrip";
            this->statusStrip->Size = System::Drawing::Size(1200, 22);
            this->statusStrip->TabIndex = 5;
            this->statusStrip->Text = L"statusStrip";

            // statusLabel
            this->statusLabel->Name = L"statusLabel";
            this->statusLabel->Size = System::Drawing::Size(1185, 17);
            this->statusLabel->Text = L"Готов. Нажмите 'Загрузить CSV...' для выбора файла.";

            // MyForm
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1200, 722);
            this->Controls->Add(this->splitContainer);
            this->Controls->Add(this->panelControls);
            this->Controls->Add(this->statusStrip);
            this->Name = L"MyForm";
            this->Text = L"Анализатор CSV - Книжные бестселлеры Amazon 2009-2019";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->MinimumSize = System::Drawing::Size(1000, 600);

            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewAllBooks))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxChart1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxChart2))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxChart3))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer))->EndInit();
            this->splitContainer->ResumeLayout(false);
            this->statusStrip->ResumeLayout(false);
            this->statusStrip->PerformLayout();
            this->tableLayoutPanel->ResumeLayout(false);
            this->panelControls->ResumeLayout(false);
            this->contextMenu->ResumeLayout(false);
            this->ResumeLayout(false);
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
                    if (dataGridViewAllBooks->Rows[hit->RowIndex]->Selected)
                    {
                        // Если строка уже выделена, снимаем выделение
                        dataGridViewAllBooks->ClearSelection();
                        selectedBook = nullptr;
                        ClearHighlights();
                        RefreshCurrentView();
                    }
                    else
                    {
                        // Выделяем новую строку
                        dataGridViewAllBooks->ClearSelection();
                        dataGridViewAllBooks->Rows[hit->RowIndex]->Selected = true;
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

            if (dataGridViewAllBooks->SelectedRows->Count > 0)
            {
                DataGridViewRow^ selectedRow = dataGridViewAllBooks->SelectedRows[0];
                if (selectedRow->Index < currentDisplayBooks->Count)
                {
                    selectedBook = currentDisplayBooks[selectedRow->Index];

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

                    statusLabel->Text = "Выбрана книга: " + selectedBook->Name + " (" + selectedBook->Year + ")";
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

        // Получение ценового диапазона
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

            // Проверяем, кликнули ли по какой-либо области
            for each (ChartClickRegion ^ region in regions)
            {
                if (region->Bounds.Contains(e->Location))
                {
                    // Если кликнули по той же области, что и активный фильтр - сбрасываем
                    if (activeChart == chartNumber && activeFilter == region->Type && activeFilterValue == region->Value)
                    {
                        ClearFilter();
                    }
                    else
                    {
                        // Устанавливаем новый фильтр
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

            // Подсвечиваем активный график
            HighlightActiveChart(chartNumber);

            // Обновляем таблицу с отфильтрованными данными
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

            // Убираем подсветку со всех графиков
            ResetChartHighlights();

            // Возвращаем обычное отображение
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
            currentDisplayBooks->Clear();

            // Применяем дополнительный фильтр от графика
            if (activeFilter == "Year")
            {
                int year = Int32::Parse(activeFilterValue);
                for each (Book ^ book in baseFiltered)
                {
                    if (book->Year == year)
                        currentDisplayBooks->Add(book);
                }
            }
            else if (activeFilter == "Author")
            {
                for each (Book ^ book in baseFiltered)
                {
                    if (book->Author == activeFilterValue)
                        currentDisplayBooks->Add(book);
                }
            }
            else if (activeFilter == "PriceRange")
            {
                for each (Book ^ book in baseFiltered)
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
                for each (Book ^ book in baseFiltered)
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
                // Если фильтр не активен, показываем все книги
                for each (Book ^ book in baseFiltered)
                {
                    currentDisplayBooks->Add(book);
                }
            }

            if (currentDisplayBooks->Count > 0)
            {
                array<Book^>^ sortedByRating = currentDisplayBooks->ToArray();
                Array::Sort(sortedByRating, gcnew Comparison<Book^>(&MyForm::CompareBooksByRating));

                UpdateAllBooksGrid(sortedByRating);

                if (!String::IsNullOrEmpty(activeFilter))
                {
                    statusLabel->Text = "Показано " + currentDisplayBooks->Count + " книг. " + GetFilterDescription();
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

                // Сохраняем текущее состояние
                String^ savedFilter = activeFilter;
                String^ savedFilterValue = activeFilterValue;
                int savedActiveChart = activeChart;

                // Сохраняем подсветки
                List<String^>^ savedAuthors = gcnew List<String^>();
                for each (String ^ author in highlightedAuthors)
                    savedAuthors->Add(author);

                List<int>^ savedYears = gcnew List<int>();
                for each (int year in highlightedYears)
                    savedYears->Add(year);

                List<String^>^ savedPriceRanges = gcnew List<String^>();
                for each (String ^ range in highlightedPriceRanges)
                    savedPriceRanges->Add(range);

                List<String^>^ savedRatingRanges = gcnew List<String^>();
                for each (String ^ range in highlightedRatingRanges)
                    savedRatingRanges->Add(range);

                Book^ savedSelectedBook = selectedBook;

                // Перерисовываем графики с новыми размерами
                List<Book^>^ filtered = FilterBooksByGenre(currentSelectedGenre);
                DrawCharts(filtered);

                // Восстанавливаем состояние фильтра
                activeFilter = savedFilter;
                activeFilterValue = savedFilterValue;
                activeChart = savedActiveChart;

                // Восстанавливаем подсветки
                highlightedAuthors->Clear();
                for each (String ^ author in savedAuthors)
                    highlightedAuthors->Add(author);

                highlightedYears->Clear();
                for each (int year in savedYears)
                    highlightedYears->Add(year);

                highlightedPriceRanges->Clear();
                for each (String ^ range in savedPriceRanges)
                    highlightedPriceRanges->Add(range);

                highlightedRatingRanges->Clear();
                for each (String ^ range in savedRatingRanges)
                    highlightedRatingRanges->Add(range);

                selectedBook = savedSelectedBook;

                // Если был активный фильтр, применяем его
                if (!String::IsNullOrEmpty(activeFilter))
                {
                    UpdateFilteredView();
                    HighlightActiveChart(activeChart);
                }
                else if (highlightedAuthors->Count > 0 || highlightedYears->Count > 0 ||
                    highlightedPriceRanges->Count > 0 || highlightedRatingRanges->Count > 0)
                {
                    // Если есть подсветки, перерисовываем графики с ними
                    DrawCharts(filtered);
                }

                isResizing = false;
            }
        }

        System::Void comboBoxGenre_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
        {
            if (isDataLoaded && comboBoxGenre->SelectedItem != nullptr)
            {
                // Преобразуем русские названия обратно в английские для фильтрации
                String^ selectedGenre = comboBoxGenre->SelectedItem->ToString();
                if (selectedGenre == "Художественная")
                    currentSelectedGenre = "Fiction";
                else if (selectedGenre == "Нехудожественная")
                    currentSelectedGenre = "Non Fiction";

                // Сбрасываем фильтр, подсветки и выделение при смене жанра
                ClearFilter();
                ClearHighlights();
                selectedBook = nullptr;
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

                // Перерисовываем только 3-й график
                if (isDataLoaded)
                {
                    List<Book^>^ filtered = FilterBooksByGenre(currentSelectedGenre);

                    // Очищаем и перерисовываем только 3-й график
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
            currentDisplayBooks->Clear();
            for each (Book ^ book in filtered)
            {
                currentDisplayBooks->Add(book);
            }

            if (filtered->Count > 0)
            {
                array<Book^>^ sortedByRating = filtered->ToArray();
                Array::Sort(sortedByRating, gcnew Comparison<Book^>(&MyForm::CompareBooksByRating));

                isProgrammaticSelection = true;
                UpdateAllBooksGrid(sortedByRating);
                isProgrammaticSelection = false;

                DrawCharts(filtered);

                statusLabel->Text = "Показано " + filtered->Count + " книг в жанре \"" +
                    (currentSelectedGenre == "Fiction" ? "Художественная" : "Нехудожественная") + "\"";
            }
        }

        List<Book^>^ FilterBooksByGenre(String^ genre)
        {
            List<Book^>^ filtered = gcnew List<Book^>();
            for each (Book ^ book in books)
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

                // Очищаем предыдущие данные
                books->Clear();
                currentDisplayBooks->Clear();
                dataGridViewAllBooks->Rows->Clear();
                dataGridViewAllBooks->ClearSelection();
                selectedBook = nullptr;

                // Очищаем изображения и освобождаем ресурсы
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

                // Очищаем области кликов и подсветки
                chart1Regions->Clear();
                chart2Regions->Clear();
                chart3Regions->Clear();
                ClearHighlights();

                // Принудительно вызываем сборщик мусора для освобождения ресурсов
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

                int validBooks = 0;
                for (int i = 1; i < lines->Length; i++)
                {
                    String^ line = lines[i];
                    if (String::IsNullOrWhiteSpace(line))
                        continue;

                    Book^ book = ParseCSVLine(line);
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

        Book^ ParseCSVLine(String^ line)
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

                    double rating;
                    if (Double::TryParse(fields[2], rating))
                        book->UserRating = rating;
                    else
                        book->UserRating = 0.0;

                    int reviews;
                    if (Int32::TryParse(fields[3], reviews))
                        book->Reviews = reviews;
                    else
                        book->Reviews = 0;

                    double price;
                    if (Double::TryParse(fields[4], price))
                        book->Price = price;
                    else
                        book->Price = 0.0;

                    int year;
                    if (Int32::TryParse(fields[5], year))
                        book->Year = year;
                    else
                        book->Year = 0;

                    book->Genre = fields[6]->Trim('"');

                    return book;
                }
            }
            catch (Exception^) {
                // Игнорируем ошибки парсинга отдельных строк
            }

            return nullptr;
        }

        System::Void buttonAnalyze_Click(System::Object^ sender, System::EventArgs^ e)
        {
            ClearFilter();
            ClearHighlights();
            selectedBook = nullptr;
            dataGridViewAllBooks->ClearSelection();
            RefreshCurrentView();
        }

        void UpdateAllBooksGrid(array<Book^>^ sorted)
        {
            dataGridViewAllBooks->Columns->Clear();
            dataGridViewAllBooks->Rows->Clear();

            dataGridViewAllBooks->ColumnCount = 5;
            dataGridViewAllBooks->Columns[0]->Name = "№";
            dataGridViewAllBooks->Columns[0]->Width = 40;
            dataGridViewAllBooks->Columns[1]->Name = "Название";
            dataGridViewAllBooks->Columns[1]->Width = 300;
            dataGridViewAllBooks->Columns[2]->Name = "Автор";
            dataGridViewAllBooks->Columns[2]->Width = 200;
            dataGridViewAllBooks->Columns[3]->Name = "Рейтинг";
            dataGridViewAllBooks->Columns[3]->Width = 70;
            dataGridViewAllBooks->Columns[4]->Name = "Год";
            dataGridViewAllBooks->Columns[4]->Width = 70;

            for (int i = 0; i < sorted->Length; i++)
            {
                Book^ book = sorted[i];
                dataGridViewAllBooks->Rows->Add(
                    (i + 1).ToString(),
                    book->Name,
                    book->Author,
                    book->UserRating.ToString("F1"),
                    book->Year.ToString()
                );
            }
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

            for each (Book ^ book in filtered)
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

            // Освобождаем предыдущее изображение
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

            for each (Book ^ book in filtered)
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

            // Освобождаем предыдущее изображение
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
                title = "Распределение по цене";
                DrawPriceDistributionWithRegions(g, bmp->Width, bmp->Height, filtered, title, chart3Regions);
            }
            else if (currentSelectedMetric == "Rating")
            {
                title = "Распределение по рейтингу";
                DrawRatingDistributionWithRegions(g, bmp->Width, bmp->Height, filtered, title, chart3Regions);
            }

            // Освобождаем предыдущее изображение
            if (pictureBoxChart3->Image != nullptr)
                delete pictureBoxChart3->Image;

            pictureBoxChart3->Image = bmp;
            delete g;
        }

        void DrawAuthorDistributionWithRegions(System::Drawing::Graphics^ g, int width, int height,
            List<Book^>^ filtered, String^ title, List<ChartClickRegion^>^ regions)
        {
            Dictionary<String^, int>^ authorCount = gcnew Dictionary<String^, int>();

            for each (Book ^ book in filtered)
            {
                if (!authorCount->ContainsKey(book->Author))
                {
                    authorCount->Add(book->Author, 0);
                }
                authorCount[book->Author]++;
            }

            // Сортируем авторов по количеству книг
            array<KeyValuePair<String^, int>>^ authors = gcnew array<KeyValuePair<String^, int>>(authorCount->Count);
            int index = 0;
            for each (KeyValuePair<String^, int> pair in authorCount)
            {
                authors[index++] = pair;
            }

            // Используем отдельный класс для сравнения вместо лямбды
            Array::Sort(authors, gcnew Comparison<KeyValuePair<String^, int>>(&AuthorComparer::Compare));

            // Берем топ-10 авторов
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

            for each (Book ^ book in filtered)
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
            for each (KeyValuePair<String^, int> pair in priceRanges)
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

            for each (Book ^ book in filtered)
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
            for each (KeyValuePair<String^, int> pair in ratingRanges)
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

            // Заголовок
            System::Drawing::Font^ titleFont = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
            g->DrawString(title, titleFont, System::Drawing::Brushes::Black,
                width / 2 - 100, 5);

            int minYear = Int32::MaxValue;
            int maxYear = Int32::MinValue;
            for each (int year in sum->Keys)
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
            for each (int year in sum->Keys)
            {
                double avgValue = sum[year] / count[year];
                if (avgValue > maxValue) maxValue = avgValue;
            }

            if (maxY > 0)
                maxValue = maxY;

            if (maxValue == 0) maxValue = 1;

            // Подпись оси Y
            System::Drawing::Font^ axisFont = gcnew System::Drawing::Font("Arial", 8);
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

            int pointRadius = 12; // Увеличили радиус для лучшей кликабельности

            for each (int year in years)
            {
                double avgValue = sum[year] / count[year];

                int x = margin + (int)((year - minYear) / yearRange * chartWidth);
                int y = height - margin - (int)((avgValue / maxValue) * chartHeight);

                x = Math::Max(margin, Math::Min(width - margin, x));
                y = Math::Max(margin + 20, Math::Min(height - margin, y));

                // Определяем цвет точки (подсвечиваем, если год выделен)
                Color pointColor = color;
                if (highlightedYears->Contains(year))
                {
                    pointColor = Color::Red;
                }

                // Рисуем точку
                g->FillEllipse(gcnew System::Drawing::SolidBrush(pointColor), x - pointRadius / 2, y - pointRadius / 2, pointRadius, pointRadius);

                // Добавляем область для клика (увеличили зону клика)
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

                // Подпись года (более компактная, без наложения)
                if (years->Length < 10 || (year % 2 == 0))
                {
                    System::Drawing::Font^ yearFont = gcnew System::Drawing::Font("Arial", 7);
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

            // Заголовок
            System::Drawing::Font^ titleFont = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
            g->DrawString(title, titleFont, System::Drawing::Brushes::Black,
                width / 2 - 100, 5);

            int minYear = Int32::MaxValue;
            int maxYear = Int32::MinValue;
            int maxValue = 0;

            for each (int year in data->Keys)
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

            // Подпись оси Y
            System::Drawing::Font^ axisFont = gcnew System::Drawing::Font("Arial", 8);
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

            for each (int year in years)
            {
                int x = margin + (int)((year - minYear) / yearRange * chartWidth) - barWidth / 2;
                int barHeight = (int)((double)data[year] / maxValue * chartHeight);

                x = Math::Max(margin, Math::Min(width - margin - barWidth, x));

                if (barHeight > 0)
                {
                    // Определяем цвет столбца (подсвечиваем, если год выделен)
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

                    // Добавляем область для клика (увеличили зону клика на 10 пикселей по высоте)
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

                // Подпись года (более компактная, без наложения)
                if (years->Length < 10 || (year % 2 == 0))
                {
                    System::Drawing::Font^ yearFont = gcnew System::Drawing::Font("Arial", 7);
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

            // Рисуем оси
            System::Drawing::Pen^ blackPen = gcnew System::Drawing::Pen(System::Drawing::Color::Black, 1);
            g->DrawLine(blackPen, margin, margin + 20, margin, height - margin);
            g->DrawLine(blackPen, margin, height - margin, width - margin, height - margin);

            // Заголовок
            System::Drawing::Font^ titleFont = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
            g->DrawString(title, titleFont, System::Drawing::Brushes::Black,
                width / 2 - 100, 5);

            // Находим максимальное значение
            int maxValue = 0;
            for (int i = 0; i < count; i++)
            {
                if (data[i].Value > maxValue)
                    maxValue = data[i].Value;
            }
            if (maxValue == 0) maxValue = 1;

            // Рисуем сетку
            System::Drawing::Pen^ gridPen = gcnew System::Drawing::Pen(System::Drawing::Color::LightGray, 1);
            gridPen->DashStyle = System::Drawing::Drawing2D::DashStyle::Dash;

            System::Drawing::Font^ axisFont = gcnew System::Drawing::Font("Arial", 8);

            for (int i = 0; i <= 5; i++)
            {
                int y = height - margin - (int)((double)i / 5 * chartHeight);
                g->DrawLine(gridPen, margin, y, width - margin, y);

                int value = (int)((double)i / 5 * maxValue);
                g->DrawString(value.ToString(), axisFont, System::Drawing::Brushes::Black, margin - 40, y - 8);
            }

            // Рисуем столбцы
            int barWidth = (chartWidth - 20) / count;
            if (barWidth > 50) barWidth = 50;

            for (int i = 0; i < count; i++)
            {
                int x = margin + 20 + i * (barWidth + 5);
                int barHeight = (int)((double)data[i].Value / maxValue * chartHeight);

                // Определяем цвет столбца (подсвечиваем, если автор/диапазон выделены)
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

                    // Добавляем область для клика (увеличили зону клика)
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

                    // Подпись значения над столбцом (только для топ-3, чтобы избежать наложения)
                    if (i < 3 || data[i].Value > maxValue * 0.7)
                    {
                        g->DrawString(data[i].Value.ToString(), axisFont, System::Drawing::Brushes::Black,
                            x, height - margin - barHeight - 20);
                    }
                }

                // Подпись под столбцом (обрезаем длинные названия)
                String^ label = data[i].Key;
                if (label->Length > 10)
                    label = label->Substring(0, 8) + "…";

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