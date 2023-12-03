#include "include/monitor.h"
#include <pthread.h>


void fifo_monitor_lock(fifo_monitor_t *monitor)
{
    pthread_mutex_lock(&monitor->lock);
    size_t thread_idx = monitor->tail;
    while (thread_idx != monitor->head)
        pthread_cond_wait(&monitor->cond, &monitor->lock);
    pthread_mutex_unlock(&monitor->lock);
}

void fifo_monitor_unlock(fifo_monitor_t *monitor)
{
    pthread_mutex_lock(&monitor->lock);
    monitor->head++;
    pthread_cond_broadcast(&monitor->cond);
    pthread_mutex_unlock(&monitor->lock);
}