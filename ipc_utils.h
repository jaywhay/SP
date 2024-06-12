#ifndef IPC_UTILS_H
#define IPC_UTILS_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_SIZE 1024 // 공유 메모리 크기를 1024 바이트로 정의

// 메시지 버퍼 구조체 정의
struct msg_buffer {
    long msg_type;          // 메시지 타입
    char msg_text[100];     // 메시지 내용
};

// IPC 키 생성 함수
// 주어진 경로명과 프로젝트 ID를 사용하여 IPC 키를 생성
key_t get_ipc_key(const char *pathname, int proj_id);

// 메시지 큐 생성 함수
// 주어진 IPC 키를 사용하여 메시지 큐를 생성
int create_message_queue(key_t key);

// 메시지 전송 함수
// 주어진 메시지 큐 ID와 메시지 버퍼를 사용하여 메시지를 전송
int send_message(int msgid, struct msg_buffer *message);

// 메시지 수신 함수
// 주어진 메시지 큐 ID와 메시지 타입을 사용하여 메시지를 수신
int receive_message(int msgid, struct msg_buffer *message, long msg_type);

// 공유 메모리 생성 함수
// 주어진 IPC 키를 사용하여 공유 메모리를 생성
int create_shared_memory(key_t key);

// 공유 메모리 연결 함수
// 주어진 공유 메모리 ID를 사용하여 메모리를 현재 프로세스 주소 공간에 연결
char* attach_shared_memory(int shmid);

// 공유 메모리 분리 함수
// 주어진 주소를 사용하여 공유 메모리를 현재 프로세스 주소 공간에서 분리
int detach_shared_memory(const void *shmaddr);

// 공유 메모리 삭제 함수
// 주어진 공유 메모리 ID를 사용하여 공유 메모리를 삭제
int destroy_shared_memory(int shmid);

#endif // IPC_UTILS_H
