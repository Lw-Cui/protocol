#ifndef _AUX_H_
#define _AUX_H_

int open_listenfd(unsigned short port);
int open_clientfd(const char *hostname, unsigned short port);

#endif
