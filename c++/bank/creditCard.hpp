#ifndef creditCard_hpp
#define creditCard_hpp
#include <stdio.h>
using namespace std;
#include <string>

#endif /* creditCard_hpp */

class creditCard : public account {
public:
    long cardNumber;
    string last10Charges[10];
};
