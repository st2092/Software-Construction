#ifndef COINS_H
#define COINS_H
#include <iostream>
#include <iomanip>
using namespace std;

const int CENTS_PER_QUARTER = 25;
const int CENTS_PER_DIME = 10;
const int CENTS_PER_NICKEL = 5;
const int CENTS_PER_PENNY = 1;

class Coins {
  public:
    Coins(int q, int d, int n, int p);
    void deposit_change(const Coins & coins);
    void extract_change(const int & cost);
    int extract_quarters(int & temp_total);
    int extract_dimes(int & temp_total);
    int extract_nickels(int & temp_total);
    bool has_sufficient_amount(const int & cost);
    void update_total();
    void print(ostream & out);
  private:
    int num_of_quarters, num_of_dimes, num_of_nickels, num_of_pennies, total;
};

ostream & operator << (ostream & out, Coins & coins);
#include "Coins.cc"
#endif
