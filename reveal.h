#ifndef REVEAL_H
#define REVEAL_H

#include "headers.h"  // Includes all necessary standard headers

// Function prototypes
void print_permissions(mode_t mode);
void reveal(const char *flags, const char *path);

#endif // REVEAL_H
