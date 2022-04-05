#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <mutex>

using namespace std;

struct attr
{
    int flag;
    pthread_mutex_t *m_id;
    attr (int f, pthread_mutex_t *m)
    {
        flag = f;
        m_id = m;
    }
};

void *f1(void *ptr)
{
    while ((int)(((attr*)ptr)->flag) == 0)
    {
        pthread_mutex_lock(((attr*)ptr)->m_id);
        for (int i = 0; i < 5; i++)
        {
            cout << "1" << flush;
            sleep (1);
        }
        pthread_mutex_unlock(((attr*)ptr)->m_id);
        sleep(1);
    }
    ((attr*)ptr)->flag = 50;
    pthread_exit(ptr);
}

void *f2(void *ptr)
{
    while ((int)(((attr*)ptr)->flag) == 0)
    {
        pthread_mutex_lock(((attr*)ptr)->m_id);
        for (int i = 0; i < 5; i++)
        {
            cout << "2" << flush;
            sleep (1);
        }
        pthread_mutex_unlock(((attr*)ptr)->m_id);
        sleep(1);
    }
    (((attr*)ptr))->flag = 40;
    pthread_exit(ptr);
}

int main()
{
    //Объявление переменных
    pthread_mutex_t m_id;
    pthread_t id1;
    pthread_t id2;
    attr a1 (0, &m_id);
    attr a2 (0, &m_id);
    //Инициализация мьютекса
    pthread_mutex_init(&m_id, NULL);
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
    pthread_mutex_destroy(&m_id);
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    cout << endl << "Flags: " << a1.flag << ", " << a2.flag  << endl << "End" << endl;
}
