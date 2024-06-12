#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ipc_utils.h"

// 서버와 채팅을 수행하는 함수
void chat_with_server(int msgid, int shmid);

// 서버와 채팅을 수행하는 함수
void chat_with_server(int msgid, int shmid) {
    // 공유 메모리 연결
    char *shared_memory = attach_shared_memory(shmid);
    struct msg_buffer message;
    message.msg_type = 1;

    while (1) {
        // 사용자로부터 메시지 입력 받기
        printf("메시지 입력: ");
        fgets(message.msg_text, sizeof(message.msg_text), stdin);
        // 개행 문자 제거
        message.msg_text[strcspn(message.msg_text, "\n")] = '\0';

        // 메시지 큐로 메시지 전송
        send_message(msgid, &message);

        // 'exit' 메시지를 입력하면 반복 종료
        if (strcmp(message.msg_text, "exit") == 0) {
            break;
        }

        // 서버가 메시지를 기록할 시간을 주기 위해 대기
        sleep(1);

        // 서버로부터 받은 메시지를 출력
        printf("서버로부터 받은 메시지: %s\n", shared_memory);
    }

    // 공유 메모리 분리
    detach_shared_memory(shared_memory);
}
