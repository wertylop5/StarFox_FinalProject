#include "3140_concur.h"
#include <stdlib.h>
#include <MK64F12.h>
#include "utils.h"
#include "realtime.h"
#include <string.h>
#include <limits.h>

const unsigned int quantum = 0x1200;
const unsigned int CYCLES_PER_MIILISECOND = 20970;
process_t * current_process = NULL;
process_t * process_queue = NULL;

realtime_t current_time;
process_t *dynamic_priority_queue = NULL;
int process_deadline_met = 0;
int process_deadline_miss = 0;

struct process_state {
	unsigned int *sp;
	unsigned int *original_sp;
	void (*f)(void);
	int n;
	
	int has_deadline;
	
	int request_time;
	int start_time;
	int finish_time;
	int deadline;
	
	int iterations;
	int has_period;
	int period;
	
	process_t *next_process;
};

// Utility functions
int realtime_to_milliseconds(realtime_t *r) {
	return 1000*r->sec + r->msec;
}

int is_process_ready(process_t *p) {
	return realtime_to_milliseconds(&current_time) >= p->start_time;
}

int did_process_meet_deadline(process_t *p) {
	return realtime_to_milliseconds(&current_time) <= p->deadline;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void enqueue(process_t *new_process, process_t **process_queue){
	if (*process_queue == NULL){
		*process_queue = new_process; 
	}
	else {
		process_t *temp = *process_queue;
		while(temp->next_process != NULL){
			temp=temp->next_process;
		}
		temp->next_process = new_process;
		new_process->next_process = NULL;
	}
}

process_t* dequeue(process_t **process_queue){
	if (*process_queue == NULL) return NULL;
	
	process_t *head = *process_queue;
	*process_queue = (*process_queue)->next_process;
	head->next_process = NULL;
	return head;
}

void add_to_tail(process_t *new_process){
	enqueue(new_process, &process_queue);
}

process_t* remove_from_head() {
	return dequeue(&process_queue);
}

//precondition: p exists in queue
void remove_process_from_queue(process_t *p) {
	if (p == dynamic_priority_queue) {
		dequeue(&dynamic_priority_queue);
		return;
	}
	
	process_t *pos = dynamic_priority_queue;
	while (pos != NULL) {
		if (pos->next_process == p) {
			pos->next_process = p->next_process;
			
			p->next_process = NULL;
			return;
		}
		
		pos = pos->next_process;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

process_t* pick_process2() {
	// if nothing is ready or dp is null and check if current process too
	
	// loop through and find earliest
	int earliest_deadline = INT_MAX;
	process_t * earliest_process = NULL;
	process_t * pos = dynamic_priority_queue;
	while (pos != NULL) {
		if (pos->deadline < earliest_deadline && is_process_ready(pos)) {
			earliest_process = pos;
			earliest_deadline = pos->deadline;
		}
		pos = pos->next_process;
	}
	
	if (current_process != NULL) {
		if (current_process->has_deadline) {
			if (current_process->deadline <= earliest_deadline &&
					is_process_ready(current_process)) {
				// note if current process implies is ready
				return current_process;
			}
			else {
				return earliest_process;
			}
		}
	}
	
	// if nothing is ready or dp is null, return proc from proc queue
	return earliest_process == NULL ? remove_from_head() : earliest_process;
}

process_t* generic_process_create(void (*f)(void), int n) {
	process_t *new_process = malloc(sizeof(process_t));
	if (new_process == NULL) {
		return NULL;
	}
	
	unsigned int * allocated_sp = process_stack_init(f, n); 
	if (allocated_sp == NULL) {
		free(new_process);
		return NULL;
	}
	
	new_process->sp = allocated_sp;
	new_process->f = f;
	new_process->original_sp = allocated_sp;
	new_process->n = n;
	new_process->next_process = NULL;
	
	return new_process;
}

int process_rt_create(void (*f)(void), int n, realtime_t* start, realtime_t* deadline) {
	process_t *new_process = generic_process_create(f, n);
	
	if (new_process == NULL) return -1;
	
	new_process->has_deadline = 1;
	new_process->request_time = realtime_to_milliseconds(&current_time);
	new_process->start_time = realtime_to_milliseconds(start);
	new_process->deadline = new_process->start_time + realtime_to_milliseconds(deadline);
	new_process->finish_time = 0;
	new_process->has_period = 0;
	enqueue(new_process, &dynamic_priority_queue);
	return 0;
}

int process_rt_periodic(void (*f)(void), int n, realtime_t *start, realtime_t *deadline, realtime_t *period) {
	process_t *new_process = generic_process_create(f, n);
	
	if (new_process == NULL) return -1;
	
	new_process->has_deadline = 1;
	new_process->request_time = realtime_to_milliseconds(&current_time);
	new_process->start_time = realtime_to_milliseconds(start);
	new_process->deadline = new_process->start_time + realtime_to_milliseconds(deadline);
	new_process->finish_time = 0;
	
	new_process->iterations = 0;
	new_process->has_period = 1;
	new_process->period = realtime_to_milliseconds(period);
	
	enqueue(new_process, &dynamic_priority_queue);
	return 0;
}

// Return -1 if fail to reinit else 0
int process_stack_reinit(process_t * process){
	process->sp = process->original_sp;
	memset(process->sp+(18-process->n), 0, process->n+18);
	
	*(process->sp+17) = 0x01000000; // xPSR
	*(process->sp+16) = (unsigned int) process->f; // PC
	*(process->sp+15) = (unsigned int) process_terminated; // LR
	*(process->sp+9) = 0xFFFFFFF9; // EXC_RETURN value, returns to thread mode
	*(process->sp+0) = 0x3; // Enable scheduling timer and interrupt
	
	return 0;
}

void process_restart(process_t * process) {
	process->request_time += process->period;
	process->start_time += process->period;
	process->deadline += process->period;
	enqueue(process, &dynamic_priority_queue);
}

int process_create (void (*f)(void), int n) {
	process_t *new_process = generic_process_create(f, n);
	
	new_process->has_deadline = 0;
	new_process->has_period = 0;
	
	if (new_process == NULL) return -1;
	
	add_to_tail(new_process);
	return 0;
}

void process_start (void){
	NVIC_SetPriority(SVCall_IRQn, 1);
	NVIC_EnableIRQ(PIT0_IRQn);
	NVIC_SetPriority(PIT0_IRQn, 2);
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT->MCR = 0x00;
	PIT->CHANNEL[0].LDVAL = quantum;
	PIT->CHANNEL[0].TCTRL |= 0b11;
	
	NVIC_EnableIRQ(PIT1_IRQn);
	NVIC_SetPriority(PIT1_IRQn, 0);
	
	// .001s * 20.97e6 Hz
	// number of cycles to run for 1 millisecond
	PIT->CHANNEL[1].LDVAL = CYCLES_PER_MIILISECOND;
	PIT->CHANNEL[1].TCTRL |= 0b11;
	
	current_time.sec = 0;
	current_time.msec = 0;
	
	if (process_queue == NULL && dynamic_priority_queue == NULL){
		//LEDGreen_On();
		return;
	}
	current_process = NULL;
	process_begin();
}

extern unsigned int * process_select (unsigned int * cursp)
{
	__disable_irq();
	//pop off the current process if its deadline is not the earliest
	

	if (cursp != NULL && (process_queue != NULL || dynamic_priority_queue != NULL)) {
		current_process->sp = cursp;
		
		process_t *temp = current_process;
		
		current_process = pick_process2();
		if (current_process != NULL) {
			if (current_process->has_deadline) {
				remove_process_from_queue(current_process);
			}
			
			if (temp->has_deadline) {
				if (temp != current_process) {
					enqueue(temp, &dynamic_priority_queue);
				}
			}
			else {
				enqueue(temp, &process_queue);
			}
		}
		else current_process = temp;
	}
	else if (cursp == NULL && (process_queue != NULL || dynamic_priority_queue != NULL)) {
		if (current_process != NULL) {
			if (current_process->has_deadline) {
				if (did_process_meet_deadline(current_process)) {
					process_deadline_met += 1;
				}
				else process_deadline_miss +=1 ;
			}
			
			if (current_process->has_period) {
				int reinited = process_stack_reinit(current_process);
				if (reinited == 0) process_restart(current_process);
				else {
					process_stack_free(current_process->original_sp, current_process->n);
					free(current_process);
				}
			}
			else {
				process_stack_free(current_process->original_sp, current_process->n);
				free(current_process);
			}
			current_process = NULL;
		}
		
		
		__enable_irq();
		do {
			current_process = pick_process2();
		} while(dynamic_priority_queue != NULL && current_process == NULL);
		__disable_irq();
		
		if (current_process != NULL && current_process->has_deadline) {
			remove_process_from_queue(current_process);
		}
	}
	else if (cursp != NULL && !(process_queue != NULL || dynamic_priority_queue != NULL)) {
		current_process->sp = cursp;
	}
	else {
		if (current_process->has_deadline) {
			if (did_process_meet_deadline(current_process)) {
				process_deadline_met += 1;
			}
			else process_deadline_miss +=1 ;
		}
		
		if (current_process->has_period) {
			int reinited = process_stack_reinit(current_process);
			if (reinited == 0) process_restart(current_process);
			else {
				process_stack_free(current_process->original_sp, current_process->n);
				free(current_process);
			}
		}
		else {
			process_stack_free(current_process->original_sp, current_process->n);
			free(current_process);
		}
		
		if (dynamic_priority_queue == NULL) {
			current_process = NULL;
		}
		
		__enable_irq();
		do {
			current_process = pick_process2();
		} while(dynamic_priority_queue != NULL && current_process == NULL);
		__disable_irq();
		
		__enable_irq();
		return current_process == NULL ? NULL : current_process->sp;
	}
	
	__enable_irq();
	return current_process->sp;
	
	
}

//PIT1 Interrupt handler
void PIT1_IRQHandler() {
	current_time.msec += 1;
	if (current_time.msec >= 1000) {
		current_time.sec += 1;
		current_time.msec %= 1000;
	}
	
	PIT->CHANNEL[1].TFLG |= 1;
}


