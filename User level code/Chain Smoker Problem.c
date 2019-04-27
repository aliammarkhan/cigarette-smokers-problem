#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>

#include<stdbool.h>



sem_t  s1,s2,s3,s4;     

static int  counts0 =0;
static int  counts1 =0;
static int  counts2 =0;
bool agent_exit = false;
bool tobacco =   false;
bool paper =    false;
bool match =   false;


void *smokers(void * ID)
{
  int id = *(int*)ID; 
 while(1)
  {
      
      if(id==0)  // Tobacco 
     {
        sem_wait(&s1);
        if(paper==true && match==true)  
        {
          paper = false;
          match = false;
          printf("\nThread %d got paper and match \n", id);
          printf("Thread %d is now smoking\n",id);
          sem_post(&s4); // allowing agent to again put the items on the table
         counts0++;
        }
        
      }

      else if(id==1)// Paper
         { 
        sem_wait(&s2);
        if(tobacco == true && match == true)  
        {
          tobacco = false;
          match = false;
           printf("\nThread %d got tobacco and match \n", id);
                 printf("Thread %d is now smoking\n",id);
          sem_post(&s4);// allowing agent to again put the items on the table
         counts1++;
        }
      
       }

     else if(id==2) // Match
       { 
        sem_wait(&s3);
        if(tobacco == true && paper == true)  
        {
          tobacco = false;
          paper = false;
          printf("\nThread %d got tobacco and paper \n", id);
                 printf("Thread %d is now smoking\n",id);
          sem_post(&s4); // allowing agent to again put the items on the table
          counts2++;
        }

       }

    
     if(counts0 + counts1 + counts2 >5 )
    {
      sem_post(&s4);
      agent_exit = true;
      break;
     
    }


    

  }

}


void *Agent(void *x)
{
  int id = 0;
  while(1)
  {


    sem_wait(&s4);  // Wait for smoker to complete then place the next items on the table

    usleep(1000000); 

    srand(time(0)); 
    id  = rand() % 3;   //generating random id


    
        if(id == 0) // needs Tobacco
          {
      printf("\nAgent puts paper and match on the table\n");
        paper=true;
        match=true;
        sem_post(&s1);
          }

       else if(id == 1)    //needs Paper
       {
          
        printf("\nAgent puts tobacco and match on the table\n");
         tobacco =true;
         match =true ;
        sem_post(&s2);
        }

       else if( id == 2)   //needs matches
    {
      printf("\nAgent puts tobacco and paper on the table\n");
        tobacco=true;
        paper=true;
        sem_post(&s3);
     }

    if(agent_exit == true )
    {
       match = false;
       tobacco = false;
       paper = false;
        sem_post(&s1);
        sem_post(&s2);
        sem_post(&s3);
     // system_call(counts0,counts2,counts3);  //calling a system call to show the number of smokers 
       break;
    }
 
  }

}


int main()
{
    srand(time(0)); 
   pthread_t smoker[3], agent;
   int id[3];

  sem_init(&s1, 0, 0);
  sem_init(&s2, 0, 0);
  sem_init(&s3, 0, 0);
  sem_init(&s4, 0, 1);


 
for(int i = 0; i < 3; i++)
  {
    id[i] = i;
    pthread_create(&smoker[i], NULL, smokers, &id[i]); //creating smokers
  }
 pthread_create(&agent, NULL, Agent, NULL); //creating agent


 

  
  for (int i = 0; i < 3; i++)
    pthread_join(smoker[i], NULL);

  
  pthread_join(agent, NULL);

  printf("\n\nProgram Exiting\n\n");

return 0;
}
