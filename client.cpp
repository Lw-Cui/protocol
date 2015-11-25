#include <deque>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "aux.h"
#include "message.h"
#define PORT 8001
/* MUST SMALLER THAN CNTMAX(0xfff) */

void ackprocess(std::deque<char *> &q, int num) {
	while (!q.empty() && extract_num(q.front()) != num) {
		destory(q.front());
		q.pop_front();
	}
	if (!q.empty() && extract_num(q.front()) == num)
		q.pop_front();
}

void sendcontrol(std::deque<char *> &q, int clientfd ,FILE *fp, ushort &count) {
	char data[MEG_LEN];
	while (q.size() < QLEN && fgets(data, MEG_LEN - 10, fp) != NULL) {
		q.push_back(create_meg(count++ % CNTMAX, data));
		write(clientfd, q.back(), MEG_LEN);
		printf("WRITE: %d\n", extract_num(q.back()));
		//printf("PUSH: %s\n", extract_data(q.back()));
	}
	if (ferror(fp)) printf("ERROR: %s\n", strerror(errno));
	else if (feof(fp)) printf("EOF\n");
}

int main(int args, char **argv) {
	int clientfd = open_clientfd("localhost", PORT);
	FILE *fp = fopen("raw.data", "r");

	std::deque<char *> q;
	bool connected = false;
	ushort count = 0;
	char recv[MEG_LEN];

	while (1) {
		fd_set read_set;
		FD_ZERO(&read_set);
		FD_SET(clientfd, &read_set);

		timeval tv = {0, 0};
		if (select(FD_SETSIZE, &read_set, NULL, NULL, &tv) < 0)
			return 0;

		int len;
		if (FD_ISSET(clientfd, &read_set) && (len = read(clientfd, recv, MEG_LEN)) > 0) {
			printf("RECV: %d\n", extract_num(recv));
			//printf("RECV: %s\n", extract_data(recv));
			if (connected) {
				if (extract_num(recv) == CNTMAX) continue;
				ackprocess(q, extract_num(recv));
				sendcontrol(q, clientfd, fp, count);
				printf("SIZE: %d\n", q.size());
				if (q.empty()) {
					printf("empty()!\n");
					break;
				}
			} else {
				if (extract_num(recv) == CNTMAX)
					connected = true;
			}
		}

		if (!connected)
			write(clientfd, init_meg(), MEG_LEN);
	}

	write(clientfd, end_meg(), MEG_LEN);
	fclose(fp);
	close(clientfd);
	return 0;
}
