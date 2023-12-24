#include <stdio.h>
#include <stdlib.h>

const int field_height = 20;
const int field_width = 50;
typedef struct
{
  int x;
  int y;
} vector_2d_t;

void sleep()
{
#ifdef _WIN32
  system("timeout 1");
#else
  system("sleep 0.09");
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

void update_ball(vector_2d_t *ball, vector_2d_t *new_ball, vector_2d_t *ball_dir)
{
  new_ball->x = ball->x;
  new_ball->y = ball->y;
  new_ball->x += ball_dir->x;
  new_ball->y += ball_dir->y;

  if (new_ball->x <= 0 || new_ball->x >= field_width)
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

int main()
{
  vector_2d_t ball = {.x = 10, .y = 10};
  vector_2d_t new_ball = {.x = ball.x, .y = ball.y};
  vector_2d_t ball_dir = {.x = 1, .y = 1};

  char *display = (char *)malloc(2 * field_height * field_width * sizeof(char));
  if (display == NULL)
  {
    printf("Could not allocate enough memory!\n");
    exit(1);
  }

  // Gameloop
  for (int i = 0; i < 200; i++)
  {

    update_ball(&ball, &new_ball, &ball_dir);

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
            display[write_index++] = ' ';
          }
        }
      }
      display[write_index++] = '\n';
    }
    display[write_index++] = '\0';
    sleep();
    clear();
    printf("%s\n", display);
  }
  return 0;
}
