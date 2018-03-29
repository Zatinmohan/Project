#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>


pthread_t t1,t2;
sem_t seat,teacher;
pthread_mutex_t l1;
int counter=-1,chair[3],ccounter=-1;

void *stud()
{
	sem_wait(&seat);
	counter++;
	if(counter<3)
	{
		if(counter==0)
		{
			printf("\nSTUDENT %d IS WAKES UP THE TA",counter+1);
			ccounter++;	
			sem_post(&teacher);	//this will wake up the teacher.
			//sleep(3);			
		}

		else
		{
			pthread_mutex_lock(&l1);
			ccounter++;				//student is sitting on the chair.
			chair[ccounter]=counter;
			pthread_mutex_unlock(&l1);
			printf("\n%d STUDENT ARE WAITING FOR THEIR TURN",counter+1);
			pthread_mutex_unlock(&l1);
		}
		
	}
		
	else
		printf("\nTHERE ARE NO AVAILABLE CHAIRS");
	
	sem_post(&seat);	
}

void *solve()
{
	sem_wait(&teacher);
	if(ccounter==-1){
		printf("\nTHERE ARE NO STUDENTS");				//no student is available;
		sem_post(&teacher);
	}
		
	else
	{	pthread_mutex_lock(&l1);					
		chair[ccounter]=0;
		printf("\nPROBLEM OF STUDENT %d SITTING ON CHAIR %d HAS BEEN SOLVED",counter,ccounter);				
		ccounter--;
		pthread_mutex_unlock(&l1);
		

		pthread_mutex_lock(&l1);							
		//chair[ccounter]=0;
		printf("\nSTUDENT %d LEAVING THE ROOM",counter);
		counter--;		
		pthread_mutex_unlock(&l1);
						
	}
	sem_post(&teacher);
}

void main()
{
	sem_init(&teacher,0,1);
	sem_init(&seat,0,1);
	
	for(int i=0;i<4;i++)
	{
		pthread_create(&t1,NULL,stud,NULL);
		pthread_join(t1,NULL);
	}
	
	for(int i=0;i<3;i++)
	{
		pthread_create(&t2,NULL,solve,NULL);
		pthread_join(t2,NULL);
	}
}
