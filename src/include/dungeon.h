#pragma once

#include <pthread.h>
#include "monitor.h"

typedef struct {
    pthread_t instance;
    int id;
    int status;
    int parties_served;
    int tanks_served;
    int healers_served;
    int dps_served;
    int total_time_served;
} Instance;

typedef struct {
    Instance *instances;
    fifo_monitor_t lfg_monitor;
    int num_instances;
    int tanks;
    int healers;
    int dps;
    int t1;
    int t2;
} Dungeon;

void runDungeon(Dungeon *d);
void printInstanceStats(Dungeon d);