src/game/swap.h

Failures:
- `_swap_block_size`, `0x00587FE4`: wrong raw-address mapping in the historical recovery. Ghidra disassembly for `swap::Save_Block_To_Drive_` and `swap::Restore_Block_From_Drive_` shows `0x00587FE4` is used as the byte count (`MOV [0x00587fe4],EAX`, later `MOV EAX,[0x00587fe4]` before `fwrite`/`fread`), so the current project type `int32_t` is plausible, but the `pre-variables` header stub pointed `_swap_block_size` at `0x00587FE4` while the live Ghidra symbol ownership is inconsistent and duplicated as `_swap_block_address` elsewhere. This header cannot be validated confidently from the available symbol ownership state.
- `_swap_block_address`, `0x00587FE8`: alias/symbol-owner failure. Ghidra shows primary symbol `_swap_block_address` at `0x00587FE8`, which matches pointer usage in disassembly (`MOV [0x00587fe8],EDI`, later `MOV ESI,[0x00587fe8]`), but Ghidra also shows another primary `_swap_block_address` label at `0x00587FE4`. Same-name competing ownership across two addresses prevents confident validation under the alias rules.

Blocker:
- Live Ghidra symbol state for this namespace is internally inconsistent: `_swap_block_address` exists at both `0x00587FE4` and `0x00587FE8`, and `_swap_block_size` is absent. Even though the disassembly strongly suggests the intended mapping is size=`0x00587FE4`, address=`0x00587FE8`, the required conservative alias/ownership validation fails.

Fail
