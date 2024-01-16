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
void objectsInitialize(struct Object object[]);

int main(int argc, char *argv[]) {

    char **input = getInput(argv);

    struct Monster monster[2];
    struct Player player;
    struct Object objects[3];


    playerInitialize(&player);
    objectsInitialize(objects);


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


void objectsInitialize(struct Object object[]){
    strcpy(object[0].name, "Archery");
    strcpy(object[1].name, "Sword");
    strcpy(object[2].name, "Axe");

    object[0].power = 3;
    object[1].power = 5;
    object[2].power = 8;
}

void pathInitialize() {

    /*Room 1*/
    map[0].north = 3;
    map[0].east = 4;
    map[0].south = 2;

    /*Room 2*/
    map[1].north = 1;
    map[1].east = 5;

    /*Room 3*/
    map[2].south = 1;
    map[2].east = 8;

    /*Room 4*/
    map[3].north = 8;
    map[3].east = 6;
    map[3].south = 5;
    map[3].west = 1;
    map[3].object = 0; /*Archery Weapon*/

    /*Room 5*/
    map[4].north = 4;
    map[4].west = 2;

    /*Room 6*/
    map[5].north = 9;
    map[5].up = 11;
    map[5].west = 4;

    /*Room 7*/
    map[6].south = 8;
    map[6].east = 10;

    /*Room 8*/
    map[7].north = 7;
    map[7].east = 9;
    map[7].south = 4;
    map[7].west = 3;

    /*Room 9*/
    map[8].north = 10;
    map[8].east = 14;
    map[8].south = 6;
    map[8].west = 8;

    /*Room 10*/
    map[9].south = 9;
    map[9].west = 7;

    /*Room 11*/
    map[10].north = 12;
    map[10].down = 6;
    map[10].south = 13;
    map[10].east = 15;

    /*Room 12*/
    map[11].south = 11;

    /*Room 13*/
    map[12].north = 11;
    map[12].east = 16;

    /*Room 14*/
    map[13].east = 9;
    map[13].object = 2; /*Weapon Axe*/

    /*Room 15*/
    map[14].west = 11;
    map[14].north = 16;

    /*Room 16*/
    map[15].west = 13;
    map[15].north = 15;

}

void setInfoCells(){

    strcpy(map[0].info,"\n");

}


