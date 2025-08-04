

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <conio.h> 

using namespace std;

struct Transaction {
    string type;
    double amount;
    string timestamp;
    string recipientIBAN; 
};

class Account {
private:
    string username;
    string password;
    string iban;
    double balance;
    vector<Transaction> history;

public:
    Account(string user, string pass, string ibanCode, double bal = 0.0)
        : username(user), password(pass), iban(ibanCode), balance(bal) {}

    bool login(string user, string pass) {
        return (user == username && pass == password);
    }

    void deposit(double amount) {
        balance += amount;
        log_transaction("Deposit", amount);
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient funds.\n";
            return false;
        }
        balance -= amount;
        log_transaction("Withdraw", amount);
        return true;
    }

    bool transfer(Account& recipient, double amount) {
        if (amount > balance) {
            cout << "Insufficient funds for transfer.\n";
            return false;
        }
        balance -= amount;
        recipient.balance += amount;
        log_transaction("Transfer", amount, recipient.iban);
        recipient.log_transaction("Received", amount, iban);
        return true;
    }

    void display_balance() {
        cout << fixed << setprecision(2);
        cout << "Current Balance: $" << balance << endl;
    }

    string get_iban() const {
        return iban;
    }

    void log_transaction(string type, double amount, string recipientIBAN = "-") {
        Transaction t = {type, amount, "2025-08-04", recipientIBAN};
        history.push_back(t);
    }

    void print_history() {
        cout << "\n--- Transaction History ---\n";
        for (auto& t : history) {
            cout << t.type << ": $" << t.amount << " on " << t.timestamp;
            if (t.recipientIBAN != "-")
                cout << " | IBAN: " << t.recipientIBAN;
            cout << endl;
        }
    }
};

string get_hidden_password() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        } else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

int main() {
    Account myAccount("samet", "1234", "TR0000000001", 1000.0);
    Account recipient("ahmet", "abcd", "TR0000000002", 500.0);

    string user, pass;
    cout << "Username: "; cin >> user;
    cout << "Password: ";
    pass = get_hidden_password();

    if (!myAccount.login(user, pass)) {
        cout << "Login failed.\n";
        return 1;
    }

    int choice;
    do {
        cout << "\n--- SecureBank Menu ---\n";
        cout << "1. View Balance\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Transaction History\n0. Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                myAccount.display_balance();
                break;
            case 2: {
                double amt;
                cout << "Amount to deposit: $";
                cin >> amt;
                myAccount.deposit(amt);
                break;
            }
            case 3: {
                double amt;
                cout << "Amount to withdraw: $";
                cin >> amt;
                myAccount.withdraw(amt);
                break;
            }
            case 4: {
                string iban;
                double amt;
                cout << "Recipient IBAN: ";
                cin >> iban;
                cout << "Amount to transfer: $";
                cin >> amt;
                if (iban == recipient.get_iban()) {
                    myAccount.transfer(recipient, amt);
                } else {
                    cout << "Recipient not found.\n";
                }
                break;
            }
            case 5:
                myAccount.print_history();
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
