#include <iostream>
#include <string>
using namespace std;

const int MAX_CATEGORIES = 7;

struct Category {
    string name;
    bool highPriority;
    float allocatedBudget;
    float spent;
};

Category categories[MAX_CATEGORIES] = {
    {"Food"},
    {"Transport"},
    {"Accessories"},
    {"Clothes"},
    {"Bills"},
    {"Savings"},
    {"Misc"}
};

float monthlyBudget;
float remainingBudget;

//protypes
void setMonthlyBudget();
void setPriorities();
void allocateBudget();
void showBudgetAllocation();
void addDailyExpense();
void readjustBudget();
void dailySummary();
void monthlySummary();
void showMenu();


int main() {
    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            addDailyExpense();
            break;
        case 2:
            dailySummary();
            break;
        case 3:
            monthlySummary();
            break;
        case 0:
            cout << "\nThank you for using Smart Expense Manager!\n";
            break;
        default:
            cout << " Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}

void showMenu() {
    cout << "\nMENU\n";
    cout << "1. Add Daily Expense\n";
    cout << "2. View Daily Summary\n";
    cout << "3. View Monthly Summary\n";
    cout << "0. Exit\n";
    cout << "Choose option: ";
}