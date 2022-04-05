/*
* Выполнил: Халатова Екатерина 8375
* Задание: 1 "Поставщик - Потребитель"
* Дата выполнения: 13.02.2022
* Версия: 0.1
*
* Скрипт для компиляции и запуска программы:
* gcc -pthread -o condition 1_Halatova_8375.c
* ./condition
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
* с одной переменной была использована условная переменная.
*/
// ------------------------------------------- // 


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


pthread_cond_t cond;
pthread_mutex_t mutex;

int Buffer_size = 5;
int Buffer = 0;


void* Supplier(void* arg);
void* Consumer(void* arg);


int main(int argc, char *argv[]) {
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);
    
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
		pthread_mutex_lock(&mutex);
        
		time_t current_time = time(NULL);
		char* c_time_string = ctime(&current_time);
		printf("Supplier starts his work: %s", c_time_string);

		if (Buffer < Buffer_size)
		{
			Buffer++;
			printf("Supplier wrote. Buffer: %i\n", Buffer);
            
			if (Buffer > 0)
				pthread_cond_signal(&cond);
		} 
		else  {
            printf("Buffer is full. Block Supplier. %i\n", Buffer);
			pthread_cond_wait(&cond, &mutex);
        }
		
		current_time = time(NULL);
		c_time_string = ctime(&current_time);
		printf("Supplier ends his work: %s\n", c_time_string);
        
		pthread_mutex_unlock(&mutex);
        
		sleep(2);
	}
	return EXIT_SUCCESS;
}


void* Consumer(void* arg) {

	while (1)
	{
		pthread_mutex_lock(&mutex);

        time_t current_time = time(NULL);
		char* c_time_string = ctime(&current_time);
		printf("Consumer starts his work: %s", c_time_string);
        
		if (Buffer > 0)
		{
			Buffer--;
			printf("Consumer read. Buffer: %i\n", Buffer);
            
			if (Buffer < Buffer_size)
				pthread_cond_signal(&cond);
		} else {
            printf("Buffer is empty. Block Consumer. %i\n", Buffer);
			pthread_cond_wait(&cond, &mutex);
        }

		current_time = time(NULL);
		c_time_string = ctime(&current_time);
		printf("Consumer ends his work: %s\n", c_time_string);
        
		pthread_mutex_unlock(&mutex);
        
		sleep(1);
	}
	return EXIT_SUCCESS;
}


// ----------------------------------------//
/*
    Consumer starts his work: Tue Feb 13 19:56:22 2022
    Consumer can't read. Buffer is emply
    Consumer ends his work: Tue Feb 13 19:56:22 2022

    Supplier starts his work: Tue Feb 13 19:56:22 2022
    Supplier wrote. Buffer: 1
    Supplier ends his work: Tue Feb 13 19:56:22 2022

    Consumer starts his work: Tue Feb 13 19:56:25 2022
    Consumer read. Buffer: 0
    Consumer ends his work: Tue Feb 13 19:56:25 2022
*/
