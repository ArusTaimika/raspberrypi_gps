import RPi.GPIO as GPIO
import time

# GPIOピンの設定
GPIO.setmode(GPIO.BCM)
pulse_pin = 16  # パルス出力用のピン番号（適宜変更）
GPIO.setup(pulse_pin, GPIO.OUT)

def output_pulse(duration=0.1):
    """指定した時間だけパルスを出力"""
    GPIO.output(pulse_pin, GPIO.HIGH)
    time.sleep(duration)
    GPIO.output(pulse_pin, GPIO.LOW)
try:
    while True:

        output_pulse(duration=0.1)  # 0.1秒間パルスを出力
        time.sleep(1)  # 定期的にチェック
        print("！！！！！！！！出力！！！！！！！！！")
finally:
    GPIO.cleanup()  # 終了時にGPIOの設定をクリア
