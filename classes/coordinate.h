#ifndef COORDINATE_H
#define COORDINATE_H


class Coordinate
{
public:
    Coordinate() { };
    void setCoord(int x,int y){
        this->_x = x;
        this->_y = y;
    };
    int getCoordX() const{
        return this->_x;
    };
    int getCoordY() const{
        return this->_y;
    };
    ~Coordinate() { };

private:

    int _x;
    int _y;

};

#endif // COORDINATE_H
