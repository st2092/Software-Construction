__Piggy Bank__

This implementation of a piggy bank provides the user with the following functionality:
* View balance
* View the quantity of each coin in the piggy bank
* Deposit coins
* Withdraw coins for a purchase

__Some notes on implementation__
* The coin domains in cent value used are: 25 (quarter), 10 (dime), 5 (nickel), and 1 (penny)
* The withdrawing coins method is implemented as a greedy algorithm
* With the chosen coin domain, if the balance is greater than or equal the amount to withdraw, making exact change is possible
