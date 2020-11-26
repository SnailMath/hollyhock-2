#include <sdk/os/mem.hpp>
#include <sdk/os/string.hpp>
#include "execs.hpp"

namespace Execs {
    struct ExecInfo g_execs[MAX_EXECS];
    int g_numExecs;
/*
	void LoadExec(wchar_t *fileName) {
		struct ExecInfo exec;
		memset(&exec, 0, sizeof(exec));

		// build the path
		strcat(exec.path, "hhk/");
		strcat(exec.path, exec.fileName);

		strcat(exec.name, "The Name!");
		strcat(exec.description, "The Description");
		strcat(exec.author, "SnailMath");
		strcat(exec.version, "0.0.0");

		};

		g_execs[g_numExecs++] = exec;
	}
*/

    void LoadExecInfo() {
		g_numExecs = 0;
        wchar_t fileName[100];

		//Check if the folder hhk exists
		char *hhk = 0; //the address of the hhk folder
		char *ram = (char*) 0x8cf80100; //The first folder name is at 8cf80100
		while( *(ram+8) != 0 ) { //The first byte of the folder address can't be 0, if it is we've searched all folders
			if(*ram=='h'){
				if(*(ram+1)=='h'){
					if(*(ram+2)=='k'){
						if(*(ram+3)==0){
							//we've found the folder
							hhk = ram; //hhk points now to the foldername of hhk
							break;
						}
					}
				}
			}
			ram += 0x10;
		}
		
		//Return if folder hhk is not present
		if ( hhk == 0 ) return;
		
		int hhk_entries = *((uint16_t*)(hhk+12)) ; //0-7 is the folder name, 8-11 is the position of the first directory table entry, 12-13 is the amount of entries in that direcory
		hhk = *((char**)(hhk+8)); //hhk points now to the first diretory table entry in the folder hhk

		while (g_numExecs < hhk_entries ) {
		
	        //For every file in the directory hhk
        
            //LoadExec(fileName);
			struct ExecInfo exec;
			memset(&exec, 0, sizeof(exec));
			
			strcat(exec.fileName, hhk); //the filename of the first file

			// build the path
			strcat(exec.path, "hhk/");
			strcat(exec.path, exec.fileName);

			strcat(exec.name, hhk);//"The Name!");
			strcat(exec.description, "The Description");
			strcat(exec.author, "SnailMath");
			strcat(exec.version, "0.0.0");

			hhk = hhk + 20 ; //Yes, 16+4 = 20
			g_execs[g_numExecs++] = exec;
        }
    }
    
	void test(){
    }
    
    EntryPoint RunExec(int i) {
        struct ExecInfo *exec = &g_execs[i];
        
		return reinterpret_cast<EntryPoint>(test);
    }
    
    
}

