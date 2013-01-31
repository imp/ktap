/*
 * ktapio.c - relay transport in userspace
 *
 * Copyright (C) 2012-2013 Jovi Zhang
 *
 * Author: Jovi Zhang <bookjovi@gmail.com>
 *         zhangwei(Jovi) <jovi.zhangwei@huawei.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <fcntl.h>


#define MAX_BUFLEN  131072

#define handle_error(str) do { perror(str); exit(-1); } while(0)

int main(int argc, char **argv)
{
	char buf[MAX_BUFLEN];
	struct pollfd pollfd;
	struct timespec tim = {.tv_sec=0, .tv_nsec=200000000};
	int timeout, fd, ret, len;

	fd = open("/sys/kernel/debug/ktap/trace0", O_RDONLY);
	if (fd < 0)
		handle_error("open failed");

	pollfd.fd = fd;
	pollfd.events = POLLIN;
	timeout = tim.tv_sec * 1000 + tim.tv_nsec / 1000000;

	do {
		ret = poll(&pollfd, 1, timeout);
		if (ret < 0)
			break;

		while ((len = read(fd, buf, sizeof(buf))) > 0) {
			write(1, buf, len);
		}
	} while (1);

	close(fd);	

	return 0;
}


