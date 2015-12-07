/*
  Player
  Allan Legemaate
  21/11/15
  The controllable player
*/

#include <allegro.h>

#ifndef PLAYER_H
#define PLAYER_H


class player{
  public:
    player(int newX, int newY, int newZ);
    virtual ~player();
    int Getx() { return x; }
    void Setx(int val) { x = val; }
    int Gety() { return y; }
    void Sety(int val) { y = val; }
    int Getz() { return z; }
    void Setz(int val) { z = val; }

    void draw( BITMAP *tempBuffer, int zoom, int offsetX, int offsetY);
    void logic();
  protected:
  private:
    int x;
    int y;
    int z;

    BITMAP *image[6];
};

#endif // PLAYER_H
