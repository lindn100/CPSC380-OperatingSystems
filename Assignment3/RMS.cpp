/*	QUESTIONS
2. Priorities of threads
3. C++ implementation of binary semaphore instead of syntax

*/
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

int miss1 = 0;
int miss2 = 0;
int miss3 = 0;
int miss4 = 0;

vector<vector<int>> board(10, vector<int> (10, 1));


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
	mtx1.lock();
	mtx2.lock();
	mtx3.lock();
	mtx4.lock();

	boolmtx1.unlock();
	boolmtx2.unlock();
	boolmtx3.unlock();
	boolmtx4.unlock();


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
			boolmtx1.lock();
			if(working1) {
				miss1++;
			}
			boolmtx1.unlock();
			mtx1.unlock();
			if(i % 2 == 0) {
				boolmtx2.lock();
				if(working2) {
					miss2++;
				}
				boolmtx2.unlock();
				mtx2.unlock();
			}
			if(i % 4 == 0) {
				boolmtx3.lock();
				if(working3) {
					miss3++;
				}
				boolmtx3.unlock();
				mtx3.unlock();
			}
			if(i % 16 == 0) {
				boolmtx4.lock();
				if(working4) {
					miss4++;
				}
				boolmtx4.unlock();
				mtx4.unlock();
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
		mtx1.lock();
		count1++;
		boolmtx1.lock(); //using a mutex to lock the critical section of the bool
		working1 = true;
		boolmtx1.unlock();
		doWork();
		boolmtx1.lock();
		working1 = false;
		boolmtx1.unlock();
	}
}

void *fast(void *) {
	while(true) {
		mtx2.lock();
		count2++;
		boolmtx2.lock();
		working2 = true;
		boolmtx2.unlock();
//causes overrun		for(int i = 0; i < 100000; ++i) 
			doWork();

		boolmtx2.lock();
		working2 = false;
		boolmtx2.unlock();
	}
}

void *medium(void *) {
	while(true) {
		mtx3.lock();
		count3++;
		boolmtx3.lock();
		working3 = true;
		boolmtx3.unlock();
//causes overrun + seg fault		for (int i = 0; i < 100000; ++i) 
			doWork();
		boolmtx3.lock();
		working3 = false;
		boolmtx3.unlock();
	}
}

void *slow(void *) {
	while(true) {
		mtx4.lock();
		count4++;
		boolmtx4.lock();
		working4 = true;
		boolmtx4.unlock();
		doWork();
		boolmtx4.lock();
		working4 = false;
		boolmtx4.unlock();
	}
}