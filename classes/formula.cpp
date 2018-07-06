#include "formula.h"

Formula::Formula(QQueue<std::string>  tokens,std::string ConstU,std::string ConstE,bool & E) {

	this->_resolutionType = 0;
    this->_ConstE = ConstE;
    this->_ConstU = ConstU;

    this->setClauses(tokens,E);
}


Formula::~Formula() { }

bool Formula::_Literal(std::string token){
    if ( token == "||" || token == "&&" || token == "->" || token == "<->" || token == "!("  )
        return false;
    else
        return true;
}



void Formula::_replacement(std::string & output,std::string variable,std::string constant){

        std::string::size_type pos = 0;
        std::string fromStr = variable;
        if(constant.size()> 1){
            std::string toStr = constant;
             while ((pos = output.find(fromStr, pos)) < std::string::npos)
            {
                 output.replace(pos, fromStr.length(), toStr);
                 pos+=toStr.size();
            }
        }
        else
        {
            std::string toStr = constant;
             while ((pos = output.find(fromStr, pos)) < std::string::npos)
            {
                 output.replace(pos, fromStr.length(), toStr);
                 pos+=toStr.size();
            }
        }

}

bool Formula::_totalQuantification(QQueue< std::string> tokens){
	unsigned int numParent = 0;
	bool OpenParent = false;
	std::string token ;
    tokens.dequeue();
	while( ! tokens.empty() ){
                token = tokens.dequeue();

                if(token == "("){
                    numParent++;
                }
                if(token == ")"){
                    numParent--;
                }

                if((numParent == 0) && (! tokens.empty())){
                    if(! (this->_Literal(token)) && token != "@" && token != "#" ){
                        std::cout<<"false"<<std::endl;
                       return false;

                    }
                }

                if((numParent == 0) && (tokens.empty())){
                    std::cout<<"true"<<std::endl;
                    return true;

                }

        }

}

bool Formula::_isOneExpression(QQueue<std::string> tokens) {
	
	std::string newToken;

	unsigned int numPar = 0;
	bool atLeastOne = false;

	while ( ! tokens.empty() ) {

		newToken = tokens.dequeue();

		if ( "(" == newToken ) {
			atLeastOne = true;
			numPar++;
		}
		else if ( ")" == newToken ) {
			numPar--;
        }
        if((numPar == 0) && ! tokens.empty())
            if(! (this->_Literal(newToken)) && newToken != "@" && newToken != "#" ){
                std::cout<<"false"<<std::endl;
               return false;

            }



	}

	return ((numPar == 0) && tokens.empty() && atLeastOne);
}

void Formula::_change(std::string _operator) {
	if ( this->_secondOperand.size() > 0 ) {
		this->_firstOperand += _operator;
		this->_firstOperand += this->_secondOperand;
	}
}

void Formula::_add(std::string element, bool jump, bool isOperator, bool prevCuant ) {
    if (!jump ){
        if(prevCuant == false){
            this->_firstOperand += element;
        }
     }
	else
		if ( !isOperator || (isOperator && !( this->_secondOperand.size() == 0 ) ) )
			this->_secondOperand += element; 
}

bool Formula::_isLiteral(QQueue<std::string> operand)const {
	std::string token;
	while ( ! operand.empty() ) {
		token = operand.dequeue();
        if ( token == "||" || token == "&&" || token == "->" || token == "<->" || token == "!" )
			return false;
	}
	return true;
}

void Formula::_print(QQueue<std::string> op) {
	while ( ! op.empty() )
		std::cout << op.dequeue();
}

void Formula::_negOperators(bool negFirstOperand, bool negSecondOperand) {
	if ( negFirstOperand ) 
		this->_neg(this->_firstOperand);
	if ( negSecondOperand )
		this->_neg(this->_secondOperand);
}

std::string Formula::_stringQueue(QQueue< char > queue) {
   		 QQueue< char >::iterator it = queue.begin();
    char output[queue.size()];
    int i=0;
    while((it != queue.end())&&(i <= queue.size()))
    {

        output[i]=(*it);
        i++;
        it++;

    }

    return output;
   }

