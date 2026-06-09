# SDL3 threaded video migration plan

Status values used below: `pending`, `in_progress`, `completed`.

This plan is intentionally limited to video, windowing, cursor, keyboard/mouse event plumbing, frame publication, and startup/threading. DirectPlay, DirectSound, WINMM, and unrelated Win32 compatibility work are out of scope for this phase.

## Goals for this branch

- Replace the DirectDraw-shaped render backend with a simple in-memory 8-bit compat layer.
- Keep the main game running on a dedicated worker thread.
- Keep all legacy drawing logic on the game thread and pointed at in-memory buffers only.
- Move SDL event handling, final blitting, scaling, and present work to the main thread.
- Do not introduce new source files for this migration. Reuse and reshape the existing `platform.cpp` and `video.cpp` split.
- Prevent tearing by ensuring the main thread never reads from a buffer currently being modified by the game thread.
- Start with a preallocated two-buffer ownership model and keep the buffer count configurable through a constant for a later N-buffer follow-up.
- Replace the current spin-wait synchronization with explicit frame-publication state and non-blocking main-thread polling at present cadence.
- Move cursor compositing to the main thread using the legacy cursor image data, instead of blitting the cursor into the game render surface.
- Keep the main-thread presenter running at present cadence even when no newer world frame was published, so cursor composition and window responsiveness do not depend on world redraws.
- Re-evaluate whether `setvesa`, vertical-blank waits, and old flip/wait behavior are still needed once the backend is memory-based.

## Current codebase analysis

### 1. Startup and thread model

- `src/main.cpp` still forwards to `platform::WinMain_Proxy(...)`.
- `src/game/platform.cpp` still owns Win32 window creation, message handling, DirectDraw setup, and startup/shutdown of the worker thread.
- The actual game already runs on a dedicated worker thread via `_beginthread(MOX2::thunk_main2_, ...)`.
- The current branch still uses a Win32 message trampoline for presentation:
  - game code calls `platform::Toggle_Pages_Stub_()` from many places,
  - `Toggle_Pages_Stub_()` sends `WM_USER + 1` and spin-waits,
  - `WndProc(...)` calls `video::Toggle_Pages_()` on the main thread.
- This split is better than the old single-thread SDL attempt, but the current render path is still tied to DirectDraw surfaces and message-driven flip semantics.

### 2. Surface and buffer model today

- `src/game/platform.cpp` owns DirectDraw-shaped front/back/cursor surfaces.
- `src/game/video.cpp` owns `g_offscreen_surface_1`, `g_offscreen_surface_2`, palette state, scan lists, and refresh flags.
- Many drawing functions all across the codebase call `platform::Lock_Appropriate_Render_Buffer()` and `platform::Unlock_Appropriate_Render_Buffer()` directly before and after touching pixels.
- `video::_current_video_page` is treated by the game as a raw 8-bit linear framebuffer pointer with a pitch.
- `video::Toggle_Pages_()` is the real present boundary. It currently also mixes together:
  - refresh bookkeeping,
  - page copies,
  - partial dump logic,
  - stencil refresh behavior,
  - palette refresh,
  - cursor save-under and cursor blits.
- The current branch effectively uses three distinct surface roles:
  - `g_offscreen_surface_1`
    - the normal software render page selected by `Set_Page_Off_()`.
  - `g_offscreen_surface_2`
    - the persistent baseline/back page selected by `Set_Page_Back_()`.
  - `g_primary_surface`
    - the platform-facing primary/on-page target used by presentation plumbing and some legacy direct-to-primary code paths.
- Only two of those are the game's persistent software pages:
  - `Off`
  - `Back`
- `Primary` is not part of the normal page-selection API and should be treated as a platform/present path surface, not as a third long-lived gameplay page.
- The active code paths do not meaningfully rotate among three gameplay pages:
  - `platform::g_render_buffer` is set to `1` by `Set_Page_Off_()` and `2` by `Set_Page_Back_()`.
  - the `g_render_buffer == 0` path corresponds to `g_primary_surface` and appears to be legacy/on-page plumbing rather than the normal software-render path.

