#include "GlobalStat.h"


GlobalStat::GlobalStat() {
    input = true;
}

GlobalStat::~GlobalStat() {}

bool GlobalStat::InputStat() {
    return input;
}

void GlobalStat::lockInput() {
    input = false;
}

void GlobalStat::unlockInput() {
    input = true;
}