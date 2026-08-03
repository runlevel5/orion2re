* Stability
  * [x] Long running game seems to leak memory somewhere

* Code cleanups

  * [x] Add helpers to work with pops instead of using raw bit operations

* Configuration

  * [ ] Expose the galaxy star-count soft limit as a configuration variable, validate it against the compile-time `MAX_STARS` capacity, and use it consistently in map generation and new-game setup.
  * [ ] Add typed build-queue presets as one configuration parameter instead of loading `BUILD.CFG` and `BUILD0.CFG` through `BUILD9.CFG` at keypress time.
    * Model the parameter as an outer fixed array of 11 presets and an inner fixed-capacity array of generated `build_queue_instruction` enum values.
    * Give the outer array semantic selectors `q`, `key0`, ..., `key9`, so assignments use syntax such as `build_queue_presets q = merge automated_factory research_lab end;` and `build_queue_presets key0 = clear end;`.
    * Represent modes, commands, products, designs, and buildings as single enum tokens. Keep `append`, `prepend`, `merge`, `clear`, `clear_refits`, `buy`, `buy10`, `buy50`, `buy75`, `unbuy`, `repeat`, `design1` through `design6`, and the documented product names; do not expose the 1.50 internal `print` and `flush` operations.
    * Use unique instruction values rather than native production IDs, and decode them explicitly in the queue interpreter.
    * Require an explicit `end` sentinel because configuration assignments are layered and may replace a longer preset with a shorter one; execution ignores all trailing array elements after the first `end`.
    * Let generated configuration parsing validate selectors, array bounds, token spelling, and enum membership. Add post-configuration validation that every preset contains `end`; retain runtime checks for sequence rules and colony-dependent legality such as `repeat`, research, buildability, ship capacity, and buying.
    * Treat the presets as synchronized configuration because applying them can alter queues, ships, and treasury state.
  * [x] Generate a human-facing, self-contained HTML configuration reference from the JSON5 schema.
    * [x] Replace the current flattened Markdown-style parameter dump with one offline `config-reference.html` containing embedded CSS and JavaScript and no external assets or network dependencies.
    * [x] Extend the schema model with a declaration-level documentation representation for public parameters, named scalar types, records, record fields, arrays, and their references. Keep the existing flattened leaf representation for runtime code generation, but do not use it as the primary documentation hierarchy or expand large arrays into repetitive entries.
    * [x] Allow named types, records, and record fields to carry human-facing title, description, remarks, and examples. Validate required documentation metadata for public definitions while allowing explicitly internal implementation definitions to remain hidden.
    * [x] Give groups, parameters, types, and records stable namespaced anchors. Link parameter types to their definitions, record fields to their types, parameters to groups, array index types to their enum definitions, and every definition back to the parameters and definitions that use it.
    * [x] Provide a persistent navigation pane organized into Overview, Groups, Parameters, Types, and Records. Make it responsive so it becomes a compact menu on narrow displays, and retain usable anchor navigation when JavaScript is disabled.
    * [x] Generate a compact embedded search index over IDs, canonical paths, titles, descriptions, enum values, mask flags, field names, aliases, and groups. Implement dependency-free instant search with exact ID/path matches ranked first, keyboard focus via `/` or Ctrl+K, Escape to close, and result links that jump directly to stable anchors.
    * [x] Show each parameter's assignment path, description, group and save/synchronization policy, declared type and shape, default, constraints, units, array selectors, remarks, and copyable configuration examples. Put native C++ bindings and targets in a collapsed implementation-details section rather than presenting them as primary user documentation.
    * [x] Show scalar definitions with their primitive base, constraints, numeric policy, enum values or mask flags, and reverse references. Show records as ordered field tables with linked field types, field descriptions, defaults, and reverse references.
    * [x] Render schema metadata as escaped text and examples as escaped code blocks; do not allow raw schema text to inject HTML, attributes, styles, scripts, or the embedded search index.
    * [x] Change the `config-docs` build target to produce `config-reference.html`, include it in documentation/release packaging, and keep generation deterministic with write-if-different behavior. Retain Markdown output only if another repository workflow still consumes it.
    * [x] Add schema-model tests for retained declarations and reference relationships, plus documentation-generator tests for deterministic output, escaping, stable anchors, valid internal link targets, non-expanded arrays, search-index coverage, reverse references, enum and record rendering, and graceful empty-section handling.

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
