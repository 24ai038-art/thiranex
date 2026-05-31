#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

struct Student {
    int id;
    string name;
    int age;
    string course;
};

// Function to add a student
void addStudent() {
    Student s;
    cout << "\nEnter Student ID: ";
    cin >> s.id;
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, s.name);
    cout << "Enter Age: ";
    cin >> s.age;
    cin.ignore();
    cout << "Enter Course: ";
    getline(cin, s.course);

    ofstream file("students.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&s), sizeof(s));
    file.close();

    cout << "\nStudent record added successfully!\n";
}

// Function to display all students
void displayStudents() {
    ifstream file("students.dat", ios::binary);
    Student s;
    cout << "\n--- Student Records ---\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name" 
         << setw(10) << "Age" << setw(20) << "Course" << endl;
    cout << "-------------------------------------------------------------\n";

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        cout << left << setw(10) << s.id 
             << setw(20) << s.name 
             << setw(10) << s.age 
             << setw(20) << s.course << endl;
    }
    file.close();
}

// Function to update a student
void updateStudent() {
    int id;
    cout << "\nEnter Student ID to update: ";
    cin >> id;

    fstream file("students.dat", ios::binary | ios::in | ios::out);
    Student s;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.id == id) {
            cout << "\nEnter new details:\n";
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, s.name);
            cout << "Enter Age: ";
            cin >> s.age;
            cin.ignore();
            cout << "Enter Course: ";
            getline(cin, s.course);

            file.seekp(-static_cast<int>(sizeof(s)), ios::cur);
            file.write(reinterpret_cast<char*>(&s), sizeof(s));
            cout << "\nRecord updated successfully!\n";
            found = true;
            break;
        }
    }
    file.close();

    if (!found) {
        cout << "\nStudent ID not found!\n";
    }
}

// Function to delete a student
void deleteStudent() {
    int id;
    cout << "\nEnter Student ID to delete: ";
    cin >> id;

    ifstream file("students.dat", ios::binary);
    ofstream temp("temp.dat", ios::binary);
    Student s;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.id != id) {
            temp.write(reinterpret_cast<char*>(&s), sizeof(s));
        } else {
            found = true;
        }
    }

    file.close();
    temp.close();

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found) {
        cout << "\nRecord deleted successfully!\n";
    } else {
        cout << "\nStudent ID not found!\n";
    }
}

// Menu-driven system
int main() {
    int choice;
    do {
        cout << "\n===== Student Management System =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display Students\n";
        cout << "3. Update Student\n";
        cout << "4. Delete Student\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: cout << "\nExiting program...\n"; break;
            default: cout << "\nInvalid choice! Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
