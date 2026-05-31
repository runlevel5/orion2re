src/game/funcs.h

- Blocker: the claimed header path was not actually provided in the conversation. The only authoritative path I could infer from the current workspace was `src/game/funcs.h` because it is modified, but your workflow requires treating the claimed header path from the previous node as authoritative and failing on uncertainty rather than guessing.
- Validation could not proceed compliantly because selecting `src/game/funcs.h` would be an inference, not the provided claimed path.
- Additional blocker: `src/game/funcs.h` is not a clean variable-validation target under this workflow because the `pre-variables` version contains a large mix of raw-address variable stubs and function wrappers, making it unsuitable to assume as the intended claimed header without explicit confirmation.
fail
---------------------------------------
