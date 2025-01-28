#include <sdk/os/debug.hpp>
#include <sdk/os/gui.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/mem.hpp>
#include <sdk/os/string.hpp>
#include "apps.hpp"
#include "bins.hpp"
#include "execs.hpp"

class Launcher : public GUIDialog {
public:
    int m_selectedProg;

    Launcher() : GUIDialog(
        GUIDialog::Height95, GUIDialog::AlignTop,
        "Hollyhock Launcher",
        GUIDialog::KeyboardStateNone
    ), m_appNames(
        GetLeftX() + 10, GetTopY() + 10, GetRightX() - 10, GetBottomY() - 10,
        APP_NAMES_EVENT_ID
    ), m_progInfo(
        GetLeftX() + 10, GetTopY() + 90, GetRightX() - 10, GetBottomY() - 10,
        // Since the app info string is immediately updated based on the
        // selected app, if no apps are found, this is the string that stays
        // displayed.
        // Use it to communicate to the user that we couldn't find any apps.
        "No apps were found on your calculator.\n\nEnsure their .hhk files have been copied to the root directory of your calculator's flash."
    ), m_run(
        GetLeftX() + 10, GetTopY() + 45, GetLeftX() + 10 + 100, GetTopY() + 45 + 35,
        "Run", RUN_EVENT_ID
    ), m_close(
        GetRightX() - 10 - 100, GetTopY() + 45, GetRightX() - 10, GetTopY() + 45 + 35,
        "Close", CLOSE_EVENT_ID
    ) {
        m_selectedProg = 0;

        Apps::LoadAppInfo();
        Bins::LoadAppInfo();
        Execs::LoadExecInfo();

        //Add apps to dropdown
        for (int i = 0; i < Apps::g_numApps; ++i) {
            struct Apps::AppInfo *app = &Apps::g_apps[i];

            const char *name = app->path;

            // first char of the name will not be \0 if a name was included.
            if (app->name[0] != '\0') {  
                name = app->name;
            }

            m_appNames.AddMenuItem(*(
                new GUIDropDownMenuItem(
                    name, i + 1,
                    GUIDropDownMenuItem::FlagEnabled |
                    GUIDropDownMenuItem::FlagTextAlignLeft
                )
            ));
        }

        //Add bins to dropdown
        for (int i = 0; i < Bins::g_numApps; ++i) {
            struct Bins::AppInfo *app = &Bins::g_apps[i];

            const char *name = app->path;

            // first char of the name will not be \0 if a name was included.
            if (app->name[0] != '\0') {  
                name = app->name;
            }

            m_appNames.AddMenuItem(*(
                new GUIDropDownMenuItem(
                    name, i + 1 + Apps::g_numApps, //Add bins after apps
                    GUIDropDownMenuItem::FlagEnabled |
                    GUIDropDownMenuItem::FlagTextAlignLeft
                )
            ));
        }

        //Add execs to dropdown
        for (int i = 0; i < Execs::g_numExecs; ++i) {
            struct Execs::ExecInfo *exec = &Execs::g_execs[i];

            const char *name = exec->fileName;

            // first char of the name will not be \0 if a name was included.
            if (exec->name[0] != '\0') {  
                name = exec->name;
            }

            m_appNames.AddMenuItem(*(
                new GUIDropDownMenuItem(
                    name, i + 1 + Apps::g_numApps + Bins::g_numApps, //Add execs after apps and bins
                    GUIDropDownMenuItem::FlagEnabled |
                    GUIDropDownMenuItem::FlagTextAlignLeft
                )
            ));
        }

        m_appNames.SetScrollBarVisibility(
            GUIDropDownMenu::ScrollBarVisibleWhenRequired
        );
        AddElement(m_appNames);

        AddElement(m_progInfo);

        // Only show the Run button if there's apps or execs to display
        if (Apps::g_numApps+Bins::g_numApps+Execs::g_numExecs > 0) {
            AddElement(m_run);
        }

        AddElement(m_close);

        UpdateAppInfo();
    }

    virtual int OnEvent(GUIDialog_Wrapped *dialog, GUIDialog_OnEvent_Data *event) {
        if (event->GetEventID() == APP_NAMES_EVENT_ID && (event->type & 0xF) == 0xD) {
            m_selectedProg = event->data - 1;

            UpdateAppInfo();

            return 0;
        }

        return GUIDialog::OnEvent(dialog, event);
    }

