#include "deps.h"

namespace sound {
    /**
     * @brief Sets the internal audio buffer and updates its available size.
     *
     * @param buffer Pointer to the memory block to be used as the audio buffer.
     */
    void __cdecl Set_Audio_Buffer_(void* buffer) {
        if (buffer == nullptr) {
            _soundBufferSize = 0;
            _soundBuffer = nullptr;
        }
        else {
            uint32_t remaining_space = allocate::Get_Remaining_Block_Space_(buffer);
            _soundBufferSize = remaining_space;

            if (remaining_space != 0) {
                _soundBuffer = buffer;
            }
            else {
                _soundBuffer = nullptr;
            }
        }

        Audio_Clear_Cache_();
    }
}