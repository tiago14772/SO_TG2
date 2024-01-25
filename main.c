/**
 * @authors Luis Viana, nº 5327
 * @authors Tiago Francisco, nº 14772
 *
 * @date 25/01/2024
 *
 * @version 1
 * gcc -o tg2 tg2_5327_14772.c -pthread
 * ./tg2
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_NAME 100
#define INITIAL_PLAYER_ENERGY 50
#define INITIAL_PLAYER_POSITION_MAP 0
#define NO_OBJECT (-1)
#define NO_TREASURE (-1)
#define MAX_MAP_DESCRIPTION 200
#define NO_WAY (-1)

static int CURRENT_PLACE = 0; // Declare a static variable for the current player position on the map
char op[10]; // Declare a character array named "op" with a size of 10
sem_t sem; // Declare a semaphore named "sem"

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
struct Map {
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

/*
 * Struct that represents the overall game data
 * Player, Monsters and Objects
 * */
struct Data {
    struct Player player;
    struct Monster monster[1];
    struct Object objects[3];
};

static struct Map map[16];

void menu();
void playerInitialize(struct Player *player);
void monsterInitialize(struct Monster monster[]);
void objectsInitialize(struct Object object[]);
void mapInitialize();
void processPlayerChoice(struct Data *d);
void changeRoom(const char *direction, struct Data *d);
void fightMonster(struct Data *d);
void checkRoom(struct Data *d);
void playerResume(struct Data *d);
void *threadPlayer(void *ptr);
void *threadMonster(void *ptr);

 /**
 * @brief Main function to start the game.
 *
 * This function initializes game entities, displays the menu, creates threads
 * for player and monster, and waits for them to finish before exiting.
 *
 * @return Returns 0 on successful execution.
 */
int main() {
    // Declare variables
    struct Monster monster[1];
    struct Player player;
    struct Object objects[3];
    struct Data d;

    // Display the game menu
    menu();
    // Initialize player, map, objects, and monsters
    playerInitialize(&player);
    mapInitialize();
    objectsInitialize(objects);
    monsterInitialize(monster);

    // Initialize semaphore for synchronization of threads
    sem_init(&sem, 1, 1);

    // Populate game data structure with initialized objects
    d.player = player;
    d.monster[0] = monster[0];
    d.objects[0] = objects[0];
    d.objects[1] = objects[1];
    d.objects[2] = objects[2];

    // Declare threads for player and monster
    pthread_t playerThread, monsterThread;

    // Initialize the command string
    strcpy(op, "");

    // Create threads for player and monster, passing game data structure
    pthread_create(&playerThread, NULL, threadPlayer, (void *) &d);
    pthread_create(&monsterThread, NULL, threadMonster, (void *) &d);

    // Wait for player and monster threads to finish
    pthread_join(playerThread, NULL);
    pthread_join(monsterThread, NULL);

    // Exit the program
    return 0;
}

/*
 * Function to display the game menu
 * */
void menu() {
    printf("#####################################################\n"
           "#                      WELCOME                      #\n"
           "#            You must find the treasure             #\n"
           "#     Be careful, there are a monster out there     #\n"
           "#####################################################\n\n");
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
    printf("Insert your Player name: ");
    scanf("%s", player->name);
    printf("You can move in different directions:\n"
           "North(n), South(s), East(e), West(w), Up(u), Down(d)\n");
    printf("To search for the treasure or a weapon press 'f'\n"
           "For player information press 'i'\n"
           "To quit the game press 'q'\n");
    player->energy = INITIAL_PLAYER_ENERGY;
    player->map = INITIAL_PLAYER_POSITION_MAP;
    player->object = NO_OBJECT;
    player->treasure = NO_TREASURE;

    printf("Are you ready?");
    scanf("%s", op);
    printf("%s, Good Luck!\n", player->name);
}

