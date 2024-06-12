# 컴파일러 설정
CC = gcc
# 컴파일러 옵션 설정: -Wall은 모든 경고 메시지를 출력하고, -g는 디버그 정보를 포함시킴
CFLAGS = -Wall -g
# 생성할 타겟 실행 파일 이름
TARGET = chat_system
# 소스 파일 목록
SRCS = main.c server.c client.c ipc_utils.c
# 오브젝트 파일 목록
OBJS = $(SRCS:.c=.o)

# 기본 목표(target)을 설정, 여기서는 타겟 실행 파일을 생성하는 것이 목표
all: $(TARGET)

# 타겟 실행 파일을 생성하는 규칙
# $@는 타겟 이름(chat_system), $^는 모든 의존성 파일 목록(여기서는 OBJS)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 각 소스 파일을 오브젝트 파일로 컴파일하는 규칙
# $<는 첫 번째 의존성 파일 이름(여기서는 소스 파일 이름), $@는 타겟 파일 이름(여기서는 오브젝트 파일 이름)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 'make clean' 명령어를 실행하면 타겟 파일과 오브젝트 파일을 삭제
clean:
	rm -f $(TARGET) $(OBJS)
