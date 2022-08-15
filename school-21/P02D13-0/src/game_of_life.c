#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define DEAD ' '  // Пробел для мертвой клетки
#define ALIVE '#'  // Решетка для живой клетки
#define INPUT_ALIVE '#'  // Символ для живой клетки при чтении нач. кофигурации

void game_main(char screen1[WIDTH][HEIGHT], char screen2[WIDTH][HEIGHT],
               int delayms);
void draw(char screen[WIDTH][HEIGHT]);
void compute(char from[WIDTH][HEIGHT], char to[WIDTH][HEIGHT]);
void empty(char screen[WIDTH][HEIGHT]);
void input(char screen[WIDTH][HEIGHT]);
int load_from_file(char *filename, char screen[WIDTH][HEIGHT]);

// Варианты запуска
// /a.out 120 file.txt       - Задержка 120мс, Конфигурация в file.txt
// /a.out 120                - Задержка 120мс, Конфигурация из stdin
// /a.out                    - Задержка обычная (100мс), Конфигурация из stdin
int main(int argc, char *argv[]) {
    char screen1[WIDTH][HEIGHT], screen2[WIDTH][HEIGHT];
    int err = 0;  // Флаг ошибки
    int delayms = 100;  // Задержка между кадрами в миллисекундах

    // Зануляем экраны
    empty(screen1);
    empty(screen2);

    // Обработка аргументов и заполнение ввода
    if (argc == 3) {  // "/a.out 120 file.txt" (3 аргумента)
        delayms = atoi(argv[1]);
        err |= delayms <= 0;
        if (err) {
            printf("Неверное значение скорости! (Целое число > 0)");
        } else {
            err |= load_from_file(argv[2], screen1);
            if (err) printf("Файл \"%s\" не существует!", argv[2]);
        }

    } else if (argc == 2) {  // "/a.out 120" (2 аргумента)
        delayms = atoi(argv[1]);
        err |= delayms <= 0;
        if (err) printf("Неверное значение скорости! (Целое число > 0)");
        input(screen1);

    } else {  // "/a.out" (1 аргумент)
        input(screen1);
    }

    if (!err) game_main(screen1, screen2, delayms);
    return 0;
}

// Главный цикл
void game_main(char screen1[WIDTH][HEIGHT], char screen2[WIDTH][HEIGHT],
               int delayms) {
    char day_flag = 1;  // Флаг четности дня

    // Отрисовать начальную конфигурацию и ждать ввод
    draw(screen1);
    getchar();

    // Главный цикл
    while (1) {
        if (day_flag) {
            compute(screen1, screen2);
            draw(screen2);
        } else {
            compute(screen2, screen1);
            draw(screen1);
        }

        // Задержка и переключение четности
        usleep(delayms * 1000);
        day_flag = !day_flag;
    }
}

// Отрисовать поле
void draw(char screen[WIDTH][HEIGHT]) {
    printf("\e[H\e[2J");  // Очистить экран, перевести курсор в начало
    for (int x, y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            printf("%c", screen[x][y] ? ALIVE : DEAD);
        }
        printf("\n");
    }
}

// Занулить экран
void empty(char screen[WIDTH][HEIGHT]) {
    for (int j, i = 0; i < WIDTH; i++)
        for (j = 0; j < HEIGHT; j++) screen[i][j] = 0;
}

// Для каждой клетки подсчитать соседе и применить правила,
// записать в следующую конфигурацию
void compute(char from[WIDTH][HEIGHT], char to[WIDTH][HEIGHT]) {
    for (int sum, j, i = 0; i < WIDTH; i++)
        for (j = 0; j < HEIGHT; j++) {
            // Подсчет суммы соседей
            sum = 0;
            sum += from[i][(j - 1 + HEIGHT) % HEIGHT];
            sum += from[i][(j + 1 + HEIGHT) % HEIGHT];
            sum += from[(i - 1 + WIDTH) % WIDTH][j];
            sum += from[(i + 1 + WIDTH) % WIDTH][j];
            sum += from[(i - 1 + WIDTH) % WIDTH][(j - 1 + HEIGHT) % HEIGHT];
            sum += from[(i + 1 + WIDTH) % WIDTH][(j - 1 + HEIGHT) % HEIGHT];
            sum += from[(i - 1 + WIDTH) % WIDTH][(j + 1 + HEIGHT) % HEIGHT];
            sum += from[(i + 1 + WIDTH) % WIDTH][(j + 1 + HEIGHT) % HEIGHT];

            // Применение правил
            if ((sum < 2) || (sum > 3))
                to[i][j] = 0;
            else if (sum == 3)
                to[i][j] = 1;
            else
                to[i][j] = from[i][j];
        }
}

// Читаем с stdin начальную конфигурацию
void input(char screen[WIDTH][HEIGHT]) {
    char ch = 0;
    for (int x, y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH;) {
            scanf("%c", &ch);
            if (ch != '\n' && ch != '\r') {  // Игнорируем переносы строк
                screen[x][y] = (ch == INPUT_ALIVE) ? 1 : 0;
                x++;
            }
        }
    }
}

// Читаем начальную конфигурацию с файла из аргументов
int load_from_file(char *filename, char screen[WIDTH][HEIGHT]) {
    int err = 0;
    FILE *file;
    file = fopen(filename, "r");  // Открываем файл на чтение
    err |= file == NULL;

    if (!err) {
        char ch = 0;
        for (int x, y = 0; y < HEIGHT; y++) {
            for (x = 0; x < WIDTH;) {
                fscanf(file, "%c", &ch);
                if (ch != '\n' && ch != '\r') {
                    screen[x][y] = (ch == INPUT_ALIVE) ? 1 : 0;
                    x++;
                }
            }
        }
    }

    fclose(file);  // Закрываем файл

    return err;
}
