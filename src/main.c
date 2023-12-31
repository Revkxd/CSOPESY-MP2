#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "include/dungeon.h"
#include "include/monitor.h"

int main(int argc, char **argv)
{
    if (argc != 7) {
        fprintf(stderr, "Expected 6 arguments.\n" "Usage: %s <num_instances> "
        "<num_tanks> <num_healers> <num_dps> <t1> <t2>\n", argv[0]);
        exit(0);
    }

    int num_instances = atoi(argv[1]);
    int tanks = atoi(argv[2]);
    int healers = atoi(argv[3]);
    int dps = atoi(argv[4]);
    int t1 = atoi(argv[5]);
    int t2 = atoi(argv[6]);

    instance_t instances[num_instances];
    for (int i = 0; i < num_instances; i++)
        instances[i] = (instance_t){(pthread_t)NULL, i + 1, 0, 0, 0, 0, 0, 0};
    fifo_monitor_t m1 = FIFO_MONITOR_INIT;
    dungeon_t d = {instances, m1, num_instances, tanks, healers, dps, t1, t2};
    runDungeon(&d);
    printInstanceStats(d);

    return 0;
}