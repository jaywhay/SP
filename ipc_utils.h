#ifndef IPC_UTILS_H
#define IPC_UTILS_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_SIZE 1024

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

key_t get_ipc_key(const char *pathname, int proj_id);
int create_message_queue(key_t key);
int send_message(int msgid, struct msg_buffer *message);
int receive_message(int msgid, struct msg_buffer *message, long msg_type);
int create_shared_memory(key_t key);
char* attach_shared_memory(int shmid);
int detach_shared_memory(const void *shmaddr);
int destroy_shared_memory(int shmid);

#endif // IPC_UTILS_H
