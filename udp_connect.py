import socket
import struct
import time

class Udp_connect:
    def __init__(self,ip, port, data_pieces):
        # UDPソケットを作成
        self.udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        
        self.ip = ip
        self.port = port
        self.data_pieces = data_pieces

        if self.ip == '0.0.0.0':
            self.udp_socket.bind((self.ip, self.port))

    def udp_server(self): 
        # データを受信（バッファサイズは1024バイト）
        data, addr = self.udp_socket.recvfrom(16)
        #バイナリデータをアンパックして複数の浮動小数点数として解釈
        
        return struct.unpack(f'!{self.data_pieces}d', data)  # data_piecesの個数のdoubleをアンパック

    def udp_client(self, data):
        # サーバーにデータを送信
        #self.udp_socket.sendto(message.encode(), (server_ip, server_port))# 文字列を想定した場合
        # data（リスト型）を展開して渡す
        self.udp_socket .sendto(struct.pack(f'!i{self.data_pieces}d', *data), (self.ip, self.port))

    def udp_close(self):
        # ソケットを閉じる
        self.udp_socket.close()


#server用
if __name__ == "__main__":
    ip = "0.0.0.0"
    port = 12345
    data_pieces = 2
    udp_connect = Udp_connect(ip, port, data_pieces)

    try:
        while True:
            received_numbers = udp_connect.udp_server()
            print(received_numbers)
    except KeyboardInterrupt:
        print("ーーーーーーーーー終了ーーーーーーーーーーーー")
    finally:
        udp_connect.udp_close()
    
"""
#client用
if __name__ == "__main__":
    ip = "192.168.132.154"
    port = 12345
    data_pieces = 2
    udp_connect = Udp_connect(ip, port, data_pieces)

    # 送信データ
    data = [0.1,0.1,0.1]
    try:
        while True:
            udp_connect.udp_client(data)
            time.sleep(1/30)
    except KeyboardInterrupt:
        print("ーーーーーーーーー終了ーーーーーーーーーーーー")
    finally:
        udp_connect.udp_close()
    
"""