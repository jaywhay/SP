#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ipc_utils.h"

#define FILE_PATH "chat_log.txt"

// 클라이언트 요청을 처리하는 함수
void handle_client(int msgid, int shmid);

// 클라이언트 요청을 처리하는 함수
void handle_client(int msgid, int shmid) {
    // 로그 파일을 append 모드로 열기
    FILE *file = fopen(FILE_PATH, "a");
    if (!file) {
        perror("파일 열기 실패");
        return;
    }

    // 공유 메모리 연결
    char *shared_memory = attach_shared_memory(shmid);
    struct msg_buffer message;

    while (1) {
        // 메시지 큐에서 메시지를 수신
        receive_message(msgid, &message, 1);
        
        // 수신한 메시지를 로그 파일에 기록
        fprintf(file, "클라이언트: %s\n", message.msg_text);
        fflush(file);

        // 수신한 메시지를 공유 메모리에 저장
        strcpy(shared_memory, message.msg_text);

        // 메시지가 'exit'이면 반복 종료
        if (strcmp(message.msg_text, "exit") == 0) {
            break;
        }
    }

    // 공유 메모리 분리
    detach_shared_memory(shared_memory);

    // 로그 파일 닫기
    fclose(file);
}
