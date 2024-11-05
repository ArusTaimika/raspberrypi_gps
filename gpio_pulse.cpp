#include "gpio_pulse.hpp"
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
    // GPIOをHIGHに設定
    gpiod_line_set_value(line_, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(pulse_duration_ms_));

    // GPIOをLOWに設定
    gpiod_line_set_value(line_, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(period_ms_ - pulse_duration_ms_));
    
}

int main() {
    try {
        GpioPulseGenerator pulseGenerator("gpiochip0", 16, 500, 1000);

        // 前回のパルス出力時間を記録する変数
        auto last_pulse_time = std::chrono::system_clock::now();

        while (true) {
            // 現在の時間を取得
            auto now = std::chrono::system_clock::now();
            auto time_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

            // システムクロックが整数秒（1.00秒）に一致した場合にパルス出力
            if (time_since_epoch % 1 == 0 && now - last_pulse_time >= std::chrono::seconds(1)) {
                pulseGenerator.start();  // パルス出力
                last_pulse_time = now;
            }

            // 余分なCPU消費を防ぐため少し待機
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}