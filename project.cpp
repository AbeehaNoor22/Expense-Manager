#include <iostream>
#include<iomanip>
#include <string>
#include <chrono>
#include <thread>
#include <windows.h>//colored output
#include <fstream>   // file handling
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
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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

//MAIN
int main() {
	int choice;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "=============================\n";
	cout << "WELCOME TO EXPENSE MANAGER\n";
	cout << "=============================\n\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	loadData();   // LOAD DATA FROM FILE

	if (monthlyBudget == 0) {
		setMonthlyBudget();
		setPriorities();
		allocateBudget(monthlyBudget);
	}

	showBudgetAllocation();
	showPerDayExpense();

	do {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "\n-------------------------------";
		cout << "\nRemaining Budget: " << remainingBudget;
		cout << "\n-------------------------------\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
		showMenu();
		cin >> choice;

		switch (choice) {
		case 1:
			if (remainingBudget > 0)
				addDailyExpense();
			else {
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				cout << "\nNo remaining budget. You cannot add more expenses.\n";
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			break;

		case 2:
			dailySummary();
			break;

		case 3:
			monthlySummary();
			break;

		case 0:
			saveData();   // SAVE DATA TO FILE
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "\nThank you for using Smart Expense Manager!\n";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;

		default:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			cout << "Invalid Choice! \n";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}
	} while (choice != 0);

	return 0;
}

void showMenu() {
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n=============================\n\t  MENU\n=============================" << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
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
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "Invalid amount! Enter again! \n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
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
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "\n=============================\n";
	cout << "     Budget Allocation\n";
	cout << "=============================\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	for (int i = 0; i < maxCategories; i++) {
		cout << categories[i].name
			<< " (" << (categories[i].highPriority ? "High" : "Low")
			<< ") : Rs " << categories[i].allocatedBudget << endl;
	}
}

void showPerDayExpense() {
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n=============================\n";
	cout << "  Budget Allocation per Day\n";
	cout << "=============================\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
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

	cout << "\nEnter date\n";

	// Day
	cout << "Day: ";
	cin >> day;
	while (day < 1 || day > 31) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "Invalid date entered! Enter (1-31) \n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cin >> day;
	}

	// Month
	cout << "Month: ";
	cin >> month;
	while (month < 1 || month > 12) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "Entered invalid month (1-12)! \n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cin >> month;
	}

	// Year
	cout << "Year: ";
	cin >> year;
	while (year < 1900 || year > 2100) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "Invalid year! \n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cin >> year;
	}

	// Display formatted date
	cout << "The date is: ";
	cout << setfill('0') << setw(2) << day << "-"
		<< setw(2) << month << "-"
		<< setw(4) << year << endl;


	while (true) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		cout << "\nSelect category:\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		for (int i = 0; i < maxCategories; i++) {
			cout << i + 1 << ". " << categories[i].name << endl;
		}
		cout << "or 0 to stop\n";
		cout << "Choice: ";
		cin >> choice;

		if (choice == 0)
			break;

		if (choice < 1 || choice > maxCategories) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			cout << "Invalid category! \n";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
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
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			cout << "Invalid amount! \n";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			goto start;
		}

		if (amount > remainingBudget) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			cout << "Not enought budget to support this expenditure! \n";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			goto start;
		}

		if (amount > dailyLimit) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			cout << "This is more than the allocated daily budget! \n";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			cout << "Press 1 to proceed anyway, 0 to re-enter amount: ";
			cin >> opt;

			if (opt == 0)
				goto start;

			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			cout << "Daily limit exceeded! Re-adjusting budgets... \n";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			waitSeconds(2);
		}

		// Store expense
		expenses[expenseCount++] =
		{ day, month, year, categories[index].name, amount };

		categories[index].spent += amount;
		remainingBudget -= amount;

		if (remainingBudget == 0) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			cout << "\nBudget exhausted. Exiting expense entry... \n";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		}

		//readjusting budgets
		if (amount > dailyLimit) {
			readjustBudget();
			showBudgetAllocation();
			showPerDayExpense();
		}
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		cout << "Expense added successfully!\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
}


void monthlySummary() {
	float totalSpent = monthlyBudget - remainingBudget;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n=============================\n";
	cout << "\tMONTHLY SUMMARY\n";
	cout << "============================= " << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
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

	cout << "\nEnter date\n";

	// Day
	cout << "Day: ";
	cin >> d;
	while (d < 1 || d > 31) {
		cout << "Enter valid day (1-31): ";
		cin >> d;
	}

	// Month
	cout << "Month: ";
	cin >> m;
	while (m < 1 || m > 12) {
		cout << "Enter valid month (1-12): ";
		cin >> m;
	}

	// Year
	cout << "Year: ";
	cin >> y;
	while (y < 1900 || y > 2100) {
		cout << "Enter valid year: ";
		cin >> y;
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE);
	cout << "\n=============================\n";
	cout << "\tDAILY SUMMARY\n";
	cout << "=============================\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
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
	//opens a .txt file in output i.e WRITE mode
	ofstream file("dataa.txt");
	//'file' acts as (fout) that prints the data on the file instead of console

	//saving monthly budget and remaining budget
	file << monthlyBudget << " " << remainingBudget << endl;

	//saving category wise data
	for (int i = 0; i < maxCategories; i++) {
		file << categories[i].name << " "
			<< categories[i].highPriority << " "
			<< categories[i].allocatedBudget << " "
			<< categories[i].spent << endl;
	}

	//saving the number of expeses
	file << expenseCount << endl;
	for (int i = 0; i < expenseCount; i++) {
		file << expenses[i].day << " "
			<< expenses[i].month << " "
			<< expenses[i].year << " "
			<< expenses[i].category << " "
			<< expenses[i].amount << endl;
	}

	//closing the file in output(write) mode so that data is saved and no over-writing occurs
	file.close();
}

void loadData() {
	//opens file in (fin) READ mode
	ifstream file("dataa.txt");

	//if file doesnt exist creates new file
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

