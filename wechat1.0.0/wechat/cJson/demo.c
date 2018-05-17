#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

void printJson(cJSON *root)
{
    if(!root)
    {
        printf("NULL JSON root.\n");
        return;
    }
    
    printf("Type=0x%02x, %s=%s\n", root->type, root->string, cJSON_Print(root));  //cJSON_Print会根据类型打印
}

static char * makeJson(void)
{
    cJSON *pJsonRoot = NULL;
    cJSON *pSubJson = NULL;
    char *p = NULL;

    pJsonRoot = cJSON_CreateObject();
    if(NULL == pJsonRoot)
    {
        printf("%s line=%d NULL\n", __func__, __LINE__);
        return NULL;
    }
    cJSON_AddStringToObject(pJsonRoot, "hello", "hello world");  //String类型
    cJSON_AddNumberToObject(pJsonRoot, "number", 10010);  //Number类型
    cJSON_AddBoolToObject(pJsonRoot, "bool", 1);  //bool类型
    pSubJson = cJSON_CreateObject();//  创建一个cJSON，用于嵌套数据
    if(NULL == pSubJson)
    {
        printf("%s line=%d NULL\n", __func__, __LINE__);
        cJSON_Delete(pJsonRoot);
        return NULL;
    }
    cJSON_AddStringToObject(pSubJson, "subjsonobj", "a sub json string");  //在子cJSON下，增加一个String类型数据
    cJSON_AddItemToObject(pJsonRoot, "subobj", pSubJson);  //将子cJSON加入到pJsonRoot

    p = cJSON_Print(pJsonRoot);
    if(NULL == p)
    {
        printf("%s line=%d NULL\n", __func__, __LINE__);
        cJSON_Delete(pJsonRoot);
        return NULL;
    }

    cJSON_Delete(pJsonRoot);

    return p;
}

static void parseJson(char * pMsg)
{
    cJSON *pJson;
    cJSON *pSub;
    cJSON * pSubSub;

    if(NULL == pMsg)
    {
    return;
    }

    pJson = cJSON_Parse(pMsg);
    if(NULL == pJson)                                                                                         
    {
        return ;
    }

    pSub = cJSON_GetObjectItem(pJson, "hello");
    printJson(pSub);

    pSub = cJSON_GetObjectItem(pJson, "number");
    printJson(pSub);

    pSub = cJSON_GetObjectItem(pJson, "bool");
    printJson(pSub);

    pSub = cJSON_GetObjectItem(pJson, "subobj");
    printJson(pSub);

    pSubSub = cJSON_GetObjectItem(pSub, "subjsonobj");
    printJson(pSubSub);

    cJSON_Delete(pJson);
}

int main(void)
{
    char *p;

    /* print the version */
    printf("Version: %s\n", cJSON_Version());  //获取cJSON版本号

    p = makeJson(); // 构造JSON数据
    if(NULL == p)
    {
        return 0;
    }
    printf("p = \n%s\n\n", p); // 打印构造的字符串
    parseJson(p); // 解析JSON数据，并打印Type-Key-Value
    free(p);
    return 0;
}
