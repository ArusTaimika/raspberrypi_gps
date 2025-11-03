# (1) コンパイラ
CC = g++

# (2) コンパイルオプション
CFLAGS = -Wall 

# (3) 実行ファイル名
TARGET = run

# (4) コンパイル対象のソースコード
SRCS = src/main.cpp src/commandline.cpp src/cpu_manager.cpp src/udp_connect.cpp src/csv_edit.cpp

# (5) オブジェクトファイル名
OBJS = $(SRCS:.cpp=.o)

# (6) インクルードファイルのあるディレクトリパス
INCDIR = -I./include

# (7) ライブラリファイルのあるディレクトリパス
#  LIBDIR = 

# (8) 追加するライブラリファイル
#LIBS = -lgpiod 

# (9) ターゲットファイル生成
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBDIR) $(LIBS)

# (10) オブジェクトファイル生成
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

# (11) "make all"でコンパイル
all: $(TARGET)

# (12) 不要ファイルを削除
clean:
	rm -f $(OBJS) $(TARGET) *.d
