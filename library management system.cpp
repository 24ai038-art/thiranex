#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Book {
    int bookID;
    string title;
    string author;
    bool issued;

public:
    Book() : bookID(0), issued(false) {}

    void addBook() {
        cout << "\nEnter Book ID: ";
        cin >> bookID;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        issued = false;
    }

    void showBook() const {
        cout << "\nBook ID: " << bookID;
        cout << "\nTitle: " << title;
        cout << "\nAuthor: " << author;
        cout << "\nStatus: " << (issued ? "Issued" : "Available") << endl;
    }

    void issueBook() {
        if (!issued) {
            issued = true;
            cout << "\nBook issued successfully!";
        } else {
            cout << "\nBook already issued!";
        }
    }

    void returnBook() {
        if (issued) {
            issued = false;
            cout << "\nBook returned successfully!";
        } else {
            cout << "\nBook was not issued!";
        }
    }

    int getBookID() const { return bookID; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isIssued() const { return issued; }

    void writeToFile(ofstream &out) {
        out.write(reinterpret_cast<char*>(this), sizeof(*this));
    }

    void readFromFile(ifstream &in) {
        in.read(reinterpret_cast<char*>(this), sizeof(*this));
    }
};

// Add new book
void addBook() {
    Book b;
    b.addBook();
    ofstream file("library.dat", ios::binary | ios::app);
    b.writeToFile(file);
    file.close();
    cout << "\nBook added successfully!\n";
}

// Display all books
void displayBooks() {
    ifstream file("library.dat", ios::binary);
    Book b;
    cout << "\n--- All Books ---\n";
    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        b.showBook();
        cout << "----------------------\n";
    }
    file.close();
}

// Issue book
void issueBook() {
    int id;
    cout << "\nEnter Book ID to issue: ";
    cin >> id;

    fstream file("library.dat", ios::binary | ios::in | ios::out);
    Book b;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.getBookID() == id) {
            b.issueBook();
            file.seekp(-static_cast<int>(sizeof(b)), ios::cur);
            file.write(reinterpret_cast<char*>(&b), sizeof(b));
            found = true;
            break;
        }
    }
    file.close();

    if (!found) cout << "\nBook not found!\n";
}

// Return book
void returnBook() {
    int id;
    cout << "\nEnter Book ID to return: ";
    cin >> id;

    fstream file("library.dat", ios::binary | ios::in | ios::out);
    Book b;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.getBookID() == id) {
            b.returnBook();
            file.seekp(-static_cast<int>(sizeof(b)), ios::cur);
            file.write(reinterpret_cast<char*>(&b), sizeof(b));
            found = true;
            break;
        }
    }
    file.close();

    if (!found) cout << "\nBook not found!\n";
}

// Search by title or author
void searchBook() {
    string keyword;
    cin.ignore();
    cout << "\nEnter Title or Author keyword: ";
    getline(cin, keyword);

    ifstream file("library.dat", ios::binary);
    Book b;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.getTitle().find(keyword) != string::npos || 
            b.getAuthor().find(keyword) != string::npos) {
            b.showBook();
            found = true;
        }
    }
    file.close();

    if (!found) cout << "\nNo matching book found!\n";
}

// Menu-driven system
int main() {
    int choice;
    do {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Search Book by Title/Author\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: searchBook(); break;
            case 6: cout << "\nExiting program...\n"; break;
            default: cout << "\nInvalid choice! Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
