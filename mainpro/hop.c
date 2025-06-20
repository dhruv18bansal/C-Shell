#include "headers.h"
#include "hop.h"
#include "commands.h"

void hop_command(char *args[], int arg_count) {

    if (arg_count == 1) {
        change_directory("~");
    } else { 
        for (int j = 1; j < arg_count; j++) {
            change_directory(args[j]);
        }
    }
}
