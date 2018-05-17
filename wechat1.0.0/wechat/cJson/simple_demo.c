#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>

int main(void)
{
    char *ptr = "{\"firstname\":\"Breet\"}";

    cJSON *cj = cJSON_Parse(ptr);
    if(!cj){
        printf("cjson_parse failed!\n");
        return -1;
    }

    cJSON *item = cJSON_GetObjectItem(cj, "firstname");
    if(item){
        printf("%s \n", item->valuestring); 
    }

    cJSON_Delete(cj);
    return 0;
}
