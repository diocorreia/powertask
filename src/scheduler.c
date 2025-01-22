#include <stdio.h>
#include <assert.h>

#include <powertask/scheduler.h>
#include <powertask/energy.h>
#include <powertask/storage.h>

#define TASK_SCHEDULER_MAX_NUMBER_OF_TASKS 255
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

/** @brief Current scheduler state*/
struct current_state_s {
    bool tasks_state[TASK_SCHEDULER_MAX_NUMBER_OF_TASKS]; /**< Array containing current task states. */
    int number_of_tasks; /**< Number of valid elements in tasks_state. */
};

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                    Private API                                                     */
/* ------------------------------------------------------------------------------------------------------------------ */

static void reset_current_state(powertask_scheduler *sched){
    for(int i = 0; i < sched->number_of_tasks; i++){
        sched->list_of_tasks[i]->complete = false;
    }
}

static void save_current_state(powertask_scheduler *sched){
    struct current_state_s to_save;

    if(sched->number_of_tasks > ARRAY_LENGTH(to_save.tasks_state)){
        return;
    }

    to_save.number_of_tasks = sched->number_of_tasks;

    for(int i = 0; i < sched->number_of_tasks; i++){
        to_save.tasks_state[i] = sched->list_of_tasks[i]->complete;
    }

    powertask_storage_save(&to_save, sizeof(struct current_state_s));
}

static void load_current_state(powertask_scheduler *sched){
    int err = 0;
    struct current_state_s loaded;
    
    err = powertask_storage_load(&loaded, sizeof(struct current_state_s));
    
    if(err < 0){
        return;
    }

    if(loaded.number_of_tasks > sched->_list_of_tasks_len){
        return;
    }

    for(int i = 0; i < loaded.number_of_tasks; i++){
        sched->list_of_tasks[i]->complete = loaded.tasks_state[i];
    }
}

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                     Public API                                                     */
/* ------------------------------------------------------------------------------------------------------------------ */

void powertask_add(powertask_scheduler *sched, powertask_task *task){
    if(sched->number_of_tasks >= sched->_list_of_tasks_len){
        return;
    }
    sched->list_of_tasks[sched->number_of_tasks++] = task;
    return;
}

void powertask_run_scheduler(powertask_scheduler *sched, powertask_energy_source_t *energy_source){

    int i = 0;
    int complete_tasks = 0;
    powertask_task *current_task;

    if(sched == NULL || energy_source == NULL){
        return;
    }

    load_current_state(sched);

    for(;i < sched->number_of_tasks; i++){
        current_task = sched->list_of_tasks[i];

        if(current_task->complete){
            complete_tasks++;
            continue;
        }

        int available_energy = powertask_get_available_energy(energy_source);

        if(available_energy <= current_task->required_energy){
            continue;
        }

        if(!current_task->condition()){
            continue;
        }

        if(current_task->action != NULL){
            current_task->action();
        }
        
        current_task->complete = true;
        complete_tasks++;
    }

    if(complete_tasks == sched->number_of_tasks){
        reset_current_state(sched);
    }

    save_current_state(sched);
}