/**
 * @brief Initializes the array of monsters in the game.
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
    monster[0].map = 9;
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
    strcpy(map[0].info, "You can go to North, South and East: ");
    /*Room 2*/
    strcpy(map[1].info, "You can go to North and East: ");
    /*Room 3*/
    strcpy(map[2].info, "You can go to South and East: ");
    /*Room 4*/
    strcpy(map[3].info, "You can go to North, East, South and West: ");
    /*Room 5*/
    strcpy(map[4].info, "You can go to North and West: ");
    /*Room 6*/
    strcpy(map[5].info, "You can go to North, Up and West: ");
    /*Room 7*/
    strcpy(map[6].info, "You can go to South and East: ");
    /*Room 8*/
    strcpy(map[7].info, "You can go to North, East, South and West: ");
    /*Room 9*/
    strcpy(map[8].info, "You can go to North, East, South and West: ");
    /*Room 10*/
    strcpy(map[9].info, "You can go to South, East and West: ");
    /*Room 11*/
    strcpy(map[10].info, "You can go to North, South, East and Down: ");
    /*Room 12*/
    strcpy(map[11].info, "You can go to South: ");
    /*Room 13*/
    strcpy(map[12].info, "You can go to North and East: ");
    /*Room 14*/
    strcpy(map[13].info, "You can go to West: ");
    /*Room 15*/
    strcpy(map[14].info, "You can go to South and West: ");
    /*Room 16*/
    strcpy(map[15].info, "You can go to West and North: ");

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

        char desc[64];
        sprintf(desc, "################# You are in Room %d #################", i + 1);
        strcpy(map[i].description, desc);
    }
    pathInitialize();
    setInfoCells();
}

/**
 * @brief Thread function representing player actions.
 *
 * This function continuously processes player actions until the "q" command is received.
 * It checks if the player is on the same map as the monster and initiates a fight if true.
 * After processing player choices, it continues the loop.
 *
 * @param ptr A pointer to the game data structure (struct Data).
 * @return NULL (void pointer) as it is a pthread-compatible function.
 */
void *threadPlayer(void *ptr) {
    // Cast the generic pointer to a pointer of struct Data
    struct Data *d = (struct Data *) ptr;
    while (strcmp(op, "q") != 0) {
        // Acquire the semaphore to ensure exclusive access to shared data
        sem_wait(&sem);
        if (d->player.map == d->monster[0].map){
            fightMonster(d);
        }
        // Release the semaphore after accessing shared data
        sem_post(&sem);
        processPlayerChoice(d);
    }
    // Return NULL as it is a pthread-compatible function
    return NULL;
}

/**
 * @brief Process player choices based on the current game state.
 *
 * This function displays information about the current room, prompts the player for choices,
 * and performs corresponding actions based on the player's input.
 *
 * @param d A pointer to the game data structure (struct Data).
 */
void processPlayerChoice(struct Data *d) {
    // Acquire the semaphore to ensure exclusive access to shared data
    sem_wait(&sem);
    // Display information about the current room and player options
    printf("\n%s", map[CURRENT_PLACE].description);
    printf("\nPlayer info(i), search Room(f)\n");
    // Display additional information about the current room directions
    printf("%s", map[CURRENT_PLACE].info);
    scanf("%s", op);
    // Release the semaphore after accessing shared data
    sem_post(&sem);
    // Show resume player information
    if (strcmp(op, "i") == 0) {
        playerResume(d);
        return;
    }
    // Check if there are any object or treasure in the room
    if (strcmp(op, "f") == 0) {
        checkRoom(d);
        return;
    }
    // Quit the game
    if (strcmp(op, "q") == 0) {
        exit(0);
    }
    sleep(1);
    // Check if the player is in the same room as the monster
    // in case of true, go back to start a fight
    if (d->player.map == d->monster[0].map){
        return;
    }
    // Acquire the semaphore to ensure exclusive access to shared data
    sem_wait(&sem);
    // Go to the method to check if it'ś possible to change the room based on the player's choice
    changeRoom(op, d);
    // Release the semaphore after accessing shared data
    sem_post(&sem);
}