    void UpdateAppInfo() {
        // If an invalid index is selected, don't do anything
        if (m_selectedProg >= (Apps::g_numApps + Bins::g_numApps + Execs::g_numExecs)) return;
        // Check if an exec is selecter or an app is selected
        if (m_selectedProg >= (Apps::g_numApps + Bins::g_numApps)){

            //An exec is selected
            struct Execs::ExecInfo *exec = &Execs::g_execs[m_selectedProg-Apps::g_numApps-Bins::g_numApps];
            bool hasName = exec->name[0] != '\0';
            bool hasDescription = exec->description[0] != '\0';
            bool hasAuthor = exec->author[0] != '\0';
            bool hasVersion = exec->version[0] != '\0';

            Mem_Memset(m_progInfoString, 0, sizeof(m_progInfoString));

            if (hasName) {
                String_Strcat(m_progInfoString, exec->name);
            } else {
                String_Strcat(m_progInfoString, "hhk/");
                String_Strcat(m_progInfoString, exec->fileName);
            }

            if (hasAuthor || hasVersion) {
                String_Strcat(m_progInfoString, "\n(");

                if (hasVersion) {
                    String_Strcat(m_progInfoString, "version ");
                    String_Strcat(m_progInfoString, exec->version);
                }

                if (hasAuthor) {
                    if (hasVersion) {
                        String_Strcat(m_progInfoString, " by ");
                    } else {
                        String_Strcat(m_progInfoString, "by ");
                    }

                    String_Strcat(m_progInfoString, exec->author);
                }

                String_Strcat(m_progInfoString, ")");
            }

            if (hasName) {
                String_Strcat(m_progInfoString, "\n(from hhk/");
                String_Strcat(m_progInfoString, exec->fileName);
                String_Strcat(m_progInfoString, ")");
            }

            if (hasDescription) {
                String_Strcat(m_progInfoString, "\n\n");
                String_Strcat(m_progInfoString, exec->description);
            }
        }
        else
        if (m_selectedProg >= (Apps::g_numApps)){

            //A bin is selected
            struct Bins::AppInfo *app = &Bins::g_apps[m_selectedProg-Apps::g_numApps];
            bool hasName = app->name[0] != '\0';
            bool hasDescription = app->description[0] != '\0';
            bool hasAuthor = app->author[0] != '\0';
            bool hasVersion = app->version[0] != '\0';

            Mem_Memset(m_progInfoString, 0, sizeof(m_progInfoString));

            if (hasName) {
                String_Strcat(m_progInfoString, app->name);
            } else {
                String_Strcat(m_progInfoString, app->path);
            }

            if (hasAuthor || hasVersion) {
                String_Strcat(m_progInfoString, "\n(");

                if (hasVersion) {
                    String_Strcat(m_progInfoString, "version ");
                    String_Strcat(m_progInfoString, app->version);
                }

                if (hasAuthor) {
                    if (hasVersion) {
                        String_Strcat(m_progInfoString, " by ");
                    } else {
                        String_Strcat(m_progInfoString, "by ");
                    }

                    String_Strcat(m_progInfoString, app->author);
                }

                String_Strcat(m_progInfoString, ")");
            }

            if (hasName) {
                String_Strcat(m_progInfoString, "\n(from ");
                String_Strcat(m_progInfoString, app->path);
                String_Strcat(m_progInfoString, ")");
            }

            if (hasDescription) {
                String_Strcat(m_progInfoString, "\n\n");
                String_Strcat(m_progInfoString, app->description);
            }
		}
		else
        {

            //An app is selected
            struct Apps::AppInfo *app = &Apps::g_apps[m_selectedProg];
            bool hasName = app->name[0] != '\0';
            bool hasDescription = app->description[0] != '\0';
            bool hasAuthor = app->author[0] != '\0';
            bool hasVersion = app->version[0] != '\0';

            Mem_Memset(m_progInfoString, 0, sizeof(m_progInfoString));

            if (hasName) {
                String_Strcat(m_progInfoString, app->name);
            } else {
                String_Strcat(m_progInfoString, app->path);
            }

            if (hasAuthor || hasVersion) {
                String_Strcat(m_progInfoString, "\n(");

                if (hasVersion) {
                    String_Strcat(m_progInfoString, "version ");
                    String_Strcat(m_progInfoString, app->version);
                }

                if (hasAuthor) {
                    if (hasVersion) {
                        String_Strcat(m_progInfoString, " by ");
                    } else {
                        String_Strcat(m_progInfoString, "by ");
                    }

                    String_Strcat(m_progInfoString, app->author);
                }

                String_Strcat(m_progInfoString, ")");
            }

            if (hasName) {
                String_Strcat(m_progInfoString, "\n(from ");
                String_Strcat(m_progInfoString, app->path);
                String_Strcat(m_progInfoString, ")");
            }

            if (hasDescription) {
                String_Strcat(m_progInfoString, "\n\n");
                String_Strcat(m_progInfoString, app->description);
            }

        // App Name (version 1.0.0 by Meme King)
        // \fls0\meme.hhk

        // Meme to your heart's content.

        }

        m_progInfo.SetText(m_progInfoString);
        m_progInfo.Refresh();
        Refresh();
    }

private:
    const uint16_t APP_NAMES_EVENT_ID = 1;
    GUIDropDownMenu m_appNames;

    GUILongLabel m_progInfo;

    // While GUILongLabel seems to copy the string into it's own memory when
    // calling SetText, when it refreshes sometimes it tries to read the old
    // memory. Not sure why, but that's why this is here - it'll never move
    // because GUIDialog has the copy/move ctor deleted. This should therefore
    // be a safe solution.
    char m_progInfoString[500];

    const uint16_t RUN_EVENT_ID = GUIDialog::DialogResultOK;
    GUIButton m_run;

    const uint16_t CLOSE_EVENT_ID = GUIDialog::DialogResultCancel;
    GUIButton m_close;
};

void main() {
    Launcher launcher;
    if (launcher.ShowDialog() == GUIDialog::DialogResultOK) {
        if (launcher.m_selectedProg >= Apps::g_numApps+Bins::g_numApps){
	        //Exec selected
			Execs::EntryPoint epE = Execs::RunExec(launcher.m_selectedProg-Apps::g_numApps-Bins::g_numApps);
			if (epE != nullptr) {
				epE();
			}
		}
		else if (launcher.m_selectedProg >= Apps::g_numApps){
	        //Bin selected
			Bins::EntryPoint epA = Bins::RunApp(launcher.m_selectedProg-Apps::g_numApps);
			if (epA != nullptr) {
				epA();
			}
		}

		else{
			//App selected
			Apps::EntryPoint epA = Apps::RunApp(launcher.m_selectedProg);
			if (epA != nullptr) {
				epA();
			}
		}
    }
}