### 3. Why the old single-thread SDL direction is wrong for this codebase

- The game does not have one large, well-behaved frame loop.
- It contains many small loops and many scattered calls to `platform::Toggle_Pages_Stub_()`.
- It also performs random refreshes specifically so the mouse can appear to move while game logic is inside those small loops.
- In a single-thread SDL model, event pumping and present cadence become hostage to arbitrary game control flow.
- That leads directly to visible clunkiness, cursor blink, uneven interactivity, and poor input responsiveness.
- A hardware cursor can hide part of the cursor symptom, but it does not solve the frame pacing or event servicing problem.

## Target architecture

### 1. Thread ownership

- Main thread owns:
  - SDL initialization and shutdown,
  - SDL window,
  - SDL renderer/texture or equivalent present path,
  - SDL event polling,
  - translation of SDL keyboard/mouse/window events into legacy engine input state,
  - frame upload/scaling/present,
  - cursor compositing during present.
- Game thread owns:
  - all game logic,
  - all legacy software drawing,
  - all refresh bookkeeping,
  - all writes into compat memory surfaces,
  - all page-toggle decisions,
  - publication of finished frames.

### 2. File ownership and threading boundary

- `platform.cpp` should contain only platform-facing and main-thread-facing responsibilities:
  - SDL startup and shutdown,
  - main-thread event loop,
  - keyboard and mouse event translation,
  - frame upload/scaling/present,
  - cursor compositing at present time,
  - worker-thread startup and shutdown,
  - any synchronization needed to hand published frames from the game thread to the main thread.
- `video.cpp` should contain all software-rendering-side responsibilities:
  - in-memory 8-bit surface ownership,
  - page selection,
  - raw framebuffer access,
  - memory-based blits and page copies,
  - scan-list and stencil refresh logic,
  - frame publication logic from the game thread,
  - cursor image generation from `video::_mouse_palette`.
- This split should be treated as the threading boundary:
  - only `platform.cpp` runs on the main thread,
  - software rendering logic stays in `video.cpp` and is executed by the game thread.
- Do not create a separate backend file layer for this migration unless a later cleanup pass makes that unavoidable.

### 3. Compat layer shape

- Replace DirectDraw-backed surfaces with simple in-memory 8-bit surfaces.
- A compat surface only needs:
  - width,
  - height,
  - pitch,
  - pixel pointer,
  - optional palette reference,
  - optional color key metadata for cursor and simple blits.
- The compat layer should preserve only the legacy behavior that game code actually depends on.
- The compat layer should not pretend to be a real SDL surface API if all callers only need raw indexed pixel memory.

### 4. Recommended buffer set

Initial recommended allocations:

- `page_off`
  - the normal persistent software render page.
- `page_back`
  - the persistent baseline/back page used by existing save/restore and incremental rebuild logic.
- `present path`
  - a main-thread presentation target or upload path that reads from the currently published off page.
- `cursor_surface`
  - 24x24 indexed cursor image generated from `video::_mouse_palette`.

Optional additional scratch/storage buffers may be kept if needed for stencil or masked dump behavior.

The important rule is that the two software pages remain stable across the lifetime of the renderer. The main thread may temporarily sample the published off page for presentation, but it must never read from that page while the game thread is modifying it.

### 5. Frame publication and anti-tearing design

- Do not use per-draw locks to protect presentation.
- The existing `Lock_Appropriate_Render_Buffer()` / `Unlock_Appropriate_Render_Buffer()` pattern is too fine-grained and is not the right primitive for cross-thread frame ownership.
- The correct synchronization boundary is `video::Toggle_Pages_()`.

Recommended publication model:

- Keep the two persistent gameplay pages stable:
  - `Off`
  - `Back`
