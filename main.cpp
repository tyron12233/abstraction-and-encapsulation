#include <iostream>
#include <limits>

#ifdef _WIN32
#include <io.h>
#endif

#include <fcntl.h>

using namespace std;

void pauseScreen() {
    wcout << "\nPress Enter to continue...";
    wcin.ignore(numeric_limits<streamsize>::max(), '\n');
    wcin.get();
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * An abstract base class BankAccount with pure virtual functions for deposit, withdraw, and checking balance.
 */
class BankAccount {
protected:
    double balance;

public:
    explicit BankAccount(double initial_balance) : balance(initial_balance) {
    }

    virtual ~BankAccount() = default;

    // Pure virtual functions for deposit, withdraw, and checking balance
    // pure virtual functions must be overridden by derived classes

    [[nodiscard]] virtual string getName() const = 0;

    virtual void deposit(double amount) = 0;

    virtual bool withdraw(double amount) = 0;

    [[nodiscard]] virtual double checkBalance() const = 0;
};


/**
 * Subclass of BankAccount which implements the deposit, withdraw, and checkBalance functions
 * for a Savings Account.
 */
class SavingsAccount final : public BankAccount {
    const double MIN_BALANCE = 1000.0;

public:
    explicit SavingsAccount(const double initial_balance) : BankAccount(initial_balance) {
    }

    void deposit(const double amount) override {
        if (amount <= 0) {
            wcout << "Invalid deposit amount. Please enter a positive value." << endl;
        } else {
            balance += amount;
            wcout << "Deposited: " << amount << ". New balance: " << balance << endl;
        }
    }

    bool withdraw(const double amount) override {
        if (amount <= 0) {
            wcout << "Invalid withdrawal amount." << endl;
            return false;
        }
        if (balance - amount < MIN_BALANCE) {
            wcout << "Insufficient balance. Minimum balance of 1000 must be maintained." << endl;
            return false;
        }
        balance -= amount;
        wcout << "Withdrawn: " << amount << ". New balance: " << balance << endl;
        return true;
    }

    [[nodiscard]] double checkBalance() const override {
        return balance;
    }

    [[nodiscard]] string getName() const override {
        return "Savings Account";
    }
};

class CurrentAccount final : public BankAccount {
public:
    explicit CurrentAccount(const double initial_balance) : BankAccount(initial_balance) {
    }

    void deposit(const double amount) override {
        if (amount <= 0) {
            wcout << "Invalid deposit amount. Please enter a positive value." << endl;
        } else {
            balance += amount;
            wcout << "Deposited: " << amount << ". New balance: " << balance << endl;
        }
    }

    bool withdraw(const double amount) override {
        if (amount <= 0) {
            wcout << "Invalid withdrawal amount." << endl;
            return false;
        }
        if (balance - amount < 0) {
            wcout << "Insufficient balance to complete the withdrawal." << endl;
            return false;
        }

        balance -= amount;
        wcout << "Withdrawn: " << amount << ". New balance: " << balance << endl;
        return true;
    }

    [[nodiscard]] double checkBalance() const override {
        return balance;
    }

    [[nodiscard]] string getName() const override {
        return "Current Account";
    }
};

// Helper function for input validation
double getValidAmount() {
    double amount;
    while (true) {
        wcin >> amount;
        if (wcin.fail() || amount < 0) {
            wcout << "Invalid input. Please enter a valid amount: ";
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            return amount;
        }
    }
}


void subMenu(BankAccount *account, const int choice) {
    int subChoice;
    do {
        clearScreen();
        if (choice == 1) {
            wcout << L"┌──────────────────────────────┐\n";
            wcout << L"│     Savings Account Menu     │\n";
            wcout << L"└──────────────────────────────┘\n\n";
        } else if (choice == 2) {
            wcout << L"┌──────────────────────────────┐\n";
            wcout << L"│     Current Account Menu     │\n";
            wcout << L"└──────────────────────────────┘\n\n";
        }

        wcout << "[1] Deposit" << endl;
        wcout << "[2] Withdraw" << endl;
        wcout << "[3] Check Balance" << endl;
        wcout << "[4] Back" << endl;
        wcout << "Select an option: ";
        wcin >> subChoice;

        if (wcin.fail()) {
            clearScreen();

            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), '\n');
            wcout << "Invalid input. Please enter a valid option." << endl;
            pauseScreen();
            continue;
        }

        switch (subChoice) {
            case 1: {
                clearScreen();
                wcout << L"┌─────────────────────────────────┐\n";
                wcout << L"│     Enter amount to deposit     │\n";
                wcout << L"└─────────────────────────────────┘\n\n";
                wcout << "Amount: ";
                const double amount = getValidAmount();
                account->deposit(amount);
                pauseScreen();
                break;
            }
            case 2: {
                clearScreen();
                wcout << L"┌──────────────────────────────────┐\n";
                wcout << L"│     Enter amount to withdraw     │\n";
                wcout << L"└──────────────────────────────────┘\n\n";
                wcout << "Amount: ";
                const double amount = getValidAmount();
                account->withdraw(amount);
                pauseScreen();
                break;
            }
            case 3: {
                clearScreen();
                wcout << L"┌─────────────────────────┐\n";
                wcout << L"│     Current Balance     │\n";
                wcout << L"└─────────────────────────┘\n\n";
                wcout << "Balance: " << account->checkBalance() << endl;
                pauseScreen();
                break;
            }
            case 4:
                clearScreen();
                wcout << "Returning to Main Menu..." << endl;
                pauseScreen();
                break;
            default:
                wcout << "Invalid option. Please try again." << endl;
                pauseScreen();
        }
    } while (subChoice != 4);
}

int main() {
    // Set console to support Unicode characters
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT);
#else
    setlocale(LC_ALL, "");
#endif


    SavingsAccount savings(2000.0);
    CurrentAccount current(3000.0);


    BankAccount *account = nullptr;

    int choice;
    do {
        clearScreen();
        wcout << L"┌───────────────────────┐\n";
        wcout << L"│       Main Menu       │\n";
        wcout << L"└───────────────────────┘\n\n";

        wcout << "[1] Savings Account" << endl;
        wcout << "[2] Current Account" << endl;
        wcout << "[3] Exit" << endl << endl;
        wcout << "Select an option: ";
        wcin >> choice;

        if (wcin.fail()) {
            clearScreen();

            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), '\n');
            wcout << "Invalid input. Please enter a valid option." << endl;
            pauseScreen();
            continue;
        }

        switch (choice) {
            case 1: {
                account = &savings;
                break;
            }
            case 2: {
                account = &current;
                break;
            }
            case 3:
                wcout << "Exiting the system. Goodbye!" << endl;
                break;
            default:
                wcout << "Invalid option. Please try again." << endl;
                pauseScreen();
                continue;
        }

        if (account == nullptr) {
            continue;
        }

        subMenu(account, choice);
    } while (choice != 3);
}