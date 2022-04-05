/*
* Выполнил: Халатова Екатерина 8375
* Задание: 2 "Вызов лифта"
* Дата выполнения: 16.02.2022
* Версия: 0.1
*
* Скрипт для компиляции и запуска программы:
* gcc -pthread -o elevator 2_Halatova_8375.c
* ./elevator
*/
// ------------------------------------------- //
/*
* Общее описание программы:
* 
* Рассматривается лестничная площадка дома на некотором этаже. 
* Около двери лифта расположена панель с двумя кнопками вызова  
* «Вверх» (Up) и «Вниз» (Down). Предполагается, что лифт может 
* находиться в одном из трех статических состояний (current_state) - 
* верху (TOP), на данном этаже (THIS_FLOOR) и внизу (BOTTOM). 
* 
* При нажатии кнопки «Верх»   или «Вниз» формируется команда 
* (посылается сигнал реального времени Up или Down) , и лифт 
* совершает движение в соответствии с таблицей , представленной 
* ниже. Предполагается,  что после того, как лифт прибыл на этаж 
* вызова, выдерживается задержка в  1с, и лифт двигается вверх 
* или вниз в зависимости от того, какая кнопка вызова была нажата.
* 
*/
// ------------------------------------------- // 

#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define	TOP				0  // лифт вверху
#define	BOTTOM			1  // лифт внизу
#define	THIS_FLOOR		2  // лифт на нашем этаже

#define	Up		  48  // Сигналы реального времени
#define	Down	  49

int current_state = 2;
		
void lift_control(int singo, siginfo_t *info, void *nk)
{
	switch (current_state)
	{
	case THIS_FLOOR:
		if (singo == Up)
		{
			printf("This Floor, door is opened \n");
			sleep(1);
			printf("Moving up\n");
			current_state = TOP;
		}
		else if (singo = Down)
		{
			printf("This Floor, door is opened \n");
			sleep(1);
			printf("Moving down\n");
			current_state = BOTTOM;
		}
		break;
		
	case TOP:
		if (singo == Up)
		{
			printf("Moving down\n");
			printf("This Floor, door is opened \n");
			sleep(1);
			printf("Moving up\n");
			current_state = TOP;
		}
		else if (singo = Down)
		{
			printf("Moving down\n");
			printf("This Floor, door is opened \n");
			sleep(1);
			printf("Moving down\n");
			current_state = BOTTOM;
		}
		break;
		
	case BOTTOM:
		if (singo == Up)
		{
			printf("Moving up\n");
			printf("This Floor, door is opened \n");
			sleep(1);
			printf("Moving up\n");
			current_state = TOP;
		}
		else if (singo = Down)
		{
			printf("Moving up\n");
			printf("This Floor, door is opened \n");
			sleep(1);
			printf("Moving down\n");
			current_state = BOTTOM;
		}
		break;
		
	default:
		break;
	}
}

void *push_button(void *args)
{
	struct sigaction act;
	
	act.sa_sigaction = lift_control;
	act.sa_flags = SA_SIGINFO;
	
	sigaction(Up, &act, NULL);
	sigaction(Down, &act, NULL);
	
	char ch;
	while (ch != 'q')
	{
		ch = getchar();
		switch (ch)
		{
		case 'u':
			raise(Up);
			break;
		case 'd':
			raise(Down);
			break;
		case 'q':
			break;
		}
	}
	
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{

	pthread_t t;
	printf("Start\n");
	
	pthread_create(&t, NULL, &push_button, NULL);
	pthread_join(t, NULL);
	
	printf("Finish\n");
	exit(EXIT_SUCCESS);
}



// ----------------------------------------//
/*
	Start
	u
	This Floor, door is opened 
	Moving up
	u
	Moving down
	This Floor, door is opened 
	Moving up
	d
	Moving down
	This Floor, door is opened 
	Moving down
	d
	Moving up
	This Floor, door is opened 
	Moving down
	u
	Moving up
	This Floor, door is opened 
	Moving up
	q
	Finish
*/
