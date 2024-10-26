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
    }#include "gpio_pulse.hpp"
#include <iostream>

GpioPulseGenerator::GpioPulseGenerator(const std::string &chip_name, unsigned int line_num, int pulse_duration_ms, int period_ms)
    : chip_name_(chip_name), line_num_(line_num), pulse_duration_ms_(pulse_duration_ms), period_ms_(period_ms), chip_(nullptr), line_(nullptr) {

    // GPIOチップを開く
    chip_ = gpiod_chip_open_by_name(chip_name_.c_str());
    if (!chip_) {
        std::cerr << "Failed to open GPIO chip" << std::endl;
        throw std::runtime_error("Failed to open GPIO chip");
    }

    // GPIOラインを取得
    line_ = gpiod_chip_get_line(chip_, line_num_);
    if (!line_) {
        std::cerr << "Failed to get GPIO line" << std::endl;
        gpiod_chip_close(chip_);
        throw std::runtime_error("Failed to get GPIO line");
    }

    // GPIOラインを出力としてリクエスト
    if (gpiod_line_request_output(line_, "pulse-generator", 0) < 0) {
        std::cerr << "Failed to set GPIO line as output" << std::endl;
        gpiod_chip_close(chip_);
        throw std::runtime_error("Failed to set GPIO line as output");
    }
}

GpioPulseGenerator::~GpioPulseGenerator() {
    if (line_) gpiod_line_release(line_);
    if (chip_) gpiod_chip_close(chip_);
}

void GpioPulseGenerator::start() {
    while (true) {
        // GPIOをHIGHに設定
        gpiod_line_set_value(line_, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(pulse_duration_ms_));

        // GPIOをLOWに設定
        gpiod_line_set_value(line_, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(period_ms_ - pulse_duration_ms_));
    }
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
