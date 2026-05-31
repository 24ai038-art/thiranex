#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class BankAccount {
    int accountNumber;
    string name;
    double balance;

public:
    BankAccount() : accountNumber(0), balance(0.0) {}

    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cin.ignore();
        cout << "Enter Account Holder Name: ";
        getline(cin, name);
        cout << "Enter Initial Balance: ";
        cin >> balance;
    }

    void showAccount() const {
        cout << "\nAccount Number: " << accountNumber;
        cout << "\nName: " << name;
        cout << "\nBalance: " << balance << endl;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "\nAmount deposited successfully!";
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            cout << "\nAmount withdrawn successfully!";
        } else {
            cout << "\nInsufficient balance!";
        }
    }

    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
    string getName() const { return name; }

    void writeToFile(ofstream &out) {
        out.write(reinterpret_cast<char*>(this), sizeof(*this));
    }

    void readFromFile(ifstream &in) {
        in.read(reinterpret_cast<char*>(this), sizeof(*this));
    }
};

// Add new account
void addAccount() {
    BankAccount acc;
    acc.createAccount();
    ofstream file("bank.dat", ios::binary | ios::app);
    acc.writeToFile(file);
    file.close();
    cout << "\nAccount created successfully!\n";
}

// Display all accounts
void displayAccounts() {
    ifstream file("bank.dat", ios::binary);
    BankAccount acc;
    cout << "\n--- All Accounts ---\n";
    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        acc.showAccount();
        cout << "----------------------\n";
    }
    file.close();
}

// Deposit money
void depositMoney() {
    int accNo;
    double amount;
    cout << "\nEnter Account Number: ";
    cin >> accNo;
    cout << "Enter Amount to Deposit: ";
    cin >> amount;

    fstream file("bank.dat", ios::binary | ios::in | ios::out);
    BankAccount acc;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.getAccountNumber() == accNo) {
            acc.deposit(amount);
            file.seekp(-static_cast<int>(sizeof(acc)), ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(acc));
            found = true;
            break;
        }
    }
    file.close();

    if (!found) cout << "\nAccount not found!\n";
}

// Withdraw money
void withdrawMoney() {
    int accNo;
    double amount;
    cout << "\nEnter Account Number: ";
    cin >> accNo;
    cout << "Enter Amount to Withdraw: ";
    cin >> amount;

    fstream file("bank.dat", ios::binary | ios::in | ios::out);
    BankAccount acc;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.getAccountNumber() == accNo) {
            acc.withdraw(amount);
            file.seekp(-static_cast<int>(sizeof(acc)), ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(acc));
            found = true;
            break;
        }
    }
    file.close();

    if (!found) cout << "\nAccount not found!\n";
}

// Balance inquiry
void balanceInquiry() {
    int accNo;
    cout << "\nEnter Account Number: ";
    cin >> accNo;

    ifstream file("bank.dat", ios::binary);
    BankAccount acc;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.getAccountNumber() == accNo) {
            cout << "\nAccount Holder: " << acc.getName();
            cout << "\nBalance: " << acc.getBalance() << endl;
            found = true;
            break;
        }
    }
    file.close();

    if (!found) cout << "\nAccount not found!\n";
}

// Menu-driven system
int main() {
    int choice;
    do {
        cout << "\n===== Bank Management System =====\n";
        cout << "1. Create Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. Balance Inquiry\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addAccount(); break;
            case 2: displayAccounts(); break;
            case 3: depositMoney(); break;
            case 4: withdrawMoney(); break;
            case 5: balanceInquiry(); break;
            case 6: cout << "\nExiting program...\n"; break;
            default: cout << "\nInvalid choice! Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