- Treat `Off` as the publishable page.
- Treat `Back` as the persistent baseline/scratch page used by existing explicit copy helpers.
- `video::Toggle_Pages_()` remains the only publication point.
- At publication time, snapshot `Off` into a separate published-frame buffer.
- The main thread presents from that published snapshot rather than sampling live `Off`.
- This preserves stable `Off`/`Back` gameplay semantics while preventing the presenter from racing with legacy post-present mutations of `Off`.
- `Back` remains game-private in the initial design and should not participate in presentation ownership.
- Buffer handoff should be implemented as explicit published-frame ownership state plus a low-contention wait primitive at the publication boundary, not as fine-grained locks around draw calls.

This ownership handoff is the core anti-tearing primitive for the new design.

### 6. Thread synchronization design

- Do not use spinlocks or busy-wait flags in the active frame path.
- The current `g_pending_toggle_pages` style synchronization should be removed from the final design.
- Synchronize only at the frame publication boundary, not around individual draw calls.

Recommended model:

- Game thread is the single producer of published frames.
- Main thread is the single consumer of published frames.
- `video::Toggle_Pages_()` is the publish point.
- Published-frame ownership is tracked with atomics using release/acquire semantics.

Preferred implementation shape:

- Keep game rendering on its normal persistent pages.
- Start with exactly two stable gameplay pages:
  - `Off`
  - `Back`
- Add one separate published-frame buffer used only for game-thread-to-main-thread handoff.
- Do not rotate arbitrary frame slots.
- Do not rebind page storage per frame.
- Publish only the `Off` page.
- Keep `Off` game-thread-owned.
- If the presenter is still using the previously published snapshot when the game wants to publish a newer frame, the game thread waits only at that publish boundary.
- `Back` remains writable by the game thread and continues to serve the existing continuity-preserving copy flows.
- Use SDL synchronization primitives for the handoff wait:
  - `SDL_Mutex`
  - `SDL_Condition`
- Do not use Win32 events, spin-waits, or `volatile` for ownership control.

Main-thread visibility of new frames:

- The main thread does not need a per-frame wakeup message from the game thread if it is already running a steady present loop.
- `platform.cpp` should run a frame-paced main loop that:
  - drains SDL events with `SDL_PollEvent()`,
  - checks whether a newer published snapshot is available,
  - uploads newly published world pixels only when a newer published snapshot is available,
  - otherwise reuses the last active world frame,
  - still composites cursor and presents every present tick even if no newer world frame was published,
  - releases the published snapshot only after the read of that snapshot is complete,
  - otherwise keeps cursor and window responsiveness at the same cadence.
- In the initial design, only publication of a newer snapshot can block on the presenter.

Event-loop behavior:

- Do not use plain `SDL_WaitEvent()` as the primary loop for this architecture.
- A wait-based loop would require an extra wakeup path from the game thread whenever a frame is published.
- Prefer `SDL_PollEvent()` in the active main loop so presentation cadence does not depend on event arrival.
- If needed later, `SDL_WaitEventTimeout()` may be considered only for minimized, inactive, or otherwise low-activity states.

Input and shutdown synchronization:

- Mouse position and button state can be shared through atomics.
- Keyboard input should continue through a queue/ring-buffer style path, protected by either single-producer/single-consumer assumptions or a small mutex if needed.
- Shutdown should use an atomic exit flag plus normal thread wait/join primitives, not a spin-wait loop.

### 7. What to do with the current "locking"

- Keep the function names temporarily so the scattered drawing code does not need a massive immediate rewrite.
- Redefine their meaning:
  - `Lock_Appropriate_Render_Buffer()` returns pitch and pointer for the already-owned in-memory surface.
  - `Unlock_Appropriate_Render_Buffer()` becomes a no-op or debug check in normal game-thread rendering.
- Do not use these calls for cross-thread synchronization.
- If we need strict ownership assertions, add lightweight debug-only tracking to verify that only the game thread touches render surfaces.

### 8. Cursor design