std::string Formula::_negLiteral(std::string  predicate) {
     QQueue< char > output;

     bool prevhigher = false;
     bool prevLess = false;



     int pos = 0;

     for(;pos<= predicate.size();pos++){
         std::cout << predicate[pos];
         std::cout << std::endl;
     }

     pos = 0;
     while(pos <= predicate.size())
     {
         if(predicate[pos] != '<')
         {
              if(predicate[pos] != '>')
              {
                    if(predicate[pos] != '!')
                    {
                        if(predicate[pos] != '='){
                            //es un simbolo no comparador
                            if(prevhigher||prevLess){
                                std::cout<<"pase por aca";
                                std::cout<< std::endl;
                                output.push_back('=');
                                output.push_back(predicate[pos]);
                                prevhigher = false;
                                prevLess = false;
                                pos++;
                            }
                            else{
                                 output.push_back(predicate[pos]);
                                 pos++;
                            }
                        }
                        else
                        {
                            if(prevhigher||prevLess){
                                prevhigher = false;
                                prevLess = false;
                                pos++;
                            }
                            else{
                                output.push_back('!');
                                output.push_back('=');
                                pos++;
                                pos++;
                            }
                        }
                    }
                    else//se detecto un "!"
                    {
                        output.push_back('=');
                        output.push_back('=');
                        pos++;
                    }
              }
              else//se detecto un mayor
              {
                  output.push_back('<');
                  pos++;
                  prevhigher = true;

              }
         }
         else{
             output.push_back('>');
             pos++;
             prevLess = true;

         }

     }//fin de ciclo


   return this->_stringQueue(output);
}

void Formula::_negCuant(QQueue<std::string> & tokens){
    std::string token = tokens.dequeue();
    while(token != "(" ){
        if(token == "#"){
            this->_firstOperand += "@";
            this->_firstOperand += tokens.dequeue();
        }
        else if(token == "@"){
            this->_firstOperand += "#";
            this->_firstOperand += tokens.dequeue();
        }
        token = tokens.dequeue();
    }
    tokens.prepend("(");

}

void Formula::_neg(QQueue<std::string> & operand) {
	std::string token;
	std::string aux;

	if ( ! operand.empty()) 
		token = operand.dequeue();

    if ( (token == "#") || (token == "@") ) {

        operand.prepend(token);
        operand.prepend("!");

    }
    else
    if ( this->_isLiteral(operand) && token != "!" ) {

        token = this->_negLiteral(token);
        operand.prepend(token);
	}
	else {
		if ( operand.size() > 1 ) {
			
			aux = operand.front();
			operand.prepend(token);

			if ( token != "!" ) {
				if ( this->_isOneExpression ( operand ) ) {

                    //A lo mejor la expresión ya traía paréntesis, ¿para qué les voy a poner otro par más
                    operand.prepend("!");



				}
				else {
					operand.prepend("(");
					operand.prepend("!");
					operand.enqueue(")");
				}
			}
			else {
				if ( this->_isOneExpression(operand) ) {
					if ( aux == "(" ) {
						operand.dequeue();
						operand.removeLast();
						operand.removeFirst();
					}
				}
			}
		}
	}
}

void Formula::_checkPar() {
	std::string token;
	if ( this->_isOneExpression(this->_firstOperand) ) {
		token = this->_firstOperand.dequeue();
		if ( token == "(" )
			this->_firstOperand.removeLast();
		else
			this->_firstOperand.prepend(token);
	}

	if ( ! this->_secondOperand.empty() && this->_isOneExpression(this->_secondOperand) ) {
		token = this->_secondOperand.dequeue();
		if ( token == "(" )
			this->_secondOperand.removeLast();
		else
			this->_secondOperand.prepend(token);
	}
}

