#include <iostream>
#include <string>
#include<chrono>
#include<thread>
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
	{ "Food", 0, 0.0, 0.0 },
	{ "Transport", 0, 0.0, 0.0 },
	{ "Accessories", 0, 0.0, 0.0 },
	{ "Clothes", 0, 0.0, 0.0 },
	{ "Bills", 0, 0.0, 0.0 },
	{ "Savings", 0, 0.0, 0.0 },
	{ "Others", 0, 0.0, 0.0 }
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
void allocateBudget(float); //added argument to make it more general
void showBudgetAllocation();
void addDailyExpense();
void readjustBudget();
void dailySummary();
void monthlySummary();
void showMenu();
void showPerDayExpense();
void waitSeconds(int sec);

int main() {
	int choice;

	setMonthlyBudget();
	setPriorities();
	allocateBudget(monthlyBudget);
	showBudgetAllocation();
	showPerDayExpense();

	do {
		showMenu();
		cin >> choice;

		switch (choice) {
		case 1:
			if (remainingBudget > 0) {
				addDailyExpense();
				break;
			}
			else {
				cout << "\nNo remaining budget. You cannot add more expenses.\n";
				break;
			}
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
	cout << "\nMENU\n"; waitSeconds(1);
	cout << "1. Add Daily Expense\n"; waitSeconds(1);
	cout << "2. View Daily Summary\n"; waitSeconds(1);
	cout << "3. View Monthly Summary\n"; waitSeconds(1);
	cout << "0. Exit\n"; waitSeconds(1);
	cout << "Choose option: ";
}
void setMonthlyBudget() {
	cout << "Enter monthly budget: Rs ";
	cin >> monthlyBudget;

	while (monthlyBudget <= 0) { //input validation
		cout << "Invalid amount! Enter again: "; waitSeconds(1);
		cin >> monthlyBudget;
	}

	remainingBudget = monthlyBudget;
}
void setPriorities() {
	int input;

	cout << "\nSet category priorities\n"; waitSeconds(1);
	cout << "(1 = High Priority, 0 = Low Priority)\n\n";

	for (int i = 0; i < maxCategories; i++) {
		do {
			cout << categories[i].name << ": ";
			cin >> input;

			if (input != 0 && input != 1) //input validation
				cout << "Enter ONLY 1 or 0!\n";

		} while (input != 0 && input != 1);

		categories[i].highPriority = input;
	}
}

void showBudgetAllocation() {
	cout << "\n Budget Allocation \n"; waitSeconds(1);
	for (int i = 0; i < maxCategories; i++) {
		cout << categories[i].name
			<< " (" << (categories[i].highPriority ? "High" : "Low") << ") : Rs " << categories[i].allocatedBudget << endl; waitSeconds(1);
	}
}

void showPerDayExpense() {
	cout << "\n Budget Allocation per Day \n"; waitSeconds(1);
	for (int i = 0; i < maxCategories; i++) {
		cout << categories[i].name << ": Rs " << categories[i].allocatedBudget / 30 << endl; waitSeconds(1);
	}
}

void allocateBudget(float total_budget) {
	int highCount = 0, lowCount = 0;

	for (int i = 0; i < maxCategories; i++) {
		if (categories[i].highPriority)
			highCount++;
		else
			lowCount++;
	}

	float highShare = (highCount > 0) ? (0.6f * total_budget) / highCount : 0;//changed the distribution to 60 /40 
	float lowShare = (lowCount > 0) ? (0.4f * total_budget) / lowCount : 0;

	for (int i = 0; i < maxCategories; i++) {
		if (categories[i].highPriority)
			categories[i].allocatedBudget = highShare;
		else
			categories[i].allocatedBudget = lowShare;
	}
}

void readjustBudget() {
	if (remainingBudget <= 0) { //added checks if budget ends
		cout << "\nNo remaining budget to redistribute.\n"; waitSeconds(1);
		return;
	}
	else {
		cout << "Equal redistribution of remaining budget\n"; waitSeconds(1);
		allocateBudget(remainingBudget);
	}
}
void addDailyExpense() {
	int day, month, year;
	int choice;

	cout << "\nEnter date (DD MM YYYY): ";
	cin >> day >> month >> year;

	while (true) {
		cout << "\nSelect category:\n"; waitSeconds(1);
		for (int i = 0; i < maxCategories; i++) {   //displaying all categories
			cout << i + 1 << ". " << categories[i].name << endl; waitSeconds(1);
		}
		cout << "or 0 to stop\n"; waitSeconds(1);
		cout << "Choice: ";
		cin >> choice;

		if (choice == 0) //exit condition
			break;

		if (choice < 0 || choice > maxCategories) {
			cout << "Invalid category!\n"; waitSeconds(1);
			continue;
		}

		int index = choice - 1; //storing the choice
		float dailyLimit = categories[index].allocatedBudget / 30;   //daily budget for the category		

		float amount;
		bool opt;
		do {        //amount input
		start:
			cout << "Enter amount you want to spend for " << categories[index].name << ": Rs ";
			cin >> amount;
			if (amount <= 0) {  //input validation
				cout << "Invalid amount!\n"; waitSeconds(1);
			}
			if (amount > dailyLimit) {//showing allert of a=exceeding limit and asking whether to proceeed or not
				cout << "This is more than the allocated budget!\n To proceed press '1' to enter another amount press '0'\n";
				cin >> opt;
				if (opt == 0) { goto start; }
				else break;
			}
		} while (amount <= 0);

		if (amount > dailyLimit) {
			cout << "Daily limit exceeded! Re-adjusting budgets...\n"; waitSeconds(2);
		}

		Expense e = { day, month, year, categories[index].name, amount };  //initializing struct
		expenses[expenseCount++] = e;   //expenses are being stored in the array,with index equal to the expense count

		// Deducting money first
		categories[index].spent += amount;
		remainingBudget -= amount;

		if (remainingBudget <= 0) { //exit condition if budget is exhausted
			cout << "\nBudget exhausted. Exiting expense entry.\n"; waitSeconds(1);
			break;
		}
		if (amount > dailyLimit) {//redistribute only if the expense is grater than the daily limit
			//redistributing remaining money
			readjustBudget();

			// displaying updated allocation
			showBudgetAllocation();
		}

		cout << "Expense added successfully!\n";
	}
}
void monthlySummary() {
	float totalSpent = monthlyBudget - remainingBudget;

	cout << "\nMONTHLY SUMMARY\n"; waitSeconds(1); //general summary
	cout << "Monthly Budget: Rs " << monthlyBudget << endl; waitSeconds(1);
	cout << "Total Spent: Rs " << totalSpent << endl; waitSeconds(1);
	cout << "Remaining Budget: Rs " << remainingBudget << endl; waitSeconds(1);

	cout << "\nCategory-wise spending:\n"; waitSeconds(1); //category wise summary
	for (int i = 0; i < maxCategories; i++) {
		cout << categories[i].name << " : Rs " << categories[i].spent << endl; waitSeconds(1);
	}
}
//daily summary
void dailySummary() {
	int d, m, y;
	float total = 0;

	cout << "\nEnter date (DD MM YYYY): "; waitSeconds(1);
	cin >> d >> m >> y;

	cout << "\nDAILY SUMMARY\n"; waitSeconds(1);

	for (int i = 0; i < expenseCount; i++) {
		if (expenses[i].day == d && expenses[i].month == m && expenses[i].year == y) {

			cout << expenses[i].category << " - Rs " << expenses[i].amount << endl; waitSeconds(1);

			total += expenses[i].amount;
		}
	}
	cout << "Total spent today: Rs " << total << endl; waitSeconds(1);
}
void waitSeconds(int sec) {
	std::this_thread::sleep_for(std::chrono::seconds(sec));
}