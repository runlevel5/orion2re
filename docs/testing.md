# Testing strategy notes

## Goals

This document describes the recommended testing foundation for the current project.

The codebase is built as C++ with CMake, but much of the code behaves like legacy C-style code with namespaces, globals, and broad cross-file coupling. That means a conventional modern unit-testing setup based on clean library boundaries is not a realistic starting assumption.

The testing plan should therefore:

- work with the current CMake build
- require minimal architectural change up front
- support incremental adoption on tightly coupled legacy code
- allow narrow test seams and fakes where platform code blocks direct testing
- remain compatible with future refactoring into cleaner internal libraries

The strategy intentionally favors a practical first step over an idealized design.

## Recommended foundation

## Test orchestration: CTest

Use CMake's built-in `CTest` support as the top-level test runner.

### Why

- already fits the existing build system
- works well in local development and CI
- provides standard pass/fail reporting
- does not impose any particular test framework on the code

### Expected usage

- `include(CTest)` in the top-level `CMakeLists.txt`
- `add_test(...)` for each test executable
- `ctest --output-on-failure` as the standard invocation

## Test framework: doctest

Use `doctest` as the initial C++ test framework.

### Why

- very small dependency footprint
- easy to vendor directly into the repository
- fast compile times relative to heavier frameworks
- simple fit for C-style code compiled as C++
- no code generation or special build integration required

### What "lightweight C++ runner" means here

A lightweight runner is a framework that provides:

- a tiny registration mechanism for test cases
- assertions such as `CHECK(...)` and `REQUIRE(...)`
- one minimal `main()` entry point to execute all tests
- text output and exit codes that CTest can consume

For `doctest`, the runner can be as small as:

```cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
```

In this repository, the header should be provided by the pinned submodule at `vendors/doctest`.

That single translation unit is enough to produce a working test executable once linked with the chosen test sources.

## Key design decision

## Do not require a library-first architecture

The project should not assume that testable code can be cleanly moved into reusable libraries before tests exist.

### Why

- much of the code appears tightly intertwined
- many source files depend on platform code, globals, or broad shared state
- forcing a clean separation first would likely stall test adoption

### Expected result

The first testing phase should build test executables from hand-picked production `.cpp` files, plus narrow support files, rather than from a fully factored internal library graph.

This means tests are allowed to compile selected production implementation files directly into the test binary.

## Preferred build model

## One executable per file under test

Use separate test executables for each production file or tightly related file cluster under test.

Each test executable should contain:

- one shared `doctest` runner source file
- one primary unit test source file under `tests/unit/`
- only the production `.cpp` files needed for that test target
- any reusable fake sources needed for linkage or assertions

### Why

- improves isolation between unrelated modules
- keeps source lists and fake dependencies explicit
- makes it easier to mock or fake only the local seam
- avoids one large legacy test binary with shared mutable state
- scales better as more coupled modules become testable

### Directory shape

```text
tests/
  test_main.cpp
  unit/
    test_fields.cpp
    test_fields_helpers.cpp
    test_lbx.cpp
    test_sound.cpp
    test_colony.cpp
  fakes/
    fake_mmio.cpp
    fake_sound.cpp
    fake_video.cpp
    fake_memory.cpp
```

Not every listed example file must exist immediately. The structure shows the intended organization.

### CMake organization

Keep test-target definitions under `cmake/tests/` and include them from the top-level `CMakeLists.txt` only when tests are enabled.

This keeps the main build focused on production targets while allowing test-only helpers and target factories to live in their own CMake files.

## Testable code selection strategy

The first source files selected for testing should be chosen pragmatically.

## Good first candidates

Prefer code that is:

- deterministic
- mostly pure or close to pure
- algorithmic rather than event-driven
- narrow in dependencies
- useful enough to justify a stable test harness

Examples include:

- math helpers
- bit or byte stream utilities
- string formatting and parsing helpers
- buffer or serialization helpers
- small container-like or utility modules
- save or load helpers that can operate on in-memory data

## Poor first candidates

Avoid starting with code that depends heavily on:

- SDL windowing, input, or audio
- installed game assets
- rendering state
- platform threads or timing
- broad mutable global state with hard-to-recreate initialization

These areas may still become testable later, but they are poor entry points for the initial foundation.

