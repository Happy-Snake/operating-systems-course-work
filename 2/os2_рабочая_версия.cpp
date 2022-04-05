#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <mutex>

pthread_mutex_t m_id;

void *f1(void *ptr)
{
    while (*((int*)ptr) == 0)
    {
        pthread_mutex_lock(&m_id);
        for (int i = 0; i < 5; i++)
        {
            std::cout << "1" << std::flush;
            sleep (1);
        }
        pthread_mutex_unlock(&m_id);
        sleep(1);
    }
    *((int*)ptr) = 50;
    pthread_exit(ptr);
}

void *f2(void *ptr)
{
    while (*((int*)ptr) == 0)
    {
        pthread_mutex_lock(&m_id);
        for (int i = 0; i < 5; i++)
        {
            std::cout << "2" << std::flush;
            sleep (1);
        }
        pthread_mutex_unlock(&m_id);
        sleep(1);
    }
    *((int*)ptr) = 40;
    pthread_exit(ptr);
}

int main()
{
    pthread_t id1;
    pthread_t id2;
    pthread_mutex_init(&m_id, NULL);
    std::cout << "Beginning" << std::endl;
    int fl1 = 0;
    int fl2 = 0;
    void *status1 = 0;
    void *status2 = 0;
    pthread_create(&id1, NULL, f1, &fl1);
    pthread_create(&id2, NULL, f2, &fl2);
    std::cout << "Press enter" << std::endl;
    getchar();
    fl1 = 1;
    fl2 = 2;
    pthread_mutex_destroy(&m_id);
    pthread_join(id1, &status1);
    pthread_join(id2, &status1);
    std::cout  << "Flags: " << fl1 << fl2  << std::endl << "End" << std::endl;
}
