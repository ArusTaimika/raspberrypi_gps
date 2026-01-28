#ifndef GPIO_PULSE_GENERATOR_HPP
#define GPIO_PULSE_GENERATOR_HPP

#include <gpiod.h>
#include <string>
#include <chrono>
#include <thread>

class GpioPulseGenerator {
public:
    GpioPulseGenerator(const std::string &chip_name, unsigned int line_num, int pulse_duration_ms, int period_ms);
    ~GpioPulseGenerator();

    // パルス生成を開始するメソッド
    void start();

private:
    std::string chip_name_;
    unsigned int line_num_;
    int pulse_duration_ms_;
    int period_ms_;
    gpiod_chip *chip_;
    gpiod_line *line_;
};

#endif // GPIO_PULSE_GENERATOR_HPP
