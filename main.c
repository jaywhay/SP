#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ipc_utils.h"

#define MSG_PROJ_ID 1  // 메시지 큐에 사용할 프로젝트 ID
#define SHM_PROJ_ID 2  // 공유 메모리에 사용할 프로젝트 ID

void run_server();
void run_client();
void chat_with_server(int msgid, int shmid);  

int main() {
    // 새로운 프로세스를 생성 (fork)
    pid_t pid = fork();

    // fork 실패 시 에러 처리
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // 자식 프로세스일 경우 클라이언트 실행
    if (pid == 0) {
        run_client();
        exit(EXIT_SUCCESS);
    } else {
        // 부모 프로세스일 경우 서버 실행
        run_server();
        // 자식 프로세스가 종료될 때까지 대기
        wait(NULL);
    }

    return 0;
}

void run_server() {
    // 메시지 큐와 공유 메모리의 IPC 키 생성
    key_t msg_key = get_ipc_key(".", MSG_PROJ_ID);
    key_t shm_key = get_ipc_key(".", SHM_PROJ_ID);

    // 메시지 큐와 공유 메모리 생성
    int msgid = create_message_queue(msg_key);
    int shmid = create_shared_memory(shm_key);

    while (1) {
        struct msg_buffer message;
        // 메시지 큐에서 메시지 수신
        receive_message(msgid, &message, 1);

        // 새로운 자식 프로세스 생성 (fork)하여 클라이언트 요청 처리
        if (fork() == 0) {
            handle_client(msgid, shmid);
            exit(0);
        }
        // 자식 프로세스 종료 대기
        wait(NULL);
    }
}

void run_client() {
    // 메시지 큐와 공유 메모리의 IPC 키 생성
    key_t msg_key = get_ipc_key(".", MSG_PROJ_ID);
    key_t shm_key = get_ipc_key(".", SHM_PROJ_ID);

    // 메시지 큐와 공유 메모리 생성
    int msgid = create_message_queue(msg_key);
    int shmid = create_shared_memory(shm_key);

    // 서버와의 채팅 수행
    chat_with_server(msgid, shmid);
}
