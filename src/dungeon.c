#include "include/dungeon.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    Dungeon *dungeon;
    int id;
} run_instance_args;

void *runInstance(void *argp)
{
    run_instance_args *args = argp;
    Dungeon *d = args->dungeon;
    Instance *i = &d->instances[args->id];

    while (1) {
        printf("Instance %d: EMPTY\n", i->id);

        // CRITICAL SECTION
        pthread_mutex_lock(&d->lfg_mutex);
        // strictly enforce party composition and don't allow leftovers to enter instance
        if (d->tanks < 1 || d->healers < 1 || d->dps < 3) {
            pthread_mutex_unlock(&d->lfg_mutex);
            break;
        }
        d->tanks--;
        i->tanks_served++;
        d->healers--;
        i->healers_served++;
        d->dps -= 3;
        i->dps_served += 3;
        pthread_mutex_unlock(&d->lfg_mutex);
        // END OF CRITICAL SECTION
        
        int duration = d->t1 + (rand() % (d->t2 - d->t1 + 1));
        printf("Instance %d: ACTIVE\n", i->id);
        sleep(duration);
        i->total_time_served += duration;
        i->parties_served++;
    }

    free(args);
    return NULL;
}

void runDungeon(Dungeon *d)
{
    srand(time(NULL));
    for (int i = 0; i < d->num_instances; i++) {
        run_instance_args *args = malloc(sizeof(run_instance_args));
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

void printInstanceStats(Dungeon d)
{
    printf("\n============ SUMMARY ============\n\n");
    for (int i = 0; i < d.num_instances; i++) {
        Instance instance = d.instances[i];
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
    }
    printf("\nLeftovers:\n\tTanks: %d\n\tHealers: %d\n\tDPS: %d\n",
            d.tanks, d.healers, d.dps);
    printf("\n========= END OF SUMMARY =========\n");
}