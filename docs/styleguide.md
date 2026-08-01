# Project code style guide

New code follows the conventions already established by the restored game source. Configuration runtime code, generated code, build tools, and tests follow the same conventions unless this document states a narrow exception.

## General style

- Write C-oriented C++20: plain structs, free functions, explicit state, namespaces, and straightforward control flow.
- Prefer data and operations with clear ownership over class hierarchies, fluent interfaces, metaprogramming, and framework-style abstractions.
- Keep implementation details in `.cpp` files. Public headers expose only the declarations required by their consumers.
- Use fixed-width integer types when size matters.
- Pass optional or mutable objects by pointer. Use `const` pointers for read-only input. Use references only where existing APIs or required standard-library interfaces make them clearer.
- Return status explicitly and write detailed results or diagnostics through output parameters when that matches surrounding code.
- Do not add exceptions or RTTI-dependent designs to project-owned code. Report parse, validation, generation, and I/O failures explicitly.
- Comments explain constraints or non-obvious reasoning; they do not narrate the code.

## Names

- Functions use `Pascal_Case_With_Underscores`, for example `Parse_Config_File` and `Generate_Config_Bindings`.
- Struct types use the existing `s_lower_case_with_underscores` form.
- Enum types use the existing `e_lower_case_with_underscores` form.
- Type aliases use the existing `t_lower_case_with_underscores` form.
- Local variables, parameters, data members, and namespace-scope data use `lower_case_with_underscores`.
- Constants and enum values follow the convention of the owning subsystem; new configuration code should use an uppercase subsystem prefix when values enter a shared namespace.
- File-local helper functions append `_`, for example `Parse_Number_`.
- Namespaces use the owning subsystem name. Configuration code uses `config`; build-tool-only code uses `config::tool`.
- Source filenames use `lower_case_with_underscores.cpp` and `.h`.
- Avoid abbreviations unless they are established game or protocol terms.

The JSON5 schema continues to use stable external `lower_snake_case` names because those names are user-facing configuration paths. The generator converts them deterministically to project-style C++ identifiers without changing the external spelling.

## C-with-namespaces organization

- Group subsystem declarations in a namespace instead of creating a static utility class or singleton class.
- Represent state with plain `s_` structs and operate on it with free functions.
- Prefer explicit initialization and shutdown functions when state owns resources.
- Keep native configuration state owned by the relevant gameplay subsystem. Generated bindings reference that state; they do not create an unrelated global object model.
- Avoid inheritance and virtual dispatch unless an existing subsystem requires them.
- Avoid operator overloading, implicit conversion APIs, and hidden global registration.

## Templates and standard-library types

Project-owned template declarations are forbidden. Do not add:

- `template <...>` functions, classes, aliases, or variables
- generic metaprogramming or concepts
- template-based dispatch or serialization frameworks
- custom allocators or template wrappers around standard containers

Direct use of selected standard-library template instantiations is allowed where dynamic parser or generator data requires them:

- `std::string` for owned text
- `std::string_view` for non-owning text whose lifetime is explicit
- `std::vector<T>` for declaration-ordered sequences and dynamic buffers
- `std::map<K, V>` only when sorted-key behavior is intentionally required
- `std::unordered_map<K, V>` only for lookup indexes whose iteration order is never observable
- `std::unique_ptr<T>` for unambiguous single ownership when a value cannot be stored directly

Use containers directly rather than hiding them behind project-owned template aliases. Do not use a map as the authoritative representation of a JSON5 object because schema member declaration order is significant. Store object members in a vector and build a separate lookup index only when measurements justify it.

Additional standard-library types require review. Prefer ordinary structs with an explicit kind enum over `std::variant`, and pointer-plus-count or vectors over ranges and generic algorithms when that keeps control flow clearer.

Third-party implementation code may internally use templates, but it must be isolated under `vendors/`, must not impose template-heavy types on project-owned public APIs, and must satisfy the dependency review described below.

## JSON5 reader

The configuration tools use vendored yyjson as their JSON5 reader rather than `mortie/json5cpp` or a project-owned parser.

yyjson is a small MIT-licensed ANSI C library distributed as one `.c` and one `.h` file. It has no transitive dependency, preserves object member order, validates UTF-8 by default, supports JSON5 through explicit read flags, and fits the project's C-with-namespaces style better than a template-based C++ JSON library. Pin an exact released revision under `vendors/` and compile it as a private C library used only by `config-tool`.

`mortie/json5cpp` is not selected because its single header depends on JsonCpp and exposes `Json::Value`, adding a second dependency and template-heavy C++ API without providing better schema diagnostics.

Project-owned adapter code lives under `tools/config/` because JSON5 is consumed only while validating and generating checked-in files. It follows these constraints:

- parse with the explicit yyjson JSON5 flags required by the accepted project syntax
- copy yyjson's ordered object members into the normalized schema model without sorting
- convert yyjson parse-error byte positions to file, line, and column with `yyjson_locate_pos`
- report successful semantic validation errors with file and canonical schema path; yyjson does not retain source offsets for successful values
- enforce a project nesting limit while converting the yyjson document
- reject duplicate object keys while iterating every object
- leave invalid-Unicode support disabled so yyjson validates UTF-8
- reject non-finite numbers even though the full `YYJSON_READ_JSON5` convenience flag permits them; combine only the approved individual flags
- produce deterministic diagnostics without throwing exceptions
- have no dependency on runtime game code
- free the yyjson document after conversion or validation; never expose `yyjson_val` in generated or runtime APIs
- be covered by focused unit tests and yyjson/JSON5 conformance fixtures relevant to the accepted subset

The adapter is not a general serialization framework and does not use yyjson's writer. Generated documentation and C++ output use dedicated emitters.

## Generated code

- Generated C++ follows this style guide and should resemble maintained project code.
- Generated files contain a short generated-file notice and identify `specs/config/main.json5` as their source.
- Generated files use deterministic order and formatting.
- Generated files are never edited by hand.
- Generated APIs expose concrete structs and functions, not templates.
- Native builds with configuration tooling available regenerate checked-in output when the canonical schema or generator changes. Cross builds and builds without the tool dependency compile the checked-in output directly.

## Dependency policy

Before adding a C or C++ dependency, verify its license, maintenance status, supported targets, diagnostics, security limits, and transitive dependencies. Prefer a small source dependency with a C-compatible boundary over a header-only C++ library that exports template-heavy types. Vendor accepted dependencies consistently with the repository's existing dependency policy and pin an exact revision.

Do not add a dependency when a small purpose-built component is required anyway to preserve source locations, declaration order, deterministic output, or project-specific diagnostics.

## Tests

- Use the repository's existing doctest setup for C++ unit tests.
- Test public behavior and diagnostics rather than private implementation details.
- Use table-driven data stored in ordinary structs and vectors; do not introduce project-owned test templates.
- Keep parser, schema validation, generation, and runtime configuration tests separate so failures identify the responsible phase.
- Include limits and malformed-input cases for every parser entry point.