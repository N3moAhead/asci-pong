#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int field_height = 20;
const int field_width = 70;
typedef struct
{
  int x;
  int y;
} vector_2d_t;

typedef struct
{
  vector_2d_t pos;
  vector_2d_t dir;
} ball_t;

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
 * Determine wether a player is on the left or the right side
 */
char is_left_player(player_t *player)
{
  if (player->pos.x < field_width / 2)
    return 1;
  return 0;
}

void move_player_to_height(player_t *player, int height) {
  // height above the player
  if (player->pos.y + player->height + 1 < height)
  {
    player->pos.y++;
  }
  // height below the player
  if (player->pos.y > height)
  {
    player->pos.y--;
  }
}

/**
 * Trying to predict where the ball will hit while ignoring direction by
 * hitting the upper or lower walls.
*/
void update_player_lvl2(player_t *player, ball_t *ball)
{
  int distance_to_wall = ball->pos.x - 2;
  int wall_hit = (ball->pos.y + (ball->dir.y * distance_to_wall)) % field_height + 1;
  wall_hit = wall_hit < 0 ? wall_hit * -1 : wall_hit;

  move_player_to_height(player, wall_hit);
}

/**
 * The player will try to always match the height of the ball
 */
void update_player_lvl1(player_t *player, ball_t *ball)
{
  move_player_to_height(player, ball->pos.y);
}

/**
 * Check if a player should be drawn at the current position
 */
char draw_player(int x, int y, player_t *player1, player_t *player2)
{
  // Left Player
  if ((y >= player1->pos.y && y <= (player1->pos.y + player1->height)) && player1->pos.x == x)
  {
    return 1;
  }
  // Right Player
  if ((y >= player2->pos.y && y <= (player2->pos.y + player2->height)) && player2->pos.x == x)
  {
    return 1;
  }
  return 0;
}

/**
 * Check if the ball hitted one of the players
 */
char player_hit(ball_t *ball, player_t *player1, player_t *player2)
{
  // Left Player
  if (ball->dir.x == -1 && ball->pos.x == 2 && ball->pos.y >= player1->pos.y && ball->pos.y <= player1->pos.y + player1->height)
    return 1;
  // Right Player
  if (ball->dir.x == 1 && ball->pos.x == field_width - 3 && ball->pos.y >= player2->pos.y && ball->pos.y <= player2->pos.y + player2->height)
    return 1;
  return 0;
}

void update_ball(ball_t *ball, vector_2d_t *new_ball, player_t *player1, player_t *player2)
{
  new_ball->x = ball->pos.x + ball->dir.x;
  new_ball->y = ball->pos.y + ball->dir.y;

  // Checking if the ball has to turn direction
  if (new_ball->x <= 0 || new_ball->x >= field_width || player_hit(ball, player1, player2))
  {
    ball->dir.x *= -1;
  }
  if (new_ball->y <= 0 || new_ball->y >= field_height)
  {
    ball->dir.y *= -1;
  }

  // Updating the ball position
  ball->pos.x += ball->dir.x;
  ball->pos.y += ball->dir.y;
}

void update_score(ball_t *ball, player_t *player1, player_t *player2)
{
  if (ball->pos.x == 1)
  {
    player1->score++;
  }
  if (ball->pos.x == field_width - 1)
  {
    player2->score++;
  }
}

void init_ball(ball_t *ball)
{
  ball->pos.x = field_width / 2;
  ball->pos.y = field_height / 2;
  ball->dir.x = rand() % 2 ? -1 : 1;
  ball->dir.y = rand() % 2 ? -1 : 1;
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
  srand(time(NULL));

  ball_t ball;
  init_ball(&ball);
  vector_2d_t new_ball = {.x = ball.pos.x, .y = ball.pos.y};

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
  for (int i = 0; player1.score < 3 && player2.score < 3; i++)
  {

    update_ball(&ball, &new_ball, &player1, &player2);
    update_score(&ball, &player1, &player2);

    /** Update the player on every fith render */
    if (i % 5 == 0)
    {
      update_player_lvl2(&player1, &ball);
      update_player_lvl1(&player2, &ball);
    }

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
          if (y == ball.pos.y && x == ball.pos.x)
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

  if (player1.score < 3) {
    printf("Right Player Won!\n");
  } else {
    printf("Left Player Won!\n");
  }

  free(display);
  return 0;
}
