#include <unistd.h>

int main(void)
{
    if (write(1, "Hello World!\n", 14) == -1)
    {
        return 1;
    }
    return 0;
}
