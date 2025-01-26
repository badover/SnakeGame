#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 20

typedef struct{
    int x, y;
} Point;

typedef struct{
    Point body [WIDTH * HEIGHT];
    int length;
    Point direction;
} Snake;

typedef struct{
    Point position;
} Food;

void initGame(Snake *snake, Food *food){
    snake->length = 1;
    snake->body[0].x = WIDTH / 2;
    snake->body[0].y = HEIGHT / 2;
    snake->direction.x = 1;
    snake->direction.y = 0;

    food->position.x = rand() % WIDTH;
    food->position.y = rand() % HEIGHT;

}

void drawGame(Snake *snake, Food *food){
    clear();
    for(int x = 0; x <= WIDTH; x++){
        mvprintw(0, x, "$");
        mvprintw(HEIGHT, x, "$");
    }

    for(int y = 0; y <= WIDTH; y++){
        mvprintw(y, 0, "$");
        mvprintw(y, WIDTH, "$");
    }

    for(int i = 0;i < snake->length; i++){
        mvprintw(snake->body[i].y, snake->body[i].x, "O");
    }

    mvprintw(food->position.y, food->position.x,"X");

    refresh();

}

void updateGame(Snake *snake, Food *food, int *gameOver){
    Point nextPosition = {
        snake->body[0].x + snake->direction.x,
        snake->body[0].y + snake->direction.y
    };

    for(int i = snake-> length - 1; i > 0; i--){
        snake->body[i] = snake->body[i-1];
    }
    
    snake->body[0] = nextPosition;

    //chek walls and self
    if (nextPosition.x < 0 || nextPosition.x >= WIDTH || nextPosition.y < 0 || nextPosition.y >= HEIGHT) {
        *gameOver = 1;
    }

    //food
     if (nextPosition.x == food->position.x && nextPosition.y == food->position.y) {
        snake->length++;  
        food->position.x = rand() % WIDTH;  
        food->position.y = rand() % HEIGHT; 
    }

    //check herself

    for (int i = 1; i < snake->length; i++) {
        if (snake->body[i].x == nextPosition.x && snake->body[i].y == nextPosition.y) {
            *gameOver = 1;
        }
    }
}

int main(){
    srand(time(0));

    initscr();
    curs_set(1);
    keypad(stdscr, TRUE);
    noecho();
    timeout(100);

    Snake snake;
    Food food;
    int gameOver = 0;

    initGame(&snake, &food);

    while (!gameOver){
        drawGame(&snake, &food);

        int input  = getch();
        switch(input){
            case KEY_UP:
                if (snake.direction.y == 0){
                    snake.direction.x = 0;
                    snake.direction.y = -1;
                }
                break;

            case KEY_DOWN:
                if (snake.direction.y == 0){
                    snake.direction.x = 0;
                    snake.direction.y = 1;
                }
                break;
            
            case KEY_LEFT:
                if (snake.direction.x == 0){
                    snake.direction.x = -1;
                    snake.direction.y = 0;
                }
                break;

            case KEY_RIGHT:
                if (snake.direction.x == 0){
                    snake.direction.x = 1;
                    snake.direction.y = 0;
                }
                break;

            case 'q':
                gameOver = 1;
                break; 
        }

        updateGame(&snake, &food, &gameOver);
    }  


        refresh();
        //getch();
        endwin();
        return 0;
}