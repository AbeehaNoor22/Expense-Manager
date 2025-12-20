#include <iostream>
#include <string>
using namespace std;

const int maxCategories = 7;

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
             << " (" << (categories[i].highPriority ? "High" : "Low")
             << ") : Rs " << categories[i].allocatedBudget << endl;
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
