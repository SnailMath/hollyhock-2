#pragma once

namespace Execs {
    struct ExecInfo {
        char fileName[9];
        //char path[200];
        char name[100];
        char description[100];
        char author[100];
        char version[100];
        //char* fp; //this throws linker error: undefined reference to `___movmem_i4_even'
        unsigned char fp[4]; //this is the address of the first byte. A pointer throws an assembler error... This has to work.
    };

    typedef void (*EntryPoint)();

    const int MAX_EXECS = 64;

    extern struct ExecInfo g_execs[MAX_EXECS];
    extern int g_numExecs;

    void LoadExecInfo();
    EntryPoint RunExec(int i);
};
