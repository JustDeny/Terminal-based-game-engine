#include <iostream>
#include "tbGameEngine.h"

class Test: public tbGameEngine
{
    bool OnUserUpdate(float elapsedTime) override
    {
        return true;
    }

    bool OnUserCreate() override
    {
        return true;
    }
public:

};

int main() {
    Test tbg;
    tbg.ConstructConsole(120,35);
    tbg.Start();
    return 0;
}
