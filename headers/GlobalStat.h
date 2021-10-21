#ifndef GLOBALSTAT_H
#define GLOBALSTAT_H

class GlobalStat {

    bool input;

    public:
    GlobalStat();
    ~GlobalStat();

    void lockInput();
    void unlockInput();

    bool InputStat();
};

#endif