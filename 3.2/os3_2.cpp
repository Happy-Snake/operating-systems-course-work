#include <fcntl.h>  
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <mutex>
#include <cstring>
#include <time.h>

using namespace std;

struct attr
{
    int flag;
    int *fd;
    attr (int f, int *f1)
    {
        flag = f;
        fd = f1;
    }
};

void *f1(void *ptr)
{
    int buf;
    while ((int)(((attr*)ptr)->flag) == 0)
    {
        //сформировать сообщение в буфере
        buf = 1;
        // записать сообщение из буфера в неименованный канал
        int res = write(*(((attr*)ptr)->fd), &buf, sizeof(buf));
        if (res != -1)
            //задержать на время
            cout << "Запись" << endl;
        else 
            cout << "Ошибка записи - " << strerror(res) << endl;
        sleep(2);
    }
    ((attr*)ptr)->flag = 50;
    pthread_exit(ptr);
}

void *f2(void *ptr)
{
    int buf;
    while ((int)(((attr*)ptr)->flag) == 0)
    {
        //Чтение сообщения из неименованного канала в буфер
        int res = read(*(((attr*)ptr)->fd), &buf, sizeof(buf)); 
        if (res != -1)
        {
            //Вывод сообщения на экран
            cout << "Вывод - " << buf << endl;
            //Очистка буфера
            buf = 0;
            cout << "Очистка буфера - " << buf << endl;
        }
        else 
            cout << "Ошибка вывода - " << strerror(res) << endl;
        sleep(2);
    }
    (((attr*)ptr))->flag = 40;
    pthread_exit(ptr);
}

int main()
{
    //Объявление переменных
    pthread_t id1;
    pthread_t id2;
    //Объявление дескрипторов
    int filedes[2];
    //Создание неименованного канала
    if (!pipe2(filedes, O_NONBLOCK))
    {
        attr a1 (0, &filedes[1]);
        attr a2 (0, &filedes[0]);
        //Начало
        cout << "Beginning" << endl << "Flags: " << a1.flag << ", " << a2.flag << endl;
        //Создание потоков
        pthread_create(&id1, NULL, f1, &a1);
        pthread_create(&id2, NULL, f2, &a2);
        //Ожидание нажатия Enter и смена флагов
        cout << "Press enter" << endl;
        getchar();
        cout << "Completing the work process" << endl;
        a1.flag = 1;
        a2.flag = 2;
        //Завершение
        pthread_join(id1, NULL);
        pthread_join(id2, NULL);
        close (filedes[0]);
        close (filedes[1]);
        cout << endl << "Flags: " << a1.flag << ", " << a2.flag  << endl << "End" << endl;
    }
    else 
        cout << "Ошибка" << endl;
}
