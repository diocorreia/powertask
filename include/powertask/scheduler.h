#ifndef POWERTASK_SCHEDULER_H
#define POWERTASK_SCHEDULER_H

#include <stdbool.h>

/** @brief Task */
typedef struct powertask_task_s {
    void (*action)(void);    /**< Action to be executed. */
    bool (*condition)(void); /**< Condition that allows execution of the task. */
    int required_energy;     /**< Required energy (in Joules) to run the task. */
    bool complete;           /**< Indicates if the task was already executed. */
} powertask_task;

/** @brief Scheduler */
typedef struct powertask_scheduler_s {
    powertask_task **list_of_tasks; /**< List with scheduled tasks. */
    int number_of_tasks;            /**< Current number of scheduled tasks in the list. */
    int _list_of_tasks_len;         /**< Maximum number of tasks allowed in the list. */
} powertask_scheduler;

/**
 * @brief Runs the scheduled tasks 
 */
void powertask_run_scheduler(powertask_scheduler *sched);

/**
 * @brief Add task to a scheduler
 * 
 * @param[in] sched Scheduler to which the task will be added.
 * @param[in] task  Task to be added to the scheduler.
*/
void powertask_add(powertask_scheduler *sched, powertask_task *task);

/** 
 * @brief Initialize scheduler
 * 
 * @param[in] _name Name to be given to the scheduler.
 * @param[in] _number_of_tasks Maximum number of tasks to be allowed on the scheduler. 
 */
#define POWERTASK_INIT(_name, _number_of_tasks)                         \
    static powertask_task * _name##_list_of_tasks[_number_of_tasks];    \
    static struct powertask_scheduler_s _name = {                       \
        .list_of_tasks = _name##_list_of_tasks ,                        \
        ._list_of_tasks_len = _number_of_tasks,                       \
}

/** @brief Implementation of run always macro. */
static bool _run_always(void)
{
    return true;
}

/** @brief Always execute the action (when energy is available). */
#define POWERTASK_RUN_ALWAYS _run_always

/** 
 * @brief Wait for a task is finished to continue.
 * 
 * @param[in] _task Task to wait for. 
 */
#define POWERTASK_WAIT_FOR(_task) _is_##_task##_complete

/**
 * @brief Declares a task.
 * 
 * @param[in] _name Name to be given to the task.
 */
#define POWERTASK_DECLARE(_name)            \
static powertask_task task_##_name = {      \
    .complete = false,                      \
};                                          \
static bool _is_##_name##_complete() {      \
    return task_##_name.complete;           \
}

/**
 * @brief Declare task
 * 
 * @param[in] _scheduler        Scheduler to which task should be added.
 * @param[in] _name             Name used to identify the task.
 * @param[in] _action           Action to be executed.
 * @param[in] _condition        Function defining in which condition the action
 * will be executed.
 * @param[in] _required_energy  Minimum amount of energy (in Joules) required to
 * execute the action.
 */
#define POWERTASK_TASK(_scheduler, _name, _action, _condition, _required_energy)    \
task_##_name = (powertask_task){                                                    \
    .action = _action,                                                              \
    .condition = _condition,                                                        \
    .required_energy = _required_energy,                                            \
};                                                                                  \
powertask_add(&_scheduler, &task_##_name);

#endif /* POWERTASK_SCHEDULER_H */
