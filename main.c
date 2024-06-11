#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ipc_utils.h"

#define MSG_PROJ_ID 1
#define SHM_PROJ_ID 2

void run_server();
void run_client();

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // 자식 프로세스 - 클라이언트 실행
        run_client();
        exit(EXIT_SUCCESS);
    } else {
        // 부모 프로세스 - 서버 실행
        run_server();
        wait(NULL);  // 자식 프로세스 종료 대기
    }

    return 0;
}

void run_server() {
    key_t msg_key = get_ipc_key(".", MSG_PROJ_ID);
    key_t shm_key = get_ipc_key(".", SHM_PROJ_ID);

    int msgid = create_message_queue(msg_key);
    int shmid = create_shared_memory(shm_key);

    while (1) {
        struct msg_buffer message;
        receive_message(msgid, &message, 1);

        if (fork() == 0) {
            handle_client(msgid, shmid);
            exit(0);
        }
        wait(NULL);  // 자식 프로세스 종료 대기
    }
}

void run_client() {
    key_t msg_key = get_ipc_key(".", MSG_PROJ_ID);
    key_t shm_key = get_ipc_key(".", SHM_PROJ_ID);

    int msgid = create_message_queue(msg_key);
    int shmid = create_shared_memory(shm_key);

    chat_with_server(msgid, shmid);
}
