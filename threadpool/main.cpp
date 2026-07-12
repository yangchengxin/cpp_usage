#include <iostream>
#include "ThreadPool.hpp"

class Math
{
public:
    Math()
    {

    }
    ~Math()
    {

    }

    int add(int a, int b)
    {
        return a + b;
    };
};

int main(int argc, char** argv)
{
    std::shared_ptr<Math> m = std::make_shared<Math>();
    threadpool* p = new threadpool(6);
    int i = 0;
    int j = 1;
    int ans1, ans2;
    while(true)
    {
        i += 1;
        j += 1;
        auto future1 = p->enqueue([m, i, j](){
            return m->add(i, j);
        });
        auto future2 = p->enqueue([m, i, j](){
            return m->add(2 * i, 2 * j);
        });

        try
        {
            ans1 = future1.get();
            ans2 = future2.get();
        }
        catch(std::exception& e)
        {
            std::cerr << "任务异常！" << e.what() << std::endl;
        }
        std::cout << "ans1: " << ans1 << std::endl << "ans2: " << ans2 << std::endl; 
    }

    return 0;
}