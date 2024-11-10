#include <iostream>
#include <cstdlib>  // For system()
#include "minigame.h"

int run_maze(int rank)
{
    std::string command = "/home/laksh/RealmWeaver/src/minigame/maze_game " + std::to_string(rank);

    // Run the .exe file
    int result = system(command.c_str());

    if (result==34048){
        return 1;
    } else{
        return 0;
    }
}

int run_connect4(int rank){
    std::string command = "/home/laksh/RealmWeaver/src/minigame/connect4 " + std::to_string(rank);

    // Run the .exe file
    int result = system(command.c_str());

    if (result==34048){
        return 1;
    } else{
        return 0;
    }
}

int main() {
    int a = 10;
    std::cout << (run_connect4(a));
    return 0;
}
