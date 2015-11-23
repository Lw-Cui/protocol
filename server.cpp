#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include "aux.h"
#define PORT 8080
#define MAX 100

int main(int args, char **argv) {
	int len;
	int listenfd = open_listenfd(PORT);

	while (1) {
		struct sockaddr_in clientaddr;
		int clientlen = sizeof(clientaddr);
		int serverfd = accept(listenfd, (struct sockaddr *)&clientaddr, 
				(socklen_t *)&clientlen); 

		while (1) {
			fd_set read_set;
			FD_ZERO(&read_set);
			FD_SET(serverfd, &read_set);
			timeval tv = {0, 0};
			if (select(FD_SETSIZE, &read_set, NULL, NULL, &tv) < 0)
				return 0;

			char recv[MAX];
			if (FD_ISSET(serverfd, &read_set)) {
				if ((len = read(serverfd, recv, MAX)) <= 0)
					break;
				printf("%s", recv);
			}
		}
		close(serverfd);
	}
	close(listenfd);
	return 0;
}
