#include "collection.h"

using StateFunc = const char* (*)();
struct DFAEngine {
    Map<const char*, Map<const char*, const char*>*> transitionTable;


    Map<const char*, StateFunc> stateFuncs;
    const char* currState;
    const char* inputBuffer="";
    const char* errState;
    const char* accState;
    const char* startState;
    DFAEngine(const char* start, const char* accept, const char* error) :startState(start), accState(accept), errState(error) {
        currState = start;
    }
    void EngineStart() {
        

        while (!Compare(currState, errState) && !Compare(currState, accState)) {
            inputBuffer = stateFuncs[currState]();
            currState = Transition(currState, inputBuffer);

        }
    }
    void AddTransition(const char* currState, const char* input, const char* nextState) {
        bool has = FindTransition(currState);

        if (has) {
            transitionTable[currState]->Add(input, nextState);
            return;
        }

        Map<const char*, const char*>* path = new Map<const char*, const char*>();
       
        path->Add(input, nextState);

        transitionTable.Add(currState, path);
    }

    void AddFunction(const char* state, const char* (*func)()) {
        stateFuncs.Add(state, func);
    }
    bool FindTransition(const char* state) {
        return transitionTable.ContainsKey(state);
    }
    const char* Transition(const char* currState, const char* input) {
        Map<const char*, const char*>* nexts = transitionTable[currState];
        if (nexts->ContainsKey(input)) {
            return (*nexts)[input];

        }
        return errState;
    }
};