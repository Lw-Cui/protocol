#include "message.h"
#include <string.h>
#define MEG_LEN 300

char *create_meg(ushort num, char *data) {
	char *meg = new char[MEG_LEN];
	memcpy(meg, &num, sizeof(ushort));
	memcpy(meg + sizeof(num), data, strlen(data));
	return meg;
}


ushort extract_num(char *meg) {
	ushort num;
	memcpy(&num, meg, sizeof(ushort));
	return num;
}

char *extract_data(char *meg) {
	return meg + sizeof(ushort);
}

bool init(int fd, ushort maxnum) {
	return true;
}

void destory(char *p) {
	delete p;
}
