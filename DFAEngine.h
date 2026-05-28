#pragma once
#include "collection.h"
extern bool TEST_MODE;
using StateFunc = const char* (*)();
struct DFAEngine {
    Map<const char*, Map<const char*, const char*>*> transitionTable;


    Map<const char*, StateFunc> stateFuncs;
    Map < const char*, StateFunc> testStateFuncs;

    Map<const char*, const char*> TestLogs;

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

            if (TEST_MODE&& testStateFuncs.ContainsKey(currState)) {

                inputBuffer = testStateFuncs[currState]();
            }
            else {
                inputBuffer = stateFuncs[currState]();

            }
            currState = Transition(currState, inputBuffer);
            TestLogs.Add(currState, inputBuffer);

        }
        if (TEST_MODE && testStateFuncs.ContainsKey(currState)) {

            inputBuffer = testStateFuncs[currState]();
        }
        else {
            inputBuffer = stateFuncs[currState]();

        }
        if (Compare(inputBuffer, "ACC") || Compare(inputBuffer, "ERR")) {

            TestLogs.Add(currState, inputBuffer);
            currState = startState;
            inputBuffer = "";
            return;
        }
        currState = Transition(currState, inputBuffer);
        this->EngineStart();
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
    void AddTestFunction(const char* state, const char* (*func)()) {
        if (!testStateFuncs.ContainsKey(state)) {
            testStateFuncs.Add(state, func);
        }
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