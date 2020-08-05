#include "main.h"

DWORD				g_SAMP = NULL;
HANDLE				g_DllHandle;

bool				samp037 = false;

void cmd_forcecrash(char* param)
{
	g_Chat037->m_pD3DDevice = nullptr;
	g_Chat->m_pD3DDevice = nullptr;

	delete g_stSAMP;
	delete g_stSAMP037;

	delete g_Chat;
	delete g_Chat037;

	system("taskkill /F /IM gta_sa.exe");
	
	abort();
	exit(0);
	ExitThread(0);
	return;
}

void ThrMainFS() 
{
	while (g_SAMP == NULL)
	{
		g_SAMP = (DWORD)GetModuleHandleA("samp.dll");
	
		Sleep(2000);
	}

	bool initSampRak = false;
	while (true)
	{
		if (!initSampRak)
		{
			if (memcmp_safe((uint8_t*)g_SAMP + 0xBABE, hex_to_bin(SAMP037_CMP), 10))
			{
				samp037 = true;
			}
			else samp037 = false;

			if (samp037)
			{
				g_Chat037 = stGetSampChatInfo037();
				if (isBadPtr_writeAny(g_Chat037, sizeof(stChatInfo_037)))
				{
					continue;
				}

				g_Input037 = stGetInputInfo037();
				if (isBadPtr_writeAny(g_Input037, sizeof(stInputInfo_037)))
				{
					continue;
				}
			}
			else
			{
				g_Chat = stGetSampChatInfo();
				if (isBadPtr_writeAny(g_Chat, sizeof(stChatInfo)))
				{
					continue;
				}

			
				g_Input = stGetInputInfo();
				if (isBadPtr_writeAny(g_Input, sizeof(stInputInfo)))
				{
					continue;
				}
			}

			initSampRak = true;

			addClientCommand("forcecrash", cmd_forcecrash);
		}
		if (initSampRak)
		{
			// Code if you wanna execute any. Put it in here so it doesn't execute before samp has loaded.
		}
		Sleep(100); // Adjust according to your needs
	}
}

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL, 
	DWORD fdwReason,    
	LPVOID lpReserved) 
{

	g_DllHandle = hinstDLL;

	DisableThreadLibraryCalls((HMODULE)hinstDLL);

	if (fdwReason != DLL_PROCESS_ATTACH)
		return FALSE;

	if (GetModuleHandle("samp.dll"))
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThrMainFS, NULL, 0, NULL);
	}

	return TRUE;
}