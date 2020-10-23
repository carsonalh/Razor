#include <razor.h>
#include <rigid.h>

#include <stdio.h>

void Game_Init(void *user_ptr)
{
    printf("Game init here.\n");
}

void Game_Update(void *user_ptr)
{
    //printf("Update game here.\n");
}

void Game_Uninit(void *user_ptr)
{
    printf("Uninit game here.\n");
}

int main(void)
{
    rz_ClientStrategy strategy;
    strategy.user_ptr = NULL;
    strategy.init_func = Game_Init;
    strategy.update_func = Game_Update;
    strategy.uninit_func = Game_Uninit;

    rz_RunApplication(&strategy);
    return 0;
}
