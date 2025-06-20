#include "headers.h"
#include "man.h"
static curl_socket_t opensocket(void *clientp, curlsocktype purpose, struct curl_sockaddr *address) {
    curl_socket_t sockfd = socket(address->family, address->socktype, address->protocol);
    if (sockfd == -1) {
        perror("socket");
    }
    return sockfd;
}

// Function to parse HTML content and strip out tags
void parsehtml(const char *str, size_t len) {
    int in_tag = 0;  // Flag to check if inside an HTML tag
    int newline_flag = 0;  // Flag to manage excessive newlines

    for (size_t i = 0; i < len; i++) {
        if (str[i] == '<') {
            in_tag = 1;  // Inside a tag
        } else if (str[i] == '>') {
            in_tag = 0;  // Exiting a tag
        } else if (!in_tag) {
            if (str[i] != '\n' && str[i] != '\r') {
                putchar(str[i]);
                newline_flag = 0;  // Reset newline flag when content is printed
            } else if (!newline_flag) {
                // Print a single newline only if it hasn't been printed before
                putchar('\n');
                newline_flag = 1;
            }
        }
    }
}

// CURL write callback to process the HTML response
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    parsehtml(ptr, total_size);
    return total_size;
}

void fetchContent(const char *ip_address, const char *path) {
    CURL *curl;
    CURLcode res;
    
    // Initialize curl
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing libcurl\n");
        return;
    }

    // Create a full URL using the IP address
    char url[1024];
    snprintf(url, sizeof(url), "http://%s%s", ip_address, path);

    // Set curl options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, opensocket);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);  // Set callback function
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);  // Disable verbose output

    // Set the Host header explicitly since we are using an IP address
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Host: man.he.net");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Perform the request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    // Cleanup
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}