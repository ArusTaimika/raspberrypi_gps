import subprocess
import csv
from datetime import datetime
import time
import gpsd

class Get_gps:
    """_summary_
    GPS時間を取得するクラス
    参考:https://www.notion.so/GNSS-10c47abc426580698d0bdc79cfbd8a20?pvs=4
    """
    def __init__(self):
        # GPSサーバーに接続
        gpsd.connect()
        self.offset = None
        self.frequency = None
        self.frequency_porm = None

    def get_gpstime(self):
        """_summary_

        Returns:
            gps_time: GPS時刻
            system_time: システムクロック
            offset: オフセット値,基準はGPS時刻
            frequency: 非同期の際のシステムクロックのずれ
        """
        
        # GPS時刻を取得
        packet = gpsd.get_current()
        #gps_time = packet.get_time()

        # 端末の現在時刻を取得
        system_time = datetime.now().timestamp()

        # chronyc trackingコマンドからオフセット値を取得
        result = subprocess.run(['chronyc', 'tracking'], capture_output=True, text=True)#出力をresult.stdoutに保存
        for line in result.stdout.splitlines():
            match line:
                case _ if "Last offset" in line:  #"Last offset"行からオフセット値を取得
                    self.offset = line.split()[3]  # 4番目の要素がオフセット値（符号付き）,スペースタブ区切り
                    break

                case _ if "Frequency" in line:
                    self.frequency = line.split()[2] # 3番目の要素がfrequency
                    self.frequency_porm = line.split()[4] # 5番目の要素がslow or fast
                    
                
        #print(result.stdout)# 出力全体を確認（デバッグ用）
        # オフセット値が取得できたか確認
        if self.offset is None:
            self.offset = "N/A"  # 取得できない場合は"N/A"を記録
        if self.frequency is None:
            self.frequency = "N/A"
        
        return system_time, self.offset


if __name__ == "__main__":
    get = Get_gps() # クラスの初期化
    
    # CSVファイルを開く/作成
    with open('offset_log.csv', mode='w', newline='') as file:
        writer = csv.writer(file)
        # CSVのヘッダーを書き込む
        writer.writerow(['GPS Time', 'System Time', 'Offset[s]', 'frequency[ppm]','slow or fast'])

        # ループで定期的にデータを取得
        while True:
            try:
                gps_time, system_time, offset, frequency,frequency_porm =  get.get_gpstime()
                # CSVに書き込む
                writer.writerow([gps_time, system_time, offset, frequency,frequency_porm])
                print(f"GPS Time: {gps_time}, System Time: {system_time}, Offset: {offset}")

                # 60秒待機
                time.sleep(60)
            except Exception as e:
                print(f"Error: {e}")
