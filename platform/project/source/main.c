
#include "xpg.h"

int main(int argc, char ** argv)
{
    XPGHandle handle = loadXpg("1.xpg");
    closeXpg(handle);
    return 0;
}

