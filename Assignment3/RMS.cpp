#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <unistd.h> //for usleep
#include <mutex>
#include <vector>

using namespace std;

int count1 = 0;
int count2 = 0;
int count3 = 0;
int count4 = 0;

int[][] board = new int[10][10];


mutex mtx1; //using mutex's to function as a binary semaphore
mutex mtx2;
mutex mtx3;
mutex mtx4;

void *scheduluer(void *);
void *superfast(void *);
void *fast(void *);
void *medium(void *);
void *slow(void *);

int main() {
	for(int i = 0; i < 10; ++i) {
		for(int j = 0; j < 10; ++j) {
			board[i][j] = 1;
		}
	}

	mtx1.lock();
	mtx2.lock();
	mtx3.lock();
	mtx4.lock();


	pthread_t myThreads[5];
	for(int i = 0; i < 5; ++i) {
		switch(i) {
			case 0:
				pthread_create(&myThreads[0], NULL, superfast, NULL);
				break;
			case 1:
				pthread_create(&myThreads[1], NULL, fast, NULL);
				break;
			case 2:
				pthread_create(&myThreads[2], NULL, medium, NULL);
				break;
			case 3:
				pthread_create(&myThreads[3], NULL, slow, NULL);
				break;
			case 4:
				pthread_create(&myThreads[4], NULL, scheduler, NULL);
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
			mtx1.unlock();
			if(i % 4 == 0) {
				mtx2.unlock();
			}
			if(i % 8 == 0) {
				mtx3.unlock();
			}
			if(i % 16 == 0) {
				mtx4.unlock();
			}
			sleep(1);
		}
	}
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
	mtx1.lock();
	doWork();
}

void *fast(void *) {
	mtx2.lock();
	doWork();
}

void *medium(void *) {
	mtx3.lock();
	doWork();
}

void *slow(void *) {
	mtx4.lock():
	doWork();
}