#ifndef __FALL_H__
#define __FALL_H__

// Global Variables
extern uint16_t count;
extern bool is_falling;
extern float falling_count;

// Function Declaration
void send(String message);
void falling_check(float az);
void fall_task(void* pvParm);

#endif