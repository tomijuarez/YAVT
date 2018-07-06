#include "generatortest.h"

void GeneratorTest::setData(QQueue<string> P,QQueue<string> Q,QQueue<string> I,QQueue<string> C,ParsedData algorithm)
{  
	//SUPONGO EL IDENTIFICADOR DE LA SENTENCIA DEL ALGORITMO ::expresion::
    this->_PROOF = NULL;
    if((algorithm.isIf()) && (! algorithm.elseFound()))
	{
		this->_TestIfThen(this->_PROOF,P,Q,algorithm);
	}
	else
    if( (algorithm.isIf()) && (algorithm.elseFound()) )
	{	
        this->_TestIfElse(this->_PROOF,P,Q,algorithm);
	}
	else
    if(algorithm.isWhile())
	{

        this->_TestWhile(this->_PROOF,P,Q,I,C,algorithm);

	}

}

void GeneratorTest::_concatenation(QQueue<string> & Q1,QQueue<string> Q2,string Operator){
	
	Q1.enqueue(Operator);
	while(! Q2.empty()){
		Q1.enqueue(Q2.dequeue());
	}
}
/** Métodos referidos al creacion dinamica de pruebas**/
GeneratorTest::_proof* GeneratorTest::_newProof(QQueue<string> _test){
 		_proof* node = new _proof;
 		node->test = _test;
 		node->nextTest = NULL;		 
}

//inserta el nodo al final de la lista
void GeneratorTest::_insertProof(_proof* & PROOF,_proof* node){

    if(PROOF == NULL)
	{
		PROOF = node;
	}
	else
	{
		this->_insertProof(PROOF->nextTest,node);
	}
}

/** Pruebas de la verficacion formal de algoritmos **/
void GeneratorTest::_TestIfThen(_proof* & PROOF,QQueue<string> P,QQueue<string> Q,ParsedData algorithm){
	
	QQueue<string> resultForm;

	this->_Test1_IF_T(resultForm,P,Q,algorithm);
	_proof* newP = this->_newProof(resultForm);
	newP->num = 1;
	this->_insertProof(PROOF,newP);
	resultForm.clear();

	this->_Test2_IF_T(resultForm,P,Q,algorithm);
	newP = this->_newProof(resultForm);
	newP->num = 2;
	this->_insertProof(PROOF,newP);
	resultForm.clear();
	
}

void GeneratorTest::_TestIfElse(_proof* & PROOF,QQueue<string> P,QQueue<string> Q,ParsedData algorithm){
	QQueue<string> resultForm;

    this->_Test1_IF_E(resultForm,P,Q,algorithm);
	_proof* newP = this->_newProof(resultForm);
	newP->num = 1;
	this->_insertProof(PROOF,newP);
	resultForm.clear();
	
    this->_Test2_IF_E(resultForm,P,Q,algorithm);
	newP = this->_newProof(resultForm);
	newP->num = 2;
	this->_insertProof(PROOF,newP);
	resultForm.clear();
	
}

void GeneratorTest::_TestWhile(_proof* & PROOF,QQueue<string> P,QQueue<string> Q,QQueue<string> I,QQueue<string> C,ParsedData algorithm){
	QQueue<string> resultForm;

	this->_Test1_W(resultForm,P,I,algorithm);
	_proof* newP = this->_newProof(resultForm);
	newP->num = 1;
	this->_insertProof(PROOF,newP);
	resultForm.clear();
	
	this->_Test2_W(resultForm,I,algorithm);
	newP = this->_newProof(resultForm);
	newP->num = 2;
	this->_insertProof(PROOF,newP);
	resultForm.clear();
	
	this->_Test3_W(resultForm,I,Q,algorithm);
	newP = this->_newProof(resultForm);
	newP->num = 3;
	this->_insertProof(PROOF,newP);
	resultForm.clear();
	
	this->_Test4_W(resultForm,I,C,algorithm);
	newP = this->_newProof(resultForm);
	newP->num = 4;
	this->_insertProof(PROOF,newP);
	resultForm.clear();
	
	this->_Test5_W(resultForm,I,C,algorithm);
	newP = this->_newProof(resultForm);
	newP->num = 5;
	this->_insertProof(PROOF,newP);
	resultForm.clear();

}



/** Pruebas de la sentencia IF-THEN **/
void GeneratorTest::_Test1_IF_T(QQueue<string> & result,QQueue<string> P,QQueue<string> Q,ParsedData algorithm){

	result = P;
    QQueue<string> B;
    algorithm.getConditionTokens(B);
    B.prepend("(");
    B.enqueue(")");

    this->_concatenation(result,B,"&&");

    WeakestPreCondition WP(algorithm,Q,6);
    Q.clear();
    WP.getWp(Q);

    Q.prepend("(");
    Q.prepend("!");
	Q.enqueue(")");

	this->_concatenation(result,Q,"&&");

}

void GeneratorTest::_Test2_IF_T(QQueue<string> & result,QQueue<string> P,QQueue<string> Q,ParsedData algorithm){
	
	result = P;
    QQueue<string> B;
    algorithm.getConditionTokens(B);
    B.prepend("(");
    B.prepend("!");
    B.enqueue(")");

    this->_concatenation(result,B,"&&");

    Q.prepend("(");
    Q.prepend("!");
	Q.enqueue(")");

	this->_concatenation(result,Q,"&&");

}

