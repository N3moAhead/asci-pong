#include <stdio.h>
#include <stdlib.h>

const int field_height = 20;
const int field_width = 100;
typedef struct
{
  int x;
  int y;
} vector_2d_t;

typedef struct
{
  int score;
  vector_2d_t pos;
  int height;
} player_t;

void sleep()
{
#ifdef _WIN32
  system("timeout 1");
#else
  system("sleep 0.07");
#endif
}

void clear()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

/**
 * Check if a player should be drawn at the current position
*/
char draw_player(int x, int y, player_t *player1, player_t *player2)
{
  // Left Player
  if ((y >= player1->pos.y && y <= (player1->pos.y + player1->height)) && player1->pos.x == x) {
    return 1;
  }
  // Right Player
  if ((y >= player2->pos.y && y <= (player2->pos.y + player2->height)) && player2->pos.x == x) {
    return 1;
  }
  return 0;
}

/**
 * Check if the ball hitted one of the players
*/
char player_hit(vector_2d_t *ball, player_t *player1, player_t *player2)
{
  // Left Player
  if (ball->x == 2 && ball->y >= player1->pos.y && ball->y <= player1->pos.y + player1->height)
    return 1;
  // Right Player
  if (ball->x == field_width - 3 && ball->y >= player2->pos.y && ball->y <= player2->pos.y + player2->height)
    return 1;
  return 0;
}

void update_ball(vector_2d_t *ball, vector_2d_t *new_ball, vector_2d_t *ball_dir, player_t *player1, player_t *player2)
{
  new_ball->x = ball->x;
  new_ball->y = ball->y;
  new_ball->x += ball_dir->x;
  new_ball->y += ball_dir->y;

  if (new_ball->x <= 0 || new_ball->x >= field_width || player_hit(ball, player1, player2))
  {
    ball_dir->x *= -1;
  }
  if (new_ball->y <= 0 || new_ball->y >= field_height)
  {
    ball_dir->y *= -1;
  }

  ball->x += ball_dir->x;
  ball->y += ball_dir->y;
}

void update_score(vector_2d_t *ball, player_t *player1, player_t *player2)
{
  if (ball->x == 1)
  {
    player1->score++;
  }
  if (ball->x == field_width - 1)
  {
    player2->score++;
  }
}

void print_score(player_t *player1, player_t *player2)
{
  int offset = (field_width / 2) - 5;
  for (int i = 0; i < offset; i++)
    printf(" ");
  printf("%d - %d\n", player1->score, player2->score);
}

int main()
{
  vector_2d_t ball = {.x = 10, .y = 2};
  vector_2d_t new_ball = {.x = ball.x, .y = ball.y};
  vector_2d_t ball_dir = {.x = 1, .y = 0};

  player_t player1 = {.score = 0, .pos.x = 1, .height = 4};
  player_t player2 = {.score = 0, .pos.x = field_width - 2, .height = 4};
  player1.pos.y = 2;
  player2.pos.y = 2;

  char *display = (char *)malloc(2 * field_height * field_width * sizeof(char));
  if (display == NULL)
  {
    printf("Could not allocate enough memory!\n");
    exit(1);
  }

  // Gameloop
  for (int i = 0; i < 200; i++)
  {

    update_ball(&ball, &new_ball, &ball_dir, &player1, &player2);
    update_score(&ball, &player1, &player2);

    int write_index = 0;
    for (int y = 0; y < field_height; y++)
    {
      for (int x = 0; x < field_width; x++)
      {
        if (y == 0 || y == field_height - 1 || x == 0 || x == field_width - 1)
        {
          display[write_index++] = '#';
        }
        else
        {
          if (y == ball.y && x == ball.x)
          {
            display[write_index++] = '0';
          }
          else
          {
            if (draw_player(x, y, &player1, &player2))
            {
              display[write_index++] = '|';
            }
            else
            {
              display[write_index++] = ' ';
            }
          }
        }
      }
      display[write_index++] = '\n';
    }
    display[write_index++] = '\0';
    sleep();
    clear();
    print_score(&player1, &player2);
    printf("%s\n", display);
  }
  free(display);
  return 0;
}
