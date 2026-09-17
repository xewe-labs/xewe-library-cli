// XeWeCli (high): commands driven from code, not just from typing.
// Uses everything this library depends on — XeWeSerial for the console and
// XeWeUtils for AsyncTimer and validate.
//
// Try: $help
//      $fade to 255 2000     <- non-blocking: the CLI stays responsive
//      $fade stop
//      $macro run            <- one command executing several others
//      $led on
#include <XeWeCli.h>

#define LED_PIN 8   // onboard on most C3/C6/S3 dev boards; change for yours

xewe::SerialPort    serial;
xewe::Cli           xewe_cli(serial);

// AsyncTimer is in the global namespace, not xewe::
AsyncTimer<uint8_t> fade(1000, 0, 0);
uint8_t             level = 0;

static void set_level(uint8_t value) {
    level = value;
    analogWrite(LED_PIN, level);
}

void setup() {
    serial.begin();
    pinMode(LED_PIN, OUTPUT);

    xewe_cli.add_group("led",   "LED");
    xewe_cli.add_group("fade",  "Fade");
    xewe_cli.add_group("macro", "Macros");

    xewe_cli.add_command("led", {"on", "Full brightness", "$led on", 0,
        [](std::span<const std::string>) { fade.terminate(); set_level(255); serial.print("on"); }});

    xewe_cli.add_command("led", {"off", "Off", "$led off", 0,
        [](std::span<const std::string>) { fade.terminate(); set_level(0); serial.print("off"); }});

    xewe_cli.add_command("fade", {"to", "Fade to a level over N ms", "$fade to 255 2000", 2,
        [](std::span<const std::string> args) {
            auto target = xewe::validate<uint8_t>(args[0], 0, 255);
            auto ms     = xewe::validate<uint32_t>(args[1], 50, 60000);
            if (!target || !ms) {
                serial.print("Usage: $fade to <0-255> <50-60000>");
                return;
            }
            fade.reset(*ms, level, *target);   // reset() clears `initiated`...
            fade.initiate();                   // ...so start it again
            serial.printf("fading %u -> %u over %lu ms", level, *target, *ms);
        }});

    xewe_cli.add_command("fade", {"stop", "Stop where it is", "$fade stop", 0,
        [](std::span<const std::string>) { fade.terminate(); serial.printf("stopped at %u", level); }});

    // A macro runs other commands by their typed form. This is the same path a
    // button handler, a schedule or a web request would use.
    xewe_cli.add_command("macro", {"run", "Run a canned sequence", "$macro run", 0,
        [](std::span<const std::string>) {
            xewe_cli.execute("$led off");           // parsed overload: void, prints its own errors
            xewe_cli.execute("$fade to 200 1500");
        }});

    serial.print("Type $help");
}

void loop() {
    xewe_cli.loop();

    // Non-blocking: the fade advances a step per iteration, so commands still
    // land while it runs. get_current_value() is what recomputes progress.
    if (fade.is_active()) {
        set_level(fade.get_current_value());
        if (fade.is_done()) {
            fade.terminate();
            serial.printf("fade done at %u", level);
        }
    }

    // A simulated button, every 30 s. The three-argument overload skips parsing
    // and returns whether a command ran; it prints nothing on failure.
    static uint32_t last = 0;
    if (millis() - last > 30000) {
        last = millis();
        const std::vector<std::string> no_args;
        if (!xewe_cli.execute("led", "on", no_args)) {
            serial.print("no $led on command registered");
        }
    }
}
