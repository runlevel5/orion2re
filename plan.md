* Stability
  * [x] Long running game seems to leak memory somewhere

* Code cleanups

  * [x] Add helpers to work with pops instead of using raw bit operations

* Networking

  * [ ] Add a server-side handshake timeout so accepted TCP clients must send HELLO promptly or be disconnected.
  * [ ] Add an idle/read timeout for established router connections to limit slowloris-style resource exhaustion.
  * [ ] Add a client-side request or session timeout policy for stalled router operations where it improves recovery behavior.
  * [ ] Add targeted tests covering handshake timeout, idle timeout, and expected disconnect behavior.

* Rendering

  * [ ] Add a dedicated presentation pipeline abstraction between the 640x480 software frame upload and final SDL output.
  * [ ] Add configurable presentation filter selection for bilinear and pixel-art modes through the existing SDL renderer path.
  * [ ] Add intermediate render-target support for future multi-pass shader effects and scaler passes.
  * [ ] Add shader compilation and loading support for vendored presentation shaders using SDL\_shadercross (https://github.com/libsdl-org/SDL\_shadercross), including build-time packaging and runtime error reporting.
  * [ ] Add shader pipeline integration points for CRT-like post-processing without changing the core fixed-resolution game render path.
  * [ ] Add nearest-integer prescale support as a distinct presentation step before any optional shader pass.
  * [ ] Add validation coverage for aspect-preserved presentation, fullscreen toggling, and future presentation mode selection.

* Sound

  * [X] Popping sound in intro movie - libsmacker sound buffer underrun. Fixed by taking care of inconsistent frame decoding.

* Mod packages

  * [x] Define and validate JSON5 mod manifests with stable identity, versions, engine compatibility, dependencies, conflicts, ordering, and configuration entry points.
  * [x] Resolve packages directly by stable ID and resolve dependencies, conflicts, cycles, version constraints, and deterministic load order without package discovery.
  * [x] Make `enable <mod-id>;` load package configuration and resources as one transaction.
  * [x] Add transactional VFS layering so failed package validation, configuration loading, or resource mounting leaves the prior state active.
  * [x] Add fixtures and integration tests for valid, malformed, incompatible, cyclic, conflicting, and rollback cases.
  * [x] Separate mod graph management and activation from CFG file and include loading.
  * [x] Replace implicit `data/*.zip` mod scanning with the single optional base layer `data/base.zip`.
  * [x] Add source-owned supported mod packages and CMake archive/deployment targets, beginning with `data/mods/ver150/`.

* Save-game and multiplayer serialization

  * [ ] Define versioned save-game and multiplayer representations independently from CFG syntax, JSON5 schema layout, C++ memory layout, and declaration order.
  * [ ] Serialize deterministic configuration by stable identity together with enabled package versions, resource identity, and engine versions.
  * [ ] Define missing-field defaults, optional-field skipping, explicit migrations, and configuration restoration before dependent save sections.
  * [ ] Synchronize deterministic gameplay configuration and resource identity while keeping approved interface settings local.
  * [ ] Add compatibility fixtures and diagnostics for save migrations, package or resource mismatches, and multiplayer configuration differences.
