#ifndef STATEMANAGER_H
#define STATEMANAGER_H

// Function pointers
typedef unsigned int (*fnPtr)();
typedef unsigned int (*fnPtrFloat)(float);

typedef struct {
    fnPtr init;
    fnPtrFloat update;
    fnPtrFloat draw;
    fnPtr destroy;
} State;

typedef struct {
    State **stack;
    int capacity;
    int top;
} StateManager;

int STATEMANAGER_init(StateManager *statemanager);
int STATEMANAGER_free(StateManager *statemanager);
int STATEMANAGER_push(StateManager *statemanager, State *state);
int STATEMANAGER_pop(StateManager *statemanager);
State *STATEMANAGER_top(StateManager *statemanager);
int STATEMANAGER_update(StateManager *statemanager, float deltatime);
int STATEMANAGER_draw(StateManager *statemanager, float deltatime);

#endif // STATEMANAGER_H
