HWND windowh;
DWORD pid;
HANDLE processhandle;
DWORD BaseAddress;
void suspend(DWORD processId)
{
	
	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	Thread32First(hThreadSnapshot, &threadEntry);
	
	do
	{
		if (threadEntry.th32OwnerProcessID == processId)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
				threadEntry.th32ThreadID);
			
			SuspendThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));
	
}
void resume(DWORD processId)
{
	
	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	Thread32First(hThreadSnapshot, &threadEntry);
	
	do
	{
		if (threadEntry.th32OwnerProcessID == processId)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
				threadEntry.th32ThreadID);
			
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));
	
	CloseHandle(hThreadSnapshot);
}
void runSetDebugPrivs()
{
	
	HANDLE __HandleProcess = GetCurrentProcess(), __HandleToken;
	TOKEN_PRIVILEGES priv;
	LUID __LUID;
	OpenProcessToken(__HandleProcess, TOKEN_ADJUST_PRIVILEGES, &__HandleToken);
	LookupPrivilegeValue(0, "seDebugPrivilege", &__LUID);
	priv.PrivilegeCount = 1;
	priv.Privileges[0].Luid = __LUID;
	priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(__HandleToken, false, &priv, 0, 0, 0);
	
	CloseHandle(__HandleToken);
	CloseHandle(__HandleProcess);
}

DWORD GetModuleNamePointer(LPSTR LPSTRModuleName, DWORD __DwordProcessId)
{
	
	MODULEENTRY32 lpModuleEntry = { 0 };
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, __DwordProcessId);
	if (!hSnapShot)
		return NULL;
	lpModuleEntry.dwSize = sizeof(lpModuleEntry);
	BOOL __RunModule = Module32First(hSnapShot, &lpModuleEntry);
	while (__RunModule)
	{
		if (!strcmp(lpModuleEntry.szModule, LPSTRModuleName))
		{
			CloseHandle(hSnapShot);
			return (DWORD)lpModuleEntry.modBaseAddr;
		}
		__RunModule = Module32Next(hSnapShot, &lpModuleEntry);
	}
	
	CloseHandle(hSnapShot);
	return NULL;
}
HMODULE GetModuleHandleExtern(char* szModuleName, DWORD dwProcessId) // GetMoguleHandle recode for external processes
{
	
	if (!szModuleName || !dwProcessId) { return NULL; } // invalid input
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (hSnap == INVALID_HANDLE_VALUE) { return NULL; }
	MODULEENTRY32 me;
	me.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(hSnap, &me)) // we go now
	{
		while (Module32Next(hSnap, &me)) // through all modules in the target process
		{
			if (!strcmp(me.szModule, szModuleName)) // is this the model we are looking for?
			{
				CloseHandle(hSnap);
				return me.hModule; // this is our module, return it.
			}
		}
	}
	
	CloseHandle(hSnap);
	return NULL; // counldn't find module
}

void CalcBase(unsigned int UserSelectedPid)
{
	MODULEENTRY32 me32;
	HANDLE hProcessSnap;

	me32.dwSize = sizeof(me32);
	hProcessSnap = CreateToolhelp32Snapshot(0x8, UserSelectedPid);
	Module32First(hProcessSnap, &me32);
	BaseAddress = (unsigned int)me32.modBaseAddr;

	CloseHandle(hProcessSnap);
}
DWORD BaseGameAddress()
{
	return BaseAddress;
}

HANDLE Handle;
template <class T>

T read(DWORD addr)
{

	T result;
	ReadProcessMemory(Handle, reinterpret_cast<LPCVOID>(addr), &result, sizeof(T), NULL);

	return (result);
}

template <class T>

void write(DWORD addr, T Value)
{
	WriteProcessMemory(Handle, reinterpret_cast<LPVOID>(addr), &Value, sizeof(T), NULL);
}


char generate_character()
{
	random_device random_device;
	mt19937 mersenne_generator(random_device());
	uniform_int_distribution<> distribution(97, 122);

	return static_cast<unsigned char>(distribution(mersenne_generator));
}
string random_string(size_t length)
{
	string str(length, 0);
	generate_n(str.begin(), length, generate_character);
	return str;
}

int Size[2];
int Position[2];
int BorderLeft;
int BorderTop;
RECT TargetRect2;

HWND Target;

bool GetSize()
{
	WINDOWINFO info;
	ZeroMemory(&info, sizeof(info));
	info.cbSize = sizeof(info);

	GetWindowInfo(Target, &info);

	Size[0] = info.rcClient.right - info.rcClient.left;
	Size[1] = info.rcClient.bottom - info.rcClient.top;

	Position[0] = info.rcClient.left;
	Position[1] = info.rcClient.top;

	GetClientRect(Target, &TargetRect2);

	BorderLeft = GetSystemMetrics(SM_CXBORDER);
	BorderTop = GetSystemMetrics(SM_CYCAPTION);

	return true;
}


bool Attach(char* targetName)
{
	Target = FindWindow(NULL, targetName);

	if (!Target)
		return false;

	if (!GetSize())
		return false;

	return true;
}


bool OpenTarget(HWND wndTarget, DWORD Access)
{
	DWORD dwProcess;

	GetWindowThreadProcessId(wndTarget, &dwProcess);
	Handle = OpenProcess(Access, NULL, dwProcess);

	if (!Handle)
		return false;

	return true;
}
