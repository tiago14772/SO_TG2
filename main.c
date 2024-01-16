#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_NAME 100
#define INITIAL_PLAYER_ENERGY 50
#define INITIAL_PLAYER_POSITION_MAP 0
#define NO_OBJECT (-1)
#define NO_TREASURE (-1)
#define MAX_MAP_DESCRIPTION 200
#define MAX_map$ 1000
#define N_WEAPONS 5
#define N_OF_ENEMIES 3

static int CURRENT_PLACE = 0;
static char op[10];

/**
 * @brief A function that takes an array of strings as input and returns the same array.
 *
 * @param argv An array of strings, typically representing command-line arguments.
 * @return char** A pointer to the same array of strings passed as input.
 */

char **getInput(char *argv[]) {
    return argv;
}

/*
 * Struct that represents the objects loaded by the players
 * */

struct Object {
    char name[MAX_NAME];
    int power;
};

/*
 * Struct that represents the monsters in the game
 * */

struct Monster {
    char name[MAX_NAME];
    int energy;
    int power;
    int map;
};

/*
 * Struct that represents the player in the game
 *
 * */

struct Player {
    char name[MAX_NAME];
    int energy;
    int map;
    int object;
    int treasure;
};

/*
 * Struct that represents the map of the game
 * */

struct map {
    int north;
    int south;
    int west;
    int east;
    int up;
    int down;
    char description[MAX_MAP_DESCRIPTION];
    int object;
    int treasure;
    char info[MAX_NAME];
};

struct data {
    struct Player player;
    struct Monster monster[2];
    struct Object objects[3];
};


static struct map map[100];

void playerInitialize(struct Player *player);

int main(int argc, char *argv[]) {

    char **input = getInput(argv);

    struct Monster monster[2];
    struct Player player;
    struct Object objects[3];


    playerInitialize(&player);

    return 0;


}


/**
 * @brief Initializes a player structure with user input.
 *
 * This function initializes the fields of a Player structure using user input.
 * It prompts the user to input their player name and sets other attributes with default values.
 *
 * @param player A pointer to the Player structure to be initialized.
 */

void playerInitialize(struct Player *player) {


    printf("Welcome!! Insert your Player name: ");
    scanf("%s", player->name);

    player->energy = INITIAL_PLAYER_ENERGY;
    player->map = INITIAL_PLAYER_POSITION_MAP;
    player->object = NO_OBJECT;
    player->treasure = NO_TREASURE;

}


