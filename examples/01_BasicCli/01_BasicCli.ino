// XeWeCli (low): a serial CLI in a few lines
// Try: $help   $led on   $led blink 5
#include <XeWeCli.h>

xewe::SerialPort serial;
xewe::Cli        xewe_cli(serial);

#define LED_PIN 8   // onboard on most C3/C6/S3 dev boards; change for yours

void setup() {
    serial.begin();
    pinMode(LED_PIN, OUTPUT);

    xewe_cli.add_group("led", "LED");

    xewe_cli.add_command("led", {"on", "Turn the LED on", "$led on", 0,
        [](std::span<const std::string>) {
            digitalWrite(LED_PIN, HIGH);
            serial.print("LED on");
        }});

    xewe_cli.add_command("led", {"off", "Turn the LED off", "$led off", 0,
        [](std::span<const std::string>) {
            digitalWrite(LED_PIN, LOW);
            serial.print("LED off");
        }});

    xewe_cli.add_command("led", {"blink", "Blink N times", "$led blink 3", 1,
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
    xewe_cli.loop();   // polls serial and executes complete lines
}
