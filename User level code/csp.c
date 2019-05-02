
/* This implementation follows the  interesting version of the chain smoker problem */


#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>


sem_t tobacco,paper,match,pusher_t,pusher_p,pusher_m,agent,mutex;

bool istobacco=false;
bool ispaper = false;
bool ismatch = false;


int count1=0,count2=0,count3=0;

 void *Agent1(void * x)
  {
      while(1)
     {
      sem_wait(&agent);    
        printf("\nAgent puts tobacco and match on the table\n");
      sem_post(&pusher_t); //passes tobacco to the pusher
      sem_post(&pusher_m);  //passes matches to the pusher
     
      }
 }

  void *Agent2(void * x)
 {
     while(1)
     {
      sem_wait(&agent);
        printf("\nAgent puts paper and tobacco on the table\n");
      sem_post(&pusher_t);  //passes tobacco to the pusher
      sem_post(&pusher_p);  //passes paper to the pusher
    
     }
 }

  void *Agent3(void * x)
  {
     while(1){
      sem_wait(&agent);
      printf("\nAgent puts paper and match on the table\n");
      sem_post(&pusher_m); //passes match to the pusher
      sem_post(&pusher_p); //passes paper to the pusher
   
    }  
}

  void *Smoker1(void *x)
  {
    while(1){
   
    sem_wait(&tobacco);
    usleep(1000000); 
    printf("\nSmoker 1 got paper and match\n");
    printf("Smoker 1 is now smoking\n");
    count1++;
    sem_post(&agent); // after smoking , agent can again put the items on the table
   
   }
  }

    void *Smoker2(void *x)
  { 
    while(1){
    
    sem_wait(&paper);
   usleep(1000000); 
    printf("\nSmoker 2 got tobacco and match\n");
    printf("Smoker 2 is now smoking\n");
    count2++;
    sem_post(&agent);
  
  }
 }


  
    void *Smoker3(void *x)
  {
    while(1){
  
    sem_wait(&match);
      usleep(1000000); 
    printf("\nSmoker 3 got tobacco and paper\n");
    printf("Smoker 3 is now smoking\n");
    count3++;
    sem_post(&agent);
    
   }
  }

  void *Pusher1(void * x)
  {
    while(1){
    sem_wait(&pusher_t); //checks if agent has pushed tobacco
    sem_wait(&mutex);
    
     if(ispaper)  //checks if Pusher2 has pushed paper
     {
       ispaper=false;
       sem_post(&match);
     }  
  else  if(ismatch)  //checks if Pusher3 has pushed match
     {
       ismatch=false;
       sem_post(&paper);
     }
   
    else   // pusher2 and pusher3 have not run so pushes tobacco 
    istobacco=true;
       
     
    sem_post(&mutex);  }

  }
   void *Pusher2(void * x)
  {
    while(1){
    sem_wait(&pusher_p);  //checks if agent has pushed paper
    sem_wait(&mutex);
    
     if(istobacco)  //checks if Pusher1 has pushed paper
     {
       istobacco=false;
       sem_post(&match); 
     }
  else  if(ismatch) //checks if Pusher3 has pushed matches
     {
       ismatch=false;
       sem_post(&tobacco);
     }
   
    else 
    ispaper=true;   // pusher1 and pusher3 have not run so pushes paper
       
    
      
  sem_post(&mutex); } 

  }

   void *Pusher3(void * x)
  {
     while(1){
    sem_wait(&pusher_m);   //checks if agent has pushed matches
    sem_wait(&mutex);
    
     if(istobacco)  //checks if Pusher1 has pushed tobacco
     {
       istobacco=false;
       sem_post(&paper);
     }
  else  if(ispaper)   //checks if Pusher2 has pushed paper
     {
       ispaper=false;
       sem_post(&tobacco);
     }
   
    else 
    ismatch=true;   // pusher1 and pusher2 have not run so pushes matches
       

    sem_post(&mutex);  
  }

  }
  
//void * Exit(void * x)
//{
 // while(1){
  //if(count1+count2+count3>5)
   //{
      
      
       //system_call(count1,count2,count3);
     //  exit(0);
   //}
       
//}
   

//}

int main()
{
  sem_init(&tobacco,0,0);
  sem_init(&match,0,0);
  sem_init(&paper,0,0);
  sem_init(&pusher_t,0,0);
  sem_init(&pusher_p,0,0);
  sem_init(&pusher_m,0,0);
  sem_init(&mutex,0,1);
  sem_init(&agent,0,1);

   pthread_t  smoker1,smoker2,smoker3,agent1,agent2,agent3,pusher1,pusher2,pusher3,exit_thread;

    
    pthread_create(&agent1,  NULL, Agent1, NULL);
    pthread_create(&agent2,  NULL, Agent2, NULL);
    pthread_create(&agent3,  NULL, Agent3, NULL);
    pthread_create(&smoker1, NULL, Smoker1, NULL);
    pthread_create(&smoker2, NULL, Smoker2, NULL);
    pthread_create(&smoker3, NULL, Smoker3, NULL);
    pthread_create(&pusher1, NULL, Pusher1, NULL);
    pthread_create(&pusher2, NULL, Pusher2, NULL);
    pthread_create(&pusher3, NULL, Pusher3, NULL);
   // pthread_create(&exit_thread, NULL, Exit, NULL);


    pthread_join(agent1,   NULL);
    pthread_join(agent2,   NULL);
    pthread_join(agent3,   NULL);
    pthread_join(smoker1,  NULL);
    pthread_join(smoker2,  NULL);
    pthread_join(smoker3,  NULL);
    pthread_join(pusher1,  NULL);
    pthread_join(pusher2,  NULL);
    pthread_join(pusher3,  NULL);
    pthread_join(exit_thread,  NULL);

  
  return 0;



}
