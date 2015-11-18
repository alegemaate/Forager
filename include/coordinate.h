

#ifndef COORDINATE_H
#define COORDINATE_H

class coordinate{
  public:
    coordinate(int newX, int newY, int newZ);
    virtual ~coordinate();
    int getX() { return x; }
    void setX(int val) { x = val; }
    int getY() { return y; }
    void setY(int val) { y = val; }
    int getZ() { return z; }
    void setZ(int val) { z = val; }
  protected:
  private:
    int x;
    int y;
    int z;
};

#endif // COORDINATE_H