- Keep `video::_mouse_palette` and legacy cursor-shape selection logic.
- Keep a compat `cursor_surface` populated from the current cursor image.
- Move cursor compositing entirely to the main thread.
- Do not draw the cursor into `offscreen_primary` or `present_front` permanently.
- Do not rely on save-under cursor restore in the game render surfaces.
- The main thread should composite the current cursor over the published frame during present using the current logical mouse position.
- Cursor updates should happen at main-thread present cadence, not only when the game happens to toggle pages.

This should eliminate the most visible cursor blink and reduce the amount of cursor-specific special casing in `video::Toggle_Pages_()`.

### 9. Palette design

- Preserve the current 8-bit palette-driven rendering model.
- Keep `_current_palette` and `g_palette_entries` as the authoritative legacy palette state.
- Add a palette generation counter or equivalent dirty flag so the main thread can cheaply know when the uploaded presentation palette needs to be refreshed.
- Reuse the SDL palette expansion / texture upload logic from the working SDL branch where possible.

## Explicit design decisions

### Decision 1: keep the dedicated game thread

Reason:

- It matches the existing structure of this branch.
- It fits the game's scattered refresh model.
- It lets the main thread keep SDL event handling responsive.
- It avoids returning to a DOS-style single-thread control flow that already proved clunky.

### Decision 2: make `video::Toggle_Pages_()` a pure game-thread operation

Reason:

- Once all surfaces are plain memory, `video::Toggle_Pages_()` no longer needs to run on the main thread.
- This removes the `WM_USER + 1` trampoline and the spin-wait in `platform::Toggle_Pages_Stub_()`.
- `Toggle_Pages_Stub_()` should eventually become a direct call path or a thin compatibility wrapper.

### Decision 3: publish by ownership transfer, do not let the presenter read live render buffers

Reason:

- This is the cleanest way to prevent tearing from in-progress writes.
- It makes the main thread independent from game-thread draw timing.
- It turns frame handoff into one well-defined synchronization point.
- The active renderer relies on persistent page continuity, so the implementation should preserve stable `Off` and `Back` pages instead of treating each frame as its own page bundle.
- It also matches the observed main-screen behavior, where `Off` may be mutated again by legacy post-present bookkeeping immediately after the frame boundary, so the presenter must consume a stable snapshot rather than live `Off` memory.

### Decision 4: keep compat API shape initially, simplify internals first

Reason:

- The codebase contains many render calls scattered across many files.
- A compatibility-preserving backend swap is safer than a large gameplay-side rewrite.
- Once the in-memory path is stable, further cleanup can remove unused DirectDraw-shaped surface semantics.

### Decision 5: cursor belongs to the presenter, not to the game render surface

Reason:

- Cursor motion must stay smooth even when the game is not actively redrawing the world.
- Main-thread cursor compositing avoids the old save-under blink path.
- It removes the cursor from scan-list bookkeeping and partial-dump hazards.

### Decision 6: do the migration inside existing files

Reason:

- This keeps the change set smaller and easier to review.
- It preserves a clear boundary that already exists in the codebase.
- `platform.cpp` is the natural place for main-thread SDL work.
- `video.cpp` is the natural place for memory-surface and software-rendering work.

### Decision 7: publish with atomics and wait at the frame boundary without spinning

Reason:

- Spinning on a shared variable wastes CPU and produces poor behavior under load.
- The game only needs synchronization when a complete frame becomes available or when the `Off` page is still owned by the presenter.
- A single-producer/single-consumer published-frame model matches this codebase well.
- The main thread can detect new frames directly from atomic publication state during its steady present loop.

### Decision 9: preserve stable page identity during the migration

Reason:

- The current renderer is incremental rather than stateless.
- `Copy_Off_To_Back_()` and `Copy_Back_To_Off_()` are used throughout the codebase as explicit save/restore and baseline rebuild operations.
- Many flows draw a stable scene to `Off`, snapshot it into `Back`, then later restore `Off` from `Back` before applying transient overlays or partial updates.
- Replacing stable pages with per-frame page storage would either break continuity or force extra copies just to reconstruct the old behavior.

