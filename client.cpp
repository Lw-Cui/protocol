#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include "aux.h"
#define PORT 8080
#define MAX 256

int main(int args, char **argv) {
	int clientfd = open_clientfd("localhost", PORT);

	while (1) {
		fd_set read_set;
		FD_ZERO(&read_set);
		FD_SET(clientfd, &read_set);

		timeval tv = {0, 0};
		if (select(FD_SETSIZE, &read_set, NULL, NULL, &tv) < 0)
			return 0;

		int len;
		char recv[MAX];
		if (FD_ISSET(clientfd, &read_set)) {
			if ((len = read(clientfd, recv, MAX)) <= 0)
				break;
			printf("%s", recv);
		}

	}
	close(clientfd);
	return 0;
}
