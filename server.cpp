#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include "message.h"
#include "aux.h"
#define PORT 8001

int main(int args, char **argv) {
	int listenfd = open_listenfd(PORT);

	while (1) {
		struct sockaddr_in clientaddr;
		int clientlen = sizeof(clientaddr);
		int serverfd = accept(listenfd, (struct sockaddr *)&clientaddr, 
				(socklen_t *)&clientlen); 
		FILE *fp = fopen("recv.data", "w");
		bool connected = false;
		char recv[MEG_LEN];

		while (1) {
			fd_set read_set;
			FD_ZERO(&read_set);
			FD_SET(serverfd, &read_set);
			timeval tv = {0, 0};
			if (select(FD_SETSIZE, &read_set, NULL, NULL, &tv) < 0)
				return 0;

			if (FD_ISSET(serverfd, &read_set) && read(serverfd, recv, MEG_LEN) > 0) {
				if (connected) {
					if (extract_num(recv) == CNTMAX + 1) break;
					if (extract_num(recv) == CNTMAX) continue;

					fputs(extract_data(recv), fp);
					printf("%s", extract_data(recv));
					write(serverfd, recv, MEG_LEN);
				} else {
					if (extract_num(recv) == CNTMAX) {
						connected = true;
						write(serverfd, recv, MEG_LEN);
					}
				}
			}
		}
		close(serverfd);
		fclose(fp);
	}
	close(listenfd);
	return 0;
}