### Decision 8: use a frame-paced polling loop on the main thread

Reason:

- The presenter is expected to run at refresh cadence anyway.
- Cursor compositing should continue even if no new SDL event arrives.
- Cursor compositing should also continue even if no newer world frame was published by the game thread.
- `SDL_WaitEvent()` would make frame pickup depend on extra wakeups.
- Polling once per present tick is simpler and matches the intended architecture.

## Reusable pieces from the working SDL branch

These should be copied or adapted rather than reinvented:

- SDL window creation and destruction.
- SDL renderer and present texture setup.
- SDL keyboard translation into the existing `key::_last_key_pressed` format.
- SDL mouse event translation and logical/window coordinate helpers.
- Focus handling and shutdown behavior.
- Palette upload logic for the SDL present path.
- Cursor image refresh logic that copies from `video::_mouse_palette` into a 24x24 indexed buffer.

These should not be copied as-is:

- The single-thread event-servicing model from `sdl-video`.
- The `software_surface` lock/unlock emulation design.
- The old SDL path that still mimics per-operation surface locking.
- Cursor save-under behavior that assumes cursor drawing belongs in the software world surface.

## `setvesa`, flip waiting, and legacy wait behavior

### 1. `setvesa`

- Do not remove `setvesa` immediately.
- First audit which parts are still real refresh bookkeeping helpers and which parts only exist to serve old VESA or DirectDraw presentation assumptions.
- Functions that only add dirty rectangles or scan-list coverage may still be useful.
- Functions that only exist for hardware mode switches, vertical blank, or page-flip timing are likely removable in the SDL path.
- Current finding: the active `setvesa` usage is refresh scan-list bookkeeping only. It should be retained until a later cleanup folds that bookkeeping into a better-named helper.

### 2. `Flip_Surface()` and vertical blank waits

- Once presentation is SDL-driven on the main thread, the old DirectDraw `Flip_Surface()` semantics should disappear.
- Any waiting for vertical blank should be re-evaluated from scratch.
- The game thread should not block on display refresh unless an exact gameplay dependency is proven.
- If pacing is needed, it should be limited to the main-thread presenter, not embedded in legacy game rendering.

### 3. `screen_dump_flag == 1` masked path

- The old `screen_dump_flag`-driven present split is no longer part of the active SDL present model.
- The masked dump path was stubbed and has been removed from the active refresh-state model.
- Remaining refresh compatibility helpers now preserve only `screen_refresh_flag`, which is the only refresh-mode state still used operationally.

## Additional findings from rendering-flow analysis

### 1. The renderer is incremental, not full-reset by default

- The drawing paths write directly into the selected page memory.
- They do not automatically clear or recreate pages between presents.
- Dirty-region tracking through `screen_refresh_flag`, scan lists, and stencil lists is active and assumes previous page contents remain meaningful.

### 2. `Toggle_Pages_()` is a presentation boundary, not a page-identity swap

- `Toggle_Pages_()` presents the current `Off` page through platform presentation surfaces.
- It then returns to `Set_Page_Off_()`.
- It does not swap the identities of `Off` and `Back`.
- This means the migration must not model presentation as alternating `Off` and `Back` ownership each frame.
- `Back` is a persistent restore/baseline page used by explicit copy helpers such as `Copy_Off_To_Back_()` and `Copy_Back_To_Off_()`, not the next frame to present.
- The present path should therefore continue to treat `Off` as the source frame that is published to the main thread, with any presenter-owned upload or staging buffer remaining separate from gameplay page identity.
- For correctness, the publication step should snapshot the completed `Off` contents before legacy post-present refresh bookkeeping mutates `Off` for the next incremental frame.

### 5. Main screen composition must be complete on `Off` before publish

