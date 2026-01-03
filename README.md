# Expense Manager
## User Manual

## Introduction
Smart Expense Manager is a console-based application that helps users plan and track their monthly expenses. The program allows users to set a monthly budget, assign priorities to expense categories, record daily spending, and view daily and monthly summaries.It also reallocates budget if overspending occurs. All data is saved automatically so that it is available the next time the program is run.

## Features
- Set and manage a monthly budget
- Assign high or low priority to expense categories
- Automatic budget allocation based on priority
- Add daily expenses with proper validation
- Automatic budget readjustment if limits are exceeded
- View daily expense summary
- View monthly expense summary
- Persistent data storage using file handling
- Colored console output for better readability (Windows only)
---
## System Requirements
- Operating System: Windows  
- Compiler: Any C++ compiler 
- Required Header Files:
  - iostream
  - fstream
  - iomanip
  - string
  - windows.h
  - chrono
  - thread
    
This user manual explains how to use the software step by step.

## 1. Starting the Program
1. Compile the program using a C++ compiler.
2. Run the executable file.
3. When the program starts:
   - If previous data exists, it is loaded automatically.
   - If no previous data exists, the program asks the user to set up a monthly budget and category priorities.
---
## 2. Setting the Monthly Budget
- Enter the monthly budget.
- The entered amount must be greater than zero.
- If an invalid amount is entered, the program will repeatedly ask until a valid value is provided.
- The remaining budget is initially set equal to the monthly budget.
---
## 3. Setting Category Priorities
The program contains the following expense categories:
- Food
- Transport
- Accessories
- Clothes
- Bills
- Savings
- Others

For each category:
- Enter `1` to mark it as **High Priority**
- Enter `0` to mark it as **Low Priority**

High priority categories are given more budget than low priority categories.

---
## 4. Budget Allocation
- 60% of the total budget is divided among high-priority categories.
- 40% of the total budget is divided among low-priority categories.
- Each category within the same priority level receives an equal share.
- The program also displays the daily budget for each category by dividing the allocated budget by 30.
---
## 5. Main Menu
After setup, the main menu is displayed:
1. Add Daily Expense
2. View Daily Summary
3. View Monthly Summary
4. Exit

Enter the number corresponding to the desired option.

---
## 6. Adding a Daily Expense
1. Select option `1` from the menu.
2. Enter the date (day, month, and year).
3. Select an expense category from the list.
4. Enter the amount spent.

The program checks:
- The amount is greater than zero.
- The amount does not exceed the remaining budget.
- The amount does not exceed the daily budget limit.

If the daily limit is exceeded:
- The user may choose to proceed or re-enter the amount.
- If the user proceeds, the program automatically readjusts the remaining budgets.

The expense is then saved.

---
## 7. Viewing Daily Summary
1. Select option `2` from the menu.
2. Enter the date for which the summary is required.

The program displays:
- All expenses recorded on that date
- Category-wise amounts
- Total amount spent on that day

---
## 8. Viewing Monthly Summary
1. Select option `3` from the menu.

The program displays:
- Monthly budget
- Total amount spent
- Remaining budget
- Category-wise total spending

---
## 9. Exiting the Program
1. Select option `0` from the menu.
2. The program saves all data to a file.
3. The program exits safely.

---
## 10. Data Storage
- All data is stored in a file named `dataa.txt`.
- The file contains:
  - Monthly budget
  - Remaining budget
  - Category priorities and allocations
  - Expense records
- Data is automatically loaded the next time the program is run.

---
## 11. Error Handling
- Invalid numeric inputs are rejected.
- Dates outside valid ranges are not accepted.
- Expenses exceeding available budget are not allowed.

---
## 12. Limitations
- The number of expense categories is fixed.
- The maximum number of expenses that can be stored is limited.
- The program is console-based and designed for Windows systems.
---
