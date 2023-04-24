// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#define MAX 30
  
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
    struct mesg_buffer m2;
    m2.mesg_type = 2;
    m2.id_one=987;
    struct mesg_buffer m1;
    m1.mesg_type=1;
    m1.id_one=654;
    strcpy(m1.mesg_text,"raz");
    while(1){
        printf("Write Data : ");
        fgets(m2.mesg_text,MAX,stdin);
        msgsnd(msgid, &m1, sizeof(m1), 0);
        msgsnd(msgid, &m2, sizeof(m2), 0);
    }
  
    return 0;
}