- The original main-screen flow renders the actual map frame to `Off`, not to `Back`.
- `Back` is populated afterward via `Copy_Off_To_Back_()` as a preserved baseline for later restore/stencil work.
- The main-screen present path therefore expects stars, fleets, links, labels, overlays, and UI data to already be present on `Off` before `Toggle_Pages_()` publishes.
- If SDL shows only background, nebulae, and UI, the likely fault is not the `Off`/`Back` page roles themselves but a mismatch in the `Draw_Main_Main_Screen_()` subtree or a page-target mismatch inside its draw primitives.

### 6. Windowed SDL mouse coordinates must be mapped from current window size into game space

- SDL mouse events are delivered in current window coordinates, not fixed 640x480 game coordinates.
- The active game input path still expects logical game-space coordinates.
- At a 640x480 window size, raw SDL coordinates happen to match the game space directly.
- At any other window size, raw SDL coordinates must be scaled into 640x480 game space or the in-game cursor will move too fast or too slow and fail to line up with the window edges.
- The correct mapping is edge-inclusive: the full current window width maps to `0..639` and the full current window height maps to `0..479`.
- Cursor warps issued by game code must apply the reverse mapping back into the current SDL window size.

### 7. Window-close behavior currently needs a platform-level fast exit

- The current in-game shutdown path can hang when triggered from an SDL window close request.
- Until the engine shutdown path is audited and fixed, the SDL close event should bypass normal game cleanup and exit the process directly with a status code.
- This is a temporary platform safeguard rather than a final engine-lifecycle design.

### 3. `Primary` is distinct from the two gameplay pages

- `Primary` exists for on-page/platform-facing output and some legacy direct-to-primary code paths.
- The normal page-selection APIs do not select `Primary`.
- The active software-render model should therefore be understood as:
  - two persistent gameplay pages: `Off` and `Back`
  - one separate main-thread/platform presentation target or upload path
- The old direct-primary compatibility layer has now been removed from the active SDL migration path.

### 4. `_copy_to_on_page_flag` appears inactive in the current branch

- The old `_copy_to_on_page_flag` branch was inactive in the SDL migration path and has been removed.
- Main animation and file-animation screen-background draws now always use the off-page path.
- The old on-page screen-background helper functions have been removed with the flag.

## Step-by-step implementation plan

### Step 1. Introduce SDL main-thread shell and keep the worker thread

Status: `completed`

- Reuse SDL window startup and shutdown from the working SDL branch.
- Keep `_beginthread(MOX2::thunk_main2_, ...)` or equivalent worker-thread startup.
- Move event pumping to the SDL main thread.
- Use a frame-paced SDL main loop that drains events with `SDL_PollEvent()` instead of relying on `SDL_WaitEvent()`.
- Remove dependency on Win32 `WndProc(...)` for keyboard, mouse, and flip dispatch.

Acceptance criteria:

- Main thread runs SDL event loop.
- Main thread does not require a per-frame wakeup event from the game thread.
- Game thread still runs legacy gameplay entry point.
- Basic keyboard/mouse input reaches the game.
- Windowed mouse input is scaled from current SDL window size into game-space coordinates correctly.

### Step 2. Add the in-memory surface compat layer

Status: `completed`

- Create a minimal page-buffer API backed by heap memory inside the existing `video.cpp` / `video.h` area.
- Keep it C-like and classless.
- Start with explicit stable buffers for:
  - `Off`
  - `Back`
  - cursor image storage
- Keep any separate main-thread present/upload storage in `platform.cpp` as a platform concern rather than making it part of the gameplay page model.
- Remove DirectDraw allocation dependency from the software page backend.
- Do not introduce new source files for this layer.

Acceptance criteria:

- No gameplay page allocation depends on DirectDraw.
- `Off` and `Back` remain stable memory-backed pages with persistent identity.

### Step 3. Rebind existing lock/unlock calls to memory-only behavior

Status: `completed`

- Make `Lock_Appropriate_Render_Buffer()` point directly at the selected page memory.
- Since the software page model remains tightly packed 8bpp memory, return linear row width for the active gameplay page.
- Make `video::_current_video_page` point directly at the selected page memory.
- Make unlock calls no-op or debug-only verification hooks.
- Do not block normal render writes to `Off`; the presenter consumes a separate published snapshot instead.
- Apply the same simplification to primary/alt/cursor lock helpers where they are still needed for legacy call compatibility.

