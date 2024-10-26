#include <iostream>
#include <gpiod.h>
#include <chrono>
#include <thread>

constexpr char CHIP_NAME[] = "gpiochip0";  // GPIOチップの名前（環境により異なる）
constexpr unsigned int LINE_NUM = 16;      // GPIOピン番号
constexpr int PULSE_DURATION_MS = 500;     // パルスのON時間（ミリ秒）
constexpr int PERIOD_MS = 1000;            // パルス周期（ミリ秒）

int main() {
    gpiod_chip *chip = gpiod_chip_open_by_name(CHIP_NAME);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip" << std::endl;
        return 1;
    }

    gpiod_line *line = gpiod_chip_get_line(chip, LINE_NUM);
    if (!line) {
        std::cerr << "Failed to get GPIO line" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_output(line, "pulse-generator", 0) < 0) {
        std::cerr << "Failed to set GPIO line as output" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    while (true) {
        // GPIOをHIGHに設定
        gpiod_line_set_value(line, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(PULSE_DURATION_MS));

        // GPIOをLOWに設定
        gpiod_line_set_value(line, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(PERIOD_MS - PULSE_DURATION_MS));
    }

    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
