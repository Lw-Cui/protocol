#include <deque>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include "aux.h"
#include "message.h"
#define PORT 8080
/* MUST SMALLER THAN CNTMAX(0xfff) */
#define QLEN 0xaff

void ackprocess(std::deque<char *> &q, int num) {
	while (!q.empty() && extract_num(q.front()) != num) {
		destory(q.front());
		q.pop_front();
	}
}

bool sendcontrol(std::deque<char *> &q, int clientfd ,FILE *fp, ushort &count) {
	char data[MEG_LEN];
	bool noerror = true;
	while (q.size() < QLEN && (noerror = fgets(data, MEG_LEN - 10, fp)) != NULL) {
		q.push_back(create_meg(count++ % CNTMAX, data));
		write(clientfd, q.back(), MEG_LEN);
	}
	if (!noerror)
		printf("%s\n", strerror(errno));
	return noerror;
}

int main(int args, char **argv) {
	int clientfd = open_clientfd("localhost", PORT);
	FILE *fp = fopen("raw.data", "r");

	std::deque<char *> q;
	bool connected = false;
	ushort count = 0;

	while (1) {
		fd_set read_set;
		FD_ZERO(&read_set);
		FD_SET(clientfd, &read_set);

		timeval tv = {0, 0};
		if (select(FD_SETSIZE, &read_set, NULL, NULL, &tv) < 0)
			return 0;

		char recv[MEG_LEN];
		if (FD_ISSET(clientfd, &read_set) && read(clientfd, recv, MEG_LEN) > 0)
			if (connected) {
				ackprocess(q, extract_num(recv));
				if (!sendcontrol(q, clientfd, fp, count)) break;
			} else {
				if (extract_num(recv) == CNTMAX)
					connected = true;
			}

		if (!connected)
			write(clientfd, init_meg(), MEG_LEN);
	}

	write(clientfd, init_meg(), MEG_LEN);
	fclose(fp);
	close(clientfd);
	return 0;
}
