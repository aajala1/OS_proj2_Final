#pragma once
#include <iostream>
#include <string>
#include <pthread.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <locale>
#include <iomanip>
#define NUM_THREADS 5

using namespace std;

void log(string msg);

int current_thread = 0;
string filename = "temp.txt";

const int BUF_SIZE = 1024;

enum ACCOUNT_TYPE {
    CHECKING,
    SAVINGS
};

enum ACCOUNT_OPERATIONS 
{
    CREDIT_SAVINGS = 1,
    DEBIT_SAVINGS,
    TRANSFER_TO_SAVINGS,

    CREDIT_CHECKING,
    DEBIT_CHECKING,
    TRANSFER_TO_CHECKING,
};

enum TRANS_TYPE
{
  CANCELED,
  CREDIT,
  DEBIT,
  TRANSFER
};

struct transaction_summary
{
  int deposits = 0;
  int withdrawals = 0;
  int rejections = 0;
  int balance = 0;
};

/* generate random value in range
 * @param min - min value
 * @param max - max value
 * @returns int
 */
int get_in_range(int min, int max)
{
    return  min + (std::rand() % (max - min + 1));
}



// structure of our params
// Global variable

struct thread_params
{
  pthread_mutex_t *mutex_ptr;
  int loop_count;
  int operation_indicator;
  int thread_count;
};

struct Account {
    int type = SAVINGS | SAVINGS; 
    int balance;
};

struct response
{
  bool success;
  string message;
  int amount; /* amount */
  int type = CREDIT | DEBIT | CANCELED | TRANSFER;
};


/* generate transaction amount based on account type
 * @param &account - reference to the account
 * @returns int - random value based on account
 */
int get_random_amount(Account &account) {
    if(account.type == CHECKING){
        return get_in_range(50, 100);
    }
    return get_in_range(100, 150);
}

bool can_debit_account(Account &acc, int amount)
{
    return acc.balance > amount;
}

string get_account_type(Account &acc)
{
    return acc.type == CHECKING ? "checking" : "savings";
}


int credit_account(Account &acc)
{
    int amount = get_random_amount(acc);
    acc.balance += amount;
   string msg = "THREAD " + std::to_string(current_thread) + " :credited (SUCCESS)" + std::to_string(amount) + " from " + get_account_type(acc) + " account"; 
   log(msg);
   return amount;
}
int credit_account(Account &acc, int amount)
{
    acc.balance += amount;
    std::cout << "credited " << amount << " to " << get_account_type(acc) << std::endl;
   string msg = "TRHEAD " + std::to_string(current_thread) + " :credited (SUCCESS)" + std::to_string(amount) + " from " + get_account_type(acc) + " account"; 
   log(msg);
   return amount;
}

int debit_account(Account &acc)
{
    int amount = get_random_amount(acc);
    if(! can_debit_account(acc, amount))
    {
       string msg = "THREAD " + std::to_string(current_thread) + " :(REJECTED) debiting " + std::to_string(amount) + " from " + get_account_type(acc) + " account"; 
       log(msg);
       return -1;
    }
    acc.balance -= amount;
   string msg = "THREAD " + std::to_string(current_thread) + " :debited " + std::to_string(amount) + " from " + get_account_type(acc) + " account"; 
   log(msg);
    return amount;
}

int debit_account(Account &acc, int amount)
{
   acc.balance -= amount;
    if(! can_debit_account(acc, amount))
    {
       string msg = "THREAD " + std::to_string(current_thread) + " :(REJECTED) debiting " + std::to_string(amount) + " from " + get_account_type(acc) + " account"; 
       log(msg);
       return -1;
    }
   string msg = "THREAD " + std::to_string(current_thread) + " :debited (SUCCESS)" + std::to_string(amount) + " from " + get_account_type(acc) + " account"; 
   log(msg);
    return amount;
}

int transfer_to(Account &curr_acc, Account &target_acc)
{
    int amount = get_random_amount(curr_acc);
    if(can_debit_account(curr_acc, amount))
    {
        // debit current account
        debit_account(curr_acc, amount);

        // credit target account
        credit_account(target_acc, amount);
        // TODO: update number of transfers
    }
    else {
        string msg = "Debited " + std::to_string(amount) + " from " + get_account_type(curr_acc) + "account";
        log(msg);
        std::cout << "could not debit " << amount << " from " << get_account_type(curr_acc) << std::endl;
        // TODO: update number of transactions
    }
    return amount;
}

void log(string msg)
{
    fstream log_file;

    log_file.open(filename, std::ios_base::app);
    if(!log_file.is_open())
    {
        std::cout << "failed to open log file" << filename << std::endl;
    }
    else
    {
        log_file << msg << std::endl;
        cout << "updated log file" << std::endl;
    }
}

void print_summary(transaction_summary &item, string acc_type)
{
    std::cout << "\n\n+++++++++++++++++++++++++++++++++++++++" << std::endl;
    std::cout << "Summary for " << acc_type << std::endl;
    std::cout << "+++++++++++++++++++++++++++++++++++++++" << std::endl;
    std::cout << "Deposits " << item.deposits << std::endl;
    std::cout << "Withdrawals " << item.withdrawals << std::endl;
    std::cout << "Rejections " << item.rejections << std::endl;
    std::cout << "Balance: " << item.balance << std::endl;
}
