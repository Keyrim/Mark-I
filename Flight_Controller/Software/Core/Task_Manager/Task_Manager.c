/*
 * Task_Manager.c
 *
 *  Created on: Nov 24, 2021
 *      Author: Théo Magne
 */

#include "macro_types.h"
#include "time.h"
#include "Task_Manager.h"

#define MAX_NUMBER_OF_TASK	5

static task_t task[MAX_NUMBER_OF_TASK] = { 0 };
static bool_e is_initialized = FALSE;
static uint8_t task_count = 0;

void TASK_MANAGER_Init(void)
{
	for(uint8_t t = 0; t < task_count; t++)
	{
		if(task[t].init.call)
		{
			task[t].init.call();
		}
	}
	is_initialized = TRUE;
}


void TASK_MANAGER_Main(void)
{
	for(uint8_t t = 0; t < task_count; t++)
	{
		if(task[t].main.call)
		{
			task[t].main.call();
		}
	}
}

void TASK_MANAGER_It_ms(void)
{
	if(!is_initialized)
	{
		return;
	}
	for(uint8_t t = 0; t < task_count; t++)
	{
		if(task[t].it.call)
		{
			task[t].it.call();
		}
	}
}

void TASK_MANAGER_Add_Task(char * name, void(*init)(void), void(*main)(void), void(*it)(void))
{
	if(task_count == MAX_NUMBER_OF_TASK)
	{
		return;
	}
	task[task_count].name = name;
	task[task_count].name_len = sizeof(name) - 1;
	task[task_count].init.call = init;
	task[task_count].main.call = main;
	task[task_count++].it.call = it;
}