/**
 * @brief Changes the current room based on the player's chosen direction.
 *
 * This function processes the player's choice of direction (north, south, east, west, up, down)
 * and updates the current room accordingly. It also handles printing messages about the movement.
 *
 * @param direction The player's chosen direction.
 */
void changeRoom(const char *direction, struct Data *d) {
    int *directionPtr;

    if (strcmp(direction, "n") == 0) directionPtr = &map[CURRENT_PLACE].north;
    else if (strcmp(direction, "s") == 0) directionPtr = &map[CURRENT_PLACE].south;
    else if (strcmp(direction, "e") == 0) directionPtr = &map[CURRENT_PLACE].east;
    else if (strcmp(direction, "w") == 0) directionPtr = &map[CURRENT_PLACE].west;
    else if (strcmp(direction, "u") == 0) directionPtr = &map[CURRENT_PLACE].up;
    else if (strcmp(direction, "d") == 0) directionPtr = &map[CURRENT_PLACE].down;
    else {
        printf("Invalid Choice!");
        return;
    }

    if (*directionPtr != -1) {
        printf("Moving to next room...\n");
        CURRENT_PLACE = *directionPtr - 1;
        d->player.map = CURRENT_PLACE;
        sleep(2);

    } else {
        printf("This path is blocked!");
        return;
    }
}

/**
 * @brief Thread function representing monster actions.
 *
 * This function continuously performs actions for the monster until the "q" command is received.
 * It randomly selects a direction to move in, updates the monster's position if valid, and checks
 * if the monster is in the same room as the player to initiate a fight.
 *
 * @param ptr A pointer to the game data structure (struct Data).
 * @return NULL (void pointer) as it is a pthread-compatible function.
 */
void *threadMonster(void *ptr) {
    // Cast the generic pointer to a pointer of struct Data
    struct Data *d = (struct Data *) ptr;
    // Variables to represent movement direction and a pointer to check the direction
    int direction;
    int *checkDirection;
    // Continuous loop until the "q" command is received
    while (strcmp(op, "q") != 0) {
        // Generate a random number to determine the monster's movement direction
        direction = rand() % 6 + 1;
        // Assign the appropriate map direction based on the random number
        if (direction == 1) checkDirection = &map[d->monster[0].map].north;
        else if (direction == 2) checkDirection = &map[d->monster[0].map].south;
        else if (direction == 3) checkDirection = &map[d->monster[0].map].east;
        else if (direction == 4) checkDirection = &map[d->monster[0].map].west;
        else if (direction == 5) checkDirection = &map[d->monster[0].map].up;
        else if (direction == 6) checkDirection = &map[d->monster[0].map].down;

        // If the selected direction is valid, update the monster's map position
        if (*checkDirection != -1) {
            d->monster[0].map = *checkDirection - 1;
            sleep(2);
        }
        // Check if the player is in the same room as the monster
        if (d->player.map == d->monster[0].map){
            // Acquire the semaphore to ensure exclusive access to shared data
            sem_wait(&sem);
            // Check again if the player is still in the same room as the monster
            if (d->player.map == d->monster[0].map) {
                // Initiate a fight
                fightMonster(d);
            }
            // Release the semaphore after accessing shared data
            sem_post(&sem);
        }
        // Sleep for 5 seconds before the next move
        sleep(5);
    }
    // Return NULL as it is a pthread-compatible function
    return NULL;
}

/**
 * @brief Simulate a fight between the player and a monster.
 *
 * This function initiates a fight scenario, allowing the player to choose between fighting or fleeing.
 * The outcome of the fight is determined based on the player and monster's power and energy levels.
 *
 * @param d A pointer to the game data structure (struct Data).
 */
