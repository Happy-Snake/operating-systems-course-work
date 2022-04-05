/*
* Выполнил: Халатова Екатерина 8375
* Задание: 1 "Поставщик - Потребитель"
* Дата выполнения: 13.02.2022
* Версия: 0.2
*
* Скрипт для компиляции и запуска программы:
* gcc -pthread -o semaphore 1_Halatova_8375.c
* ./semaphore
*/
// ------------------------------------------- //
/*
* Общее описание программы:
* 
* Два потока  -  «Поставщик» и «Потребитель» - работают 
* с некоторой общей целочисленной переменной Buffer, при 
* этом каждое обращение «Поставщика» увеличивает значение 
* Buffer на 1, а  «Потребителя» - уменьшает.
* 
* Для обеспечения безопасного взаимодействия двух потоков 
* с одной переменной был использован семафор. Поставщик не
* может записывать в буфер, если тот полон (Buffer_size),
* Потребитель не может считывать из пустого буфера. 
*/
// ------------------------------------------- // 

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


sem_t 	sem;
sem_t   sem_is_full;
sem_t   sem_is_empty;
int 	Buffer_size = 5;
int 	Buffer = 0;


void* Supplier(void* arg);
void* Consumer(void* arg);


int main(int argc, char *argv[]) {
	sem_init(&sem,0,1);
	sem_init(&sem_is_full,0,Buffer_size);
	sem_init(&sem_is_empty,Buffer,Buffer);
	
	pthread_t thread_Cons, thread_Sup;
	
	pthread_create(&thread_Cons, NULL, &Consumer, NULL);
	pthread_create(&thread_Sup, NULL, &Supplier, NULL);
	
	pthread_join(thread_Cons, NULL);
	pthread_join(thread_Sup, NULL);
	
	return EXIT_SUCCESS;  
}


void* Supplier(void* arg) {

	while (1)
	{
		printf("Try to start Supplier\n");
        
		sem_wait(&sem_is_full);
		sem_wait(&sem);
        
		time_t current_time = time(NULL);
		char* c_time_string = ctime(&current_time);
		printf("Supplier starts his work: %s", c_time_string);

        Buffer++;

        current_time = time(NULL);
		c_time_string = ctime(&current_time);
		printf("Supplier ends his work: %sBuffer - %i\n\n", c_time_string, Buffer);
        
		sem_post(&sem);
		sem_post(&sem_is_empty);
        
		sleep(3);
	}
	return EXIT_SUCCESS;
}


void* Consumer(void* arg) {

	while (1)
	{        
		printf("Try to start Consumer\n");
        
		sem_wait(&sem_is_empty);
		sem_wait(&sem);
        
		time_t current_time = time(NULL);
		char* c_time_string = ctime(&current_time);
		printf("Consumer starts his work: %s", c_time_string);
        
		Buffer--;            
		
		current_time = time(NULL);
		c_time_string = ctime(&current_time);
		printf("Consumer ends his work: %sBuffer - %i\n\n", c_time_string, Buffer);
        
		sem_post(&sem);
		sem_post(&sem_is_full);
		
        sleep(3);
	}
	return EXIT_SUCCESS;
}


// ----------------------------------------//
/*
	Consumer starts his work: Sun Feb 13 19:34:12 2022
	Consumer can't read. Buffer is emply
	Consumer ends his work: Sun Feb 13 19:34:12 2022

	Supplier starts his work: Sun Feb 13 19:34:12 2022
	Supplier wrote. Buffer: 1
	Supplier ends his work: Sun Feb 13 19:34:12 2022

	Consumer starts his work: Sun Feb 13 19:34:15 2022
	Consumer read. Buffer: 0
	Consumer ends his work: Sun Feb 13 19:34:15 2022
*/
