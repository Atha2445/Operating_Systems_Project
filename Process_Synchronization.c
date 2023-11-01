// Consider a server program running in an online market place firm. The program receives buy and
// sell orders for one type of commodity from external clients. For every buy or sell request received
// by the server, the main process spawns a new buy or sell thread. We require that every buy thread
// waits until a sell thread arrives, and vice versa. A matched pair of buy and sell threads will both
// return a response to the clients and exit. You may assume that all buy/sell requests are identical
// to each other, so that any buy thread can be matched with any sell thread. The code executed
// by the buy thread is shown below (the code of the sell thread would be symmetric). You have
// to write the synchronization logic that must be run at the start of the execution of the thread to
// enable it to wait for a matching sell thread to arrive (if none exists already). Once the threads
// are matched, you may assume that the function completeBuy() takes care of the application
// logic for exchanging information with the matching thread, communicating with the client, and
// finishing the transaction. You may use any synchronization technique of your choice.
// //declare any variables here
// buy_thread_function:
// //start of sync logic
// //end of sync logic
// completeBuy();
// Ans:
// sem buyer = 0; sem seller = 0;
// Buyer thread:
// up(buyer)
// down(seller)
// completeBuy()



//Reg-Number:12217958
//Name:Atharva Vaibhav Yeola
//Roll-No:16
//Section:K22SM


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mtx;
pthread_cond_t buyCV;
pthread_cond_t sellCV;
int isBuyThreadReady = 0;
int isSellThreadReady = 0;

void completeBuy(){
    printf("Buy thread cmpleted\n");
}

void comleteSell(){
    printf("Sell thread completed\n");
}

void* buyThread(void* arg){
    pthread_mutex_lock(&mtx);
    isBuyThreadReady = 1;
    pthread_cond_broadcast(&buyCV);
    
    while(!isSellThreadReady){
        pthread_cond_wait(&sellCV,&mtx);
    }
    
    isBuyThreadReady=0;
    isSellThreadReady=0;
    
    pthread_mutex_unlock(&mtx);
    
    return NULL;
}

void* sellThread(void* arg){
    pthread_mutex_lock(&mtx);
    isSellThreadReady = 1;
    pthread_cond_broadcast(&sellCV);
    
    while(!isBuyThreadReady){
        pthread_cond_wait(&buyCV,&mtx);
    }
    completeBuy();
    comleteSell();
    
    isBuyThreadReady=0;
    isSellThreadReady=0;
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main(){
    pthread_t buyThreadID,sellThreadID;
    pthread_mutex_init(&mtx,NULL);
    pthread_cond_init(&buyCV,NULL);
    pthread_cond_init(&sellCV,NULL);
    
    pthread_create(&buyThreadID,NULL,buyThread,NULL);
    pthread_create(&sellThreadID,NULL,sellThread,NULL);
    
    pthread_join(buyThreadID,NULL);
    pthread_join(sellThreadID,NULL);
    
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&buyCV);
    pthread_cond_destroy(&sellCV);
    
    return 0;
}