Acceptance criteria:

- Existing drawing code can keep running unchanged against plain memory.
- No SDL surface lock or platform lock is required for normal pixel writes.

### Step 4. Convert page-copy and partial-refresh helpers to pure memory operations

Status: `completed`

- Rewrite:
  - `Copy_Off_To_Back_()`
  - `Copy_Back_To_Off_()`
  - `_Partial_Screen_Dump()`
  - `Stencil_Out_Background_()`
- Keep their existing semantics but implement them as memory copies.
- Keep scan-list and stencil bookkeeping intact until behavior is verified.
- Keep these software rendering operations in `video.cpp`, not in `platform.cpp`.

Acceptance criteria:

- `video::Toggle_Pages_()` no longer depends on DirectDraw blits or locks.
- Differential and stencil refresh paths still operate on in-memory pages.

### Step 5. Remove cursor rendering from the game render surface

Status: `completed`

- Keep cursor image generation from `video::_mouse_palette`.
- Stop using save-under cursor blits into primary/offscreen pages for final presentation.
- Keep only the data needed by the presenter:
  - current logical mouse position,
  - current cursor shape,
  - current cursor image buffer.
- Main-thread presentation now composites the active cursor directly over the presented world frame.
- The old game-surface cursor save/restore presentation path has been removed from the active platform layer.

Acceptance criteria:

- Cursor image exists as its own indexed compat surface.
- Game-thread render buffers no longer need final cursor compositing.

### Step 6. Add frame publication from game thread to main thread

Status: `in_progress`

- Add an explicit publish step at the end of `video::Toggle_Pages_()`.
- First implementation should publish the stable `Off` page only.
- Do not alternate publication between `Off` and `Back`; that would change the legacy meaning of the pages and break code that assumes `Back` remains a persistent baseline surface.
- Add published-buffer ownership state and a generation counter or equivalent metadata so the main thread can detect newly published content.
- Snapshot `Off` into a dedicated published buffer at the publication boundary.
- Replace the current spin-wait pattern with SDL mutex/condition synchronization used only when the game attempts to publish a newer snapshot while the presenter still owns the previous one.
- Prefer a single-producer/single-consumer ownership handoff with release/acquire semantics around publication metadata.
- Keep the software-side publication logic in `video.cpp`, with `platform.cpp` only consuming the published frame.
- Ensure the snapshot occurs before legacy stencil/refresh bookkeeping mutates `Off` for the next incremental frame.

Acceptance criteria:

- The main thread can always read a stable published frame.
- The main thread never samples live `Off` memory.
- The game thread continues rendering directly into `Off` without per-draw presentation waits.
- If the presenter is still using the previous published snapshot, the game thread waits only when it needs to publish a newer snapshot.
- Existing `Back` continuity behavior remains intact.
- The presenter consumes pixels derived from `Off`, typically by blitting or uploading `Off` into a main-thread-owned present buffer, rather than by reinterpreting `Back` as the next visible page.
- No busy-wait remains in the frame publication path.
- No per-frame SDL wakeup event is required in the normal active path.

### Step 7. Present published frames on the main thread

Status: `in_progress`

- Reuse the SDL texture upload / palette expansion path from the working SDL branch.
- Treat the most recently published world frame as the active frame.
- Upload a newer world frame only when publication metadata changes.
- Still present every main-thread tick, even if the active world frame did not change.
- Composite the cursor on the main thread after the world frame is prepared.
- Keep scaling and aspect behavior aligned with current SDL expectations.
- Keep all of this code in `platform.cpp`.

Acceptance criteria:

- World rendering appears stable.
- Cursor remains responsive even during periods where the game thread does not publish a newer world frame.
- Cursor is composited during present and no longer blinks due to game-thread save-under paths.

### Step 8. Remove the Win32 message flip trampoline

