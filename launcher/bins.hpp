#pragma once

namespace Bins {
    struct AppInfo {
        char fileName[100];
        char path[200];
        char name[100];
        char description[100];
        char author[100];
        char version[100];
    };

    typedef void (*EntryPoint)();

    const int MAX_APPS = 64;

    extern struct AppInfo g_apps[MAX_APPS];
    extern int g_numApps;

    void LoadAppInfo();
    EntryPoint RunApp(int i);
};