void Formula::setClauses(QQueue<std::string> & tokens,bool & E) {
	
	std::string token;
	bool previousIsNot = false;
	bool isOperand = false;
	bool jump = false;
	bool negFirstOperand = false;
	bool negSecondOperand = false;

	bool firstToken = true;

	//implicacion
	bool implication = false;
	bool negIff = false;
	bool iff = false;

	unsigned int numPar = 0;
	//cuantificadores
	bool CUANT = false;
	QQueue<std::string> auxCuant;
 	//formulas con cuantificador Existencial
	bool prevCuantExist = false;
    bool litCuant = false;
	std::string constant ;
	// formulas con cuantificador Universal
	bool prevCuantUniv = false;
	std::string quantified;
	//para probar el cuantificador universal
	this->_auxConstant.enqueue("(*)");
    //nuevo
    bool replac = false;

		//Para el iff.
	QQueue<std::string> aux;


	while ( ! tokens.empty() ) {

		token = tokens.dequeue();

		if ( token == "(" )
			numPar++;

		if ( token == ")" ) 
			numPar--;

        if ( (token == Formula::_NOT) && ( numPar == 0 ) && (!CUANT) && this->_resolutionType == 0) {

            if ( (this->_isOneExpression(tokens) && ( firstToken ))  ) {

				//Sé que hay un elemento porque antes había un {!}.
				token = tokens.dequeue();  

                //existe la negacion de un cuantificador
				if(token == "#"){

					this->_firstOperand += "@";
					//luego viene la constante cuantificada
					token = tokens.dequeue();
					this->_firstOperand += token;
					//niego la expresion
                    this->_negCuant(tokens);
					this->_firstOperand += "!";
                    this->_resolutionType = _REGLA_AND_CUANT;

                }
                else if(token == "@"){

                    this->_firstOperand += "#";
                    token = tokens.dequeue();
                    this->_firstOperand += token;
                    this->_negCuant(tokens);
                    this->_firstOperand += "!";
                    this->_resolutionType = _REGLA_AND_CUANT;

                }

                else if ( token == "(" ) {
					//Como era una sola expresión, debe haber un cierre al final.

                    if(this->_isLiteral(tokens)){
                        tokens.removeLast();
                        token = tokens.dequeue();
                        this->_firstOperand.enqueue(this->_negLiteral(token));
                        this->_resolutionType = 4;

                    }else
                    {

                    tokens.removeLast();
                    previousIsNot = true;

                   }

				}
                else {

                    this->_add("!", jump, true,false);
                    this->_add(token, jump, false,false);
                    previousIsNot = false;

				}
			}
			else
				if ( ! jump )
					this->_firstOperand += "!";
				else
					this->_secondOperand += "!";

				
		}

		else if ( token == Formula::_OR && ( numPar == 0 ) &&  (!CUANT) ) {
			if ( previousIsNot ) {
				if ( this->_resolutionType == Formula::_REGLA_OR || ! isOperand ) {
					this->_resolutionType = Formula::_REGLA_AND;
				    //Cuando lo reemplaza, pongo el operador.
                    this->_change(this->__operator);
					this->__operator = "||";
					this->_secondOperand.clear();
					jump = true;
					isOperand = true;
					negFirstOperand = true;
					negSecondOperand = true;
				}
				
                this->_add("||", jump, true,false);
			}
			else if( ! isOperand ) {
				jump = true;
                this->_add("||", jump, true,false);

                this->__operator = "||";

				isOperand = true;
				this->_resolutionType = Formula::_REGLA_OR;
			}
			else
                this->_add("||", jump, true,false);
		}
		else if ( token == Formula::_AND && ( numPar == 0 ) &&  (!CUANT) ) {
			if ( previousIsNot ) {
				if ( ! isOperand ) {
					this->_resolutionType = Formula::_REGLA_OR;

					this->__operator = "&&";
					
					this->_secondOperand.clear();
					jump = true;
					isOperand = true;
					negFirstOperand = true;
					negSecondOperand = true;
				}
                this->_add("&&", jump, true,false);
			}
			else if ( !isOperand || ( isOperand && this->_resolutionType == Formula::_REGLA_OR ) ) {
						implication = false;
						negIff = false;

						isOperand = true;
						jump = true;
						this->_resolutionType = Formula::_REGLA_AND;
				
						this->_change(this->__operator);
						this->__operator = "&&";
				
						this->_secondOperand.clear();
                		this->_add("&&", jump, true,false);
			}	
			else {
                this->_add("&&", jump, true,false);
			}
        }
        else if( (token == Formula::_CuantEXIST) && (this->_resolutionType == 0) && (this->_totalQuantification(tokens))){
                        this->_resolutionType = Formula::_REGLA_AND_CUANT;
                        prevCuantExist = true;
                        CUANT = true;
                        E = true;
                        constant = tokens.dequeue();//le saque los corchetes porque en una prueba siemrpe va haber un predicado con 2 partes



        }
        else if((token == Formula::_CuantUNIV)  && (this->_resolutionType == 0) && (this->_totalQuantification(tokens)) ){
                    this->_resolutionType = Formula::_REGLA_AND_CUANT;
                    CUANT = true;
                    prevCuantUniv = true;
                    constant =  tokens.dequeue();


        }
		else if ( token == Formula::_IMPLIES && ( numPar == 0 ) && (!CUANT)) {
            if ( previousIsNot ) {
                if ( ! isOperand || this->_resolutionType == Formula::_REGLA_OR ) {
                    this->_resolutionType = Formula::_REGLA_AND;

                    this->_change(this->__operator);
                    this->__operator = "->";

                    this->_secondOperand.clear();
                    jump = true;
                    isOperand = true;
                    negSecondOperand = true;

                    negIff = false;
                }
                this->_add("->", jump, true,false);
            }
            else if( ! isOperand ) {
                jump = true;
                this->_add("->", jump, true,false);

                this->__operator = "->";

                isOperand = true;
                this->_resolutionType = Formula::_REGLA_OR;
                implication = true;
            }
            else {
                this->_add("->", jump, true,false);
            }
        }

        else if ( token == Formula::_IFF && ( numPar == 0 ) && (!CUANT) ) {
            //tipo beta.
            if ( previousIsNot ) {
                if ( ! isOperand ) {
                    this->_resolutionType = Formula::_REGLA_OR;
                    jump = true;
                    this->__operator = "<->";

                    isOperand = true;
                    iff = false;
                    negIff = true;
                }
                this->_add("<->", jump, true,false);
			}
			else if ( ! isOperand || Formula::_resolutionType == Formula::_REGLA_OR ) {
				this->_resolutionType = Formula::_REGLA_AND;

				this->_change(this->__operator);
				this->__operator = "<->";
					
				this->_secondOperand.clear();
				jump = true;
				isOperand = true;

				iff = true;
				negIff = false;
			}
			else {
                this->_add("<->", jump, true,false);
			}
		}


        else
		{
            if ( !jump && (CUANT == true)){
                //std::cout << "AGREGO " << token << " EN LA PRIMERA." << std::endl;
                if(prevCuantExist == true){

                    this->_replacement(token,constant,this->_ConstE);
                    this->_firstOperand += token;
                    replac = true;

                }
                else
                {
                    this->_replacement(token,constant,this->_ConstU);
                    this->_firstOperand += token;
                    replac = true;
                }
				
                }
                else{

                 }
                this->_add(token, jump, false,replac);
                replac = false;
		}

		firstToken = false;
	}
	
	//El entonces.
	if ( implication )
		negFirstOperand = true;

	if ( iff ) {
		std::cout << std::endl;
		this->_print(this->_firstOperand);
		std::cout<< std::endl;
		this->_print(this->_secondOperand);
		std::cout << std::endl;
		this->_applyIff();
	}
	if ( negIff ) {
		negFirstOperand = true;
		negSecondOperand = true;
		this->_applyIff();
	}

	//Si hay que negar niego, sino no [CUAK].
	this->_negOperators(negFirstOperand, negSecondOperand);

	//Si hay una expresión del tipo (a&&b||c) le elimino los paréntesis. 
	this->_checkPar();
}//END_SETCLAUSES



