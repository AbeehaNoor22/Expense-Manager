#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>   // FILE HANDLING
using namespace std;

const int maxCategories = 7;
const int max_expenses = 300;

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

struct Expense {
	int day, month, year;
	string category;
	float amount;
};

Expense expenses[max_expenses];
int expenseCount = 0;

float monthlyBudget = 0;
float remainingBudget = 0;

// prototypes
void setMonthlyBudget();
void setPriorities();
void allocateBudget(float);
void showBudgetAllocation();
void addDailyExpense();
void readjustBudget();
void dailySummary();
void monthlySummary();
void showMenu();
void showPerDayExpense();
void waitSeconds(int);

// file handling
void saveData();
void loadData();

int main() {
	int choice;

	cout << "=============================\n";
	cout << "WELCOME TO EXPENSE MANAGER\n";
	cout << "=============================\n\n";

	loadData();   // LOAD DATA FROM FILE

	if (monthlyBudget == 0) {
		setMonthlyBudget();
		setPriorities();
		allocateBudget(monthlyBudget);
	}

	showBudgetAllocation();
	showPerDayExpense();

	do {
		showMenu();
		cin >> choice;

		switch (choice) {
		case 1:
			if (remainingBudget > 0)
				addDailyExpense();
			else
				cout << "\nNo remaining budget. You cannot add more expenses.\n";
			break;

		case 2:
			dailySummary();
			break;

		case 3:
			monthlySummary();
			break;

		case 0:
			saveData();   // SAVE DATA TO FILE
			cout << "\nThank you for using Smart Expense Manager!\n";
			break;

		default:
			cout << " Invalid choice!\n";
		}
	} while (choice != 0);

	return 0;
}

void showMenu() {
	cout << "\n=============================\n";
	cout << "\t  MENU\n";
	cout << "=============================\n";
	cout << "1. Add Daily Expense\n";
	cout << "2. View Daily Summary\n";
	cout << "3. View Monthly Summary\n";
	cout << "0. Exit\n";
	cout << "Choose option: ";
}

void setMonthlyBudget() {
	cout << "Enter monthly budget: Rs ";
	cin >> monthlyBudget;

	while (monthlyBudget <= 0) {
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
		} while (input != 0 && input != 1);

		categories[i].highPriority = input;
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

	float highShare = (highCount > 0) ? (0.6f * total_budget) / highCount : 0;
	float lowShare = (lowCount > 0) ? (0.4f * total_budget) / lowCount : 0;

	for (int i = 0; i < maxCategories; i++) {
		categories[i].allocatedBudget =
			categories[i].highPriority ? highShare : lowShare;
	}
}

void showBudgetAllocation() {
	cout << "\n=============================\n";
	cout << "     Budget Allocation\n";
	cout << "=============================\n";
	for (int i = 0; i < maxCategories; i++) {
		cout << categories[i].name
			<< " (" << (categories[i].highPriority ? "High" : "Low")
			<< ") : Rs " << categories[i].allocatedBudget << endl;
	}
}

void showPerDayExpense() {
	cout << "\n=============================\n";
	cout << "  Budget Allocation per Day\n";
	cout << "=============================\n";
	for (int i = 0; i < maxCategories; i++) {
		cout << categories[i].name << ": Rs "
			<< categories[i].allocatedBudget / 30 << endl;
	}
}

void readjustBudget() {
	if (remainingBudget > 0)
		allocateBudget(remainingBudget);
}

