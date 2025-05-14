#pragma once

#include "Library.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::IO;

namespace LibraryApp {

    public ref class Form1 : public Form {
    private:

        Library^ library;
        DataGridView^ dataGridView;
        TextBox^ txtTitle, ^ txtAuthor, ^ txtYear, ^ txtReader, ^ txtPhone, ^ txtSearch;
        Button^ btnAdd, ^ btnDelete, ^ btnLend, ^ btnReturn, ^ btnSearch, ^ btnDateUpdate;
        NumericUpDown^ numDaysToReturn;
        DateTimePicker^ dateTimePicker;
        Label^ lblOverdue;

        void InitializeComponent() {
            this->Text = "Библиотечная система";
            this->Size = Drawing::Size(800, 600);

            //Определяем дату и срок
            DateTime issueDate = DateTime::Now;
            DateTime dueDate = issueDate.AddDays(14);

            // Преобразуем в ticks
            long long issueTicks = issueDate.Ticks;
            long long dueTicks = dueDate.Ticks;

            // DataGridView
            dataGridView = gcnew DataGridView();
            dataGridView->Dock = DockStyle::Top;
            dataGridView->Height = 400;
            dataGridView->ReadOnly = true;
            dataGridView->AllowUserToAddRows = false;
            dataGridView->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            dataGridView->Columns->Add("Title", "Название");
            dataGridView->Columns->Add("Author", "Автор");
            dataGridView->Columns->Add("Year", "Год");
            dataGridView->Columns->Add("Status", "Статус");
            dataGridView->Columns->Add("Reader", "Читатель");
            dataGridView->Columns->Add("Phone", "Телефон");
            dataGridView->Columns->Add("IssueDate", "Дата выдачи");
            dataGridView->Columns->Add("DueDate", "Срок возврата");
            dataGridView->Columns->Add("Overdue", "Просрочено");

            // Текстовые поля
            txtTitle = gcnew TextBox();
            txtTitle->Text = "Название";
            txtTitle->Width = 150;

            txtAuthor = gcnew TextBox();
            txtAuthor->Text = "Автор";
            txtAuthor->Width = 150;

            txtYear = gcnew TextBox();
            txtYear->Text = "Год";
            txtYear->Width = 80;

            txtReader = gcnew TextBox();
            txtReader->Text = "Имя читателя";
            txtReader->Width = 150;

            txtPhone = gcnew TextBox();
            txtPhone->Text = "Телефон";
            txtPhone->Width = 120;

            txtSearch = gcnew TextBox();
            txtSearch->Width = 200;

            // Числовое поле
            numDaysToReturn = gcnew NumericUpDown();
            numDaysToReturn->Minimum = 1;
            numDaysToReturn->Maximum = 365;
            numDaysToReturn->Value = 14;
            numDaysToReturn->Width = 50;

            // DateTimePicker
            dateTimePicker = gcnew DateTimePicker();
            dateTimePicker->Width = 120;

            // Кнопки
            btnAdd = gcnew Button();
            btnAdd->Text = "Добавить";
            btnAdd->Width = 80;

            btnDelete = gcnew Button();
            btnDelete->Text = "Удалить";
            btnDelete->Width = 80;

            btnLend = gcnew Button();
            btnLend->Text = "Выдать";
            btnLend->Width = 80;

            btnReturn = gcnew Button();
            btnReturn->Text = "Вернуть";
            btnReturn->Width = 80;

            btnSearch = gcnew Button();
            btnSearch->Text = "Поиск";
            btnSearch->Width = 80;

            btnDateUpdate = gcnew Button();
            btnDateUpdate->Text = "Обновить дату";
            btnDateUpdate->Width = 100;

            // Label для просроченных книг
            lblOverdue = gcnew Label();
            lblOverdue->Dock = DockStyle::Bottom;
            lblOverdue->Height = 40;
            lblOverdue->ForeColor = Color::Red;

            // Панель управления
            FlowLayoutPanel^ panel = gcnew FlowLayoutPanel();
            panel->FlowDirection = FlowDirection::LeftToRight;
            panel->Dock = DockStyle::Bottom;
            panel->Height = 60;

            // Создаем Label для "Срок:"
            Label^ lblDueDate = gcnew Label();
            lblDueDate->Text = "Срок:";
            lblDueDate->AutoSize = true;

            // Добавляем элементы на панель
            panel->Controls->Add(txtTitle);
            panel->Controls->Add(txtAuthor);
            panel->Controls->Add(txtYear);
            panel->Controls->Add(txtReader);
            panel->Controls->Add(txtPhone);
            panel->Controls->Add(lblDueDate);
            panel->Controls->Add(numDaysToReturn);
            panel->Controls->Add(dateTimePicker);
            panel->Controls->Add(btnDateUpdate);
            panel->Controls->Add(txtSearch);
            panel->Controls->Add(btnSearch);
            panel->Controls->Add(btnAdd);
            panel->Controls->Add(btnDelete);
            panel->Controls->Add(btnLend);
            panel->Controls->Add(btnReturn);

            // Добавляем элементы на форму
            this->Controls->Add(dataGridView);
            this->Controls->Add(panel);
            this->Controls->Add(lblOverdue);

            // Инициализация библиотеки
            library = gcnew Library();
            dateTimePicker->Value = library->CurrentDate;
            RefreshDataGrid();

            // Подписка на события
            btnAdd->Click += gcnew EventHandler(this, &Form1::OnAddClick);
            btnDelete->Click += gcnew EventHandler(this, &Form1::OnDeleteClick);
            btnLend->Click += gcnew EventHandler(this, &Form1::OnLendClick);
            btnReturn->Click += gcnew EventHandler(this, &Form1::OnReturnClick);
            btnSearch->Click += gcnew EventHandler(this, &Form1::OnSearchClick);
            btnDateUpdate->Click += gcnew EventHandler(this, &Form1::OnDateUpdateClick);
        }

        void RefreshDataGrid() {
            RefreshDataGrid(library->GetAllBooks());
        }

        void RefreshDataGrid(List<Book^>^ booksToShow) {
            dataGridView->Rows->Clear();
            for each(Book ^ book in booksToShow) {
                String^ issueDateStr = "";
                String^ dueDateStr = "";

                if (book->IssueDate > DateTime::MinValue) {
                    issueDateStr = book->IssueDate.ToShortDateString();
                }

                if (book->DueDate > DateTime::MinValue) {
                    dueDateStr = book->DueDate.ToShortDateString();
                }

                dataGridView->Rows->Add(
                    book->Title,
                    book->Author,
                    book->Year,
                    book->IsAvailable ? "Доступна" : "Выдана",
                    book->ReaderName,
                    book->ReaderPhone,
                    issueDateStr, 
                    dueDateStr,
                    (!book->IsAvailable && library->CurrentDate > book->DueDate) ? "ДА" : ""
                );
            }
            CheckOverdue();
        }

        //Просрочка книг (появляется перед запуском системы, как предупреждение)
        void CheckOverdue() {
            String^ overdue = library->CheckOverdueBooks();
            lblOverdue->Text = overdue;
            if (!String::IsNullOrEmpty(overdue)) {
                MessageBox::Show(overdue, "Просроченные книги", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            }
        }

        //События для нажатия кнопок
        void OnAddClick(Object^ sender, EventArgs^ e) {
            if (!String::IsNullOrEmpty(txtTitle->Text) && txtTitle->Text != "Название") {
                int year = 0;
                Int32::TryParse(txtYear->Text, year);
                library->AddBook(txtTitle->Text, txtAuthor->Text, year);
                RefreshDataGrid();
                txtTitle->Text = "Название";
                txtAuthor->Text = "Автор";
                txtYear->Text = "Год";
            }
        }

        void OnDeleteClick(Object^ sender, EventArgs^ e) {
            if (dataGridView->SelectedRows->Count > 0) {
                library->DeleteBook(dataGridView->SelectedRows[0]->Index);
                RefreshDataGrid();
            }
        }

        void OnLendClick(Object^ sender, EventArgs^ e) {
            if (dataGridView->SelectedRows->Count > 0 &&
                !String::IsNullOrEmpty(txtReader->Text) &&
                txtReader->Text != "Имя читателя" &&
                !String::IsNullOrEmpty(txtPhone->Text) &&
                txtPhone->Text != "Телефон")
            {
                library->LendBook(
                    dataGridView->SelectedRows[0]->Index,
                    txtReader->Text,
                    txtPhone->Text,
                    (int)numDaysToReturn->Value
                );
                RefreshDataGrid();
                txtReader->Text = "Имя читателя";
                txtPhone->Text = "Телефон";
            }
        }

        void OnReturnClick(Object^ sender, EventArgs^ e) {
            if (dataGridView->SelectedRows->Count > 0) {
                library->ReturnBook(dataGridView->SelectedRows[0]->Index);
                RefreshDataGrid();
            }
        }

        void OnSearchClick(Object^ sender, EventArgs^ e) {
            if (!String::IsNullOrEmpty(txtSearch->Text)) {
                List<Book^>^ results = library->SearchBooks(txtSearch->Text);
                RefreshDataGrid(results);
            }
            else {
                RefreshDataGrid();
            }
        }

        void OnDateUpdateClick(Object^ sender, EventArgs^ e) {
            library->CurrentDate = dateTimePicker->Value;
            RefreshDataGrid();
        }

    public:
        //Запуск библиотечной системы
        Form1() {
            InitializeComponent();
        }
    };
}