void Formula::_applyIff() {
    QQueue<std::string> aux;
    aux = this->_firstOperand;
    this->_firstOperand += "->";
    this->_firstOperand += this->_secondOperand;
    this->_secondOperand += "->";
    this->_secondOperand += aux;
}


/**
 * dequeue -> first
 */

void Formula::printOperands() {
    std::cout << "EL OPERANDO 1 ES: " << std::endl;
    while ( ! this->_firstOperand.empty() )
        std::cout << this->_firstOperand.dequeue();
    std::cout << std::endl << "EL OPERANDO 2 ES: " << std::endl;
    while ( ! this->_secondOperand.empty() )
        std::cout << this->_secondOperand.dequeue();
    std::cout << std::endl << "EL TIPO DE OPERACION ES " << this->_resolutionType << std::endl;
}

void Formula::_applyInvolution(QQueue<std::string> & tokens) {

}

void Formula::_applyAnd(QQueue<std::string> & tokens) {

}

void Formula::_applyOr(QQueue<std::string> & tokens) {

}

void Formula::_applyNotOr(QQueue<std::string> & tokens) {

}

void Formula::_applyNotAnd(QQueue<std::string> & tokens) {

}

void Formula::_applyImplication(QQueue<std::string> & tokens) {

}

void _applyNotImplication(QQueue<std::string> & tokens) {

}
//agregadas para el impresion del arbol
const unsigned int Formula::getType() const{
     if(this->_resolutionType == 1)
         return this->_REGLA_OR;
     else if(this->_resolutionType == 2){
         return this->_REGLA_AND;
     }
     else if(this->_resolutionType == 3){
         return this->_REGLA_AND_CUANT;
     }
     else if(this->_resolutionType == 4){
         return this->_REGLA_LITERAL;
     }
     else{
         return 0;
     }

}

void Formula::getFirstOperant(QQueue<std::string> & Q){
    QQueue<std::string>::iterator it = this->_firstOperand.begin();

    while(it != (this->_firstOperand.end())){
        Q.push_back(*it);
        it++;
    }
    std::cout<<std::endl;
}

void Formula::getSecondOperant(QQueue<std::string> & Q){
    QQueue<std::string>::iterator it = this->_secondOperand.begin();

    while(it != (this->_secondOperand.end())){
        Q.push_back(*it);
        it++;
    }
    std::cout<<std::endl;
}

