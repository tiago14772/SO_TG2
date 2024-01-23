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
#define NO_WAY (-1)
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
    struct Monster monster[1];
    struct Object objects[3];
};


static struct map map[100];

void playerInitialize(struct Player *player);

void mapInitialize();

void monsterInitialize(struct Monster monster[]);

void objectsInitialize(struct Object object[]);

void processPlayerChoice(struct data *d);

void checkRoom(struct data *d);

void searchTreasure(struct data *d);

void searchWeapon(struct data *d);

void playerResume(struct data *d);

void *threadPlayer(void *ptr);

void *threadMonster(void *ptr);


int main(int argc, char *argv[]) {

    char **input = getInput(argv);

    struct Monster monster[1];
    struct Player player;
    struct Object objects[3];


    playerInitialize(&player);
    mapInitialize();
    objectsInitialize(objects);
    monsterInitialize(monster);


    struct data d;

    d.player = player;

    d.monster[0] = monster[0];

    d.objects[0] = objects[0];
    d.objects[1] = objects[1];
    d.objects[2] = objects[2];

    pthread_t playerThread, playerMonster;

    strcpy(op, "");


    pthread_create(&playerThread, NULL, threadPlayer, (void *) &d);
    pthread_create(&playerMonster, NULL, threadMonster, (void *) &d);

    pthread_join(playerThread, NULL);
    pthread_join(playerMonster, NULL);


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

/**
 * @brief Initializes the array of objects in the game.
 *
 * This function sets up the initial values for the objects in the game.
 * It assigns names and power values to each object in the 'object' array.
 *
 * @param object An array of Object structures to be initialized.
 */
void objectsInitialize(struct Object object[]) {
    strcpy(object[0].name, "Archery");
    strcpy(object[1].name, "Sword");
    strcpy(object[2].name, "Axe");

    object[0].power = 3;
    object[1].power = 5;
    object[2].power = 8;
}


/**
 * @brief Initializes paths and features for each room in the game map.
 *
 * This function sets up paths and additional features for each room in the game map.
 * It defines the connections between rooms (north, south, east, west, up, down) and assigns objects and treasures to specific rooms.
 */
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
    map[7].object = 1; /*Sword Weapon*/

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
    map[13].west = 9;
    map[13].object = 2; /*Axe Weapon*/

    /*Room 15*/
    map[14].west = 11;
    map[14].south = 16;

    /*Room 16*/
    map[15].west = 13;
    map[15].north = 15;
    map[15].treasure = 1;

}

/**
 * @brief Set additional information about room connections.
 *
 * This function sets information about possible directions a player can go from each room.
 * The information is stored in the 'info' field of each room in the 'map' array.
 * It provides details about available directions for navigation.
 */
void setInfoCells() {

    /*Room 1*/
    strcpy(map[0].info, "You can go to North, South and East\n");
    /*Room 2*/
    strcpy(map[1].info, "You can go to North and East\n");
    /*Room 3*/
    strcpy(map[2].info, "You can go to South and East\n");
    /*Room 4*/
    strcpy(map[3].info, "You can go to North, East, South and West\n");
    /*Room 5*/
    strcpy(map[4].info, "You can go to North and West\n");
    /*Room 6*/
    strcpy(map[5].info, "You can go to North, Up and West\n");
    /*Room 7*/
    strcpy(map[6].info, "You can go to South and East\n");
    /*Room 8*/
    strcpy(map[7].info, "You can go to North, East, South and West\n");
    /*Room 9*/
    strcpy(map[8].info, "You can go to North, East, South and West\n");
    /*Room 10*/
    strcpy(map[9].info, "You can go to South, East and West\n");
    /*Room 11*/
    strcpy(map[10].info, "You can go to North, South, East and Down\n");
    /*Room 12*/
    strcpy(map[11].info, "You can go to South\n");
    /*Room 13*/
    strcpy(map[12].info, "You can go to North and East\n");
    /*Room 14*/
    strcpy(map[13].info, "You can go to West\n");
    /*Room 15*/
    strcpy(map[14].info, "You can go to South and West\n");
    /*Room 16*/
    strcpy(map[15].info, "You can go to West and North\n");

}

/**
 * @brief Initializes the game map.
 *
 * This function initializes the game map represented by the 'map' array.
 * It sets up the initial values for directions, treasures, objects, and room descriptions.
 * Calls auxiliary functions to configure paths and provide information about room connections.
 */
void mapInitialize() {
    for (int i = 0; i < 16; ++i) {
        map[i].north = NO_WAY;
        map[i].south = NO_WAY;
        map[i].west = NO_WAY;
        map[i].east = NO_WAY;
        map[i].up = NO_WAY;
        map[i].down = NO_WAY;
        map[i].treasure = NO_TREASURE;
        map[i].object = NO_OBJECT;

        char desc[25];
        sprintf(desc, "You are in Room %d\n", i + 1);
        strcpy(map[i].description, desc);
    }
    pathInitialize();
    setInfoCells();
}


