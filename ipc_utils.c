#include "ipc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

key_t get_ipc_key(const char *pathname, int proj_id) {
    key_t key = ftok(pathname, proj_id);
    if (key == -1) {
        perror("키 생성 실패");
        exit(1);
    }
    return key;
}

int create_message_queue(key_t key) {
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("메시지 큐 생성 실패");
        exit(1);
    }
    return msgid;
}

int send_message(int msgid, struct msg_buffer *message) {
    if (msgsnd(msgid, message, sizeof(message->msg_text), 0) == -1) {
        perror("메시지 전송 실패");
        return -1;
    }
    return 0;
}

int receive_message(int msgid, struct msg_buffer *message, long msg_type) {
    if (msgrcv(msgid, message, sizeof(message->msg_text), msg_type, 0) == -1) {
        perror("메시지 수신 실패");
        return -1;
    }
    return 0;
}

int create_shared_memory(key_t key) {
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("공유 메모리 생성 실패");
        exit(1);
    }
    return shmid;
}

char* attach_shared_memory(int shmid) {
    char *str = (char*) shmat(shmid, NULL, 0);
    if (str == (char*)-1) {
        perror("공유 메모리 첨부 실패");
        exit(1);
    }
    return str;
}

int detach_shared_memory(const void *shmaddr) {
    if (shmdt(shmaddr) == -1) {
        perror("공유 메모리 분리 실패");
        return -1;
    }
    return 0;
}

int destroy_shared_memory(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("공유 메모리 삭제 실패");
        return -1;
    }
    return 0;
}
