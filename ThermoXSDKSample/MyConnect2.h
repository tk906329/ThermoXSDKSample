#pragma once

extern CString		OutInforStr;
// MyConnect

void ProcessData2();

BOOL		SendRS4852(char *sendbuf, int len);
UINT	OpenRS4852(CString RS485Port, int RS485Baud);
void	CloseRS4852();