/**
 * @brief Initializes the array of monsters (enemies) in the game.
 *
 * This function sets up the initial values for the monsters in the game.
 * It assigns names, energy, power, and map locations to each monster in the 'monster' array.
 *
 * @param monster An array of Monster structures to be initialized.
 */
void monsterInitialize(struct Monster monster[]) {
    /*Predator*/
    strcpy(monster[0].name, "Predator");
    monster[0].energy = 50;
    monster[0].power = 5;
    monster[0].map = 5;
}


/**
 * @brief Changes the current room based on the player's chosen direction.
 *
 * This function processes the player's choice of direction (north, south, east, west, up, down)
 * and updates the current room accordingly. It also handles printing messages about the movement.
 *
 * @param direction The player's chosen direction.
 */
void changeRoom(const char *direction, struct data *d) {
    int *directionPtr;

    if (strcmp(direction, "n") == 0) directionPtr = &map[CURRENT_PLACE].north;
    else if (strcmp(direction, "s") == 0) directionPtr = &map[CURRENT_PLACE].south;
    else if (strcmp(direction, "e") == 0) directionPtr = &map[CURRENT_PLACE].east;
    else if (strcmp(direction, "w") == 0) directionPtr = &map[CURRENT_PLACE].west;
    else if (strcmp(direction, "u") == 0) directionPtr = &map[CURRENT_PLACE].up;
    else if (strcmp(direction, "d") == 0) directionPtr = &map[CURRENT_PLACE].down;
    else {
        printf("Invalid Choice!\n");
        return;
    }

    if (*directionPtr != -1) {
        printf("Moving to next room...\n\n");
        CURRENT_PLACE = *directionPtr - 1;
        d->player.map = CURRENT_PLACE;
        sleep(2);


    } else {
        printf("This path is blocked!\n");
    }
}

void checkRoom(struct data *d) {
    searchTreasure(d);
    searchWeapon(d);
}

void searchTreasure(struct data *d) {
    char op[2];
    printf("Do you want to search for a treasure? yes(y), no(n)\n");
    scanf("%1s", op);

    if (strcmp(op, "y") == 0) {
        printf("Searching.....\n");
        sleep(2);
        if (map[CURRENT_PLACE].treasure == 1) {
            d->player.treasure = 1;
            printf("You are carrying a treasure!!!!\n");
        } else {
            printf("No treasure in the room!!!\n");
        }
    }
}

void searchWeapon(struct data *d) {
    char op[2];
    printf("Do you want to search for a Weapon? yes(y), no(n)\n");
    scanf("%1s", op);

    if (strcmp(op, "y") == 0) {
        printf("Searching.....\n");
        sleep(2);

        if (map[CURRENT_PLACE].object >= 0 && map[CURRENT_PLACE].object < 3) {
            d->player.object = map[CURRENT_PLACE].object;
            printf("Weapon found!!\nYou carrying a %s\n", d->objects[d->player.object].name);
        } else {
            printf("No weapon in the room!!!\n\n");
        }
    }
}

void playerResume(struct data *d) {
    printf("PLAYER RESUME\n");
    printf("\tName: %s\n", d->player.name);
    printf("\tWeapon: %s\n", d->objects[d->player.object].name);
    printf("\tEnergy: %d\n", d->player.energy);
}


/**
 * @brief Processes the player's choice of direction.
 *
 * This function prompts the player to choose a direction and then calls the changeRoom function
 * to handle the actual room change based on the chosen direction.
 */
void processPlayerChoice(struct data *d) {
    printf("\n%s", map[CURRENT_PLACE].description);
    checkRoom(d);
    playerResume(d);
    printf("\n%s\n", map[CURRENT_PLACE].info);
    printf("Choose a direction (n, s, e, w, u, d): ");
    scanf("%s", op);
    changeRoom(op, d);
}


void *threadPlayer(void *ptr) {
    struct data *d = (struct data *) ptr;

    while (strcmp(op, "q") != 0) {
        printf("Player position %d",d->player.map);
        /*int monsterPosition = d->monster[0].map;
        int playerPosition = d->player.map;

        printf("%d\n", monsterPosition);
        printf("%d\n", playerPosition);
        if (monsterPosition == playerPosition)*/

        processPlayerChoice(d);
    }
    return NULL;
}

void *threadMonster(void *ptr) {
    struct data *d = (struct data *) ptr;
    int mPos;

    while (strcmp(op, "q") != 0) {
        mPos = rand() % 9 + 8;
        d->monster[0].map = mPos;
        printf("Monster position %d\n",d->monster[0].map);

        if (d->player.map == d->monster[0].map) printf("UAAAAAAHHHHHH WE GONNA FIGHT!!!");

        sleep(4);
    }

    return NULL;
}


