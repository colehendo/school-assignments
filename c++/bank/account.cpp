#include <iostream>
#include <string>
#include "account.hpp"
#include "checking.hpp"
#include "savings.hpp"
#include "creditCard.hpp"
using namespace std;

void account::setName(string cinName) {
    cout << "Enter the name on the account: ";
    cin >> cinName;
    this->name = cinName;
}

void account::setTaxID(long cinTaxID) {
    cout << "Enter the ssn on the account: ";
    cin >> cinTaxID;
    this->taxID = cinTaxID;
}

void account::setBalance(double cinBalance) {
    this->balance = cinBalance;
}

string account::getName() {
    return name;
}

long account::getTaxID() {
    return taxID;
}

double account::getBalance() {
    return balance;
}

void account::makeDeposit(double amount) {
    this->balance += amount;
    int j = 9;
    for (int i = 8; i > 0; --i) {
        last10Deposits[i] = last10Deposits[j];
        --j;
    }
    last10Deposits[0] = amount;
}

void account::makeWithdrawal(double amount) {
    this->balance -= amount;
    int j = 9;
    for (int i = 8; i > 0; --i) {
        last10Withdraws[i] = last10Withdraws[j];
        --j;
    }
    last10Withdraws[0] = amount;
}

void account::display() {
    cout << "Name: " << getName() << endl;
    cout << "Tax ID: " << getTaxID() << endl;
    cout << "Balance: " << getBalance();
}



