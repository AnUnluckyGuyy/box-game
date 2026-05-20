#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define HEIGHT 7
#define WIDTH 15

typedef struct Player {
    int x;
    int y;
} Player;

Player player = {
    .x = WIDTH / 2,
    .y = HEIGHT / 2
};

typedef struct Enemy {
    int x;
    int y;
} Enemy;

Enemy enemy = {
    .x = 1,
    .y = 1
};

int dist_x = 0;
int dist_y = 0;

bool can_move(int new_x, int new_y) {
    if (new_x >= WIDTH-1 || new_x <= 0 || new_y >= HEIGHT-1 || new_y <= 0) {
        return false;
    }
    return true;
}

bool enemy_should_move() {
    int r = rand()  % 5; // 0-4
    if (dist_x >= 2 || dist_y >= 2) {
        if (r < 1) { // moves 4 times out of 5
            return false;
        } 
    } else {
        if (r < 2) { // moves 3 times out of 5
            return false;
        } 
    }
    return true;
}

bool enemy_is_adjacent() {
    for (int i=-1; i<2; i++) {
        if ((player.x+i == enemy.x) && (player.y == enemy.y || player.y+1 == enemy.y || player.y-1 == enemy.y)) {
            return true;
        }
    }
    return false;
}

void push_enemy_back(char direction) {
    if (direction == 'd') {
        enemy.x++;
    } else if (direction == 'a') {
        enemy.x--;
    } else if (direction == 's') {
        enemy.y++;
    } else if (direction == 'w') {
        enemy.y--;
    }
}

bool is_valid_direction(char c) {
    if (c == 'w' || c == 'a' || c == 's' || c == 'd') { return true; }
    return false;
}

void move_enemy() {
    char direction_x = 0;
    char direction_y = 0;

    // get distance from player in x axis and what direction should move to
    if (player.x > enemy.x) {
        dist_x = player.x - enemy.x;
        direction_x = 'e';
    } else if (player.x < enemy.x) {
        dist_x = enemy.x - player.x;
        direction_x = 'w';
    }
    
    // get distance from player in y axis and what direction should move to
    if (player.y > enemy.y) {
        dist_y = player.y - enemy.y;
        direction_y = 's';
    } else if (player.y < enemy.y) {
        dist_y = enemy.y - player.y;
        direction_y = 'n';
    }

    if (dist_x > dist_y && direction_x == 'e') {
        enemy.x++;
    } else if(dist_x > dist_y && direction_x == 'w') {
        enemy.x--;
    } else if (dist_y > dist_x && direction_y == 's') {
        enemy.y++;
    } else if (dist_y > dist_x && direction_y == 'n') {
        enemy.y--;
    } else { // equal dist_x and dist_y
        int r = rand() % 2;
        if (r == 0) { // x axis move
            if (direction_x == 'e') { enemy.x++; }
            else if (direction_x == 'w') { enemy.x--; }
        } else if (r == 1) { // y axis move
            if (direction_y == 's') { enemy.y++; }
            else if (direction_y == 'n') { enemy.y--; }
        }
    }
}

void debug_player_coords() {
    printf("Player X: %d\n", player.x);
    printf("Player Y: %d\n", player.y);
}

void debug(const char *message) {
    printf("%s", message);
}

char grid[HEIGHT][WIDTH];

void fill_board() {
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            if (i == player.y && j == player.x) { // player position at the middle
                grid[i][j] = '@';
            } else if (i == enemy.y && j == enemy.x) { // enemy position at top corner
                grid[i][j] = 'E';
            } 
            else if (i == 0 || i == HEIGHT-1 || j == 0 || j == WIDTH-1) { // border
                grid[i][j] = '#';
            } else { // walkable field
                grid[i][j] = '.';
            }
        }
    }
}

void render_board() {
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
}

void clear_screen() {
    printf("\033[H\033[J");
}

int main() {
    srand(time(NULL));
    fill_board();
    clear_screen();
    render_board();

    while (true) {
        bool is_enemy_stunned = false;
        printf("move: ");
        char c = getchar();
        while (getchar() != '\n');

        if (c == 'q') {
            break;
        } else if (c == 'w') {
            if (can_move(player.x, player.y-1)) { player.y--; }
        } else if (c == 's') {
            if (can_move(player.x, player.y+1)) { player.y++; }
        } else if (c == 'a') {
            if (can_move(player.x-1, player.y)) { player.x--; }
        } else if(c == 'd') {
            if (can_move(player.x+1, player.y)) { player.x++; }
        } else if (c == 'e') {
            if (!enemy_is_adjacent()) {
                printf("enemy is not adjacent to use push on him!\n");
                continue;
            } 
            printf("push direction: ");
            char push_direction = getchar();
            while (getchar() != '\n');
            if (!is_valid_direction(push_direction)) { 
                printf("invalid push direction!\n");
                continue; 
            }
            push_enemy_back(push_direction);
            is_enemy_stunned = true;
        } else if (c == 'x') {
            // wait
        } else {
            printf("invalid input!");        
            continue;
        }

        if (enemy_should_move() && !is_enemy_stunned) { move_enemy(); }
        
        if (player.x == enemy.x && player.y == enemy.y) {
            clear_screen();
            printf("Game over\n");
            break;
        }
        fill_board();
        clear_screen();
        render_board();
    }
    return 0;
}