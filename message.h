#ifndef _MESSAGE_H_
#define _MESSAGE_H_
typedef unsigned short ushort;
typedef unsigned int uint;

const uint MEG_LEN = 300;
const ushort CNTMAX = 10;
const ushort QLEN = 3;

char *init_meg();
char *end_meg();

char *create_meg(ushort num, char *data);
ushort extract_num(char *meg);
char *extract_data(char *meg);
void destory(char *);

bool init(ushort length);

#endif
