// XeWeCommandExecutor: a serial CLI in a few lines
// Try: $help   $led on   $led blink 5
#include <XeWeCommandExecutor.h>

xewe::SerialPort      serial;
xewe::CommandExecutor cli(serial);

const int LED_PIN = 8;

void setup() {
    serial.begin();
    pinMode(LED_PIN, OUTPUT);

    cli.add_group("led", "LED");

    cli.add_command("led", {"on", "Turn the LED on", "$led on", 0,
        [](std::span<const std::string>) {
            digitalWrite(LED_PIN, HIGH);
            serial.print("LED on");
        }});

    cli.add_command("led", {"off", "Turn the LED off", "$led off", 0,
        [](std::span<const std::string>) {
            digitalWrite(LED_PIN, LOW);
            serial.print("LED off");
        }});

    cli.add_command("led", {"blink", "Blink N times", "$led blink 3", 1,
        [](std::span<const std::string> args) {
            int count = atoi(args[0].c_str());
            for (int i = 0; i < count; ++i) {
                digitalWrite(LED_PIN, HIGH); delay(150);
                digitalWrite(LED_PIN, LOW);  delay(150);
            }
        }});

    serial.print("Type $help");
}

void loop() {
    cli.loop();   // polls serial and executes complete lines
}
