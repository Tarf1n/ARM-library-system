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
    //Конструктор
    Library() {
        books = gcnew List<Book^>();
        currentDate = DateTime::Now;
        LoadBooks();
    }
    //Определение даты
    property DateTime CurrentDate{
        DateTime get() { return currentDate; }
        void set(DateTime value) { currentDate = value; }
    }

        //Загрузка книг из файла
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

    //Функция сохранения книг
    void SaveBooks() {
        StreamWriter^ sw = gcnew StreamWriter("books.txt");
        for each(Book ^ book in books) {
            book->SaveToFile(sw);
        }
        sw->Close();
    }

    //Функция добавления книг в библиотеку
    void AddBook(String^ title, String^ author, int year) {
        Book^ newBook = gcnew Book();
        newBook->Title = title;
        newBook->Author = author;
        newBook->Year = year;
        newBook->IsAvailable = true;
        books->Add(newBook);
        SaveBooks();
    }

    //Функция удаления книг из библиотеки
    void DeleteBook(int index) {
        if (index >= 0 && index < books->Count) {
            books->RemoveAt(index);
            SaveBooks();
        }
    }

    //Функция присваивающая книгу читателю
    void LendBook(int index, String^ readerName, String^ readerPhone, int daysToReturn) {
        if (index >= 0 && index < books->Count && books[index]->IsAvailable) {
            books[index]->IsAvailable = false;
            books[index]->ReaderName = readerName; //Имя читателя
            books[index]->ReaderPhone = readerPhone; //Номер читателя
            books[index]->IssueDate = currentDate; //Дата получения
            books[index]->DueDate = currentDate.AddDays(daysToReturn); //Срок
            SaveBooks();
        }
    }

    //Возврат книги читателем
    void ReturnBook(int index) {
        if (index >= 0 && index < books->Count && !books[index]->IsAvailable) {
            books[index]->IsAvailable = true;
            books[index]->ReaderName = "";
            books[index]->ReaderPhone = "";
            books[index]->IssueDate = DateTime::MinValue;  // Сбрасываем дату
            books[index]->DueDate = DateTime::MinValue;    // Сбрасываем дату
            SaveBooks();
        }
    }

    //Функция поиска книги читателем
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

    //Получаем список всех книг
    List<Book^>^ GetAllBooks() {
        return books;
    }

    //Проверка на просрочку возврата
    String^ CheckOverdueBooks() {
        String^ result = "";
        for each(Book ^ book in books) {
            if (!book->IsAvailable && currentDate > book->DueDate) {
                result += String::Format("Читатель {0} не возвратил книгу '{1}' (до {2}). Телефон: {3}\n",
                    book->ReaderName, book->Title, book->DueDate.ToShortDateString(), book->ReaderPhone);
            }
        }
        return result;
    }
};

