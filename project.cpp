#include <iostream>
#include <string>
using namespace std;

const int maxCategories = 7; 
const int max_expenses = 300; //limiting max expense input for a month

struct Category {
    string name;
    bool highPriority;
    float allocatedBudget;
    float spent;
};

Category categories[maxCategories] = {
    {"Food"},
    {"Transport"},
    {"Accessories"},
    {"Clothes"},
    {"Bills"},
    {"Savings"},
    {"Misc"}
};

struct Expense { //struct for expense input function
    int day, month, year;
    string category;
    float amount;
};

Expense expenses[max_expenses]; //array to store expenses
int expenseCount = 0; 


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

    setMonthlyBudget();
    setPriorities();
    allocateBudget();
    showBudgetAllocation();

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
void setMonthlyBudget() {
    cout << "Enter monthly budget: Rs ";
    cin >> monthlyBudget;

    while (monthlyBudget <= 0) { //input validation
        cout << "Invalid amount! Enter again: ";
        cin >> monthlyBudget;
    }

    remainingBudget = monthlyBudget;
}
void setPriorities() {
    int input;

    cout << "\nSet category priorities\n";
    cout << "(1 = High Priority, 0 = Low Priority)\n\n";

    for (int i = 0; i < maxCategories; i++) {
        do {
            cout << categories[i].name << ": ";
            cin >> input;

            if (input != 0 && input != 1) //input validation
                cout << "Enter ONLY 1 or 0!\n";

        } while (input != 0 && input != 1);

        categories[i].highPriority = input;
        categories[i].spent = 0;
    }
}

void showBudgetAllocation() {
    cout << "\n Budget Allocation \n";
    for (int i = 0; i < maxCategories; i++) {
        cout << categories[i].name
            << " (" << (categories[i].highPriority ? "High" : "Low") << ") : Rs " << categories[i].allocatedBudget << endl;
    }
}

void allocateBudget() {
    int highCount = 0, lowCount = 0;

    for (int i = 0; i < maxCategories; i++) {
        if (categories[i].highPriority)
            highCount++;
        else
            lowCount++;
    }

    float highShare = (highCount > 0) ? (0.7f * monthlyBudget) / highCount : 0;
    float lowShare = (lowCount > 0) ? (0.3f * monthlyBudget) / lowCount : 0;

    for (int i = 0; i < maxCategories; i++) {
        if (categories[i].highPriority)
            categories[i].allocatedBudget = highShare;
        else
            categories[i].allocatedBudget = lowShare;
    }
}

void readjustBudget() {
    cout << "!! Equal redistribution of remaining budget\n";

    float equalShare = remainingBudget / maxCategories;

    for (int i = 0; i < maxCategories; i++) {
        categories[i].allocatedBudget = equalShare;
    }
}
void addDailyExpense() {
    int day, month, year;
    int choice;

    cout << "\nEnter date (DD MM YYYY): ";
    cin >> day >> month >> year;

    while (true) {
        cout << "\nSelect category:\n";
        for (int i = 0; i < maxCategories; i++) {   //displaying all categories
            cout << i + 1 << ". " << categories[i].name << endl;
        }
        cout << "or 0 to stop\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) //exit condition
            break;

        if (choice < 0 || choice > maxCategories) {
            cout << "Invalid category!\n";
            continue;
        }

        int index = choice - 1; //storing the choice
        float amount;
        do {        //amount input
            cout << "Enter amount for " << categories[index].name << ": Rs ";
            cin >> amount;
            if (amount <= 0) {  //input validation
                cout << "Invalid amount!\n";
            }
        } while (amount <= 0);

        float dailyLimit = categories[index].allocatedBudget / 30; //daily budget for the category

        if (amount > dailyLimit) {
            cout << "Daily limit exceeded!Re-adjusting budgets...\n";
            readjustBudget();
        }

        Expense e = {   //initializing struct
        day,month,year,categories[index].name,amount
        };

        expenses[expenseCount++] = e; //expenses are being stored in the array,with index equal to the expense count

        categories[index].spent += amount;
        categories[index].allocatedBudget -= amount;
        remainingBudget -= amount;

        cout << "Expense added successfully!\n";
    }
}
void monthlySummary() {
    float totalSpent = monthlyBudget - remainingBudget;

    cout << "\nMONTHLY SUMMARY\n"; //general summary
    cout << "Monthly Budget: Rs " << monthlyBudget << endl;
    cout << "Total Spent: Rs " << totalSpent << endl;
    cout << "Remaining Budget: Rs " << remainingBudget << endl;

    cout << "\nCategory-wise spending:\n"; //category wise summary
    for (int i = 0; i <maxCategories; i++) {
        cout << categories[i].name<<" : Rs " << categories[i].spent << endl;
    }
}
