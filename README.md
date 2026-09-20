# XeWeCli

> Full reference: [`doc/`](doc/) · Agent rules: [`doc/AGENTS.md`](doc/AGENTS.md)

A serial command line. Commands are grouped and typed as
`$<group> <command> [args...]`; quoted arguments may contain spaces.


Supported cores: ESP32, ESP8266, RP2040, Renesas (Uno R4) and the Arduino mbed cores —
anything whose toolchain provides C++17 and a C++ standard library. 8-bit AVR (Uno R3, Nano,
Nano Every) is **not** supported: avr-gcc ships no `<string>`, `<vector>` or `<string_view>`.
Only ESP32-C3/C6/S3 are compile-verified on hardware; the rest are verified at the language
level by the host portability check in `publish-arduino-library`.

```cpp
#include <XeWeCli.h>

xewe::SerialPort serial;
xewe::Cli        xewe_cli(serial);

void setup() {
    serial.begin();
    xewe_cli.add_group("led", "LED");
    xewe_cli.add_command("led", {"set", "Set level 0-255", "$led set 128", 1,
        [](xewe::span<const std::string> args) { analogWrite(8, atoi(args[0].c_str())); }});
}

void loop() {
    xewe_cli.loop();
}
```

* `$help` prints every group; `$help <group>` or `$<group>` prints one.
* Argument counts are checked before a command runs; the sample usage is shown on mismatch.
* `execute("$led set 10")` runs a line from code (buttons, schedules, web requests);
  `execute("led", "set", args)` skips parsing.

Depends on XeWeSerial and XeWeUtils. Three examples in [`examples/`](examples/):
`01_BasicCli`, `02_GroupsAndArgs`, `03_AutomationCli`.