void addDailyExpense() {
	int day, month, year;
	int choice;

	cout << "\nEnter date (DD MM YYYY): ";
	cin >> day >> month >> year;

	while (true) {
		cout << "\nSelect category:\n";
		for (int i = 0; i < maxCategories; i++) {
			cout << i + 1 << ". " << categories[i].name << endl;
		}
		cout << "or 0 to stop\n";
		cout << "Choice: ";
		cin >> choice;

		if (choice == 0)
			break;

		if (choice < 1 || choice > maxCategories) {
			cout << "Invalid category!\n";
			continue;
		}

		int index = choice - 1;
		float dailyLimit = categories[index].allocatedBudget / 30;

		float amount;
		bool opt;

	start:
		cout << "Enter amount you want to spend for "
			<< categories[index].name << ": Rs ";
		cin >> amount;

		if (amount <= 0) {
			cout << "Invalid amount!\n";
			goto start;
		}

		if (amount > dailyLimit) {
			cout << "This is more than the allocated daily budget!\n";
			cout << "Press 1 to proceed anyway, 0 to re-enter amount: ";
			cin >> opt;

			if (opt == 0)
				goto start;

			cout << "Daily limit exceeded! Re-adjusting budgets...\n";
			waitSeconds(2);
		}

		// Store expense
		expenses[expenseCount++] =
		{ day, month, year, categories[index].name, amount };

		categories[index].spent += amount;
		remainingBudget -= amount;

		if (remainingBudget <= 0) {
			cout << "\nBudget exhausted. Exiting expense entry.\n";
			break;
		}

		if (amount > dailyLimit) {
			readjustBudget();
			showBudgetAllocation();
			showPerDayExpense();
		}

		cout << "Expense added successfully!\n";
	}
}


void monthlySummary() {
	float totalSpent = monthlyBudget - remainingBudget;
	cout << "\n=============================\n";
	cout << "\tMONTHLY SUMMARY\n";
	cout << "=============================\n";
	cout << "Monthly Budget: Rs " << monthlyBudget << endl;
	cout << "Total Spent: Rs " << totalSpent << endl;
	cout << "Remaining Budget: Rs " << remainingBudget << endl;

	cout << "\nCategory-wise spending:\n";
	for (int i = 0; i < maxCategories; i++) {
		cout << categories[i].name
			<< " : Rs " << categories[i].spent << endl;
	}
}

void dailySummary() {
	int d, m, y;
	float total = 0;

	cout << "\nEnter date (DD MM YYYY): ";
	cin >> d >> m >> y;

	cout << "\n=============================\n";
	cout << "\tDAILY SUMMARY\n";
	cout << "=============================\n";

	for (int i = 0; i < expenseCount; i++) {
		if (expenses[i].day == d &&
			expenses[i].month == m &&
			expenses[i].year == y) {
			cout << expenses[i].category
				<< " - Rs " << expenses[i].amount << endl;
			total += expenses[i].amount;
		}
	}
	cout << "Total spent today: Rs " << total << endl;
}

// -------- FILE HANDLING --------

void saveData() {
	ofstream file("data.txt");

	file << monthlyBudget << " " << remainingBudget << endl;

	for (int i = 0; i < maxCategories; i++) {
		file << categories[i].name << " "
			<< categories[i].highPriority << " "
			<< categories[i].allocatedBudget << " "
			<< categories[i].spent << endl;
	}

	file << expenseCount << endl;
	for (int i = 0; i < expenseCount; i++) {
		file << expenses[i].day << " "
			<< expenses[i].month << " "
			<< expenses[i].year << " "
			<< expenses[i].category << " "
			<< expenses[i].amount << endl;
	}
	file.close();
}

void loadData() {
	ifstream file("data.txt");
	if (!file) return;

	file >> monthlyBudget >> remainingBudget;

	for (int i = 0; i < maxCategories; i++) {
		file >> categories[i].name
			>> categories[i].highPriority
			>> categories[i].allocatedBudget
			>> categories[i].spent;
	}

	file >> expenseCount;
	for (int i = 0; i < expenseCount; i++) {
		file >> expenses[i].day
			>> expenses[i].month
			>> expenses[i].year
			>> expenses[i].category
			>> expenses[i].amount;
	}
	file.close();
}

void waitSeconds(int sec) {
	this_thread::sleep_for(chrono::seconds(sec));
}