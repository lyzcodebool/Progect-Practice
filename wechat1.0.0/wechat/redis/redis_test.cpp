#include "redis.h"

int main(int argc, char **argv)
{
    Redis *redis_test = new Redis();
    redis_test->redis_connect("127.0.0.1", 6379, 0);
    redis_test->setkey("name", "value");
    std::cout<<"get the name is:"<<redis_test->getkey("name").c_str()<<std::endl;
    delete redis_test;
    return 0;
}
