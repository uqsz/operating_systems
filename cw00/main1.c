// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
  
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
    int id;
    int id_one;
} message;
  
typedef enum{
    ALL=1,
    ONE=2,
}ACTION;

int main()
{
    key_t key;
    int msgid;
  
    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    while(1){
        msgrcv(msgid, &message, sizeof(message), 2, 0);
        printf("Data Received is : %ld %d %s \n", message.mesg_type, message.id_one, message.mesg_text);
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        printf("Data Received is : %ld %d %s \n", message.mesg_type, message.id_one, message.mesg_text);
    }
    msgctl(msgid, IPC_RMID, NULL);
  
    return 0;
}