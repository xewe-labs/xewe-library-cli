# AGENTS.md — xewe-library-cli

Rules for coding agents working **anywhere in this repository**, not only in `doc/`.
Organization-wide rules are in
[`.github/AGENTS.md`](https://github.com/xewe-labs/.github/blob/main/AGENTS.md) and win where this
file is silent: never publish, never tag or push, never commit unasked, never flash a board.

## This library

* **`src/` has exactly one top-level header,** `src/XeWeCli.h`. Everything else lives in
  `src/Cli/`. Arduino puts every library's `src/` on the include path, so a second top-level
  header collides with other libraries.
* **XeWeSerial and XeWeUtils are included only through their entry headers**
  (`#include <XeWeSerial.h>`). Every library added to an `#include` must be declared in
  `library.properties` → `depends=`.
* **This library must not include `XeWeOS.h`.** It works standalone; XeWe OS depends on it.

## Do not break these

* **Never name a `Cli` object `cli`.** The ESP32 Arduino core defines `cli` as a function-like
  macro, so `xewe::Cli cli(serial);` does not compile. Examples and docs use `xewe_cli`; keep it
  that way in every snippet you write.
* **The handler's `xewe::span` is a view into a local vector.** Do not change `execute` to hand out
  something that looks storable, and do not "fix" a handler by keeping the span — copy the
  strings.
* **The two `execute` overloads match differently on purpose:** the parsed path on name only, the
  direct path on name *and* `arg_count`. Unifying them changes which handler runs for existing
  firmware.
* **`$<group>`, `$<group> help` and `$help <group>` all print that group's help.** All three are
  used in device docs and example comments.
* **Error strings are part of the interface.** They are documented verbatim in
  [`doc/execution.md`](execution.md) and users grep for them; change one and update the page in
  the same commit.
* **Argument count is checked before the handler runs.** Handlers index `args[0]` without
  bounds-checking because of that guarantee.
* **`add_group` on an existing id must keep its commands.** XeWe OS modules rely on it when a
  group is touched twice.
* This library defines **no** `DEBUG_` flag. If you add one, default it to `0` behind
  `#ifndef DEBUG_<Class>` and document it.

## When changing this library

* `library.json` is **generated** from `library.properties`
  (`python3 publish.py manifest` in
  [`publish-arduino-library`](https://github.com/xewe-labs/publish-arduino-library)). Never
  hand-edit it; `check` fails when it is stale.
* Versions are **lockstep** across all XeWe libraries. Never bump this one alone.
* Source files start with the SPDX header from
  [`.github/guidelines/license-header.txt`](https://github.com/xewe-labs/.github/blob/main/guidelines/license-header.txt).
  Markdown files do not.
* **Documentation is part of the change.** A new or changed public function updates its page in
  `doc/` in the same breath — this reference is written to be exhaustive, so a gap is a bug.
* Check your work without publishing anything:

  ```bash
  python3 publish-arduino-library/publish.py check xewe-library-cli
  ```
