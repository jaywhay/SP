#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ipc_utils.h"

#define FILE_PATH "chat_log.txt"

void handle_client(int msgid, int shmid);

void handle_client(int msgid, int shmid) {
    FILE *file = fopen(FILE_PATH, "a");
    if (!file) {
        perror("파일 열기 실패");
        return;
    }

    char *shared_memory = attach_shared_memory(shmid);
    struct msg_buffer message;

    while (1) {
        receive_message(msgid, &message, 1);
        fprintf(file, "클라이언트: %s\n", message.msg_text);
        fflush(file);

        strcpy(shared_memory, message.msg_text);

        if (strcmp(message.msg_text, "exit") == 0) {
            break;
        }
    }

    detach_shared_memory(shared_memory);
    fclose(file);
}
