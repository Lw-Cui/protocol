#ifndef _MESSAGE_H_
#define _MESSAGE_H_

typedef unsigned short ushort;
typedef unsigned int uint;

char *create(ushort num, uint len, char *data);
void destory(char *);

bool init(ushort length);

#endif
