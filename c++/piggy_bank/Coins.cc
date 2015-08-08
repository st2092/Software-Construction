#include "Coins.h"

///////////////////////
//constructor
///////////////////////
Coins::Coins (int q = 0, int d = 0, int n = 0, int p = 0)
:num_of_quarters(q), num_of_dimes(d), num_of_nickels(n), num_of_pennies(p), total(0)
{
  update_total();
}

/////////////////////////////////
// deposit change function
// Add coins into the piggy bank
/////////////////////////////////
void
Coins::deposit_change(const Coins & coins)
{
  //update the quantity of each coin in piggy bank
  num_of_quarters += coins.num_of_quarters;
  num_of_dimes += coins.num_of_dimes;
  num_of_nickels += coins.num_of_nickels;
  num_of_pennies += coins.num_of_pennies;
  
  cout << "Added " << coins.num_of_quarters << " quarters, "
       << coins.num_of_dimes << " dimes, " << coins.num_of_nickels << " nickels, "
       << coins.num_of_pennies << " pennies into piggy bank." << endl;
       
  update_total();
}

/////////////////////////////////
// extract change function
// Takes a certain amount each coin out of piggy bank to pay for items
/////////////////////////////////
void
Coins::extract_change(const int & cost)
{
  if (!has_sufficient_amount(cost)) // does not have enough money
  {
    cout << "You have insufficient amount of money to make this purchase." << endl;
    return;
  }
  int temp_total = cost;
  int quarters_needed = 0;
  int dimes_needed = 0;
  int nickels_needed = 0;
  int pennies_needed = 0;
  // extract coins with an greedy approach (go from largest coin domain down to lowest)
  // cost divide by cent value of coin -> maximum quantity of that coin domain that is divisible into cost
  
  quarters_needed = extract_quarters(temp_total);
  dimes_needed = extract_dimes(temp_total);
  nickels_needed = extract_nickels(temp_total);
  
  //check if we have enough pennies to make exact change
  if (temp_total <= num_of_pennies) // have enough pennies
  {
    pennies_needed = temp_total;
    // update the amount of each coin in piggy bank
    num_of_quarters -= quarters_needed;
    num_of_dimes -= dimes_needed;
    num_of_nickels -= nickels_needed;
    num_of_pennies -= pennies_needed;
    update_total();
    cout << "Withdrew " << quarters_needed << " quarters, " << dimes_needed << " dimes, "
         << nickels_needed << " nickels, and " << pennies_needed << " pennies." << endl;
    return;
  }
  else // does not have enough pennies
  {
    cout << "Do not have coins for exact change." << endl;
    return;
  }
}

///////////////////////////////
// Extract quarters function
///////////////////////////////
int
Coins::extract_quarters(int & temp_total)
{
  int quarters_needed;
  // total quantity of quarters needed is less than or equal to the total quantity of quarters in piggy bank
  if (temp_total/CENTS_PER_QUARTER <= num_of_quarters)
  {
    quarters_needed = temp_total/CENTS_PER_QUARTER;
    temp_total = temp_total % CENTS_PER_QUARTER; // this is the remainder after extracting n quarters
  }
  else //we need more quarters than we currently have
  {
    quarters_needed = num_of_quarters; // take all the quarters we have
    temp_total -= quarters_needed*CENTS_PER_QUARTER; // subtract from cost the total of all the quarters summed up
  }
  return quarters_needed;
}

///////////////////////////////
// Extract dimes function
///////////////////////////////
int
Coins::extract_dimes(int & temp_total)
{
  int dimes_needed;
  //total quantity of dimes needed is less than or equal to the total quantity of dimes in piggy bank
  if (temp_total/CENTS_PER_DIME <= num_of_dimes)
  {
    dimes_needed = temp_total/CENTS_PER_DIME;
    temp_total = temp_total % CENTS_PER_DIME; //this is the remainder after extracting n dimes
  }
  else
  {
    dimes_needed = num_of_dimes; //take all the dimes we have
    temp_total -= dimes_needed*CENTS_PER_DIME; // subtract from cost the total of all the dimes
  }
  return dimes_needed;
}

///////////////////////////////
// Extract nickels function
///////////////////////////////
int
Coins::extract_nickels(int & temp_total)
{
  int nickels_needed;
  
  //total quantity of dimes needed is less than or equal to the total quantity of dimes in piggy bank
  if (temp_total/CENTS_PER_NICKEL <= num_of_nickels)
  {
    nickels_needed = temp_total/CENTS_PER_NICKEL;
    temp_total = temp_total % CENTS_PER_NICKEL; //this is the remainder after extracting n nickels
  }
  else
  {
    nickels_needed = num_of_nickels; //take all the nickels we have
    temp_total -= nickels_needed*CENTS_PER_NICKEL; // subtract from cost the total of all the dimes
  }
  return nickels_needed;
}

///////////////////////////////
// update total function
// Update the total amount of money that is in the piggy bank
///////////////////////////////
void
Coins::update_total()
{
  total = (num_of_quarters*CENTS_PER_QUARTER) + (num_of_dimes*CENTS_PER_DIME)
          + (num_of_nickels*CENTS_PER_NICKEL) + (num_of_pennies);
}

//////////////////////////////
// has sufficient amount function
// Checks if the user has enough money in piggy bank to make a purchase
//////////////////////////////
bool
Coins::has_sufficient_amount(const int & cost)
{
  return total >= cost;
}

/////////////////////////////
// Print function
// Prints out the quantity of each coin in piggy bank and also the total value of the coins
/////////////////////////////
void
Coins::print (ostream & out)
{
  out << "The piggy bank contains:\n" << "\t" << num_of_quarters << " quarter(s),\n"
      << "\t" << num_of_dimes << " dime(s),\n"
      << "\t" << num_of_nickels << " nickel(s),\n"
      << "\t" << num_of_pennies << " pennies\n"
      << "Total value: $ " << fixed << setprecision(2) << total/100.00;
}

///////////////////////////
// Operator <<
// Overload the '<<' operator to work with coin class
///////////////////////////
ostream &
operator << (ostream & out, Coins & coins)
{
  coins.print(out);
  return out;
}
