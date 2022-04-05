/*
* Выполнил: Халатова Екатерина 8375
* Задание: 4 "Модель циклической системы управления"
* Дата выполнения: 16.02.2022
* Версия: 0.1
*
* Скрипт для компиляции и запуска программы:
* gcc -lpthread -lrt -o manage 4_Halatova_8375.c
* ./manage
*/
// ------------------------------------------- //
/*
* Общее описание программы:
* 
* Для управления некоторым объектом необходимо обеспечить 
* периодическое выполнение трех независимых управляющих процедур.  
* Периоды вызова процедур должны составлять 500, 750 и 1000 
* миллисекунд  соответственно, и запуск каждой из них следует 
* осуществлять от периодических программных таймеров, используя 
* сигналы в качестве схемы уведомления. При каждом выполнении 
* процедуры должен отображаться момент ее текущего вызова и время, 
* прошедшее с момента предыдущего вызова.
* 
*/
// ------------------------------------------- // 

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t mutex;

long prev_global[3] = {0,0,0};

void doControle( int sig, siginfo_t *si, void *uc )
{
	pthread_mutex_lock(&mutex);
	
	long prev = 0;
	long now = (long)clock() / 1000;
	switch (si->si_value.sival_int) {
	
		case 500:
			prev = prev_global[0];
			prev_global[0] = now;
			break;

		case 750:
			prev = prev_global[1];
			prev_global[1] = now;
			break;
			
		case 1000:
			prev = prev_global[2];
			prev_global[2] = now;
			break;
	}
	
	printf("Procedure #%i executs at %ld ms. Time after previous excecution: %ld ms\n", si->si_value.sival_int, now, now - prev);
	
	pthread_mutex_unlock(&mutex);
}

void makeTimer(int interval)
{
	struct sigevent	te;
	struct itimerspec  its;
	struct sigaction   sa;
	
	int sigNo = SIGRTMIN;

	// Set up signal handler.
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = doControle;
	sigemptyset(&sa.sa_mask);

	sigaction(sigNo, &sa, NULL);

	// Set up timer
	te.sigev_notify = SIGEV_SIGNAL;
	te.sigev_signo = sigNo;
	te.sigev_value.sival_int = interval;

	timer_t timerID;

	timer_create(CLOCK_REALTIME, &te, &timerID);

	int time = interval * 1000000;
	its.it_value.tv_sec = time / 1000000000;
	its.it_value.tv_nsec = time % 1000000000;
	its.it_interval.tv_sec = time / 1000000000;
	its.it_interval.tv_nsec = time % 1000000000;

	timer_settime(timerID, 0, &its, NULL);

	return;
}

int main(int argc, char *argv[])
{
	pthread_mutex_init(&mutex, NULL);
	
	prev_global[0] = (long)clock() / 1000;
	makeTimer(500);
	prev_global[1] = (long)clock() / 1000;
	makeTimer(750);
	prev_global[2] = (long)clock() / 1000;
	makeTimer(1000);

	while (1);

	return(0);
}

// ----------------------------------------//
/*
	Procedure #500 executs at 502 ms. Time after previous excecution: 500 ms
	Procedure #750 executs at 752 ms. Time after previous excecution: 750 ms
	Procedure #500 executs at 1002 ms. Time after previous excecution: 500 ms
	Procedure #1000 executs at 1002 ms. Time after previous excecution: 1000 ms
	Procedure #500 executs at 1502 ms. Time after previous excecution: 500 ms
	Procedure #750 executs at 1502 ms. Time after previous excecution: 750 ms
	Procedure #500 executs at 2002 ms. Time after previous excecution: 500 ms
	Procedure #1000 executs at 2002 ms. Time after previous excecution: 1000 ms
	Procedure #750 executs at 2252 ms. Time after previous excecution: 750 ms
	Procedure #500 executs at 2502 ms. Time after previous excecution: 500 ms
	Procedure #500 executs at 3002 ms. Time after previous excecution: 500 ms
	Procedure #750 executs at 3002 ms. Time after previous excecution: 750 ms
	Procedure #1000 executs at 3002 ms. Time after previous excecution: 1000 ms
	Procedure #500 executs at 3502 ms. Time after previous excecution: 500 ms
	Procedure #750 executs at 3752 ms. Time after previous excecution: 750 ms
	Procedure #500 executs at 4002 ms. Time after previous excecution: 500 ms
	Procedure #1000 executs at 4002 ms. Time after previous excecution: 1000 ms
	Procedure #500 executs at 4502 ms. Time after previous excecution: 500 ms
	Procedure #750 executs at 4502 ms. Time after previous excecution: 750 ms
	Procedure #500 executs at 5002 ms. Time after previous excecution: 500 ms
	Procedure #1000 executs at 5002 ms. Time after previous excecution: 1000 ms
	Procedure #750 executs at 5252 ms. Time after previous excecution: 750 ms
	Procedure #500 executs at 5502 ms. Time after previous excecution: 500 ms
	Procedure #500 executs at 6002 ms. Time after previous excecution: 500 ms
	Procedure #750 executs at 6002 ms. Time after previous excecution: 750 ms
	Procedure #1000 executs at 6002 ms. Time after previous excecution: 1000 ms
*/
