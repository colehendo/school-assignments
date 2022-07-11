#include <iostream>
#include "account.hpp"
#include "checking.hpp"
#include "savings.hpp"
#include "creditCard.hpp"
using namespace std;

int main() {
    checking myChecking;
    savings mySavings;
    creditCard myCreditCard;
    myChecking.setBalance(100);
    mySavings.setBalance(100);
    myCreditCard.setBalance(100);
    
    cout << "Welcome to Henderson Bank Inc." << endl << endl;
    long selection = 0;
    while (selection != 10) {
        cout << "Please select one of the following menu items:" << endl;
        cout << "1. Savings Deposit" << endl << "2. Savings withdrawal" << endl;
        cout << "3. Checking Deposit" << endl << "4. Write A Check" << endl;
        cout << "5. Credit Card Payment" << endl << "6. Make A Charge" << endl;
        cout << "7. Display Savings" << endl << "8. Display Checking" << endl;
        cout << "9. Display Credit Card" << endl << "10. Exit" << endl << "Enter menu selection here: ";
        cin >> selection;
        while (selection > 10 || selection < 1) {
            cout << endl << "Invalid selection. Please pick a number between 1 and 10: ";
            cin >> selection;
        }
        
        if (selection == 1) {
            double amount;
            cout << endl << endl << "Enter deposit amount: ";
            cin >> amount;
            mySavings.makeDeposit(amount);
            cout << endl;
        }
        
        else if (selection == 2) {
            double amount;
            cout << endl << endl << "Enter withdrawal amount: ";
            cin >> amount;
            mySavings.makeWithdrawal(amount);
            cout << endl;
        }
        
        else if (selection == 3) {
            double amount;
            cout << endl << endl << "Enter deposit amount: ";
            cin >> amount;
            myChecking.makeDeposit(amount);
            cout << endl;
        }
        
        else if (selection == 4) {
            double amount;
            int checkNumber;
            cout << endl << endl << "Enter check amount: ";
            cin >> amount;
            cout << endl << "Enter the check number: ";
            cin >> checkNumber;
            myChecking.makeWithdrawal(amount);
            int j = 9;
            for(int i = 8; i > 0; --i) {
                myChecking.last10Checks[i] = myChecking.last10Checks[j];
                --j;
            }
            myChecking.last10Checks[0] = checkNumber;
            cout << endl;
        }
        
        else if (selection == 5) {
            double amount;
            cout << endl << endl << "Enter payment amount: ";
            cin >> amount;
            myCreditCard.makeDeposit(amount);
            cout << endl;
        }
        
        else if (selection == 6) {
            double amount;
            cout << endl << endl << "Enter charge amount: ";
            cin >> amount;
            myCreditCard.makeWithdrawal(amount);
            cout << endl;
        }
        
        else if (selection == 7)
            cout << endl << "Savings balance: " << mySavings.getBalance() << endl << endl;
        
        else if (selection == 8)
            cout << endl << "Checking balance: " << myChecking.getBalance() << endl << endl;
        
        else if (selection == 9)
            cout << endl << "Credit Card balance: " << myCreditCard.getBalance() << endl << endl;
        
        else {
            cout << endl << endl << "Thank you for being a loyal Henderson Bank Inc. customer.";
            selection = 10;
        }
    }
    return 0;
}
