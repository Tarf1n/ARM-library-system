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
            this->Text = "������������ �������";
            this->Size = Drawing::Size(800, 600);

            //���������� ���� � ����
            DateTime issueDate = DateTime::Now;
            DateTime dueDate = issueDate.AddDays(14);

            // ����������� � ticks
            long long issueTicks = issueDate.Ticks;
            long long dueTicks = dueDate.Ticks;

            // DataGridView
            dataGridView = gcnew DataGridView();
            dataGridView->Dock = DockStyle::Top;
            dataGridView->Height = 400;
            dataGridView->ReadOnly = true;
            dataGridView->AllowUserToAddRows = false;
            dataGridView->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            dataGridView->Columns->Add("Title", "��������");
            dataGridView->Columns->Add("Author", "�����");
            dataGridView->Columns->Add("Year", "���");
            dataGridView->Columns->Add("Status", "������");
            dataGridView->Columns->Add("Reader", "��������");
            dataGridView->Columns->Add("Phone", "�������");
            dataGridView->Columns->Add("IssueDate", "���� ������");
            dataGridView->Columns->Add("DueDate", "���� ��������");
            dataGridView->Columns->Add("Overdue", "����������");

            // ��������� ����
            txtTitle = gcnew TextBox();
            txtTitle->Text = "��������";
            txtTitle->Width = 150;

            txtAuthor = gcnew TextBox();
            txtAuthor->Text = "�����";
            txtAuthor->Width = 150;

            txtYear = gcnew TextBox();
            txtYear->Text = "���";
            txtYear->Width = 80;

            txtReader = gcnew TextBox();
            txtReader->Text = "��� ��������";
            txtReader->Width = 150;

            txtPhone = gcnew TextBox();
            txtPhone->Text = "�������";
            txtPhone->Width = 120;

            txtSearch = gcnew TextBox();
            txtSearch->Width = 200;

            // �������� ����
            numDaysToReturn = gcnew NumericUpDown();
            numDaysToReturn->Minimum = 1;
            numDaysToReturn->Maximum = 365;
            numDaysToReturn->Value = 14;
            numDaysToReturn->Width = 50;

            // DateTimePicker
            dateTimePicker = gcnew DateTimePicker();
            dateTimePicker->Width = 120;

            // ������
            btnAdd = gcnew Button();
            btnAdd->Text = "��������";
            btnAdd->Width = 80;

            btnDelete = gcnew Button();
            btnDelete->Text = "�������";
            btnDelete->Width = 80;

            btnLend = gcnew Button();
            btnLend->Text = "������";
            btnLend->Width = 80;

            btnReturn = gcnew Button();
            btnReturn->Text = "�������";
            btnReturn->Width = 80;

            btnSearch = gcnew Button();
            btnSearch->Text = "�����";
            btnSearch->Width = 80;

            btnDateUpdate = gcnew Button();
            btnDateUpdate->Text = "�������� ����";
            btnDateUpdate->Width = 100;

            // Label ��� ������������ ����
            lblOverdue = gcnew Label();
            lblOverdue->Dock = DockStyle::Bottom;
            lblOverdue->Height = 40;
            lblOverdue->ForeColor = Color::Red;

            // ������ ����������
            FlowLayoutPanel^ panel = gcnew FlowLayoutPanel();
            panel->FlowDirection = FlowDirection::LeftToRight;
            panel->Dock = DockStyle::Bottom;
            panel->Height = 60;

            // ������� Label ��� "����:"
            Label^ lblDueDate = gcnew Label();
            lblDueDate->Text = "����:";
            lblDueDate->AutoSize = true;

            // ��������� �������� �� ������
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

            // ��������� �������� �� �����
            this->Controls->Add(dataGridView);
            this->Controls->Add(panel);
            this->Controls->Add(lblOverdue);

            // ������������� ����������
            library = gcnew Library();
            dateTimePicker->Value = library->CurrentDate;
            RefreshDataGrid();

            // �������� �� �������
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
                    book->IsAvailable ? "��������" : "������",
                    book->ReaderName,
                    book->ReaderPhone,
                    issueDateStr, 
                    dueDateStr,
                    (!book->IsAvailable && library->CurrentDate > book->DueDate) ? "��" : ""
                );
            }
            CheckOverdue();
        }

        //��������� ���� (���������� ����� �������� �������, ��� ��������������)
        void CheckOverdue() {
            String^ overdue = library->CheckOverdueBooks();
            lblOverdue->Text = overdue;
            if (!String::IsNullOrEmpty(overdue)) {
                MessageBox::Show(overdue, "������������ �����", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            }
        }

        //������� ��� ������� ������
        void OnAddClick(Object^ sender, EventArgs^ e) {
            if (!String::IsNullOrEmpty(txtTitle->Text) && txtTitle->Text != "��������") {
                int year = 0;
                Int32::TryParse(txtYear->Text, year);
                library->AddBook(txtTitle->Text, txtAuthor->Text, year);
                RefreshDataGrid();
                txtTitle->Text = "��������";
                txtAuthor->Text = "�����";
                txtYear->Text = "���";
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
                txtReader->Text != "��� ��������" &&
                !String::IsNullOrEmpty(txtPhone->Text) &&
                txtPhone->Text != "�������")
            {
                library->LendBook(
                    dataGridView->SelectedRows[0]->Index,
                    txtReader->Text,
                    txtPhone->Text,
                    (int)numDaysToReturn->Value
                );
                RefreshDataGrid();
                txtReader->Text = "��� ��������";
                txtPhone->Text = "�������";
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
        //������ ������������ �������
        Form1() {
            InitializeComponent();
        }
    };
}



