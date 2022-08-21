// #define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h> /* srand, rand */
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>
#include <unistd.h>
#include "utils_m1.h"

using namespace std;

static pthread_mutex_t acc_mutex;

void *account_thread(void *params_ptr);

Account savings_acc = {CHECKING, 0};
Account checking_acc = {CHECKING, 0};

// transaction_summary savings_summary[NUM_THREADS];
// transaction_summary checking_summary[NUM_THREADS];
transaction_summary savings_summary = {0, 0, 0, 0};
transaction_summary checking_summary = {0, 0, 0, 0};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
       printf("Usage: %s max loop count\n", argv[0]);
        exit(1);
    }

    int loop_count = atoi(argv[1]); // total number of accounts to create

    if (loop_count == 0)
    {
        printf("Usage: %s max loop cont\n", argv[0]);
        exit(1);
    }

    srand(time(0)); /* help seed random function */

    pthread_t threads[NUM_THREADS];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (long int i = 1; i <= NUM_THREADS; i++)
    {
        thread_params *params = new thread_params;

        params->loop_count = loop_count;
        params->thread_count = i;

        /* create a thread
         * @param &t_account_thread[index] - Current thread location in array
         * @param &attr - thread attribut to help close thread gracefully
         * @param account_thread - thread function to run 
         * @param params - set of parameters to be used in the thread
         */
        pthread_create(&threads[i - 1], &attr, account_thread, (void *)params);
    }

    // free attribute and wait for the other threads
    pthread_attr_destroy(&attr);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // mutex to access the buffer
        void *status;
        pthread_join(threads[i], &status);
    }

    return 0;
}

void *account_thread(void *params_ptr)
{
    filename = "sync-threads.txt";
    thread_params *params = (thread_params *)params_ptr;

    // usleep(rand() % 500000); /* incread probability for race condition */
    pthread_mutex_lock(&acc_mutex); // lock mutex to control threads
                                
    // get random operation
    for(int counter = 1; counter <= params->loop_count; counter++)
    {
        current_thread = params->thread_count;
        int amount = 0;
        int op = get_in_range(1, 6);
        // perform random operation
        switch(op)
        {
            case CREDIT_SAVINGS:
                std::cout << "crediting savings" << endl;
                amount = credit_account(savings_acc);
                savings_summary.balance += amount;
                savings_summary.deposits++;
                break;
            case DEBIT_SAVINGS:
                std::cout << "debiting savings" << endl;
                amount = debit_account(savings_acc);
                if(amount > 0) 
                {
                    savings_summary.balance -= amount;
                    savings_summary.withdrawals++;
                }
                else {
                    savings_summary.rejections++;
                }
                break;
            case CREDIT_CHECKING:
                std::cout << "crediting checking" << endl;
                amount = credit_account(checking_acc);
                if(amount > 0) 
                {
                    checking_summary.balance += amount;
                    checking_summary.deposits++;
                }
                else {
                    checking_summary.rejections++;
                }
                break;
            case DEBIT_CHECKING:
                std::cout << "debiting checking" << endl;
                amount = debit_account(checking_acc);
                if(amount > 0) 
                {
                    checking_summary.balance -= amount;
                    checking_summary.withdrawals++;
                }
                else {
                    checking_summary.rejections++;
                }
                break;
            case TRANSFER_TO_SAVINGS:
                std::cout << "transfering from checking to savings" << endl;
                amount = transfer_to(checking_acc, savings_acc);
                if(amount > 0) 
                {
                    checking_summary.balance -= amount;
                    checking_summary.withdrawals++;

                    savings_summary.balance += amount;
                    savings_summary.deposits++;
                }
                else {
                    checking_summary.withdrawals++;
                    savings_summary.deposits++;
                }
                break;
            case TRANSFER_TO_CHECKING:
                std::cout << "transfering from savings to checking" << endl;
                amount = transfer_to(savings_acc, checking_acc);
                if(amount > 0) 
                {
                    savings_summary.balance -= amount;
                    savings_summary.withdrawals++;

                    checking_summary.balance += amount;
                    checking_summary.deposits++;
                }
                else {
                    savings_summary.withdrawals++;
                    checking_summary.deposits++;
                }
               break;
        }
    }
    // log data to file
    // log result to console
    std::cout << "Thread loop count: " << params->thread_count << std::endl;
    
    print_summary(checking_summary, "checking");
    print_summary(savings_summary, "savings");


    pthread_mutex_unlock(&acc_mutex); // unlock mutex when thread is done
    pthread_exit(NULL);
}
