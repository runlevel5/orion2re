#include	"pch.h"

namespace	patch
{
	// Create jump (0xE9xxxxxxxx at targetAddr pointing to funcAddr)
	bool Create_Jmp(void* targetAddr, void* funcAddr)
	{
		DWORD oldProtect;

		if (!VirtualProtect(targetAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
			return false;

		BYTE* temp = (BYTE*)targetAddr;
		*(temp++) = 0xE9;
		*(DWORD*)(temp) = (DWORD)funcAddr - (DWORD)targetAddr - 5;

		VirtualProtect(targetAddr, 5, oldProtect, &oldProtect);

		return true;
	}

    bool Patch_Data(void* targetAddr, const void* patchData, uint32_t patchDataLen)
    {
        DWORD oldProtect = 0;

        if (!VirtualProtect(targetAddr, patchDataLen, PAGE_EXECUTE_READWRITE, &oldProtect))
            return false;

        // Copy bytes without CRT memcpy
        BOOL ok = WriteProcessMemory(
            GetCurrentProcess(),
            targetAddr,
            patchData,
            patchDataLen,
            nullptr
        );

        // Restore protection no matter what
        DWORD tmp = 0;
        VirtualProtect(targetAddr, patchDataLen, oldProtect, &tmp);

        // Make sure CPU doesn't execute stale instructions
        FlushInstructionCache(GetCurrentProcess(), targetAddr, patchDataLen);

        return ok != FALSE;
    }
}