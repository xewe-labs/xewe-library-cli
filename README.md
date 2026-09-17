# XeWeCli

> Full reference: [`doc/`](doc/) · Agent rules: [`doc/AGENTS.md`](doc/AGENTS.md)

A serial command line for ESP32. Commands are grouped and typed as
`$<group> <command> [args...]`; quoted arguments may contain spaces.

```cpp
#include <XeWeCli.h>

xewe::SerialPort serial;
xewe::Cli        xewe_cli(serial);

void setup() {
    serial.begin();
    xewe_cli.add_group("led", "LED");
    xewe_cli.add_command("led", {"set", "Set level 0-255", "$led set 128", 1,
        [](std::span<const std::string> args) { analogWrite(8, atoi(args[0].c_str())); }});
}

void loop() {
    xewe_cli.loop();
}
```

* `$help` prints every group; `$help <group>` or `$<group>` prints one.
* Argument counts are checked before a command runs; the sample usage is shown on mismatch.
* `execute("$led set 10")` runs a line from code (buttons, schedules, web requests);
  `execute("led", "set", args)` skips parsing.

Depends on XeWeSerial and XeWeUtils. See `examples/BasicCli`.
