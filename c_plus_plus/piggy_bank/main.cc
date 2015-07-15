#include "Coins.h"

////////////////////////////////////
// Display menu function
////////////////////////////////////
void
display_menu()
{
  cout << "\n+--------------------------------------------------+\n"
       << "|                                                  |\n"
       << "|                 Piggy Bank Menu                  |\n"
       << "|     'B' or 'b': Show balance in piggy bank       |\n"
       << "|     'C' or 'c': Show coins in the piggy bank     |\n"
       << "|     'D' or 'd': Deposit coins                    |\n"
       << "|     'P' or 'p': Withdraw coins for purchase      |\n"
       << "|     'Q' or 'q': Quit                             |\n"
       << "|                                                  |\n"
       << "+--------------------------------------------------+"
       << endl;
}

///////////////////////////////////
// Get amount to withdraw
// Prompts the user for how much money to withdraw in cents
///////////////////////////////////
int
get_amount_to_withdraw()
{
  cout << "How much would you like to withdraw? [enter in cents]: ";
  int withdraw_amount;
  cin >> withdraw_amount;
  return withdraw_amount;
}

////////////////////////////////////
// Make Deposit function
////////////////////////////////////
void
make_deposit (Coins & piggy_bank)
{
    int num_of_quarters_to_deposit, num_of_dimes_to_deposit, num_of_nickles_to_deposit, num_of_pennies_to_deposit;
    cout << "Input how many of each coin to deposit." << endl;
    //prompt user for quantity of quarters to deposit
    cout << "Quarter(s): ";
    cin >> num_of_quarters_to_deposit;
    //prompt user for quantity of dimes to deposit
    cout << "Dime(s): ";
    cin >> num_of_dimes_to_deposit;
    //prompt user for quantity of nickels to deposit
    cout << "Nickels: ";
    cin >> num_of_nickles_to_deposit;
    //prompt user for quantity of pennies to deposit
    cout << "Pennies: ";
    cin >> num_of_pennies_to_deposit;
    
    //create Coins object
    Coins coins_to_deposit (num_of_quarters_to_deposit, 
                            num_of_dimes_to_deposit,
                            num_of_nickles_to_deposit,
                            num_of_pennies_to_deposit);
    piggy_bank.deposit_change(coins_to_deposit);
}

////////////////////////////////////
// Perform action function
////////////////////////////////////
bool
perform_action (Coins & piggy_bank, const char & user_choice)
{
    switch (user_choice)
    {
      case 'B': case 'b': // view balance
        cout << piggy_bank << endl;
        break;
      case 'C': case 'c': // view coins
        cout << piggy_bank << endl;
        break;
      case 'D': case 'd': // deposit
        make_deposit(piggy_bank);
        break;
      case 'P': case 'p': // withdraw coins for purchase
        piggy_bank.extract_change( get_amount_to_withdraw() );
        break;
      case 'Q': case 'q': // quit
        cout << "Good bye!" << endl;
        return false;
        break;
      default:
        cout << "Invalid option! Please try again." << endl;
        break;
    }
    return true; // returns true unless user wants to quit
}

int main()
{
  char user_choice;
  Coins piggy_bank(0,0,0,0);
  
  bool keep_running = true;
  while (keep_running)
  {
    display_menu();
    cout << "Pick an action: ";
    cin >> user_choice;
    keep_running = perform_action (piggy_bank, user_choice);
  }
  return 0;
}
