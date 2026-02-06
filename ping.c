#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_SIZE 3
#define WIN_SCORE 21

void clear_screen(void) {
    printf("\033[H\033[J");
}

void draw_field(int ball_x, int ball_y, int paddle1_y, int paddle2_y) {
    /* Верхняя граница */
    for (int i = 0; i < WIDTH + 2; ++i) {
        printf("#");
    }
    printf("\n");
    
    /* Игровое поле */
    for (int y = 0; y < HEIGHT; ++y) {
        printf("#"); /* Левая граница */
        
        for (int x = 0; x < WIDTH; ++x) {
            int is_ball = (x == ball_x && y == ball_y);
            int is_paddle1 = (x == 1 && y >= paddle1_y && y < paddle1_y + PADDLE_SIZE);
            int is_paddle2 = (x == WIDTH - 2 && y >= paddle2_y && y < paddle2_y + PADDLE_SIZE);
            
            /* Если мяч и ракетка в одной ячейке - показываем мяч */
            if (is_ball) {
                printf("O");
            } else if (is_paddle1 || is_paddle2) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        
        printf("#\n"); /* Правая граница */
    }
    
    /* Нижняя граница */
    for (int i = 0; i < WIDTH + 2; ++i) {
        printf("#");
    }
    printf("\n");
}

void print_winner(int winner, int score1, int score2) {
    printf("\n");
    printf("########################################\n");
    if (winner == 1) {
        printf("   ПОБЕДИТЕЛЬ: ИГРОК 1! Поздравляем!\n");
    } else {
        printf("   ПОБЕДИТЕЛЬ: ИГРОК 2! Поздравляем!\n");
    }
    printf("   Финальный счет: %d - %d\n", score1, score2);
    printf("########################################\n");
}

int main(void) {
    /* Координаты мяча */
    int ball_x = WIDTH / 2;
    int ball_y = HEIGHT / 2;
    int ball_dx = 1;
    int ball_dy = 0;  /* Начинаем с горизонтального движения */
    
    /* Координаты ракеток */
    int paddle1_y = HEIGHT / 2 - PADDLE_SIZE / 2;
    int paddle2_y = HEIGHT / 2 - PADDLE_SIZE / 2;
    
    /* Счет */
    int score1 = 0;
    int score2 = 0;
    
    /* Текущий игрок (1 - левый, 2 - правый) */
    int current_player = 1;
    int turn_completed = 0;
    
    /* Переменная для ввода */
    char input;
    
    /* Основной игровой цикл */
    while (score1 < WIN_SCORE && score2 < WIN_SCORE) {
        clear_screen();
        draw_field(ball_x, ball_y, paddle1_y, paddle2_y);
        
        printf("Счет: %d - %d\n", score1, score2);
        printf("Ход игрока %d. Введите команду: ", current_player);
        
        /* Сброс флага завершения хода */
        turn_completed = 0;
        
        /* Ожидание корректного ввода от текущего игрока */
        while (!turn_completed) {
            input = getchar();
            
            /* Пропускаем перевод строки */
            if (input == '\n') {
                continue;
            }
            
            /* Обработка ввода текущего игрока */
            if (current_player == 1) {
                if (input == 'a' || input == 'A') {
                    if (paddle1_y > 0) {
                        --paddle1_y;
                    }
                    turn_completed = 1;
                } else if (input == 'z' || input == 'Z') {
                    if (paddle1_y < HEIGHT - PADDLE_SIZE) {
                        ++paddle1_y;
                    }
                    turn_completed = 1;
                } else if (input == ' ') {
                    turn_completed = 1;
                } else if (input == 'k' || input == 'K' || input == 'm' || input == 'M') {
                    printf("Сейчас ход игрока 1! Ваши клавиши: A/Z/SPACE\n");
                    printf("Ввод: ");
                    fflush(stdout);
                } else {
                    printf("Неверная команда! Для игрока 1: A/Z/SPACE\n");
                    printf("Ввод: ");
                    fflush(stdout);
                }
            } else { /* current_player == 2 */
                if (input == 'k' || input == 'K') {
                    if (paddle2_y > 0) {
                        --paddle2_y;
                    }
                    turn_completed = 1;
                } else if (input == 'm' || input == 'M') {
                    if (paddle2_y < HEIGHT - PADDLE_SIZE) {
                        ++paddle2_y;
                    }
                    turn_completed = 1;
                } else if (input == ' ') {
                    turn_completed = 1;
                } else if (input == 'a' || input == 'A' || input == 'z' || input == 'Z') {
                    printf("Сейчас ход игрока 2! Ваши клавиши: K/M/SPACE\n");
                    printf("Ввод: ");
                    fflush(stdout);
                } else {
                    printf("Неверная команда! Для игрока 2: K/M/SPACE\n");
                    printf("Ввод: ");
                    fflush(stdout);
                }
            }
            
            /* Пропускаем оставшиеся символы в буфере */
            while (getchar() != '\n') {
                /* Пустое тело цикла */
            }
        }
        
        /* ВРЕМЕННО сохраняем новую позицию мяча */
        int new_ball_x = ball_x + ball_dx;
        int new_ball_y = ball_y + ball_dy;
        
        /* Проверка столкновения с верхней/нижней границами */
        if (new_ball_y < 0) {
            ball_dy = -ball_dy;
            new_ball_y = 0;
        } else if (new_ball_y >= HEIGHT) {
            ball_dy = -ball_dy;
            new_ball_y = HEIGHT - 1;
        }
        
        /* Проверка столкновения с левой ракеткой */
        if (new_ball_x == 1) {
            if (new_ball_y >= paddle1_y && new_ball_y < paddle1_y + PADDLE_SIZE) {
                /* Отскок от ракетки с изменением угла */
                ball_dx = -ball_dx;
                
                /* Угол отскока зависит от того, в какую часть ракетки попал мяч */
                int hit_pos = new_ball_y - paddle1_y;
                if (hit_pos == 0) { /* Верхняя часть ракетки */
                    ball_dy = -1;
                } else if (hit_pos == 1) { /* Центр ракетки */
                    ball_dy = 0;
                } else if (hit_pos == 2) { /* Нижняя часть ракетки */
                    ball_dy = 1;
                }
                
                /* Корректируем позицию, чтобы мяч не был в ракетке */
                new_ball_x = 2;
            } else {
                /* Мяч прошел мимо ракетки - гол! */
                ++score2;
                /* Сброс мяча */
                new_ball_x = WIDTH / 2;
                new_ball_y = HEIGHT / 2;
                ball_dx = 1;
                ball_dy = 0;
            }
        }
        
        /* Проверка столкновения с правой ракеткой */
        if (new_ball_x == WIDTH - 2) {
            if (new_ball_y >= paddle2_y && new_ball_y < paddle2_y + PADDLE_SIZE) {
                /* Отскок от ракетки с изменением угла */
                ball_dx = -ball_dx;
                
                /* Угол отскока зависит от того, в какую часть ракетки попал мяч */
                int hit_pos = new_ball_y - paddle2_y;
                if (hit_pos == 0) { /* Верхняя часть ракетки */
                    ball_dy = -1;
                } else if (hit_pos == 1) { /* Центр ракетки */
                    ball_dy = 0;
                } else if (hit_pos == 2) { /* Нижняя часть ракетки */
                    ball_dy = 1;
                }
                
                /* Корректируем позицию, чтобы мяч не был в ракетке */
                new_ball_x = WIDTH - 3;
            } else {
                /* Мяч прошел мимо ракетки - гол! */
                ++score1;
                /* Сброс мяча */
                new_ball_x = WIDTH / 2;
                new_ball_y = HEIGHT / 2;
                ball_dx = -1;
                ball_dy = 0;
            }
        }
        
        /* Проверка выхода за левую границу (гол игроку 2) */
        if (new_ball_x < 0) {
            ++score2;
            new_ball_x = WIDTH / 2;
            new_ball_y = HEIGHT / 2;
            ball_dx = 1;
            ball_dy = 0;
        }
        
        /* Проверка выхода за правую границу (гол игроку 1) */
        if (new_ball_x >= WIDTH - 1) {
            ++score1;
            new_ball_x = WIDTH / 2;
            new_ball_y = HEIGHT / 2;
            ball_dx = -1;
            ball_dy = 0;
        }
        
        /* Применяем вычисленную позицию */
        ball_x = new_ball_x;
        ball_y = new_ball_y;
        
        /* Смена игрока */
        current_player = (current_player == 1) ? 2 : 1;
    }
    
    /* Финальный экран */
    clear_screen();
    draw_field(ball_x, ball_y, paddle1_y, paddle2_y);
    
    if (score1 >= WIN_SCORE) {
        print_winner(1, score1, score2);
    } else {
        print_winner(2, score1, score2);
    }
    
    printf("\nНажмите Enter для выхода...\n");
    while (getchar() != '\n') {
        /* Пустое тело цикла */
    }
    
    return 0;
}