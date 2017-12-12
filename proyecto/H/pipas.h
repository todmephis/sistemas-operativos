#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PATH "/tmp/mi_fifo"
#define PATH2 "/tmp/mi_fifo2"
#define KB_SIZE 1024.0
#define LINUX "Linux"
#define MACOS "Darwin"