## Test seam strategy

Tightly coupled legacy code often becomes testable with narrow seams rather than large redesigns.

## Preferred seam types

Use the smallest workable mechanism:

- link-time stubs for external functions
- thin wrapper functions around platform services
- compile-time seams guarded by local test-only defines when necessary
- minimal test initialization of required global state

### Link-time fakes

If a production file calls external functions that are irrelevant to the behavior under test, provide fake definitions in reusable test support source files.

Example shape:

```cpp
int16_t fake_pointer_x;

namespace mouse {
    int16_t Pointer_X_() {
        return fake_pointer_x;
    }
}
```

These fakes may also expose small helper state so tests can control return values and inspect calls.

### Wrapper seams

Where practical, introduce very small wrappers around platform APIs so tests can replace only that wrapper boundary.

This should be preferred over spreading preprocessor conditionals across many production files.

### Preprocessor seams

Test-only compile-time seams are acceptable if they are localized, small, and clearly named.

They should be treated as a fallback, not as the main architecture.

## Test style guidance

## Prefer characterization tests first

For legacy gameplay code, the first tests often should not try to prove that the implementation is elegant or ideal.

They should document and lock in actual current behavior for controlled inputs.

### Why

- reduces fear of changing brittle code
- makes later refactoring safer
- exposes hidden assumptions in edge cases

### Expected style

- feed deterministic inputs
- capture outputs or state transitions
- avoid depending on timing or environment
- name the scenario after observed behavior, not intended architecture

## Scope tests narrowly

Each early test should target one behavior, one file cluster, or one utility path.

Avoid building large integration scenarios into the very first test harness.

## Build-system integration plan

## Phase 1: minimal test enablement

- add `include(CTest)`
- add `ORION2RE_BUILD_TESTS` option
- vendor `doctest`
- add one test runner source file
- add a shared unit-test target helper under `cmake/tests/`
- add the first per-file test executables
- register each one with `add_test`

This phase establishes the tooling without forcing broad source movement.

## Phase 2: subset testing of selected modules

- add a few test files for selected modules under `tests/unit/`
- compile required production `.cpp` files directly into each test target
- add reusable fakes under `tests/fakes/` where linkage requires them
- run tests under normal Debug builds

This phase proves the viability of the approach on real code.

## Phase 3: sanitizer-backed debug coverage

- run the test executable in Debug builds
- pair tests with AddressSanitizer-enabled builds where available
- use `ctest --output-on-failure` as the standard command

This phase increases defect-finding value without changing test structure.

## Phase 4: opportunistic library extraction

Only after the subset-build pattern identifies stable clusters should the project consider introducing internal libraries such as:

- `orion2re_core`
- smaller utility libraries for genuinely shared testable code

This is an optimization and cleanup phase, not a prerequisite.

## Future evolution

If some areas naturally separate over time, tests should migrate to link those internal libraries instead of compiling the same source lists repeatedly.

However, the project should not block test adoption on that future cleanup.

## Interaction with networking work

This testing strategy should be kept in mind when planning future networking changes.

## Implications for networking redesign

- new networking code should prefer explicit seams around transport and runtime services
- router protocol parsing and framing logic should be designed for deterministic tests from the start
- control-plane behavior should be testable independently of SDL integration
- packet framing, routing metadata, and serialization code should be treated as prime candidates for early tests

The new networking work should avoid deepening the current coupling where practical.

In particular, new networking modules should prefer:

- narrow interfaces around socket or event-loop operations
- testable parsing and encoding helpers
- deterministic state machines for session and connection handling
- isolation between transport runtime code and gameplay packet interpretation

That will make the networking redesign easier to validate than the current legacy subsystems.

## Summary of the preferred approach

The recommended testing foundation is:

- CTest as the top-level test runner
- `doctest` as the lightweight C++ framework
- separate executables per file under test
- direct compilation of selected production `.cpp` files into each test target
- reusable hand-written fakes to bypass platform dependencies
- characterization tests for legacy behavior
- test CMake files isolated under `cmake/tests/`
- gradual expansion, with internal libraries introduced only when they become naturally useful

This approach matches the current state of the codebase better than a mandatory library-first test design. It gives the project a realistic path to useful automated tests now while still leaving room for cleaner target structure later.