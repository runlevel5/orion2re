* Stability
  * [ ] Long running game seems to leak memory somewhere

* Code cleanups

  * [ ] Add helpers to work with pops instead of using raw bit operations

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
