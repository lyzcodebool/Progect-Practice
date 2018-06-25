#include <stdio.h>
#include <cjson/cJSON.h>
#include <cjson/cJSON.c>
#include <stdlib.h>

/*构建cjson*/
char *makeJson(int size)
{
    cJSON *pJsonRoot = NULL;
    pJsonRoot = cJSON_CreateArray();
    if(pJsonRoot == NULL){  
        printf("array create failed!\n");
        return NULL;
    }

    for(int i = 0; i < size; ++i){
        cJSON_AddNumberToObject(pJsonRoot, "hehe", i);
    }

    //将内存的存储格式转化为字符串
    char *out = cJSON_Print(pJsonRoot);
    cJSON_Delete(pJsonRoot);
    
    return out;
}

/*解析cjson*/
void parseArray(char *pJson)
{
    if(NULL == pJson){
        return;
    }

    cJSON *root = NULL;
    if((root = cJSON_Parse(pJson)) == NULL)
        return;

    int size = cJSON_GetArraySize(root);
    for(int i = 0; i < size; ++i){
        cJSON *pSub = cJSON_GetArrayItem(root, i);
//      cJSON *ret = cJSON_GetObjectItem(root, "hehe");
        if(pSub == NULL){
            continue;
        }
        int value = pSub->valueint;
        printf("value[%d] : %d\n", i, value);
    }

    cJSON_Delete(root);
    return;
}

int main(void)
{
    char *pjson = makeJson(5);
    printf("\n%s\n\n", pjson);
    parseArray(pjson);
    return 0;
}
