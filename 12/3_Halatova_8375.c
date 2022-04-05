/*
* Выполнил: Халатова Екатерина 8375
* Задание: 3 "Обеспечение таймаута"
* Дата выполнения: 16.02.2022
* Версия: 0.1
*
* Скрипт для компиляции и запуска программы:
* gcc -o timeout 3_Halatova_8375.c
* ./timeout
*/
// ------------------------------------------- //
/*
* Общее описание программы:
* 
* Предполагается, что некоторая система управления 
* циклически, с заданным периодом измеряет состояние 
* объекта , выполняет действия по принятию решения, 
* и формируют управляющие воздействия. Все эти действия 
* реализуются некоторой функцией doControl(). Предполагается, 
* что расчетное время работы функции doControl() составляет 
* 0.5 секунды и может увеличиваться в результате воздействия 
* случайных факторов. Допустимое превышение времени выполнения 
* (время выполнения - «мягкий» deadline, soft_dt) не должно 
* составлять более 4% расчетного времени.
* 
* Предполагается, что расчетное время работы функции 
* doControl() составляет 0.5 секунды и может увеличиваться 
* в результате воздействия случайных факторов. Допустимое 
* превышение времени выполнения (время выполнения - «мягкий» 
* deadline, soft_dt) не должно составлять более 4% расчетного времени.
* 
* Далее, предполагается, что в результате неисправности исполнительного 
* оборудования функция управления может «зависнуть», и в этом случае 
* система требует перезагрузки. В данной работе вероятность "зависания" 
* берется 20%. Таймаут обработки зависания - 2 секунды
* 
*/
// ------------------------------------------- // 


#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>


int doControl() {
	// Рассчитываем время работы
	int max = 520;
	int time = 500;
	int dt = rand() % (40 + 1);
	time += dt;
	printf("Work time: 0.%d\n", time % 1000);
	
	//Считаем вероятность зависания
	int error = rand() % (100 + 1);
	if (error > 80) 
		while(1);
	
	usleep(time * 1000);
	return time;
}

void Deadline(int singo, siginfo_t* info, void* nk){
	printf("####   Deadline exceeded   ####\n");
}

void Restart(int singo, siginfo_t* info, void* nk){
	printf("#####-- Restart requered! --#####\n");
	raise(SIGUSR1);
}

int main() {
	srand(time(NULL));   //Инициализация рандомайзера
	
	struct sigaction act_deadline;
	act_deadline.sa_sigaction = Deadline;
	act_deadline.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR2, &act_deadline, NULL);

	struct sigaction act_restart;
	act_restart.sa_sigaction = Restart;
	sigaction(SIGALRM, &act_restart, NULL);

	while (1) {
		alarm(2);
		
		time_t current_time = time(NULL);
		char* c_time_string = ctime(&current_time);
		printf("Start time: %s", c_time_string);
		
		int action_time = doControl();
		
		if (action_time > 520) {
			raise(SIGUSR2);
		}
		
		current_time = time(NULL);
		c_time_string = ctime(&current_time);
		printf("End time: %s\n", c_time_string);

		alarm(0);
		sleep(3);
	};

}


// ----------------------------------------//
/*
    Start time: Wed Feb 16 14:11:04 2022
    Work time: 0.522
    ####   Deadline exceeded   ####
    End time: Wed Feb 16 14:11:05 2022

    Start time: Wed Feb 16 14:11:08 2022
    Work time: 0.526
    ####   Deadline exceeded   ####
    End time: Wed Feb 16 14:11:08 2022

    Start time: Wed Feb 16 14:11:11 2022
    Work time: 0.511
    End time: Wed Feb 16 14:11:12 2022

    Start time: Wed Feb 16 14:11:15 2022
    Work time: 0.508
    End time: Wed Feb 16 14:11:15 2022

    Start time: Wed Feb 16 14:11:18 2022
    Work time: 0.533
    ####   Deadline exceeded   ####
    End time: Wed Feb 16 14:11:19 2022

    Start time: Wed Feb 16 14:11:22 2022
    Work time: 0.538
    ####   Deadline exceeded   ####
    End time: Wed Feb 16 14:11:22 2022

    Start time: Wed Feb 16 14:11:25 2022
    Work time: 0.535
    #####-- Restart requered! --#####
    Определяемый пользователем сигнал 1
*/
