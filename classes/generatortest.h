#ifndef GENERATORTEST_H
#define GENERATORTEST_H
#include <iostream>
#include <string>
#include <QQueue>
#include "parser.h"
#include "weakestprecondition.h"
#include "parseddata.h"

using namespace std;

class GeneratorTest
{
public:
	/** La estructura que almacena los datos del codigo debe traer identificado que tipo de sentencia posee el algoritmo(IF-THEN / WHILE / IF-ELSE) **/
    GeneratorTest() { }
    void setData(QQueue<string> P,QQueue<string> Q,QQueue<string> I,QQueue<string> C,ParsedData algorithm);
    void getTest(unsigned int Test,QQueue<string> & Output,bool & endProof);
    bool endProof();
    ~GeneratorTest();

private:

	struct _proof
	{
		QQueue<string> test;
        bool view;
		unsigned int num;
		_proof* nextTest;
	};
	/** Genera las formulas pruebas de forma dinamica **/
	_proof* _PROOF;

   /**Tipos de sentencia incluida en el algoritmo a verificar **/
   const unsigned int _IfThen = 1;
   const unsigned int _IfElse = 2;
   const unsigned int _While = 3;

   //Genera un nuevo nodo de ese tipo, una nueva prueba(formula)
   _proof* _newProof(QQueue<string> _test);
   void _insertProof(_proof* & PROOF,_proof* node);
   //Concatena 2 colas con un conector logico que le ingreses
   void _concatenation(QQueue<string> & Q1,QQueue<string> Q2,string Operator);
   /**Pruebas de algoritmos con bucle**/
   void _TestWhile(_proof* & PROOF,QQueue<string> P,QQueue<string> Q,QQueue<string> I,QQueue<string> C,ParsedData algorithm);
   //Este tipo de algoritmo debe verificar las siguientes cinco pruebas
   void _Test1_W(QQueue<string> & result,QQueue<string> P,QQueue<string> I,ParsedData algorithm);//Calculo wp
   void _Test2_W(QQueue<string> & result,QQueue<string> I,ParsedData algorithm);//Calculo wp y condicion B
   void _Test3_W(QQueue<string> & result,QQueue<string> I,QQueue<string> Q,ParsedData algorithm);//Condicion B
   void _Test4_W(QQueue<string> & result,QQueue<string> I,QQueue<string> C,ParsedData algorithm);//Condicion B
   void _Test5_W(QQueue<string> & result,QQueue<string> I,QQueue<string> C,ParsedData algorithm);//Calculo wp y condicion B
   /** Pruebas de algoritmos con sentencia IF-THEN **/
   void _TestIfThen(_proof* & PROOF,QQueue<string> P,QQueue<string> Q,ParsedData algorithm);
   //Este algoritmo debe verificar las siguientes dos pruebas
   void _Test1_IF_T(QQueue<string> & result,QQueue<string> P,QQueue<string> Q,ParsedData algorithm);//Calculo wp y condicion B
   void _Test2_IF_T(QQueue<string> & result,QQueue<string> P,QQueue<string> Q,ParsedData algorithm);//Condicion B
   /** Pruebas de algoritmo con sentencia IF-ELSE **/
   void _TestIfElse(_proof* & PROOF,QQueue<string> P,QQueue<string> Q,ParsedData algorithm);
   //Este algoritmo debe verificar las siguientes tres pruebas
   void _Test1_IF_E(QQueue<string> & result,QQueue<string> P,QQueue<string> Q,ParsedData algorithm);//Calculo wp y condicion B
   void _Test2_IF_E(QQueue<string> & result,QQueue<string> P,QQueue<string> Q,ParsedData algorithm);//Calculo wp y condicion B


   bool _endProof(_proof* proof);

   _proof* _getProof(unsigned int num,unsigned int test,_proof* proof);
};

#endif // GENERATORTEST_H
