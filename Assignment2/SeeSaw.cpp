#include <iostream>
#include <pthread.h>
#include <unistd.h> //for usleep
#include <mutex>

using namespace std;

double fredHeight = 1;
double wilmaHeight = 7;
int fredCounter = 0; //keeping track of how many times the seesaw goes up + down
int wilmaCounter = 0;


mutex mtx1; //using mutex's to function as a binary semaphore
mutex mtx2;

void *fredSee(void *);
void *wilmaSaw(void *);

int main() {
	mtx2.lock(); //locking the second lock to follow the line of logic from Chp 5
	pthread_t myThreads[2];
	cout << "Fred's Height: " << fredHeight << "   Wilma's Height: " << wilmaHeight << endl;
	sleep(1);
	for(int i = 0; i < 2; ++i) {
		switch(i) {
			case 0:
				pthread_create(&myThreads[0], NULL, fredSee, NULL);
				break;
			case 1:
				pthread_create(&myThreads[1], NULL, wilmaSaw, NULL);
				break;
		}
	}

	for (int i = 0; i < 2; ++i) {
		pthread_join(myThreads[i], NULL);
	}

	return 0;
}

void *fredSee(void *) {
	int counter = 0;
	while(counter < 10) {
		++counter;
		mtx1.lock();
		while(fredHeight < 7) {
			fredHeight += 1;
			wilmaHeight -= 1;
			cout << "Fred's Height: " << fredHeight << "   Wilma's Height: " << wilmaHeight << endl;
			sleep(1);
		}
		mtx2.unlock();
	}

}

void *wilmaSaw(void *) {
	int counter = 0;
	while(counter < 10) {
		++counter;
		mtx2.lock();
		while(wilmaHeight < 7) {
			fredHeight -= 1.5;
			wilmaHeight += 1.5;
			cout << "Fred's Height: " << fredHeight << "   Wilma's Height: " << wilmaHeight << endl;
			sleep(1);
		}
		mtx1.unlock();
	}
}