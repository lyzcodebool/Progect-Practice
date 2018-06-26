#include "redis.h"
#include <time.h>

int main(int argc, char **argv)
{
    Redis *redis_test = new Redis();
    redis_test->redis_connect("127.0.0.1", 6379, 0);
    redis_test->setkey("name", "value");
    
    clock_t start = clock();
    for(int i = 0; i < 10; ++i)
    {
        redis_test->getkey("name");
    }
    clock_t end = clock();
    double time = end - start;
    printf("usetime = %f\n", time/CLOCKS_PER_SEC);
//    std::cout<<"get the name is:"<<redis_test->getkey("name").c_str()<<std::endl;
    delete redis_test;
    return 0;
}