/** Pruebas de la sentancia IF-ELSE **/
void GeneratorTest::_Test1_IF_E(QQueue<string> & result,QQueue<string> P,QQueue<string> Q,ParsedData algorithm){
	
	result = P;
    QQueue<string> B;
    algorithm.getConditionTokens(B);
    B.prepend("(");
    B.enqueue(")");

    this->_concatenation(result,B,"&&");

    WeakestPreCondition WP(algorithm,Q,4);
    Q.clear();
    WP.getWp(Q);

    Q.prepend("(");
    Q.prepend("!");
	Q.enqueue(")");

	this->_concatenation(result,Q,"&&");

}

void GeneratorTest::_Test2_IF_E(QQueue<string> & result,QQueue<string> P,QQueue<string> Q,ParsedData algorithm){

	result = P;
    QQueue<string> B;
    algorithm.getConditionTokens(B);
    B.prepend("(");
    B.prepend("!");
    B.enqueue(")");

    this->_concatenation(result,B,"&&");

    WeakestPreCondition WP(algorithm,Q,5);
    Q.clear();
    WP.getWp(Q);

    Q.prepend("(");
    Q.prepend("!");
	Q.enqueue(")");

	this->_concatenation(result,Q,"&&");

}

/** Pruebas de la sentancia While **/

void GeneratorTest::_Test1_W(QQueue<string> & result,QQueue<string> P,QQueue<string> I,ParsedData algorithm){

	result = P;

    WeakestPreCondition WP(algorithm,I,1);
    I.clear();
    WP.getWp(I);  
    I.prepend("(");
    I.prepend("!");
	I.enqueue(")");

	this->_concatenation(result,I,"&&");

	
}

void GeneratorTest::_Test2_W(QQueue<string> & result,QQueue<string> I,ParsedData algorithm){
	
	result = I;
    QQueue<string> B;
    algorithm.getConditionTokens(B);
    B.prepend("(");
    B.enqueue(")");

    this->_concatenation(result,B,"&&");

    WeakestPreCondition WP(algorithm,I,2);
    I.clear();
    WP.getWp(I);
    //NOME RECONOCE LAS { APRA DISTINGUIR LOS LITERALES
    I.prepend("(");
    I.prepend("!");
	I.enqueue(")");

	this->_concatenation(result,I,"&&");

}

void GeneratorTest::_Test3_W(QQueue<string> & result,QQueue<string> I,QQueue<string> Q,ParsedData algorithm){
	
	result = I;
    QQueue<string> B;
    algorithm.getConditionTokens(B);
    //NO ME NIEGA ESTA LA CONDICION
    B.prepend("(");
    B.prepend("!");
    B.enqueue(")");

    this->_concatenation(result,B,"&&");

    Q.prepend("(");
    Q.prepend("!");
	Q.enqueue(")");

	this->_concatenation(result,Q,"&&");

}

void GeneratorTest::_Test4_W(QQueue<string> & result,QQueue<string> I,QQueue<string> C,ParsedData algorithm){
	result = I;
    QQueue<string> B;
    algorithm.getConditionTokens(B);
    B.prepend("(");
    B.enqueue(")");

    this->_concatenation(result,B,"&&");

	result.enqueue("&&");
	result.enqueue("(");
    string exp = "{" + C.dequeue() + "<0" + "}";
    result.enqueue(exp);
	result.enqueue(")");


}

void GeneratorTest::_Test5_W(QQueue<string> & result,QQueue<string> I,QQueue<string> C,ParsedData algorithm){
	
	result = I;
    QQueue<string> B;
    QQueue<string> T = C;

    algorithm.getConditionTokens(B);
    B.prepend("(");
    B.enqueue(")");

    this->_concatenation(result,B,"&&");

    WeakestPreCondition WP(algorithm,C,3);//en este caso hago el {C = t}A{C < T}, me lo devuleve ya negado
    C.clear();
    WP.getWp(C);

    this->_concatenation(C,T,">=");

    C.enqueue(")");
    C.prepend("(");

    this->_concatenation(result,C,"&&");

}


bool GeneratorTest::_endProof(_proof* proof){
	if(proof != NULL)
		return proof->view && (this->_endProof(proof->nextTest));

}
bool GeneratorTest::endProof()
{
    return this->_endProof(this->_PROOF);
}

GeneratorTest::_proof* GeneratorTest::_getProof(unsigned int num, unsigned int test, _proof* proof)
{
    if(num == test || proof == NULL)
            return proof;

    else
        return this->_getProof(num+1,test,proof->nextTest);

}

void GeneratorTest::getTest(unsigned int Test,QQueue<string> & Output,bool & endProof)
{
    int num = 1;
    _proof* newTest;

    if(! (this->_endProof(this->_PROOF))){

       newTest = this->_getProof(num,Test,this->_PROOF);

    }

    std::cout<<std::endl;

    if(newTest != NULL)
        Output = newTest->test;
    else
        std::cout<<"La prueba no existe"<<std::endl;


}


GeneratorTest::~GeneratorTest()
{

}

