.include "os/functions/_util.inc"


					!Syscall index and address on the cg50
DEFINE_OS_FUNC Serial_Open 		0x8008533e	!1bb7 8030da30
DEFINE_OS_FUNC Serial_Close		0x800853e6	!1bb8 8030dace
DEFINE_OS_FUNC Serial_ReadSingle	0x8008547a	!1bb9 8030db82
DEFINE_OS_FUNC Serial_Read		0x800854b8	!1bba 8030dbc6
DEFINE_OS_FUNC Serial_Peek		0x80084f22	!1bbb 8030dc86
DEFINE_OS_FUNC Serial_WriteSingle	0x800855d0	!1bbc 8030dcce
DEFINE_OS_FUNC Serial_WriteUnbuffered	0x80085616	!1bbd 8030dd14
DEFINE_OS_FUNC Serial_Write		0x80085652	!1bbe 8030dd50
DEFINE_OS_FUNC Serial_PollRX		0x800856b0	!1bbf 8030ddae
DEFINE_OS_FUNC Serial_PollTX		0x800856c4	!1bc0 8030ddc2
DEFINE_OS_FUNC Serial_ClearRX		0x800856d8	!1bc1 8030ddd6
DEFINE_OS_FUNC Serial_ClearTX		0x800856fe	!1bc2 8030ddfc
DEFINE_OS_FUNC Serial_IsOpen		0x8008587c	!1bc6 8030df76

