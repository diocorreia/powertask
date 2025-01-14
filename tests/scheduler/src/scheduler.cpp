#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <stdio.h>
#include <stdbool.h>

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

extern "C"
{
	#include <powertask/scheduler.h>
	#include <powertask/energy.h>
}

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                               Test groups declaration                                              */
/* ------------------------------------------------------------------------------------------------------------------ */

TEST_GROUP(test_scheduler_regular){
	void setup(){

	}

	void teardown(){
		mock().clear();
	}
};

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                    Internal variables - test_scheduler_regular                                     */
/* ------------------------------------------------------------------------------------------------------------------ */

/** @brief Successful condition */
bool condition_success() {
	return true;
}

/** @brief Failing condition */
bool condition_fails() {
	return false;
}

/** @brief Action of the 1st task */
void task1(){
	mock().actualCall("task1");
}

/** @brief Action of the 2nd task */
void task2(){
	mock().actualCall("task2");
}

/** Tasks declaration */
POWERTASK_DECLARE(task1);
POWERTASK_DECLARE(task2);

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                        Unit Tests - test_scheduler_regular                                         */
/* ------------------------------------------------------------------------------------------------------------------ */

/**
 * @brief Scheduler initialization
 *
 * The scope of this unit test is to validate if the scheduler instance is
 * initialized correctly.
 *
 * It is expected that a scheduler instance is created with the 0 assigned tasks
 * to it and with a maximum number of tasks set.  
 */
TEST(test_scheduler_regular, test_scheduler_init){
	POWERTASK_INIT(scheduler, 1);

	CHECK_EQUAL(0, scheduler.number_of_tasks);
	CHECK_EQUAL(1, scheduler._list_of_tasks_len);
}

/**
 * @brief Task setup
 *
 * The scope of this unit test is to validate if the POWERTASK_TASK(...) sets up
 * the given task correctly. 
 *
 * It is expected the given task to have the action, condition and required
 * energy that were given to the macro. It is also expected, the task to have
 * the complete flag as false. And the task to be added to the given scheduler. 
 */
TEST(test_scheduler_regular, test_task_setup){
	const int required_energy = 400;

	POWERTASK_INIT(scheduler, 1);
	POWERTASK_TASK(scheduler, task1, task1, POWERTASK_RUN_ALWAYS, required_energy);

	CHECK_EQUAL(task_task1.action, task1);
	CHECK_EQUAL(task_task1.condition, POWERTASK_RUN_ALWAYS);
	CHECK_EQUAL(task_task1.required_energy, required_energy);
	CHECK_EQUAL(task_task1.complete, false);

	CHECK_EQUAL(scheduler.list_of_tasks[0], &task_task1);
}

/**
 * @brief Scheduler - More tasks than allowed
 * 
 * The scope of this unit test is to validate if scheduler rejects adding more
 * tasks than its the maximum allow amount.
 * 
 * Is is expected the scheduler to only have one task. And this task to be the
 * first added.
 */
TEST(test_scheduler_regular, test_scheduler_task_overflow)
{
	POWERTASK_INIT(scheduler, 1);
	POWERTASK_TASK(scheduler, task1, task1, POWERTASK_RUN_ALWAYS, 400);
	POWERTASK_TASK(scheduler, task2, task2, POWERTASK_RUN_ALWAYS, 400);

	CHECK_EQUAL(1, scheduler.number_of_tasks);
	CHECK_EQUAL(&task_task1,scheduler.list_of_tasks[0]);
}

/**
 * @brief Scheduler - run_scheduler with invalid arguments
 * 
 * The scope of this unit test is to validate if the function does not proceed
 * after checking one or more arguments are valid.
 * 
 * Is is expected the function to not proceed further than the argument check
 * and not load the previous scheduler state.
 */
TEST(test_scheduler_regular, test_scheduler_run_scheduler_with_invalid_arguments)
{
	// Both arguments are NULL
	powertask_run_scheduler(NULL, NULL);

	// Energy source is NULL
	POWERTASK_INIT(scheduler, 1);
	powertask_run_scheduler(&scheduler, NULL);

	// Scheduler is NULL
	powertask_energy_source_t energy_src = {0}; 
	powertask_run_scheduler(NULL, &energy_src);

	mock().expectNoCall("powertask_storage_load");

	mock().checkExpectations();
}

