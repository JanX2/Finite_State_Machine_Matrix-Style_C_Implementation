//
//  main.m
//  Finite_State_Machine_Matrix-Style_C_Implementation
//
//  Created by Jan on 30/03/17.
//  Copyright © 2017 Jan. All rights reserved.
//

//#import <Foundation/Foundation.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>

#define USE_FUNCTION_POINTERS   1

// typedefs

typedef enum {
    STATE_0 = 0,
    STATE_1,
    STATE_2,
    STATE_COUNT // Needs to be the last entry.
} state_t;

typedef enum {
    NIL_EVENT = 0,
    EVENT_1,
    EVENT_2,
    EVENT_COUNT // Needs to be the last entry.
} event_t;

#if USE_FUNCTION_POINTERS
typedef void (*action_t)(void);
#else
typedef enum {
    NIL_ACTION = 0,
    ACTION_1,
    ACTION_2,
    ACTION_3,
    ACTION_4,
    ACTION_COUNT // Needs to be the last entry.
} action_t;
#endif

typedef struct {
    state_t nextState;
    action_t actionToTrigger;
} state_element_t;


// Evil global.
// Could be encapsulated in a state machine state struct that is passed around.
state_t _currentState;


#if USE_FUNCTION_POINTERS
void stateEvaluation(event_t e);
#else
action_t stateEvaluation(event_t e);
#endif


#if USE_FUNCTION_POINTERS
void nil_action() {
    exit(EXIT_FAILURE); // Should never be triggered.
}
#endif


// Actions
void action_1() {
    printf("Triggered " "%s" " \n", __func__);
}

void action_2() {
    printf("Triggered " "%s" " \n", __func__);
}

void action_3() {
    printf("Triggered " "%s" " \n", __func__);
}

void action_4() {
    printf("Triggered " "%s" " \n", __func__);
}


#if USE_FUNCTION_POINTERS
state_element_t stateMatrix[STATE_COUNT][EVENT_COUNT] = {
    { {STATE_0, nil_action}, {STATE_1, action_1}, {STATE_2, action_4} },
    { {STATE_1, nil_action}, {STATE_1, action_3}, {STATE_2, action_2} },
    { {STATE_2, nil_action}, {STATE_2, action_2}, {STATE_2, action_3} },
};
#else
stateElement stateMatrix[STATE_COUNT][EVENT_COUNT] = {
    { {STATE_0, NIL_ACTION}, {STATE_1, ACTION_1}, {STATE_1, ACTION_4} },
    { {STATE_1, NIL_ACTION}, {STATE_2, ACTION_3}, {STATE_2, ACTION_2} },
    { {STATE_2, NIL_ACTION}, {STATE_0, ACTION_2}, {STATE_1, ACTION_3} },
};
#endif


bool isValidState(state_t state) {
    return ((STATE_0 <= state) && (state < STATE_COUNT));
}

bool isValidEvent(event_t event) {
    return ((NIL_EVENT <= event) && (event < EVENT_COUNT));
}


#if USE_FUNCTION_POINTERS
void stateEvaluation(event_t event) {
#else
action_t stateEvaluation(event_t event) {
#endif
    
    assert(isValidState(_currentState));
    assert(isValidEvent(event));
    
    // Determine the state matrix element depending on the current state and the triggered event.
    state_element_t stateTransition = stateMatrix[_currentState][event];
    
    // Transition to the next state (set current state to the next state obtained from the matrix)…
    _currentState = stateTransition.nextState;

    // … and trigger the appropriate action.
#if USE_FUNCTION_POINTERS
    if (stateTransition.actionToTrigger != nil_action) {
        stateTransition.actionToTrigger();
    }
}
#else
    return stateTransition.actionToTrigger;
}
#endif


void processCurrentStateArgument(const char *argument) {
    char *endPointer;
    errno = 0;
    
    _currentState = (state_t)strtoimax(argument, &endPointer, 10);
    
    if (*endPointer != '\0' || errno != 0) {
        exit(EXIT_FAILURE);
    }
    
    if (!isValidState(_currentState)) {
        exit(EXIT_FAILURE);
    }
}

event_t processEventArgument(const char *argument) {
    event_t eventOccured = NIL_EVENT;
    
    char *endPointer;
    errno = 0;
    
    eventOccured = (event_t)strtoimax(argument, &endPointer, 10);
    
    if (*endPointer != '\0' || errno != 0) {
        exit(EXIT_FAILURE);
    }
    
    if (!isValidEvent(eventOccured)) {
        exit(EXIT_FAILURE);
    }
    
    return eventOccured;
}

int main(int argc, const char *argv[]) {
    _currentState = STATE_0;
    
    event_t eventOccured = NIL_EVENT;

    switch (argc) {
        case 0:
        case 1:
            exit(EXIT_FAILURE);
            break;
            
        case 2: {
            processCurrentStateArgument(argv[1]);
            
            break;
        }
            
        case 3: {
            processCurrentStateArgument(argv[1]);
            eventOccured = processEventArgument(argv[2]);
            
            break;
        }
            
        default:
            exit(EXIT_FAILURE);
            break;
    }
    
#if !USE_FUNCTION_POINTERS
    action_t actionToTrigger =
#endif
    stateEvaluation(eventOccured);
    
#if !USE_FUNCTION_POINTERS
    // Trigger action.
    switch (actionToTrigger) {
        case ACTION_1:
            action_1();
            break;
            
        case ACTION_2:
            action_2();
            break;
            
        case ACTION_3:
            action_3();
            break;
            
        case ACTION_4:
            action_4();
            break;
            
        case NIL_ACTION:
        default:
            break;
    }
#endif

    return EXIT_SUCCESS;
}
