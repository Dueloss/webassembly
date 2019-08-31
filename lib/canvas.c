#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emscripten.h>

#define NUM_CIRCLES 500

struct Circle
{
  int x;
  int y;
  int r;
  int cr;
  int cg;
  int cb;
};

struct CircleAnimationData
{
  int x;
  int y;
  int r;
  int xv;
  int yv;
  int xd;
  int yd;
};

struct Circle circles[NUM_CIRCLES];
struct CircleAnimationData animationData[NUM_CIRCLES];


int getRend(min, max){
  return ((rand() % max) + min);
}

int main(){
srand(time(NULL));
  for(int i=0; i<NUM_CIRCLES; i++){
    int radius = getRend(1, 50);

    int x = getRend(0, 1920) + radius;
    int y = getRend(0, 1080) + radius;

    animationData[i].x = x;
    animationData[i].y = y;
    animationData[i].r = radius;
    animationData[i].xv = getRend(1, 10);
    animationData[i].yv = getRend(1, 10);
    animationData[i].xd = getRend(1, 2) - 1;
    animationData[i].yd = getRend(1, 2) - 1;
    circles[i].x = x;
    circles[i].y = y;
    circles[i].r = radius;
    circles[i].cr = getRend(0, 255);
    circles[i].cg = getRend(0, 255);
    circles[i].cb = getRend(0, 255);
  }
  EM_ASM({render($0, $1);}, NUM_CIRCLES*6, 6);
}

struct Circle * getCircles(int canvasWidth, int canvasHeight)
{
  for(int i = 0; i < NUM_CIRCLES; i++){
    if ((animationData[i].x + animationData[i].r) >= canvasWidth)
    {
      animationData[i].xv = getRend(1, 10);
      animationData[i].xd = 0;
    }
    if ((animationData[i].x - animationData[i].r) <= 0)
    {
      animationData[i].xv = getRend(1, 10);
      animationData[i].xd = 1;
    }
    if ((animationData[i].y + animationData[i].r) >= canvasHeight)
    {
      animationData[i].yv = getRend(1, 10);
      animationData[i].yd = 0;
    }
    if ((animationData[i].y - animationData[i].r) <= 0)
    {
      animationData[i].yv = getRend(1, 10);
      animationData[i].yd = 1;
    }

    if (animationData[i].xd == 1)
    {
      animationData[i].x += animationData[i].xv;
    } else
    {
        animationData[i].x -= animationData[i].xv;
    }
    if (animationData[i].yd == 1)
    {
      animationData[i].y += animationData[i].yv;
    } else
    {
        animationData[i].y -= animationData[i].yv;
    }

    circles[i].x = animationData[i].x;
    circles[i].y = animationData[i].y;
  }
  return circles;
}