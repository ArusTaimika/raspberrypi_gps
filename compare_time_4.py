import time
import csv
import RPi.GPIO as GPIO

from concurrent.futures import ProcessPoolExecutor

from offset_log import Get_gps
from udp_connect import Udp_connect



class Compare_time:
    def __init__(self) -> None:
        
        # GPIOピンの設定
        GPIO.setmode(GPIO.BCM)
        self.pulse_pin = 16  # パルス出力用のピン番号（適宜変更）
        GPIO.setup(self.pulse_pin, GPIO.OUT)

    def client(self, ip, port, data_pieces):
        #初期化
        udp_connect = Udp_connect(ip, port, data_pieces)

        try:
            while True:                
                udp_connect.udp_client(data = [time.time(),0.1]);GPIO.output(self.pulse_pin, GPIO.HIGH)
                time.sleep(0.5)
                GPIO.output(self.pulse_pin, GPIO.LOW)
        except KeyboardInterrupt:
            print("ーーーーーーーーー終了ーーーーーーーーーーーー")
        finally:
            udp_connect.udp_close()


    def server(self, port, data_pieces):
        # 初期化
        ip = "0.0.0.0"
        udp_connect = Udp_connect(ip, port, data_pieces)

        # CSVファイルを開く/作成
        with open('delay_time.csv', mode='w', newline='') as file:
            writer = csv.writer(file)
            # CSVのヘッダーを書き込む
            writer.writerow(['Server System Time', 'delay Time[s]'])

            try:
                while True:
                    #UDP受信
                    recieved_data = udp_connect.udp_server(); server_system_time  = time.clock_gettime_ns(time.CLOCK_REALTIME);GPIO.output(self.pulse_pin, GPIO.HIGH)
                    delay_time = recieved_data[0] - server_system_time #通信遅延計算

                    # CSVに書き込む
                    writer.writerow([server_system_time, delay_time])
                    GPIO.output(self.pulse_pin, GPIO.LOW)
                    print(f"時間遅延：{delay_time}")

            except KeyboardInterrupt:
                print("ーーーーーーーーー終了ーーーーーーーーーーーー")
            finally:
                udp_connect.udp_close()



if __name__ == "__main__":

    compare_time_client_1 = Compare_time()
    compare_time_client_2 = Compare_time()
    with ProcessPoolExecutor(max_workers=2) as executor:
        # 並行して関数を実行
        executor.submit(compare_time_client_1.client, ip = "192.168.132.144",port = 22222, data_pieces = 2)#b
        executor.submit(compare_time_client_2.client, ip = "100.66.251.116",port = 11111, data_pieces = 2)#a
    


    #server用
    #compare_time.server(port = 22222, data_pieces = 2)

    #client用
    #compare_time.client(ip = "192.168.132.144",port = 22222, data_pieces = 2)