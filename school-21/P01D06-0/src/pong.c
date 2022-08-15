#include <stdio.h>


//  Объявление функций
void drawing(int coor_car_l, int coor_car_r, int coor_ball_x, int coor_ball_y,
             int left_score, int right_score);
void clean_screen();
int check_scanf(int coor_l, int coor_r);
int car_motion_left(int result, int coor_l);
int car_motion_right(int result, int coor_r);
int ball_motion_x(int coor_x, int speed_x);
int ball_motion_y(int coor_x, int coor_y, int speed_y);
int ball_reflect_x(int coor_car_l, int coor_car_r, int coor_x, int coor_y,
                   int speed_x);
int ball_reflect_y(int coor_car_l, int coor_car_r, int coor_x, int coor_y,
                   int speed_y);
int score_counter_left(int l_score, int coor_x);
int score_counter_right(int r_score, int coor_x);

// main с циклом, заканчивающимся по break
int main() {
  int coor_l = 13, coor_r = 13, coor_b_x = 39, coor_b_y = 2, speed_x = 1,
      speed_y = 1, left_score = 0, right_score = 0, win_cond = 21;
  while (1) {
    clean_screen();
    drawing(coor_l, coor_r, coor_b_x, coor_b_y, left_score, right_score);
    if (left_score == win_cond) {
      printf("Победил игрок слева! Поздравляем!");
      break;
    }
    if (right_score == win_cond) {
      printf("Победил игрок справа! Поздравляем!");
      break;
    }
    int result = check_scanf(coor_l, coor_r);
    if ((result == 1) || (result == -1)) {
      coor_l = car_motion_left(result, coor_l);
    } else {
      if ((result == 2) || (result == -2)) {
        coor_r = car_motion_right(result, coor_r);
      } else {
        if (result == 3) {
          printf("Игра была завершена до набора нужного количества очков.\n");
          printf("Победила дружба!");
          break;
        }
      }
    }
    speed_x = ball_reflect_x(coor_l, coor_r, coor_b_x, coor_b_y, speed_x);
    speed_y = ball_reflect_y(coor_l, coor_r, coor_b_x, coor_b_y, speed_y);
    coor_b_x = ball_motion_x(coor_b_x, speed_x);
    coor_b_y = ball_motion_y(coor_b_x, coor_b_y, speed_y);
    left_score = score_counter_left(left_score, coor_b_x);
    right_score = score_counter_right(right_score, coor_b_x);
  }
  return 0;
}
// Рисование
void drawing(int coor_car_l, int coor_car_r, int coor_ball_x, int coor_ball_y,
             int left_score, int right_score) {
  int coor_y = 0, field_height = 25, field_width = 80, l_car_pos = 4,
      r_car_pos = 74, scores_pos = 1;
  char empty_cell = ' ', border_cell = '=', left_center_right_cell = '|',
       caret_cell = 'X', ball_cell = 'O';
  while (coor_y < field_height) {
    int coor_x = 0;
    while (coor_x < field_width) {
      if ((coor_y == 0) || (coor_y == field_height - 1)) {
        printf("%c", border_cell);
      } else {
        if (((coor_x == coor_ball_x) && (coor_y == coor_ball_y))) {
          printf("%c", ball_cell);
        } else {
          if ((coor_x == l_car_pos) &&
              ((coor_y == coor_car_l) || (coor_y == coor_car_l + 1) ||
               (coor_y == coor_car_l - 1))) {
            printf("%c", caret_cell);
          } else {
            if (((coor_x == r_car_pos) &&
                 ((coor_y == coor_car_r) || (coor_y == coor_car_r + 1) ||
                  (coor_y == coor_car_r - 1)))) {
              printf("%c", caret_cell);
            } else {
              if ((((coor_x == 36) && (coor_y == scores_pos)))) {
                printf("%d", left_score / 10);
              } else {
                if (((coor_x == 37) && (coor_y == scores_pos))) {
                  printf("%d", left_score % 10);
                } else {
                  if (((coor_x == 41) && (coor_y == scores_pos))) {
                    printf("%d", right_score / 10);
                  } else {
                    if ((coor_x == 42) && (coor_y == scores_pos)) {
                      printf("%d", right_score % 10);
                    } else {
                      if ((coor_x == 39) || (coor_x == 0) ||
                          (coor_x == field_width - 1)) {
                        printf("%c", left_center_right_cell);
                      } else {
                        printf("%c", empty_cell);
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      coor_x += 1;
    }
    printf("%c", '\n');
    coor_y += 1;
  }
}
// Очистка экрана
void clean_screen() { printf("\33c\e[3J"); }
// Считывание символов
int check_scanf(int coor_l, int coor_r) {
  char c;
  scanf("%c", &c);
  if (c == 'a' || c == 'k') {
    if (c == 'a') {
      if (coor_l > 2) {
        return 1;
      } else {
        return 0;
      }
    } else {
      if (coor_r > 2) {
        return 2;
      } else {
        return 0;
      }
    }
  } else if (c == 'z' || c == 'm') {
    if (c == 'z') {
      if (coor_l < 22) {
        return -1;
      } else {
        return 0;
      }
    } else {
      if (coor_r < 22) {
        return -2;
      } else {
        return 0;
      }
    }
  } else {
    if (c == 'b') {
      return 3;
    }
  }
  return 0;
}
// Движение шарика по координате Х
int ball_motion_x(int coor_x, int speed_x) {
  int l_border = 1, r_border = 78;
  int coor_x_1, coor_x_2;
  coor_x_1 = coor_x;
  coor_x_2 = coor_x_1 + speed_x;
  if ((coor_x >= r_border) || (coor_x <= l_border)) {
    return 39;
  } else {
    return coor_x_2;
  }
}

// Движение шарика по координате Y
int ball_motion_y(int coor_x, int coor_y, int speed_y) {
  int coor_y_1, coor_y_2, l_border = 1, r_border = 78;
  coor_y_1 = coor_y;
  coor_y_2 = coor_y_1 + speed_y;
  if ((coor_x >= r_border) || (coor_x <= l_border)) {
    return 2;
  } else {
    return coor_y_2;
  }
}

// Изменение направления шарика из-за отражения по координате Y
int ball_reflect_y(int coor_car_l, int coor_car_r, int coor_x, int coor_y,
                   int speed_y) {
  int l_car_pos = 4, r_car_pos = 74, h_border = 1, b_border = 23;
  if ((coor_y == b_border) || (coor_y == h_border)) {
    speed_y = -speed_y;
  } else {
    if (((coor_x == l_car_pos) &&
         ((coor_y == coor_car_l - 2) || (coor_y == coor_car_l + 2))) ||
        ((coor_x == r_car_pos) &&
         ((coor_y == coor_car_r - 2) || (coor_y == coor_car_r + 2)))) {
      speed_y = -speed_y;
    } else {
    }
  }
  return speed_y;
}

// Изменение направления шарика из-за отражения по координате Х
int ball_reflect_x(int coor_car_l, int coor_car_r, int coor_x, int coor_y,
                   int speed_x) {
  int l_car_pos = 4, r_car_pos = 74, l_border = 1, r_border = 78;
  if ((coor_x == l_car_pos + 1) &&
      ((coor_y == coor_car_l - 1) || (coor_y == coor_car_l) ||
       (coor_y == coor_car_l + 1))) {
    speed_x = -speed_x;
  } else {
    if ((coor_x == r_car_pos - 1) &&
        ((coor_y == coor_car_r - 1) || (coor_y == coor_car_r) ||
         (coor_y == coor_car_r + 1))) {
      speed_x = -speed_x;
    } else {
      if (((coor_x == l_car_pos + 1) &&
           ((coor_y == coor_car_l - 2) || (coor_y == coor_car_l + 2))) ||
          ((coor_x == r_car_pos - 1) &&
           ((coor_y == coor_car_r - 2) || (coor_y == coor_car_r + 2)))) {
        speed_x = -speed_x;
      } else {
        if (coor_x == l_border) {
          speed_x = -speed_x;
        } else {
          if (coor_x == r_border) {
            speed_x = -speed_x;
          }
        }
      }
    }
  }

  return speed_x;
}

// Подсчет очков игрока слева
int score_counter_left(int l_score, int coor_x) {
  int r_border = 78;
  if (coor_x == r_border) {
    l_score += 1;
  }
  return l_score;
}

// Подсчет очков игрока справа
int score_counter_right(int r_score, int coor_x) {
  int l_border = 1;
  if (coor_x == l_border) {
    r_score += 1;
  }
  return r_score;
}

// Движение каретки слева
int car_motion_left(int result, int coor_l) {
  if (result == 1) {
    return --coor_l;
  } else {
    if (result == -1) {
      return ++coor_l;
    }
  }
  return coor_l;
}

// Движение каретки справаы
int car_motion_right(int result, int coor_r) {
  if (result == 2) {
    return --coor_r;
  } else {
    if (result == -2) {
      return ++coor_r;
    }
  }
  return coor_r;
}
