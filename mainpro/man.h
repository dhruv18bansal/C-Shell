#ifndef MAN_H
#define MAN_H

#include "headers.h"

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
void fetchContent(const char *ip_address, const char *path);
static curl_socket_t opensocket(void *clientp, curlsocktype purpose, struct curl_sockaddr *address);
void parsehtml(const char *str, size_t len);
#endif // COMMANDS_H