#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "link.h"
#include "exceptions.h"
#include "files.h"

void getprop_hostname(char* ret){
    gethostname(ret, 200);
}