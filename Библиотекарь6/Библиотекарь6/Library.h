#pragma once
#include "Book.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

public ref class Library {
private:
    List<Book^>^ books;
    DateTime currentDate;

public:
    //�����������
    Library() {
        books = gcnew List<Book^>();
        currentDate = DateTime::Now;
        LoadBooks();
    }
    //����������� ����
    property DateTime CurrentDate{
        DateTime get() { return currentDate; }
        void set(DateTime value) { currentDate = value; }
    }

        //�������� ���� �� �����
        void LoadBooks() {
        if (!File::Exists("books.txt")) return;

        StreamReader^ sr = gcnew StreamReader("books.txt");
        while (!sr->EndOfStream) {
            Book^ book = Book::LoadFromFile(sr);
            if (book != nullptr) {
                books->Add(book);
            }
        }
        sr->Close();
    }

    //������� ���������� ����
    void SaveBooks() {
        StreamWriter^ sw = gcnew StreamWriter("books.txt");
        for each(Book ^ book in books) {
            book->SaveToFile(sw);
        }
        sw->Close();
    }

    //������� ���������� ���� � ����������
    void AddBook(String^ title, String^ author, int year) {
        Book^ newBook = gcnew Book();
        newBook->Title = title;
        newBook->Author = author;
        newBook->Year = year;
        newBook->IsAvailable = true;
        books->Add(newBook);
        SaveBooks();
    }

    //������� �������� ���� �� ����������
    void DeleteBook(int index) {
        if (index >= 0 && index < books->Count) {
            books->RemoveAt(index);
            SaveBooks();
        }
    }

    //������� ������������� ����� ��������
    void LendBook(int index, String^ readerName, String^ readerPhone, int daysToReturn) {
        if (index >= 0 && index < books->Count && books[index]->IsAvailable) {
            books[index]->IsAvailable = false;
            books[index]->ReaderName = readerName; //��� ��������
            books[index]->ReaderPhone = readerPhone; //����� ��������
            books[index]->IssueDate = currentDate; //���� ���������
            books[index]->DueDate = currentDate.AddDays(daysToReturn); //����
            SaveBooks();
        }
    }

    //������� ����� ���������
    void ReturnBook(int index) {
        if (index >= 0 && index < books->Count && !books[index]->IsAvailable) {
            books[index]->IsAvailable = true;
            books[index]->ReaderName = "";
            books[index]->ReaderPhone = "";
            books[index]->IssueDate = DateTime::MinValue;  // ���������� ����
            books[index]->DueDate = DateTime::MinValue;    // ���������� ����
            SaveBooks();
        }
    }

    //������� ������ ����� ���������
    List<Book^>^ SearchBooks(String^ query) {
        List<Book^>^ results = gcnew List<Book^>();
        String^ lowerQuery = query->ToLower();

        for each(Book ^ book in books) {
            if (book->Title->ToLower()->Contains(lowerQuery) ||
                book->Author->ToLower()->Contains(lowerQuery) ||
                book->ReaderName->ToLower()->Contains(lowerQuery) ||
                book->ReaderPhone->Contains(query))
            {
                results->Add(book);
            }
        }
        return results;
    }

    //�������� ������ ���� ����
    List<Book^>^ GetAllBooks() {
        return books;
    }

    //�������� �� ��������� ��������
    String^ CheckOverdueBooks() {
        String^ result = "";
        for each(Book ^ book in books) {
            if (!book->IsAvailable && currentDate > book->DueDate) {
                result += String::Format("�������� {0} �� ��������� ����� '{1}' (�� {2}). �������: {3}\n",
                    book->ReaderName, book->Title, book->DueDate.ToShortDateString(), book->ReaderPhone);
            }
        }
        return result;
    }
};

