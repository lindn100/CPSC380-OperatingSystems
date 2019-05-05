#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <unistd.h> //for usleep
#include <mutex>
#include <vector>
#include <semaphore.h>

using namespace std;

int count1 = 0;
int count2 = 0;
int count3 = 0;
int count4 = 0;

int miss1 = 0;
int miss2 = 0;
int miss3 = 0;
int miss4 = 0;

vector<vector<int>> board(10, vector<int> (10, 1));

sem_t s1;
sem_t s2;
sem_t s3;
sem_t s4;

sem_t bools1;
sem_t bools2;
sem_t bools3;
sem_t bools4;


mutex mtx1; //using mutex's to function as a binary semaphore
mutex mtx2;
mutex mtx3;
mutex mtx4;

mutex boolmtx1;
mutex boolmtx2;
mutex boolmtx3;
mutex boolmtx4;

bool working1 = false; //bools used to see if the thread is still doing work when scheduled again
bool working2 = false;
bool working3 = false;
bool working4 = false;

void *scheduler(void *);
void *superfast(void *);
void *fast(void *);
void *medium(void *);
void *slow(void *);

int main() {
	struct sched_param params;

	

	cpu_set_t mask;
	int cpuAffinity = 1;

	CPU_ZERO(&mask);
	CPU_SET(cpuAffinity, &mask);

	sem_init(&s1, 0, 0);
	sem_init(&s2, 0, 0);
	sem_init(&s3, 0, 0);
	sem_init(&s4, 0, 0);

	sem_init(&bools1, 0, 1);
	sem_init(&bools2, 0, 1);
	sem_init(&bools3, 0, 1);
	sem_init(&bools4, 0, 1);

	mtx1.lock();
	mtx2.lock();
	mtx3.lock();
	mtx4.lock();

	boolmtx1.unlock();
	boolmtx2.unlock();
	boolmtx3.unlock();
	boolmtx4.unlock();

	params.sched_priority = sched_get_priority_max(SCHED_FIFO);


	pthread_t myThreads[5];
	for(int i = 0; i < 5; ++i) {
		switch(i) {
			case 0:
				pthread_create(&myThreads[0], NULL, superfast, NULL);
				pthread_setaffinity_np(myThreads[0], sizeof(mask), &mask);
				params.sched_priority--;
				pthread_setschedparam(myThreads[0], SCHED_FIFO, &params);
				break;
			case 1:
				pthread_create(&myThreads[1], NULL, fast, NULL);
				pthread_setaffinity_np(myThreads[1], sizeof(mask), &mask);
				params.sched_priority--;
				pthread_setschedparam(myThreads[1], SCHED_FIFO, &params);
				break;
			case 2:
				pthread_create(&myThreads[2], NULL, medium, NULL);
				pthread_setaffinity_np(myThreads[2], sizeof(mask), &mask);
				params.sched_priority--;
				pthread_setschedparam(myThreads[2], SCHED_FIFO, &params);
				break;
			case 3:
				pthread_create(&myThreads[3], NULL, slow, NULL);
				pthread_setaffinity_np(myThreads[3], sizeof(mask), &mask);
				params.sched_priority--;
				pthread_setschedparam(myThreads[3], SCHED_FIFO, &params);
				break;
			case 4:
				pthread_create(&myThreads[4], NULL, scheduler, NULL);
				pthread_setaffinity_np(myThreads[4], sizeof(mask), &mask);
				params.sched_priority = sched_get_priority_max(SCHED_FIFO);
				pthread_setschedparam(myThreads[4], SCHED_FIFO, &params);
				break;
		}

	}

	for (int i = 0; i < 5; ++i) {
		pthread_join(myThreads[i], NULL);
	}
	return 0;
}

void* scheduler(void *) {
	int counter = 0;
	while(counter < 10) {
		++counter;
		for(int i = 1; i < 17; ++i) {
			sem_wait(&bools1);//boolmtx1.lock();
			if(working1) {
				miss1++;
			}
			sem_post(&bools1);//boolmtx1.unlock();
			sem_post(&s1);//mtx1.unlock();
			if(i % 2 == 0) {
				sem_wait(&bools2);//boolmtx2.lock();
				if(working2) {
					miss2++;
				}
				sem_post(&bools2);//boolmtx2.unlock();
				sem_post(&s2);//mtx2.unlock();
			}
			if(i % 4 == 0) {
				sem_wait(&bools3);//boolmtx3.lock();
				if(working3) {
					miss3++;
				}
				sem_post(&bools3);//boolmtx3.unlock();
				sem_post(&s3);//mtx3.unlock();
			}
			if(i % 16 == 0) {
				sem_wait(&bools4);//boolmtx4.lock();
				if(working4) {
					miss4++;
				}
				sem_post(&bools4);//boolmtx4.unlock();
				sem_post(&s4);//mtx4.unlock();
			}
		usleep(10000);
		}
	}
	cout << "Super Fast ran: " << count1 << "times" << endl;
	cout << "Fast ran: " << count2 << "times" << endl;
	cout << "Medium ran: " << count3 << "times" << endl;
	cout << "Slow ran: " << count4 << "times" << endl;
	cout << "Super Fast missed: " << miss1 << "times" << endl;
	cout << "Fast missed: " << miss2 << "times" << endl;
	cout << "Medium missed: " << miss3 << "times" << endl;
	cout << "Slow missed: " << miss4 << "times" << endl;
	exit(0);
}

void doWork() {
	for(int i = 0; i < 10; i += 5) {
		for(int j = 0; j < 10; ++j) {
			board[0][0] *= board[j][i];
		}
		if(i > 4) {
			i -= 4;
		}
		if(i == 9) {
			break;
		}
	}
}

void *superfast(void *) {
	while(true) {
		sem_wait(&s1);//mtx1.lock();
		count1++;
		sem_wait(&bools1);//boolmtx1.lock(); //using a mutex to lock the critical section of the bool
		working1 = true;
		sem_post(&bools1);//boolmtx1.unlock();
		doWork();
		sem_wait(&bools1);//boolmtx1.lock();
		working1 = false;
		sem_post(&bools1);//boolmtx1.unlock();
	}
}

void *fast(void *) {
	while(true) {
		sem_wait(&s2);//mtx2.lock();
		count2++;
		sem_wait(&bools2);//boolmtx2.lock();
		working2 = true;
		sem_post(&bools2);//boolmtx2.unlock();
//		for(int i = 0; i < 100000; ++i) 
			doWork();

		sem_wait(&bools2);//boolmtx2.lock();
		working2 = false;
		sem_post(&bools2);boolmtx2.unlock();
	}
}

void *medium(void *) {
	while(true) {
		sem_wait(&s3);//mtx3.lock();
		count3++;
		sem_wait(&bools3);//boolmtx3.lock();
		working3 = true;
		sem_post(&bools3);//boolmtx3.unlock();
		for (int i = 0; i < 100000; ++i) 
			doWork();
		sem_wait(&bools3);//boolmtx3.lock();
		working3 = false;
		sem_post(&bools3);//boolmtx3.unlock();
	}
}

void *slow(void *) {
	while(true) {
		sem_wait(&s4);//mtx4.lock();
		count4++;
		sem_wait(&bools4);//boolmtx4.lock();
		working4 = true;
		sem_post(&bools4);//boolmtx4.unlock();
		doWork();
		sem_wait(&bools4);//boolmtx4.lock();
		working4 = false;
		sem_post(&bools4);//boolmtx4.unlock();
	}
}