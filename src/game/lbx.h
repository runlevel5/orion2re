#pragma once
#include <cstdint>

namespace lbx {
    /* Initializes an LBX stream header and loads its record offset table.
     * @param header Stream state to initialize.
     * @param filename LBX file path to open.
     */
    void __cdecl LBX_init_(s_lbx_stream_header* header, const char* filename);

    /* Returns the byte size of a record in an opened LBX stream.
     * @param header Initialized LBX stream state.
     * @param record_idx Zero-based record index to query.
     */
    uint32_t __cdecl LBX_getRecordLength_(s_lbx_stream_header* header, int32_t record_idx);

    /* Closes an LBX stream and clears its cached state.
     * @param header Stream state to shut down.
     */
    void __cdecl LBX_uninit_(s_lbx_stream_header* header);

    /* Opens an LBX-backed wave stream and loads its record offset table.
     * @param stream Wave stream state to initialize.
     * @param filename LBX file path to open.
     */
    void __cdecl LBX_init_stream_(s_wave_stream_header* stream, const char* filename);

    /* Reads one record from an opened LBX stream into a caller-provided buffer.
     * @param header Initialized LBX stream state.
     * @param record_idx Zero-based record index to read.
     * @param buffer Destination buffer for the record contents.
     */
    void __cdecl LBX_readRecord_(s_lbx_stream_header* header, int32_t record_idx, void* buffer);
}