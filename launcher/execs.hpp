#pragma once

namespace Execs {
    struct ExecInfo {
        char fileName[100];
        char path[200];
        char name[100];
        char description[100];
        char author[100];
        char version[100];
    };

    typedef void (*EntryPoint)();

    const int MAX_EXECS = 64;

    extern struct ExecInfo g_execs[MAX_EXECS];
    extern int g_numExecs;

    void LoadExecInfo();
    EntryPoint RunExec(int i);
};
