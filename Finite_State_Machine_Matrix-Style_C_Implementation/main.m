//
//  main.m
//  Finite_State_Machine_Matrix-Style_C_Implementation
//
//  Created by Jan on 30/03/17.
//  Copyright © 2017 Jan. All rights reserved.
//

//#import <Foundation/Foundation.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define USE_FUNCTION_POINTERS   1

// typedefs

typedef enum {
    STATE_0 = 0,
    STATE_1,
    STATE_2,
    STATE_COUNT // Needs to be the last entry.
} state;

typedef enum {
    NIL_EVENT = 0,
    EVENT_1,
    EVENT_2,
    EVENT_COUNT // Needs to be the last entry.
} event;

#if USE_FUNCTION_POINTERS
typedef void (*action)(void);
#else
typedef enum {
    NIL_ACTION = 0,
    ACTION_1,
    ACTION_2,
    ACTION_3,
    ACTION_4,
    ACTION_COUNT // Needs to be the last entry.
} action;
#endif

typedef struct {
    state nextState;
    action actionToTrigger;
} stateElement;


// Evil global.
// Could be encapsulated in a state machine state struct that is passed around.
state _currentState;


#if USE_FUNCTION_POINTERS
void stateEvaluation(event e);
#else
action stateEvaluation(event e);
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
stateElement stateMatrix[STATE_COUNT][EVENT_COUNT] = {
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

#if USE_FUNCTION_POINTERS
void stateEvaluation(event e) {
#else
action stateEvaluation(event e) {
#endif
    // Determine the state matrix element depending on the current state and the triggered event.
    stateElement stateTransition = stateMatrix[_currentState][e];
    
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


int main(int argc, const char *argv[]) {
    _currentState = STATE_0;
    
    event eventOccured = NIL_EVENT;
    
    // Determine event.
    if (true) {
        eventOccured = EVENT_1;
    }

#if !USE_FUNCTION_POINTERS
    action actionToTrigger =
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
