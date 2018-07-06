#include "weakestprecondition.h"

WeakestPreCondition::WeakestPreCondition(ParsedData algorithms,QQueue<string> assertion,unsigned int test){
    this->_algorithms = algorithms;
    this->_assertion = assertion;
    this->_test = test;
}

void WeakestPreCondition::_replacement(string & predicate,string variable, string expression){
    string::size_type pos = 0;
    string fromStr = variable;
    if(expression.size()> 1){
        string toStr = "("+expression+")";
        while ((pos = predicate.find(fromStr, pos)) < string::npos){
             predicate.replace(pos, fromStr.length(), toStr);
             pos+=toStr.size();
        }
    }
    else{
        string toStr = expression;
        while ((pos = predicate.find(fromStr, pos)) < string::npos){
             predicate.replace(pos, fromStr.length(), toStr);
             pos+=toStr.size();
        }
    }
}

void WeakestPreCondition::getWp(QQueue<string> & Output)
{
    QMap<string,string> assignations;
    if(this->_test == 1  || this->_test == 4 || this->_test == 5 || this->_test == 6) {
       this->_algorithms.getOutsideAssignations(assignations);
       this->_getWp(assignations,this->_assertion,Output,false);
    }
    else if(this->_test == 2){
        this->_algorithms.getAssignationsTrue(assignations);
        this->_getWp(assignations,this->_assertion,Output,false);
    }
    else if(this->_test == 3){
        this->_algorithms.getAssignationsTrue(assignations);
        this->_getWp(assignations,this->_assertion,Output,true);
    }
}

void WeakestPreCondition::_getWp(QMap<string,string> assignations,QQueue<string> assertion,QQueue<string> & Output,bool C)
{
    while(! assertion.empty())
    {
        string token = assertion.dequeue();
        if(token[0]== '{' || C)
        {
            QMap<string,string>::iterator itMap = assignations.begin();
            while(itMap != assignations.end()){
                this->_replacement(token,itMap.key(),itMap.value());
                itMap++;
            }
        }
        Output.enqueue(token);
    }
}

WeakestPreCondition::~WeakestPreCondition(){}

