#include <ncurses.h>
#include <stdio.h>
//#include <stdlib.h>

#define VERTICAL 25
#define HORIZONTAL 80

void input(int arrPast[VERTICAL][HORIZONTAL]);
void output(int arrPast[VERTICAL][HORIZONTAL]);

void speed(int *delay);
void lifeOrDeath(int arrPast[VERTICAL][HORIZONTAL]);

int main() {
    int arrPast[VERTICAL][HORIZONTAL] = {0};
    int delay = 10;

    initscr();       // инициализация ncursis
    noecho();        // отображение вводимых символов
    input(arrPast);  // initializeGame
    if (freopen("/dev/tty", "r", stdin) == NULL) {  // перенаправление ввода
        keypad(stdscr, TRUE);  // включает режим работы с клавишами
    }
    while (1) {
        curs_set(0);  // курсора не будет отображаться
        halfdelay(delay);  // включает режим, устанавливает задержку ввода в полсекунды
        output(arrPast);  // отрисовка
        lifeOrDeath(arrPast);
        speed(&delay);  // настройка скорости
        if (getch() == 'Q' || getch() == 'q') {
            break;
        }
    }
    endwin();  // завершаем работу
    return 0;
}
void lifeOrDeath(int arrPast[VERTICAL][HORIZONTAL]) {
    int arrPresent[VERTICAL][HORIZONTAL] = {0};
    for (int i = 0; i < VERTICAL; i++) {
        for (int j = 0; j < HORIZONTAL; j++) {
            int neighbours = 0;
            for (int n = -1; n <= 1; n++) {
                for (int m = -1; m <= 1; m++) {
                    int x = (i + n + VERTICAL) % VERTICAL;
                    int y = (j + m + HORIZONTAL) % HORIZONTAL;
                    neighbours += arrPast[x][y];
                }
            }
            neighbours -= arrPast[i][j];  //исключаем момент где проверяем клетку на которой находимся
            if (neighbours == 3 && arrPast[i][j] == 0) {
                arrPresent[i][j] = 1;  // рождается
            } else if ((neighbours < 2 || neighbours > 3) && arrPast[i][j] == 1) {
                arrPresent[i][j] = 0;  // умирает
            } else {
                arrPresent[i][j] = arrPast[i][j];
            }
        }
    }
    // Копирование значений из arrPresent обратно в arrPast
    for (int i = 0; i < VERTICAL; i++) {
        for (int j = 0; j < HORIZONTAL; j++) {
            arrPast[i][j] = arrPresent[i][j];
        }
    }
}
// условия жизни клетки
// 1. если у клетки три соседа, она оживает
// 2. если у клетки 2 соседа, она живёт
// 3. во всех остальных случаях она умирает

//генерация мира
void input(int arrPast[VERTICAL][HORIZONTAL]) {
    for (int i = 0; i < VERTICAL; i++) {
        for (int j = 0; j < HORIZONTAL; j++) {
            scanf("%d", &arrPast[i][j]);
        }
    }
}

void output(int arrPast[VERTICAL][HORIZONTAL]) {
    clear();
    for (int i = 0; i < VERTICAL; i++) {
        for (int j = 0; j < HORIZONTAL; j++) {
            move(i, j);  //перемещениие курсора
            if (arrPast[i][j] == 1) {
                printw("X");
            } else {
                printw(".");
            }
        }
    }
    printw("\n To increase the speed, press: + \n To decrease the speed, press: - \n To exit, press: Q");
    refresh();
}

void speed(int *delay) {
    int comand = getch();  //возвращает введённый символ
    if (comand == '+' && *delay > 1) {
        *delay -= 1;
    } else if (comand == '-' && *delay < 15) {
        *delay += 1;
    }
}