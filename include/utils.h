#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 4096

void err_ret(const char*);
void err_quit(const char*);
void err_sys(const char*);
void AddFL(int fd, int arg);
void RemoveFL(int fd, int arg);
#endif  //UTILS_H_
