src/game/replace.h

- s_Shift_Palette_Range_exceeds_25_0055b7c4 @ 0x0055B7C4: fail. Old stub was `inline const char*` to binary string storage, but current recovery changed storage kind and namespace ownership to `replace::const char[41]`. In Ghidra, 0x0055B7C4 is owned by primary symbol `s_Shift_Palette_Range()_exceeds_25_0055b7c4` in the Global namespace, defined as one read-only string object at that address, not a `replace` namespace object. Per alias rules, old stub address resolving to a different namespace owner is an alias candidate and must fail.

- blocker for full validation confidence: `_palette_color_list` @ 0x0057E07C is defined in Ghidra only as `undefined4 00000000h`, and while disassembly at `replace::Establish_Color_Gradient_` and `replace::Interlaced_Gray_Scale_Off_Page_` proves writable pointer-like use and null default, I could not obtain direct xref listings from the RPC helper for this data address. That does not by itself contradict the recovered `uint8_t*`, but because the header already fails on the string alias/namespace issue, overall validation fails.

fail
---------------------------------------
