#pragma once

namespace	patch
{
	bool	Create_Jmp(void* targetAddr, void* funcAddr);
	bool	Patch_Data(void* targetAddr, void* patchData, uint32_t patchDataLen);
};
