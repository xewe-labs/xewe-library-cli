# XeWeCli documentation

Complete reference for `xewe::Cli`. The [README](../README.md) is the short version: what the
library is and a minimal sketch.

**[AGENTS.md](AGENTS.md) — read this first if you are a coding agent.** It applies to the whole
repository, not just this folder.

| Page | Covers |
|---|---|
| [commands.md](commands.md) | `Command`, `CommandGroup`, the constructor, `loop`, `add_group`, `add_command`, `remove_group`, `get_group`, `get_groups` |
| [execution.md](execution.md) | both `execute` overloads, the dispatch order, every error string, the tokenizer |
| [help.md](help.md) | `print_help`, `print_all_commands` and the generated `$help` tables |

```cpp
#include <XeWeCli.h>
```

Commands are typed as `$<group> <command> [args...]`. `$help` lists every group; `$help <group>`,
`$<group>` and `$<group> help` all list one.

## Things that surprise people

* **Never name the object `cli`.** The ESP32 core defines `cli` as a function-like macro; XeWe
  code uses `xewe_cli`.
* `execute(line)` returns `void` and prints its errors; `execute(group, cmd, args)` returns `bool`
  and prints nothing.
* The direct overload matches on **name and argument count**; the parsed path matches on name
  alone and takes the first hit.
* Group ids and command names are matched case-insensitively; ids are trimmed and lowercased when
  registered.
* The `std::span` handed to a handler **does not outlive the call**.
* `$help` skips groups that have no commands.
* `add_command` does not reject duplicate names.

## Dependencies

[XeWeSerial](https://github.com/xewe-labs/xewe-library-serial) and
[XeWeUtils](https://github.com/xewe-labs/xewe-library-utils). Used by
[XeWeOS](https://github.com/xewe-labs/xewe-library-os), where every module gets a `$<id>` group.

See [`examples/BasicCli`](../examples/BasicCli) for a runnable sketch.
