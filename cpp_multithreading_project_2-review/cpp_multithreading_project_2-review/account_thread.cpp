#include <iostream>
#include "checking.h"
#include "account.h"
#include "savings.h"
#include <semaphore.h>
#include <pthread.h>
#include "config.h"

// global stats for saving account
extern int buffer[BUF_SIZE];
extern sem_t buffer_access;
extern SavingsAccount savings_account_stats;
extern CheckingAccount checking_account_stats;

void *checking_account_thread(void *params_ptr)
{
  thread_params *params = (thread_params *)params_ptr;

  switch (params->operation_indicator)
  {
  case 1: /* deposit in checking account */
    checking_account_stats.deposit();
    break;
  case 2: /* withdraw from checking account */
    checking_account_stats.withdraw();
    break;
  case 3: /* transfer from checking to savings account */
    transfer_to<CheckingAccount, SavingsAccount>(checking_account_stats, savings_account_stats);
    break;
  }
  return params_ptr;
}

void *savings_account_thread(void *params_ptr)
{
  thread_params *params = (thread_params *)params_ptr;

  switch (params->operation_indicator)
  {
  case 1: /* deposit in savings account */
    savings_account_stats.deposit();
    break;
  case 2: /* withdraw from savings account */
    savings_account_stats.withdraw();
    break;
  case 3: /* transfer from savings to checking account */
    transfer_to<SavingsAccount, CheckingAccount>(savings_account_stats, checking_account_stats);
    break;
  }

  return params_ptr;
}