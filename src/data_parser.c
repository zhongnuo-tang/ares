#include "cJSON.h"
#include <stdio.h>
#include <math.h>
#include "data.h"

int get_time_str(const char *json_text, char *buffer, size_t bufsize)
{
    if (!json_text || !buffer || bufsize == 0)
        return 0;

    cJSON *root = cJSON_Parse(json_text);
    if (!root || !cJSON_IsObject(root)) {
        cJSON_Delete(root);
        return 0;
    }

    cJSON *time_item = cJSON_GetObjectItem(root, "time");
    if (!time_item || !cJSON_IsString(time_item)) {
        cJSON_Delete(root);
        return 0;
    }

    strncpy(buffer, time_item->valuestring, bufsize - 1);
    buffer[bufsize - 1] = '\0';

    cJSON_Delete(root);
    return 1;
}
