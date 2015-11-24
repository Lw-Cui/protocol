#ifndef _MESSAGE_H_
#define _MESSAGE_H_

typedef unsigned short ushort;
typedef unsigned int uint;

char *create_meg(ushort num, char *data);
ushort extract_num(char *meg);
char *extract_data(char *meg);
void destory(char *);

bool init(ushort length);

#endif
