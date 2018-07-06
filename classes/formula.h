#ifndef FORMULA_H
#define FORMULA_H

#include <iostream>
#include <string>
#include <QList>
#include <QQueue>


class Formula {
public:
    Formula(QQueue<std::string>  tokens,std::string ConstU,std::string ConstE,bool & E);
    ~Formula();
    void setClauses(QQueue<std::string> & tokens,bool & E);
    const unsigned int getType()const;
    void getFirstOperant(QQueue<std::string> & Q);
    void getSecondOperant(QQueue<std::string> & Q);

    void printOperands();

private:
    QQueue<std::string> _firstOperand;
    QQueue<std::string> _secondOperand;
    //guarda constante utilizadas en la simplificacion del cuantificador EXISTENCIAL
    QQueue<std::string> _auxConstant;

    std::string _ConstU;//Nuevo
    std::string _ConstE;//Nuevo

   // QQueue<std::string>  _inputForm;
    
    bool isFirstOperatorNegated;
    bool isSecondOperatorNegated;
    
    unsigned int _resolutionType;

     unsigned int _beforeJump;

	const std::string _NOT       = "!";
	const std::string _AND       = "&&";
	const std::string _OR        = "||";
	const std::string _IMPLIES   = "->";
	const std::string _IFF       = "<->";
	const std::string _OPEN_PAR  = "(";
	const std::string _CLOSE_PAR = ")";
    const std::string _CuantUNIV = "@";
    const std::string _CuantEXIST = "#";

    /**
     * Criterios para separar. {AND|OR}.
     */
    const unsigned int _REGLA_OR  = 1;
    const unsigned int _REGLA_AND = 2;
    //NUEVO
    const unsigned int _REGLA_AND_CUANT = 3;
    const unsigned int _REGLA_LITERAL = 4;

    const unsigned int _AND_CODE     = 0;
    const unsigned int _OR_CODE      = 1;
    const unsigned int _IMPLIES_CODE = 2;
    const unsigned int _IFF_CODE     = 3;

        std::string __operator;

    /**
     * Simplificaciones de fórmulas. [Helpers].
     */
    void _applyInvolution(QQueue<std::string> & tokens);
    void _applyAnd(QQueue<std::string> & tokens);
    void _applyOr(QQueue<std::string> & tokens);
    void _applyNotOr(QQueue<std::string> & tokens);
    void _applyNotAnd(QQueue<std::string> & tokens);
    void _applyImplication(QQueue<std::string> & tokens);
    void _applyNotImplication(QQueue<std::string> & tokens);

    bool _isOneExpression(QQueue<std::string> tokens);
    void _change(std::string _operator);
    void _add(std::string _element, bool jump, bool isOperator, bool prevCuant);
    void _negOperators(bool negFirstOperand, bool negSecondOperand);
    void _neg(QQueue<std::string> & operand);
   	bool _isLiteral(QQueue<std::string> operand)const;
   	void _addParIfNot( QQueue<std::string> & operand );
   	void _getNextExpression(QQueue<std::string> nextExpression);
   	void _checkPar();
    void _applyIff();
    std::string _negLiteral(std::string predicate);
    std::string _stringQueue(QQueue< char > queue);
    bool _totalQuantification(QQueue< std::string> tokens);
   	void _print(QQueue<std::string> op);
    void _negCuant(QQueue<std::string> & tokens);
    bool _Literal(std::string token);
    void _replacement(std::string & output,std::string variable, std::string constant);//NUEVO PARA REEMPLAZAR LAS CUANTIFICACIONES
};

#endif // FORMULA_H
