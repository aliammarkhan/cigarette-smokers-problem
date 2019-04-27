
#include <linux/init.h>
#include <linux/semaphore.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/time.h>
#include<linux/timer.h>
#include<linux/delay.h> 
#include<linux/random.h>
//#include<stdlib.h>

int counts0 = 0;
int counts1 = 0;
int counts2 = 0;
static struct semaphore s1,s2,s3,s4;                   //sem_t  s1,s2,s3,s4;     


bool tobacco =   false;
bool paper =    false;
bool match =   false;
bool agent_exit =false;


int smokers(void * ID)
{
int id = *(int*)ID; 
 while(1)
  {
      
      if(id==0)  // Tobacco 
     {
        down(&s1);
        if(paper==true && match==true)  
        {
          paper = false;
          match = false;
          printk("\nThread %d got paper and match \n", id);
          printk("Thread %d is now smoking\n",id);
          counts0++;
          up(&s4); // allowing agent to again put the items on the table
        }
        
      }

      else if(id==1)// Paper
         { 
        down(&s2);
        if(tobacco == true && match == true)  
        {
          tobacco = false;
          match = false;
           printk("\nThread %d got tobacco and match \n", id);
                 printk("Thread %d is now smoking\n",id);
          counts1++;
          up(&s4);// allowing agent to again put the items on the table
        }
      
       }

     else if(id==2) // Match
       { 
        down(&s3);
        if(tobacco == true && paper == true)  
        {
          tobacco = false;
          paper = false;
          printk("\nThread %d got tobacco and paper \n", id);
                 printk("Thread %d is now smoking\n",id);
          counts2++;
          up(&s4); // allowing agent to again put the items on the table
        }
        
     }

   
     if(counts0+counts1+counts2 >10)
      {
        up(&s4);
        agent_exit=true;
        break;
        
       // system_call(counts0,counts2,counts3);  //calling a system call 
          
      }      
    
   
  }

  return 0;
}


int Agent(void *x)
{
int id = 0;
  while(1)
  {


    down(&s4);  // Wait for smoker to complete then place the next items on the table

   // msleep(1000000); 
    
 
      get_random_bytes(&id, sizeof(id));   //generating random id
      if(id<0)
       id = -1*id;
      id = id % 3 ;
    
        if(id == 0) // needs Tobacco
          {
      printk("\nAgent puts paper and match on the table\n");
        paper=true;
        match=true;
        up(&s1);
          }

       else if(id == 1)    //needs Paper
       {
          
        printk("\nAgent puts tobacco and match on the table\n");
         tobacco =true;
         match =true ;
        up(&s2);
        }

       else if( id == 2)   //needs matches
    {
      printk("\nAgent puts tobacco and paper on the table\n");
        tobacco=true;
        paper=true;
        up(&s3);
     }

   if(agent_exit == true)
     {
         match = false;
       tobacco = false;
       paper = false;
        up(&s1);
        up(&s2);
        up(&s3);
       break;
            
     }

  }
return 0;
}


int init_module(void)
{
   
 static  struct task_struct *smoker[3], *agent;
   int id[3],i=0;

  sema_init(&s1,  0);
  sema_init(&s2,  0);
  sema_init(&s3,  0);
  sema_init(&s4,  1);

 
for( i = 0; i < 3; i++)
  {
    id[i] = i;
    //pthread_create(&smoker[i], NULL, smokers, &id[i]);
      smoker[i]=kthread_create(smokers,&id[i],"thread");    //creating smokers
      if((smoker[i]))
      {
         wake_up_process(smoker[i]);
      }
  }
     //pthread_create(&agent, NULL, Agent, NULL); 

   agent=kthread_create(Agent,0,"agent");   //creating agent  

    if((agent))
      {
         wake_up_process(agent);
      }
 
 

  
  for ( i = 0; i < 3; i++)
    kthread_stop(smoker[i]);

  
  kthread_stop(agent);

return 0;
}
MODULE_LICENSE("GPL"); 
void cleanup_module(void) { printk(KERN_INFO " bye bye !! [INFO] Smokers! \n "); } 
