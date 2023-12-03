#include "include/dungeon.h"
#include "include/monitor.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define EMPTY 0
#define ACTIVE 1

typedef struct {
    dungeon_t *dungeon;
    int id;
} run_instance_args_t;

void printInstances(dungeon_t *d)
{
    printf("\e[1;1H\e[2J"); 
    printf("Remaining:\n\tTanks: %d\n\tHealers: %d\n\tDPS: %d\n\n",
            d->tanks, d->healers, d->dps);
    for (int i = 0; i < d->num_instances; i += 4) {
        for (int j = 0; j < 4 && i + j < d->num_instances; j++) {
            instance_t tmp = d->instances[i + j];
            printf("Instance %d: %s\t", tmp.id, tmp.status ? "ACTIVE" : "EMPTY");
        }
        printf("\n");
    }
}

void *runInstance(void *argp)
{
    run_instance_args_t *args = argp;
    dungeon_t *d = args->dungeon;
    instance_t *i = &d->instances[args->id];

    while (1) {
        // CRITICAL SECTION
        fifo_monitor_lock(&d->lfg_monitor);
        printInstances(d);
        // strictly enforce party composition and don't allow leftovers to enter instance
        if (d->tanks < 1 || d->healers < 1 || d->dps < 3) {
            fifo_monitor_unlock(&d->lfg_monitor);
            break;
        }
        d->tanks--;
        i->tanks_served++;
        d->healers--;
        i->healers_served++;
        d->dps -= 3;
        i->dps_served += 3;
        i->status = ACTIVE;
        printInstances(d);
        fifo_monitor_unlock(&d->lfg_monitor);
        // END OF CRITICAL SECTION
        
        int duration = d->t1 + (rand() % (d->t2 - d->t1 + 1));
        sleep(duration);
        i->total_time_served += duration;
        i->parties_served++;
        i->status = EMPTY;
    }

    free(args);
    return NULL;
}

void runDungeon(dungeon_t *d)
{
    srand(time(NULL));
    for (int i = 0; i < d->num_instances; i++) {
        run_instance_args_t *args = malloc(sizeof(run_instance_args_t));
        if (args == NULL) {
            fprintf(stderr, "Error: Failed to create instance args (id=%d)\n", i);
            continue;
        }
        args->dungeon = d;
        args->id = i;
        if (pthread_create(&d->instances[i].instance, NULL, runInstance, args) != 0)
            fprintf(stderr, "Error: Failed to run instance (id=%d)", i);
    }
    for (int i = 0; i < d->num_instances; i++)
        pthread_join(d->instances[i].instance, NULL);
}

void printInstanceStats(dungeon_t d)
{
    printf("\n============ SUMMARY ============\n\n");
    int total_served = 0;
    for (int i = 0; i < d.num_instances; i++) {
        instance_t instance = d.instances[i];
        printf("Instance %d:\n"
                "\tParties Served: %d\n"
                "\t\tTanks Served: %d\n"
                "\t\tHealers Served: %d\n"
                "\t\tDPS Served: %d\n"
                "\tTotal Time Served: %d\n",
                instance.id, instance.parties_served, instance.tanks_served,
                instance.healers_served, instance.dps_served,
                instance.total_time_served
        );
        total_served += instance.parties_served;
    }
    printf("\nTotal Parties Served: %d\n", total_served);
    printf("\nLeftovers:\n\tTanks: %d\n\tHealers: %d\n\tDPS: %d\n",
            d.tanks, d.healers, d.dps);
    printf("\n========= END OF SUMMARY =========\n");
}