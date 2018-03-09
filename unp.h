#ifndef __UNP_H
#define __UNP_H
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define MAXLINE 1024
#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
#endif
