/* win_entry.cԴ�ļ� */
/* �����õ����ⲿ�������� */
int	  __stdcall	MessageBoxA(void* hWnd, char* lpText,  
														char* lpCaption,  int uType);
int __stdcall GetModuleHandleA(char* lpModuleName);
char* __stdcall GetCommandLineA();
/*********************************************************** 
 * ����:	WinMain������
 **********************************************************/
int __stdcall WinMain (void* hInstance, void* hPrevInstance,
                    char* szCmdLine, int iCmdShow)
{
	  int MB_OK = 0;  /* 0x00000000L */
	  int MB_ICONINFORMATION = 64; /* 0x00000040L */
    MessageBoxA(0,szCmdLine,"֧�ִ��������в�����Win32Ӧ�ó���:", MB_OK + MB_ICONINFORMATION); 
    return 1 ;
}

/*********************************************************** 
 * ����:	��ں���
 **********************************************************/
int _entry()
{
	char *szCmd; 

	szCmd = GetCommandLineA();	
	exit(WinMain(GetModuleHandleA(0), 0, szCmd,10));
}
