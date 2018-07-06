#ifndef DATA_H
#define DATA_H
#include <string>
#include <QString>
class data
{
public:
    data() { };

    void setData(std::string formula,int x,int y, bool isLiteral,int xf, int yf){
        this->_formula = formula;
        this->_X = x;
        this->_Y = y;
        this->_yf = yf;
        this->_xf = xf;
        this->_isLiteral = isLiteral;
    }

    int getCoordX() const{ return this->_X; }
    int getCoordY() const { return this->_Y;}
    int getFatherCoordX() const{ return this->_xf;}
    int getFatherCoordY() const{ return this->_yf;}
    QString getFormula() {
      char output[this->_formula.size()];
      this->_conversion(output,this->_formula);
      QString formula(output);
      return formula;
    }
    bool isLiteral() const { return this->_isLiteral;}
    ~data() { };

private:

    std::string _formula;
    int _X;
    int _Y;
    int _xf;
    int _yf;
    bool _isLiteral;

    void _conversion(char output[],std::string input){
        unsigned int pos = 0;

        while(pos <= input.size()){
            output[pos] = input[pos];
            pos++;
        }
    };
};

#endif // DATA_H
