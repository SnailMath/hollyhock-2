#include <sdk/os/file.hpp>
#include <sdk/os/mem.hpp>
#include <sdk/os/string.hpp>
#include "apps.hpp"
#include "elf.h"
#include <sdk/os/serial.hpp>

#define hex2asc(x) ((x)>9?((x)+'A'-10):((x)+'0'))

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

namespace Apps {
	const char *HHK_FOLDER[] = {
		"\\fls0\\",
		"\\drv0\\"
	};
	const char FILE_MASK[] = "*.hhk";

    struct AppInfo g_apps[MAX_APPS];
    int g_numApps;

    const Elf32_Ehdr *LoadELF(File f, const Elf32_Shdr **sectionHeaders) {
        const Elf32_Ehdr *elf;
        int ret = f.getAddr(0, (const void **) &elf);
        if (ret < 0) {
            return nullptr;
        }

        // Check magic number
        if (!(
            elf->e_ident[EI_MAG0] == ELFMAG0 &&
            elf->e_ident[EI_MAG1] == ELFMAG1 &&
            elf->e_ident[EI_MAG2] == ELFMAG2 &&
            elf->e_ident[EI_MAG3] == ELFMAG3
        )) {
            return nullptr;
        }

        // Check file class
        if (elf->e_ident[EI_CLASS] != ELFCLASS32) {
            return nullptr;
        }

        // Check data encoding
        if (elf->e_ident[EI_DATA] != ELFDATA2MSB) {
            return nullptr;
        }

        // Check ELF version
        if (elf->e_ident[EI_VERSION] != EV_CURRENT) {
            return nullptr;
        }

        // Check ABI (ignore ABI version EI_ABIVERSION)
        if (elf->e_ident[EI_OSABI] != ELFOSABI_SYSV) {
            return nullptr;
        }

        // Check ELF is an executable file
        if (elf->e_type != ET_EXEC) {
            return nullptr;
        }

        // Check machine
        if (elf->e_machine != EM_SH) {
            return nullptr;
        }

        // Check version
        if (elf->e_version != EV_CURRENT) {
            return nullptr;
        }

        *sectionHeaders = reinterpret_cast<const Elf32_Shdr *>(
            reinterpret_cast<const uint8_t *>(elf) + elf->e_shoff
        );

        return elf;
    }

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
		//String_Strcat(app.path, "\\fls0\\");
		String_Strcat(app.path, folder);
		String_Strcat(app.path, app.fileName);

		File f;
		int ret = f.open(app.path, FILE_OPEN_READ);
		if (ret < 0) {
			return;
		}

		const Elf32_Shdr *sectionHeaders;
		const Elf32_Ehdr *elf = LoadELF(f, &sectionHeaders);

		if (elf == nullptr) {
			return;
		}

		const Elf32_Shdr *sectionHeaderStringTable = &sectionHeaders[elf->e_shstrndx];
		for (int i = 0; i < elf->e_shnum; ++i) {
			const Elf32_Shdr *sectionHeader = &sectionHeaders[i];

			// skip the first empty section header
			if (sectionHeader->sh_type == SHT_NULL) {
				continue;
			}

			const char *sectionName = reinterpret_cast<const char *>(
				reinterpret_cast<const uint8_t *>(elf) +
				sectionHeaderStringTable->sh_offset +
				sectionHeader->sh_name
			);

			const char *sectionData = reinterpret_cast<const char *>(
				reinterpret_cast<const uint8_t *>(elf) +
				sectionHeader->sh_offset
			);

			if (String_Strcmp(sectionName, ".hollyhock_name") == 0) {
				String_Strcat(app.name, sectionData);
			} else if (String_Strcmp(sectionName, ".hollyhock_description") == 0) {
				String_Strcat(app.description, sectionData);
			} else if (String_Strcmp(sectionName, ".hollyhock_author") == 0) {
				String_Strcat(app.author, sectionData);
			} else if (String_Strcmp(sectionName, ".hollyhock_version") == 0) {
				String_Strcat(app.version, sectionData);
			}
		}

		g_apps[g_numApps++] = app;
	}

	void LoadAppInfo() {
		g_numApps = 0;

		for (unsigned int dirNr=0; dirNr<sizeof(HHK_FOLDER)/sizeof(HHK_FOLDER[0]);dirNr++){
			Find find;

			wchar_t fileName[100];
			struct File_FindInfo findInfoBuf;

			wchar_t findDir[100];
			int i=0;
			while (HHK_FOLDER[dirNr][i]!=0){
				findDir[i] = (wchar_t)HHK_FOLDER[dirNr][i]; 
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
					LoadApp(HHK_FOLDER[dirNr],fileName);
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

        const Elf32_Shdr *sectionHeaders;
        const Elf32_Ehdr *elf = LoadELF(f, &sectionHeaders);

		if (elf == nullptr) {
			return nullptr;
		}

		for (int i = 0; i < elf->e_shnum; ++i) {
			const Elf32_Shdr *sectionHeader = &sectionHeaders[i];

			// skip the first empty section header
			if (sectionHeader->sh_type == SHT_NULL) {
				continue;
			}

			const void *sectionData = reinterpret_cast<const void *>(
				reinterpret_cast<const uint8_t *>(elf) +
				sectionHeader->sh_offset
			);

			if ((sectionHeader->sh_flags & SHF_ALLOC) == SHF_ALLOC) {
				void *dest = reinterpret_cast<void *>(sectionHeader->sh_addr);

				if (sectionHeader->sh_type == SHT_PROGBITS) {
					Mem_Memcpy(dest, sectionData, sectionHeader->sh_size);
				} else if (sectionHeader->sh_type == SHT_NOBITS) {
					Mem_Memset(dest, 0, sectionHeader->sh_size);
				}
			}
		}

		return reinterpret_cast<EntryPoint>(elf->e_entry);
    }
}
