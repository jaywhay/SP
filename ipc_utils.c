#include "ipc_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// IPC 키 생성 함수
key_t get_ipc_key(const char *pathname, int proj_id) {
    // 주어진 경로명과 프로젝트 ID를 사용하여 IPC 키 생성
    key_t key = ftok(pathname, proj_id);
    if (key == -1) {
        // 키 생성 실패 시 에러 메시지 출력 후 프로그램 종료
        perror("키 생성 실패");
        exit(1);
    }
    return key;
}

// 메시지 큐 생성 함수
int create_message_queue(key_t key) {
    // 주어진 키를 사용하여 메시지 큐 생성
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        // 메시지 큐 생성 실패 시 에러 메시지 출력 후 프로그램 종료
        perror("메시지 큐 생성 실패");
        exit(1);
    }
    return msgid;
}

// 메시지 전송 함수
int send_message(int msgid, struct msg_buffer *message) {
    // 주어진 메시지 큐 ID를 사용하여 메시지 전송
    if (msgsnd(msgid, message, sizeof(message->msg_text), 0) == -1) {
        // 메시지 전송 실패 시 에러 메시지 출력
        perror("메시지 전송 실패");
        return -1;
    }
    return 0;
}

// 메시지 수신 함수
int receive_message(int msgid, struct msg_buffer *message, long msg_type) {
    // 주어진 메시지 큐 ID와 메시지 타입을 사용하여 메시지 수신
    if (msgrcv(msgid, message, sizeof(message->msg_text), msg_type, 0) == -1) {
        // 메시지 수신 실패 시 에러 메시지 출력
        perror("메시지 수신 실패");
        return -1;
    }
    return 0;
}

// 공유 메모리 생성 함수
int create_shared_memory(key_t key) {
    // 주어진 키를 사용하여 공유 메모리 생성
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        // 공유 메모리 생성 실패 시 에러 메시지 출력 후 프로그램 종료
        perror("공유 메모리 생성 실패");
        exit(1);
    }
    return shmid;
}

// 공유 메모리 연결 함수
char* attach_shared_memory(int shmid) {
    // 주어진 공유 메모리 ID를 사용하여 메모리를 현재 프로세스 주소 공간에 연결
    char *str = (char*) shmat(shmid, NULL, 0);
    if (str == (char*)-1) {
        // 공유 메모리 연결 실패 시 에러 메시지 출력 후 프로그램 종료
        perror("공유 메모리 첨부 실패");
        exit(1);
    }
    return str;
}

// 공유 메모리 분리 함수
int detach_shared_memory(const void *shmaddr) {
    // 주어진 주소를 사용하여 공유 메모리를 분리
    if (shmdt(shmaddr) == -1) {
        // 공유 메모리 분리 실패 시 에러 메시지 출력
        perror("공유 메모리 분리 실패");
        return -1;
    }
    return 0;
}

// 공유 메모리 삭제 함수
int destroy_shared_memory(int shmid) {
    // 주어진 공유 메모리 ID를 사용하여 공유 메모리 삭제
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        // 공유 메모리 삭제 실패 시 에러 메시지 출력
        perror("공유 메모리 삭제 실패");
        return -1;
    }
    return 0;
}
