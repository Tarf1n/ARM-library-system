#pragma once
#include <fstream>

using namespace System;
using namespace System::IO;

public ref class Book {
private:
    String^ _title;
    String^ _author;
    int _year;
    bool _isAvailable;
    String^ _readerName;
    String^ _readerPhone;
    DateTime _issueDate;
    DateTime _dueDate;

public:
    //базовый конструктор
    Book() : _year(0), _isAvailable(true), _issueDate(DateTime::MinValue), _dueDate(DateTime::MinValue) {}

    property String^ Title{
        String ^ get() { return _title; }
        void set(String ^ value) { _title = value; }
    }

        property String^ Author{
            String ^ get() { return _author; }
            void set(String ^ value) { _author = value; }
    }

        property int Year{
            int get() { return _year; }
            void set(int value) { _year = value; }
    }

        property bool IsAvailable{
            bool get() { return _isAvailable; }
            void set(bool value) { _isAvailable = value; }
    }

        property String^ ReaderName{
            String ^ get() { return _readerName; }
            void set(String ^ value) { _readerName = value; }
    }

        property String^ ReaderPhone{
            String ^ get() { return _readerPhone; }
            void set(String ^ value) { _readerPhone = value; }
    }

        property DateTime IssueDate{
            DateTime get() { return _issueDate; }
            void set(DateTime value) { _issueDate = value; }
    }

        property DateTime DueDate{
            DateTime get() { return _dueDate; }
            void set(DateTime value) { _dueDate = value; }
    }

        //Сохранение книг в файл (при добавлении новой книги в программе)
        void SaveToFile(StreamWriter^ sw) {
        if (sw == nullptr) return;

        String^ issueTicks = (IssueDate > DateTime::MinValue) ? IssueDate.Ticks.ToString() : "0";
        String^ dueTicks = (DueDate > DateTime::MinValue) ? DueDate.Ticks.ToString() : "0";

        sw->WriteLine(String::Format("{0},{1},{2},{3},{4},{5},{6},{7}",
            _title, _author, _year, _isAvailable,
            _readerName, _readerPhone,
            issueTicks, dueTicks));
    }

    //Выгрузка книг из файла в систему
    static Book^ LoadFromFile(StreamReader^ sr) {
        if (sr == nullptr) return gcnew Book();

        String^ line = sr->ReadLine();
        if (String::IsNullOrEmpty(line)) return gcnew Book();

        array<String^>^ parts = line->Split(',');
        if (parts->Length < 6) return gcnew Book();

        Book^ book = gcnew Book();
        book->Title = parts[0];
        book->Author = parts[1];

        // Обработка года
        if (parts->Length > 2 && !String::IsNullOrEmpty(parts[2])) {
            book->Year = Int32::Parse(parts[2]);
        }

        // Обработка наличия
        if (parts->Length > 3 && !String::IsNullOrEmpty(parts[3])) {
            book->IsAvailable = Boolean::Parse(parts[3]);
        }

        // Остальные поля
        if (parts->Length > 4) book->ReaderName = parts[4];
        if (parts->Length > 5) book->ReaderPhone = parts[5];

        // Обработка дат
        if (parts->Length > 6 && !String::IsNullOrEmpty(parts[6])) {
            Int64 ticks = Int64::Parse(parts[6]);
            if (ticks > 0) {
                book->IssueDate = DateTime(ticks);
            }
        }

        if (parts->Length > 7 && !String::IsNullOrEmpty(parts[7])) {
            Int64 ticks = Int64::Parse(parts[7]);
            if (ticks > 0) {
                book->DueDate = DateTime(ticks);
            }
        }

        return book;
    }
};




