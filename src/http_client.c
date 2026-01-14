#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <protocols/webclient.h>

#define REQUEST_URL          "http://example.com/"
#define REQUEST_BUFFER_SIZE  ( 1024 * 4 )

int http_client(void) {
    struct http_client_request_t request;
    struct http_client_response_t response;
    int ret;

    memset(&request, 0, sizeof(request));
    request.method   = WGET_MODE_GET;
    request.url      = REQUEST_URL;
    request.buflen   = REQUEST_BUFFER_SIZE;
    request.encoding = CONTENT_LENGTH;
    request.headers  = malloc(sizeof(struct http_keyvalue_list_t));
    if (!request.headers)
    {
        printf("Failed to allocate headers\n");
        return -1;
    }
    http_keyvalue_list_init(request.headers);

    if (http_client_response_init(&response) < 0)
    {
        printf("Failed to init response\n");
        free(request.headers);
        return -1;
    }

    ret = http_client_send_request(&request, NULL, &response);
    if (ret < 0)
    {
        printf("Request failed\n");
        http_client_response_release(&response);
        free(request.headers);
        return -1;
    }

    printf("HTTP Response:\n%s\n%s\n", response.message, response.entity);

    http_client_response_release(&response);
    free(request.headers);

    return 0;
}