/**
 * @brief Scheduler - Task with more than enough energy
 * 
 * The scope of this unit test is to validate if the scheduled task is executed
 * when there is more available energy than the minimum required by the task.
 * 
 * Is is expected the action setup on the task to be executed.
 */
TEST(test_scheduler_regular, test_single_task_enough_energy)
{
	const int required_energy = 400;

	POWERTASK_INIT(scheduler, 1);
	POWERTASK_TASK(scheduler, task1, task1, POWERTASK_RUN_ALWAYS, required_energy);

	mock().expectOneCall("powertask_get_available_energy").andReturnValue(required_energy+1);
	mock().expectOneCall("task1");
	mock().ignoreOtherCalls();

	powertask_energy_source_t energy_src = {0};
	powertask_run_scheduler(&scheduler, &energy_src);

	mock().checkExpectations();
};

/**
 * @brief Scheduler - Task with not enough energy
 * 
 * The scope of this unit test is to validate if the scheduled task is not
 * executed when there is no enough available energy to execute the task.
 * 
 * It is expected the action setup on the task to not be executed.
 */
TEST(test_scheduler_regular, test_single_task_no_energy)
{
	const int required_energy = 400;

	POWERTASK_INIT(scheduler, 1);
	POWERTASK_TASK(scheduler, task1, task1, POWERTASK_RUN_ALWAYS, required_energy);

	mock().expectOneCall("powertask_get_available_energy").andReturnValue(required_energy-1);
	mock().expectNoCall("task1");
	mock().ignoreOtherCalls();

	powertask_energy_source_t energy_src = {0};
	powertask_run_scheduler(&scheduler, &energy_src);

	mock().checkExpectations();
};

/**
 * @brief Scheduler - Scheduler executes a task when its conditions are met.
 * 
 * The scope of this unit test is to validate if the scheduled task is executed
 * when its condition is met (and there is enough available energy).
 * 
 * Is is expected the action setup on the task to be executed.
 */
TEST(test_scheduler_regular, test_single_task_condition_success)
{
	const int required_energy = 400;

	POWERTASK_INIT(scheduler, 1);
	POWERTASK_TASK(scheduler, task1, task1, condition_success, required_energy);

	mock().expectOneCall("powertask_get_available_energy").andReturnValue(required_energy+1);
	mock().expectOneCall("task1");
	mock().ignoreOtherCalls();

	powertask_energy_source_t energy_src = {0};
	powertask_run_scheduler(&scheduler, &energy_src);

	mock().checkExpectations();
};

/**
 * @brief Scheduler - Scheduler executes a task when its conditions are not met.
 * 
 * The scope of this unit test is to validate if the scheduled task is not
 * executed when its condition is not met (despite having enough available
 * energy to run the task).
 * 
 * It is expected the action setup on the task to not be executed.
 */
TEST(test_scheduler_regular, test_single_task_condition_fails)
{
	const int required_energy = 400;

	POWERTASK_INIT(scheduler, 1);
	POWERTASK_TASK(scheduler, task1, task1, condition_fails, required_energy);

	mock().expectOneCall("powertask_get_available_energy").andReturnValue(required_energy+1);
	mock().expectNoCall("task1");
	mock().ignoreOtherCalls();

	powertask_energy_source_t energy_src = {0};
	powertask_run_scheduler(&scheduler, &energy_src);

	mock().checkExpectations();
};

/**
 * @brief Scheduler - Task depends on previous task completion (not complete)
 * 
 * The scope of this unit test is to validate if the scheduler does not run a
 * task that needs a previous task to complete (and that task has not ran).
 * 
 * It is expected the actions in both tasks to not be executed.
 */
TEST(test_scheduler_regular, test_two_tasks_task2_depends_on_task1_and_task1_not_running)
{
	const int required_energy = 400;

	POWERTASK_INIT(scheduler, 2);

	POWERTASK_TASK(scheduler, task1, task1, condition_fails, required_energy);
	POWERTASK_TASK(scheduler, task2, task2, POWERTASK_WAIT_FOR(task1), required_energy);

	mock().expectNCalls(2, "powertask_get_available_energy").andReturnValue(required_energy+1);
	mock().expectNoCall("task1");
	mock().expectNoCall("task2");
	mock().ignoreOtherCalls();

	powertask_energy_source_t energy_src = {0};
	powertask_run_scheduler(&scheduler, &energy_src);

	mock().checkExpectations();
};

