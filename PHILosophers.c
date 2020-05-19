//			Dining Table scenerio
//			by: Francisco J Segarra Jr
//	This is probably the hardest most confusing code ive ever written
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
 
#define people 7
#define timesEaten 20
#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock

// an array of names
const char *names[people] = { "Rick", "Morty", "Summer", "Jerry",
							"Beth", "Mr. Meeseeks", "Bird-Person"};
pthread_mutex_t forks[people]; // an array of forks

void pickup_fork(int id)
{
	int status;
	int f[2];	// array of forks
	f[0] = f[1] = id;
	f[!id] = (id + 1) % people;

	char command[80];
	for (int i = 0; i < 2; i++) 
	{
		lock(forks + f[i]); 
		
		printf(" %s is picking up fork %d \n", names[id], f[i]);	// print terminal
		sprintf(command, "echo '%s is picking up fork: %d\n' >> philosopher_output.txt", names[id], f[i]);	//print txt
		
		status = system(command);
	} // end for loop
} // end pick up fork

void putdown_fork(int id)
{
	int status;
	int f[2];	// array of forks
	f[0] = f[1] = id;

	f[!id] = (id + 1) % people;

	char command[80];
	// done nomming, give up forks (order doesn't matter)
	for (int i = 0; i < 2; i++)
	{
		unlock(forks + f[i]);
		
		printf(" %s is putting down fork %d\n", names[id], f[i]);	// print terminal
		sprintf(command, "echo '%s is putting down fork: %d\n' >> philosopher_output.txt", names[id], f[i]);	//print txt
		
		status = system(command);	
	} // end for loop
} // end put down fork

void eat(int id)
{	
	int status;
	sleep(rand() % 5) + 1;
	
	char command[80];
	printf(" %s is eating\n", names[id]);											// print terminal
	sprintf(command, "echo '%s is eat\n' >> philosopher_output.txt", names[id]);	//print txt
	
	status = system(command);
} // end eat

void think(int id)
{
	int status;
	char command[80];
	
	printf(" %s is thinking\n", names[id]);												//print terminal
	sprintf(command, "echo '%s is thinking\n' >> philosopher_output.txt", names[id]);	//print txt
	
	sleep(rand() % 3) + 1;
	status = system(command);
} // end think
 
void* philosophize(void *a)
{
	int id = *(int*)a;
	
	int cycles = 0;
	while(cycles < timesEaten)
	{
		think(id);
		pickup_fork(id);
		eat(id);
		putdown_fork(id);
		cycles++;
	} // end while loop
} // end method
 
int main()
{
	int id[people];
	pthread_t tid[people];
 
	for (int i = 0; i < people; i++)
		pthread_mutex_init(forks + (id[i] = i), 0);
 
	for (int i = 0; i < people; i++)
		pthread_create(tid + i, 0, philosophize, id + i);
 
	for(int i = 0; i < people; i++)
		pthread_join(tid[i], NULL);

	//delete the mutex
	pthread_mutex_destroy(forks);
	exit(0);
} // end main