void fightMonster(struct Data *d) {
    // Display a message indicating the presence of a monster
    printf("\nUAAAAAAHHHHHH A MONSTER!!!");

    // Calculate player's power based on equipped object or default power
    int playerPower;
    if(d->player.object != -1) {
        playerPower = d->objects[d->player.object].power;
    }
    // Default power if no object is equipped
    else{
        playerPower = 1;
    }
    // Prompt the player to choose whether to flee or fight
    printf("\nFugir? (y): ");
    scanf("%s", op);

    // If the player chooses to run, decrease player's energy, display a message, and change room
    if(strcmp(op, "y") == 0){
        d->player.energy = d->player.energy - d->monster[0].power;
        printf("Perdeu %d de energia!\n", d->monster[0].power);
        printf("%s", map[CURRENT_PLACE].info);
        scanf("%s", op);
        changeRoom(op, d);
        return;
    }
    // If the player chooses to fight(op != y), go to a fight scenario
    printf("WE WILL FIGHT!!!\n");
    sleep(2);
    // Determine the outcome of the fight based on energy and power levels
    if(d->player.energy / d->monster[0].power <  d->monster[0].energy / playerPower){
        printf("YOU LOSE");
        exit(0);
    }
    else{
        printf("Congratulations!! You killed the monster.");
        d->monster[0].energy = 50; // Reset monster's energy
        d->monster[0].map = rand() % 6 + 10; // Move monster to a new random room between 10 and 16.
    }
}

/**
 * @brief Simulate searching a room for treasures or objects.
 *
 * This function simulates the player searching a room. It checks if the room contains a treasure,
 * and if so, declares the player as the winner. If the room contains an object, it prompts the player
 * to pick it up as a new weapon.
 *
 * @param d A pointer to the game data structure (struct Data).
 */
void checkRoom(struct Data *d) {
    // Acquire the semaphore to ensure exclusive access to shared data
    sem_wait(&sem);
    // Display a message indicating that the player is searching
    printf("Searching.....\n");
    sleep(2);
    // Check if the room contains a treasure
    if (map[CURRENT_PLACE].treasure == 1) {
        d->player.treasure = 1;
        printf("A treasure was found!!!! Congratulations, you WIN.\n");
        exit(0);
    }
    // Check if the room contains an object (weapon)
    if (map[CURRENT_PLACE].object >= 0 && map[CURRENT_PLACE].object < 3) {
        // Display information about the found object
        printf("You find a %s\n", d->objects[map[CURRENT_PLACE].object].name);
        // Prompt the player to pick up the new weapon
        printf("Pick the new weapon? (y): ");
        scanf("%s", op);
        // If the player chooses to pick up the new weapon, update the player's object and display a message
        if(strcmp(op, "y") == 0) {
            d->player.object = map[CURRENT_PLACE].object;
            printf("You are carrying a %s\n", d->objects[d->player.object].name);
        }
    }
    // Display a message indicating that the room is empty
    else {
        printf("The room is empty!!!\n");
    }
    // Release the semaphore after accessing shared data
    sem_post(&sem);
}

/**
 * @brief Display a summary of the player's status.
 *
 * This function displays information about the player's name, energy level, and equipped weapon (if any).
 * If the player has an equipped weapon, it also displays the power of the weapon.
 *
 * @param d A pointer to the game data structure (struct Data).
 */
void playerResume(struct Data *d) {
    // Acquire the semaphore to ensure exclusive access to shared data
    sem_wait(&sem);
    // Display a summary of the player's status
    printf("PLAYER RESUME:\n");
    printf("\tName: %s\n", d->player.name);
    printf("\tEnergy: %d\n", d->player.energy);
    // Check if the player has an equipped weapon
    if(d->player.object != -1) {
        // Display information about the equipped weapon and its power
        printf("\tWeapon: %s\n", d->objects[d->player.object].name);
        printf("\tPower: %d", d->objects[d->player.object].power);
    }
    else{
        // Display a message indicating that the player has no equipped weapon
        printf("\tWeapon: None\n");
        printf("\tPower: %d", 1); // Default power if no weapon is equipped
    }
    // Release the semaphore after accessing shared data
    sem_post(&sem);
}