Status: `completed`

- Delete the `WM_USER + 1` presentation dispatch path.
- Delete the spin-wait in `platform::Toggle_Pages_Stub_()`.
- Make `Toggle_Pages_Stub_()` call directly into the in-memory toggle/publish path, or inline away the wrapper if practical.

Acceptance criteria:

- No page presentation depends on a window message callback.
- No busy-wait remains around frame publication.

### Step 9. Audit `setvesa`, legacy waits, and unused DirectDraw-era code

Status: `in_progress`

- Identify which helpers are still required for refresh bookkeeping.
- Remove or stub out hardware-flip and vertical-blank specific behavior that no longer applies.
- Keep behavior only where the game logic truly depends on it.
- Remove dead synchronization leftovers from the active path once publication no longer borrows live `Off`.
- Update: the unreferenced `_Partial_Screen_Dump()` and alt-offscreen compatibility chain have now been removed.
- Update: the remaining direct-primary compatibility layer has also been removed because it was no longer referenced by live callers.

Acceptance criteria:

- No dead DirectDraw-era wait path remains in the active SDL video path.
- Remaining `setvesa` usage is justified by real refresh bookkeeping or removed.

### Step 10. Cleanup pass

Status: `pending`

- Rename backend objects away from DirectDraw-specific terminology where practical.
- Remove no-longer-needed Win32 video code.
- Minimize remaining fake-surface API surface area.
- Keep compatibility wrappers only where they still reduce risk.
- Revisit the temporary fast-exit-on-window-close behavior once the engine shutdown path is known not to hang.
- Write-only compatibility state such as `DAT_00578e20` has been pruned where it no longer participates in live logic.
- Fixed-size compatibility globals such as `scaleX` / `scaleY` have been collapsed into constants where they were no longer mutable in practice.
- Active SDL cursor positioning and fullscreen sizing no longer rely on Win32 screen or cursor APIs.
- Remaining Win32 usage in `platform.cpp` is now limited primarily to handle/signature compatibility, with the SDL video path no longer depending on Win32 destruction fallbacks.

Acceptance criteria:

- Active SDL3 video path is conceptually simple: game thread writes memory, main thread presents memory.

## Risk areas to validate during implementation

- Correct ownership of palette updates between game thread and main thread.
- Whether some code paths implicitly assume `Toggle_Pages_()` blocks until an on-screen present is complete.
- Whether `Toggle_Pages_()` currently publishes too early relative to the legacy main-screen layering order, and whether it should be simplified later so publication always reflects the fully rendered `Off` page rather than an intermediate state.
- Whether any gameplay logic depends on the cursor having been drawn into the world surface rather than only composited at display time.
- Whether `_Masked_Screen_Dump()` or any old `screen_dump_flag` values are still reachable in real gameplay.
- Whether forced cursor warps must remain on the main thread for SDL correctness.
- Whether some partial-refresh code can be simplified further once cursor compositing leaves the game thread.

## Files most central to this migration

- `plan.md`
- `src/main.cpp`
- `src/game/platform.h`
- `src/game/platform.cpp`
- `src/game/video.h`
- `src/game/video.cpp`
- `src/game/mouse.h`
- `src/game/mouse.cpp`
- `src/game/key.h`
- `src/game/key.cpp`
- `src/game/mox2.cpp`

The preferred implementation shape is to concentrate the migration primarily into `platform.cpp`, `platform.h`, `video.cpp`, and `video.h`, copying code from the working SDL branch where useful instead of spreading new backend code across additional files.

## Final implementation target

The desired end state is simple:

- game thread renders only into in-memory 8-bit pages,
- `video::Toggle_Pages_()` stays in the game thread and publishes a stable completed frame,
- main thread handles SDL events continuously,
- main thread presents the latest published frame at display cadence,
- main thread composites the cursor from its own cursor surface,
- no SDL surface locking is used for normal legacy drawing,
- and no live render buffer is ever sampled by the presenter while the game thread is modifying it.