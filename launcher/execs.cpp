#include <sdk/os/mem.hpp>
#include <sdk/os/string.hpp>
#include "execs.hpp"

#include <sdk/os/debug.hpp>
#include <sdk/os/lcd.hpp>

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
		char *directoryEntry = hhk; //points to the directory table entry of the file we are currently checking
		char *file = 0; //The actual address of the file

		int current = 0;
		while (current < hhk_entries ) {
		
		//the file starts at the location:  first directory table entry location + the offset stored in the file's directory table entry + 0x0d
			file = hhk + 0x0d + *((uint32_t*)(directoryEntry+8)); 
			//The name is stored from 0-7, the offset from 8-11, the size from 12-15 and the type at 16 

			//Check if the type is 'program' (aka 0x47)
			if ( *(directoryEntry+16) == 0x47 ) {

				struct ExecInfo exec;
				memset(&exec, 0, sizeof(exec));
				
				for(int i = 0; i<8 ; i++){
					exec.fileName[i]=*(directoryEntry+i);
				}
		
				exec.fp[0] = ((uint32_t)file & 0xff000000) >> 24;
				exec.fp[1] = ((uint32_t)file & 0x00ff0000) >> 16;
				exec.fp[2] = ((uint32_t)file & 0x0000ff00) >> 8;
				exec.fp[3] = ((uint32_t)file & 0x000000ff);
/*
        Debug_Printf(0,0,false,0,"Address %02x %02x %02x %02x",exec.fp[0],exec.fp[1],exec.fp[2],exec.fp[3]);
        Debug_Printf(0,1,false,0,"%x",((exec.fp[0]<<24)+(exec.fp[1]<<16)+(exec.fp[2]<<8)+(exec.fp[3])));
        LCD_Refresh();
        Debug_WaitKey();
*/
				int j = 0; //offset in file
				//Extract Name from the var
				if (*(file+j) == '\''){ //If it contains a name, add it to hollyhock
					j++;
					for (int i = 0; i<100; i++){
						char ch = *(file+j);
						j++;
						if( ch==0 || ch=='\n' || ch=='\r') break; //End of the text
						exec.name[i] = ch;
					}
					if (*(file+j) == '\n' || *(file+j) == '\r' ) j++;
					if (*(file+j) == '\''){ //If it contains a description, add it to hollyhock
						j++;
						for (int i = 0; i<100; i++){
							char ch = *(file+j);
							j++;
							if( ch==0 || ch=='\n' || ch=='\r') break; //End of the text
							exec.description[i] = ch;
						}
						if (*(file+j) == '\n' || *(file+j) == '\r' ) j++;
						if (*(file+j) == '\''){ //If it contains an author, add it to hollyhock
							j++;
							for (int i = 0; i<100; i++){
								char ch = *(file+j);
								j++;
								if( ch==0 || ch=='\n' || ch=='\r') break; //End of the text
								exec.author[i] = ch;
							}
							if (*(file+j) == '\n' || *(file+j) == '\r' ) j++;
							if (*(file+j) == '\''){ //If it contains a version, add it to hollyhock
								j++;
								for (int i = 0; i<100; i++){
									char ch = *(file+j);
									j++;
									if( ch==0 || ch=='\n' || ch=='\r') break; //End of the text
									exec.version[i] = ch;
								}
							}
						}
					}
				}
			
				g_execs[g_numExecs++] = exec;
			}
			//go to the next entry
			current++;
			directoryEntry = directoryEntry + 20 ; //Yes, 16+4 = 20
        }
    }
    
	void test(){
    }
    
    EntryPoint RunExec(int i) {
        struct ExecInfo *exec = &g_execs[i];
        //Execute the exec. First load it from the var into the ram at 8CFF0000
        char *buf = (char*) 0x8CFF0000;
        /*
        Debug_Printf(0,0,false,0,"Address %02x %02x %02x %02x",exec->fp[0],exec->fp[1],exec->fp[2],exec->fp[3]);
        Debug_Printf(0,1,false,0,"%x",((exec->fp[0]<<24)+(exec->fp[1]<<16)+(exec->fp[2]<<8)+(exec->fp[3])));
        LCD_Refresh();
        Debug_WaitKey();
        */
        char *var = (char*)((exec->fp[0]<<24)+(exec->fp[1]<<16)+(exec->fp[2]<<8)+(exec->fp[3]));
        //Convert from hex to bin
        char currentNibble = 0; //0 searching first nibble; 1 searching 2nd nibble; 2 read whole byte
        char currentByte = 0;
        while(1==1){
        	char ch = *var;
        	if (ch=='\''){ //Comment
        		while(*var!='\r' && *var!='\n' && *var!=0) var++;
        		continue;
        	}
        	if (ch>='0' && ch<='9'){
        		currentByte = (currentByte << 4) + (ch - '0');
        		currentNibble++;
        	}
        	if (ch>='a' && ch<='f'){
        		currentByte = (currentByte << 4) + (ch +(10 - 'a'));
        		currentNibble++;
        	}
        	if (ch>='A' && ch<='F'){
        		currentByte = (currentByte << 4) + (ch +(10 - 'A'));
        		currentNibble++;
        	}
        	if(currentNibble==2){ //read whole byte
        		*buf = currentByte; buf++;
        		currentByte = 0;
        	}
        	if(ch==0){
        	break;
        	}
        	var++;
        }
		//return reinterpret_cast<EntryPoint> .....
		//return (EntryPoint) ((exec->fp[0]<<24)+(exec->fp[1]<<16)+(exec->fp[2]<<8)+(exec->fp[3]));        
		//return (EntryPoint) (test);
		
		/*
        buf = (char*) 0x8CFF0000;
        *buf = 0x00; buf++; *buf = 0x0B; buf++; //000B RTS
        *buf = 0x00; buf++; *buf = 0x09; //0009 NOP
        */
		return (EntryPoint) 0x8CFF0000;
    }
    
    
}

