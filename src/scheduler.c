#include <stdio.h>

#include <powertask/scheduler.h>
#include <powertask/energy.h>
#include <powertask/storage.h>

#define TASK_SCHEDULER_MAX_NUMBER_OF_TASKS 255

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                    Private API                                                     */
/* ------------------------------------------------------------------------------------------------------------------ */

static void reset_current_state(powertask_scheduler *sched){
    for(int i = 0; i < sched->number_of_tasks; i++){
        sched->list_of_tasks[i]->complete = false;
    }
}

static void save_current_state(powertask_scheduler *sched){
    bool tasks_state[TASK_SCHEDULER_MAX_NUMBER_OF_TASKS];

    for(int i = 0; i < sched->number_of_tasks; i++){
        tasks_state[i] = sched->list_of_tasks[i]->complete;
    }

    powertask_storage_save(tasks_state, sizeof(tasks_state));
}

static void load_current_state(powertask_scheduler *sched){
    bool tasks_state[TASK_SCHEDULER_MAX_NUMBER_OF_TASKS];
    
    powertask_storage_load(tasks_state, sizeof(tasks_state));

    if(sched->number_of_tasks > TASK_SCHEDULER_MAX_NUMBER_OF_TASKS){
        return;
    }

    for(int i = 0; i < sched->number_of_tasks; i++){
        sched->list_of_tasks[i]->complete = tasks_state[i];
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

void powertask_run_scheduler(powertask_scheduler *sched){

    int i = 0;
    int complete_tasks = 0;
    powertask_task *current_task;

    load_current_state(sched);

    for(;i < sched->number_of_tasks; i++){
        current_task = sched->list_of_tasks[i];

        if(current_task->complete){
            complete_tasks++;
            continue;
        }

        int available_energy = powertask_get_available_energy();

        if(available_energy <= current_task->required_energy){
            continue;
        }

        if(!current_task->condition()){
            continue;
        }

        if(current_task->action != NULL){
            current_task->action();
            current_task->complete = true;
            complete_tasks++;
        }
    }

    if(complete_tasks == sched->number_of_tasks){
        reset_current_state(sched);
    }

    save_current_state(sched);
}
