#pragma once

#include <pthread.h>

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    size_t head;
    size_t tail;
} fifo_monitor_t;

void fifo_monitor_lock(fifo_monitor_t *monitor);
void fifo_monitor_unlock(fifo_monitor_t *monitor);