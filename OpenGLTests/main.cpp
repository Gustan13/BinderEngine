#define STB_IMAGE_IMPLEMENTATION
#include "game.h"

int main(void)
{   
    Game LOD;
    LOD.init();
    LOD.update();
    LOD.destroy();
    return 0;
}