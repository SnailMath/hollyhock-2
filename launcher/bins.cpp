#include <sdk/os/file.hpp>
#include <sdk/os/mem.hpp>
#include <sdk/os/string.hpp>
#include "bins.hpp"
#include <sdk/os/serial.hpp>

#define hex2asc(x) ((x)>9?((x)+'A'-10):((x)+'0'))

#define Serial_print(s) Serial_Write((const unsigned char*)s,sizeof(s)-1)

#define Serial_printhex(x)	Serial_WriteSingle( hex2asc( ((x)>>28)&0xf ) );\
							Serial_WriteSingle( hex2asc( ((x)>>24)&0xf ) );\
							Serial_WriteSingle( hex2asc( ((x)>>20)&0xf ) );\
							Serial_WriteSingle( hex2asc( ((x)>>16)&0xf ) );\
							Serial_WriteSingle( hex2asc( ((x)>>12)&0xf ) );\
							Serial_WriteSingle( hex2asc( ((x)>> 8)&0xf ) );\
							Serial_WriteSingle( hex2asc( ((x)>> 4)&0xf ) );\
							Serial_WriteSingle( hex2asc( ((x)>> 0)&0xf ) );

class File {
public:
	File() : m_opened(false), m_fd(-1) {

	}

	~File() {
		if (m_opened) {
			File_Close(m_fd);
		}
	}

	int open(const char *path, int flags) {
		m_fd = ::File_Open(path, flags);
		m_opened = true;
		return m_fd;
	}

	int getAddr(int offset, const void **addr) {
		return ::File_GetAddr(m_fd, offset, addr);
	}

	int read(void *buf, int count) {
		return ::File_Read(m_fd, buf, count);
	}

private:
	bool m_opened;
	int m_fd;
};

class Find {
public:
	Find() : m_opened(false), m_findHandle(-1) {

	}

	~Find() {
		if (m_opened) {
			File_FindClose(m_findHandle);
		}
	}

	int findFirst(const wchar_t *path, wchar_t *name, struct File_FindInfo *findInfoBuf) {
		int ret = ::File_FindFirst(path, &m_findHandle, name, findInfoBuf);
		m_opened = true;
		return ret;
	}

	int findNext(wchar_t *name, struct File_FindInfo *findInfoBuf) {
		return ::File_FindNext(m_findHandle, name, findInfoBuf);
	}

private:
	bool m_opened;
	int m_findHandle;
};

namespace Bins {
	const char *BIN_FOLDER[] = {
		"\\fls0\\",
		"\\drv0\\"
	};
	const char FILE_MASK[] = "*.bin";

    struct AppInfo g_apps[MAX_APPS];
    int g_numApps;

	void LoadApp(const char *folder, wchar_t *fileName) {
		struct AppInfo app;
		Mem_Memset(&app, 0, sizeof(app));

		// copy the file name (converting to a non-wide string in the
		// process)
		for (int i = 0; i < 100; ++i) {
			wchar_t c = fileName[i];
			app.fileName[i] = c;
			if (c == 0x0000) {
				break;
			}
		}

		// build the path
		//strcat(app.path, "\\fls0\\");
		String_Strcat(app.path, folder);
		String_Strcat(app.path, app.fileName);

		File f;
		int ret = f.open(app.path, FILE_OPEN_READ);
		if (ret < 0) {
			return;
		}
		char *binInfo;
		f.getAddr(0x10, (const void**)&binInfo);

		if(*binInfo>=32&&*binInfo<127){
			for(int i=0;*binInfo!=0;i++)
				app.name[i] = *(binInfo++);
			while(*binInfo==0)binInfo++;
			for(int i=0;*binInfo!=0;i++)
				app.description[i] = *(binInfo++);
			while(*binInfo==0)binInfo++;
			for(int i=0;*binInfo!=0;i++)
				app.author[i] = *(binInfo++);
			while(*binInfo==0)binInfo++;
			for(int i=0;*binInfo!=0;i++)
				app.version[i] = *(binInfo++);
		}
		g_apps[g_numApps++] = app;
	}

	void LoadAppInfo() {
		g_numApps = 0;

		for (unsigned int dirNr=0; dirNr<sizeof(BIN_FOLDER)/sizeof(BIN_FOLDER[0]);dirNr++){
			Find find;

			wchar_t fileName[100];
			struct File_FindInfo findInfoBuf;

			wchar_t findDir[100];
			int i=0;
			while (BIN_FOLDER[dirNr][i]!=0){
				findDir[i] = (wchar_t)BIN_FOLDER[dirNr][i]; 
				i++;
			}
			int j=0;
			while (FILE_MASK[j]!=0){
				findDir[i] = (wchar_t)FILE_MASK[j];
				i++; j++;
			}
			findDir[i] = 0;
			int ret = find.findFirst(findDir, fileName, &findInfoBuf);
			while (ret >= 0) {
				if (findInfoBuf.type == findInfoBuf.EntryTypeFile) {
					const char* exclude = "run.bin";
					char different = 0;
					for(unsigned int i=0;i<sizeof(exclude);i++) different|=(exclude[i]^fileName[i]);
					if(different){
						LoadApp(BIN_FOLDER[dirNr],fileName);
					}
				}
				ret = find.findNext(fileName, &findInfoBuf);
			}
		}
	}

	EntryPoint RunApp(int i) {
		struct AppInfo *app = &g_apps[i];

		File f;
		int ret = f.open(app->path, FILE_OPEN_READ);
		if (ret < 0) {
		    return nullptr;
		}
		//get address where the program should be loaded
		unsigned char* address;
		f.getAddr(0x0c, (const void**)&address);
		EntryPoint entrypoint = (EntryPoint)0x8cff0000;
		if (address[0]==0x8c)
				entrypoint = (EntryPoint)((address[0]<<24) + (address[1]<<16) + (address[2]<<8) + (address[3]));

		int length = f.read((void*)entrypoint,0x20000);
		//unsigned char mode[] = {0,5,0,0,0,0};
		//Serial_Open(mode);
		//Serial_print("The length is: 0x");
		//Serial_printhex(length);
		//unsigned char c;
		//while(c!='\n') {
		//		Serial_ReadSingle(&c);
		//		if(c=='.'){
		//				Serial_Close(0);
		//				return nullptr;
		//		}
		//}
		//Serial_Close(0);
		return entrypoint;
    }
}
