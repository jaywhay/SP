#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ipc_utils.h"

void chat_with_server(int msgid, int shmid);

void chat_with_server(int msgid, int shmid) {
    char *shared_memory = attach_shared_memory(shmid);
    struct msg_buffer message;
    message.msg_type = 1;

    while (1) {
        printf("메시지 입력: ");
        fgets(message.msg_text, sizeof(message.msg_text), stdin);
        message.msg_text[strcspn(message.msg_text, "\n")] = '\0'; // 개행 문자 제거

        send_message(msgid, &message);

        if (strcmp(message.msg_text, "exit") == 0) {
            break;
        }

        sleep(1); // 서버가 메시지를 기록할 시간을 줌

        printf("서버로부터 받은 메시지: %s\n", shared_memory);
    }

    detach_shared_memory(shared_memory);
}
