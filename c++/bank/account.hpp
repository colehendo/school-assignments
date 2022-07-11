#ifndef account_hpp
#define account_hpp

#include <stdio.h>
#include <string>
using namespace std;

#endif /* account_hpp */

class account {
private:
    string name;
    long taxID;
    double balance;
    
protected:
    double last10Withdraws[10];
    double last10Deposits[10];
    int numWithdraws;
    int numDeposits;
    
public:
    void setName(string cinName);
    void setTaxID(long cinTaxID);
    void setBalance(double cinBalance);
    string getName();
    long getTaxID();
    double getBalance();
    void makeDeposit(double amount);
    void makeWithdrawal(double amount);
    void display();
};
