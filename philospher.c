#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#define Thinking 0
#define Hungry 1
#define Eating 2
#define Left (ph_num+(num_phil-1))%num_phil
#define Right (ph_num+1)%num_phil

int num_cycles;
int num_phil;
void * philospher(void *num);
void take_fork(int);
void put_fork(int);
void test(int);
int *state;
int *full;
int *phil_num;
sem_t mutex;
sem_t *S;

int main(int argc, char *argv[])
{
	if(argc < 3 || !atoi(argv[1]) || !atoi(argv[2]))
  	{
	    printf("Missing or Incorrect args!\n\t%s <num eat cycles> <num philosophers>\n",argv[0]);
	    return 0;
  	}
	num_cycles = atoi(argv[1]);
	num_phil = atoi(argv[2]);
	state = calloc(num_phil, sizeof(int));
	phil_num = calloc(num_phil, sizeof(int));
	S = calloc(num_phil, sizeof(sem_t));
	int i; 
	pthread_t thread_id[num_phil];
	sem_init(&mutex, 0, 1);
	for(i=0;i < num_phil;i++)
	{
		phil_num[i]=i;
	}
	for(i=0; i < num_phil; i++)
		sem_init(&S[i],0,0);
	for(i=0; i<num_phil;i++)
	{
		pthread_create(&thread_id[i],NULL,philospher,&phil_num[i]);
		printf("Philospher %d is thinking\n",i+1);
	}
	for(i=0;i<num_phil;i++)
		pthread_join(thread_id[i],NULL);
}

void *philospher(void *num)
{
	int full = num_cycles;
	while(full!= 0)
	{
		int *i = num;
		sleep(1);
		take_fork(*i);
		sleep(5);
		put_fork(*i);
		full = full-1;
	}
}

void take_fork(int ph_num)
{
	sem_wait(&mutex);
	state[ph_num] = Hungry;
	//printf("Philospher %d is Hungry\n",ph_num+1);
	test(ph_num);
	sem_post(&mutex);
	sem_wait(&S[ph_num]);
	sleep(1);
}

void test(int ph_num)
{
	if(state[ph_num] == Hungry && state[Left] != Eating && state[Right] !=Eating)
	{
		state[ph_num] = Eating;
		sleep(2);
		printf("Philospher %d takes fork %d\n",ph_num+1,Left+1);
		sleep(3);
		printf("Philospher %d takes fork %d\n",ph_num+1,ph_num+1);
		sleep(2);
		printf("Philospher %d is eating\n",ph_num+1);
		sem_post(&S[ph_num]);
	}
}

void put_fork(int ph_num)
{
	sem_wait(&mutex);
	state[ph_num] = Thinking;
	printf("Philospher %d putting fork %d and %d down\n",ph_num+1, Left+1,ph_num+1);	
	printf("Philospher %d is thinking\n",ph_num+1);
	test(Left);
	test(Right);
	sem_post(&mutex);
}