/**
 * @brief Scheduler - Task depends on previous task completion (complete)
 * 
 * The scope of this unit test is to validate if the scheduler does runs a
 * task that needs a previous task to complete (and that task is complete).
 * 
 * It is expected the actions in both tasks to be executed.
 */
TEST(test_scheduler_regular, test_two_tasks_task2_depends_on_task1_and_task1_is_complete)
{
	const int required_energy = 400;

	POWERTASK_INIT(scheduler, 2);
	POWERTASK_TASK(scheduler, task1, task1, POWERTASK_RUN_ALWAYS, required_energy);
	POWERTASK_TASK(scheduler, task2, task2, POWERTASK_WAIT_FOR(task1), required_energy);

	mock().expectNCalls(2, "powertask_get_available_energy").andReturnValue(required_energy+1);
	mock().expectOneCall("task1");
	mock().expectOneCall("task2");
	mock().ignoreOtherCalls();

	powertask_energy_source_t energy_src = {0};
	powertask_run_scheduler(&scheduler, &energy_src);

	mock().checkExpectations();
};

/**
 * @brief Scheduler - Resets current state after all tasks are run
 * 
 * The scope of this unit test is to validate if the scheduler resets the
 * current state of the scheduler when all tasks are run.
 * 
 * It is expected all tasks on the scheduler to be flagged as not completed. 
 */
TEST(test_scheduler_regular, test_schedular_state_is_reset_after_all_tasks_are_completed){
	const int required_energy = 400;

	POWERTASK_INIT(scheduler, 2);

	POWERTASK_TASK(scheduler, task1, task1, POWERTASK_RUN_ALWAYS, required_energy);
	POWERTASK_TASK(scheduler, task2, task2, POWERTASK_RUN_ALWAYS, required_energy);

	mock().expectNCalls(2, "powertask_get_available_energy").andReturnValue(required_energy+1);
	mock().expectOneCall("task1");
	mock().expectOneCall("task2");
	mock().ignoreOtherCalls();

	powertask_energy_source_t energy_src = {0};
	powertask_run_scheduler(&scheduler, &energy_src);

	CHECK_FALSE(scheduler.list_of_tasks[0]->complete);
	CHECK_FALSE(scheduler.list_of_tasks[1]->complete);

	mock().checkExpectations();
};

TEST(test_scheduler_regular, test_schedular_current_state_is_stored){
	const int required_energy = 400;

	POWERTASK_INIT(scheduler, 2);

	POWERTASK_TASK(scheduler, task1, task1, POWERTASK_RUN_ALWAYS, required_energy);
	POWERTASK_TASK(scheduler, task2, task2, POWERTASK_RUN_ALWAYS, required_energy);

	mock().expectOneCall("powertask_get_available_energy").andReturnValue(required_energy+1);
	mock().expectOneCall("powertask_get_available_energy").andReturnValue(required_energy-1);

	mock().expectOneCall("task1");
	mock().expectNoCall("task2");
	mock().expectOneCall("powertask_storage_save");
	mock().ignoreOtherCalls();

	powertask_energy_source_t energy_src = {0};
	powertask_run_scheduler(&scheduler, &energy_src);

	mock().expectOneCall("powertask_get_available_energy").andReturnValue(required_energy+1);

	mock().expectNoCall("task1");
	mock().expectOneCall("task2");
	mock().expectOneCall("powertask_storage_save");

	powertask_run_scheduler(&scheduler, &energy_src);

	mock().checkExpectations();
}


TEST(test_scheduler_regular, test_schedular_save_current_state_overflow){
	const int load_current_state_max_number_of_tasks = 255;

	POWERTASK_INIT(scheduler, load_current_state_max_number_of_tasks+1);

	powertask_task task = {
		.action = task1,
		.condition = POWERTASK_RUN_ALWAYS,
		.required_energy = 400
	};

	for(int i = 0; i < scheduler._list_of_tasks_len; i++){
		powertask_add(&scheduler, &task);
	}

	mock().expectNoCall("powertask_storage_save");
	mock().ignoreOtherCalls();

	powertask_energy_source_t energy_src = {0};
	powertask_run_scheduler(&scheduler, &energy_src);

	mock().checkExpectations();
}
