#include "refutationtree.h"

RefutationTree::RefutationTree() { }

std::string RefutationTree::_getFormula(QQueue<std::string> Q){
    std::string token;
    QQueue<std::string>::iterator it = Q.begin();
    while(it != Q.end()){
        token += (*it);
        it++;
    }
    return token;
}

RefutationTree::~RefutationTree() { }

void RefutationTree::generateTree(QQueue<std::string> Q,TernaryTree &  TernaryTree){
    QQueue<std::string> Q1;
    QQueue<std::string> Q2;
    TernaryTree.setTree(this->_getFormula(Q));
    QQueue<QQueue<std::string> > cola;
    cola.prepend(Q);
    std::string previousFormula;
    std::string operantOne;
    std::string operantTwo;
    bool E = false;
    QQueue<std::string> aux;
    QQueue<std::string> cuant;
    QList<std::string> listCuant;

    while(!cola.empty())
    {
        aux = cola.dequeue();
        cuant = aux;
        previousFormula = this->_getFormula(aux);
        Formula form(aux,TernaryTree.ConstantU(),TernaryTree.ConstantE(),E);
        if(form.getType() == 2 || form.getType() == 1)
        {
           form.getFirstOperant(Q1);
           form.getSecondOperant(Q2);
           operantOne = this->_getFormula(Q1);
           operantTwo = this->_getFormula(Q2);

           Formula formAux1(Q1,TernaryTree.ConstantU(),TernaryTree.ConstantE(),E);
           Formula formAux2(Q2,TernaryTree.ConstantU(),TernaryTree.ConstantE(),E);

           if(!Q1.empty() && formAux1.getType() == 1) {cola.push_back(Q1);}
           else { cola.push_front(Q1); }

           if(!Q2.empty() && formAux2.getType() == 1) {cola.push_back(Q2);}
           else { cola.push_front(Q2);}
           Q1.clear();
           Q2.clear();
           TernaryTree.addFormulas(form.getType(),operantOne,operantTwo,previousFormula,E);
         }
        else if(form.getType() == 3 || form.getType() == 4 )
        {
                 form.getFirstOperant(Q1);
                 operantOne = this->_getFormula(Q1);
                 cola.push_front(Q1);
                 TernaryTree.addFormulas(3,operantOne,"Empty",previousFormula,E);
                 Q1.clear();
                 Q2.clear();
             if(previousFormula[0] == '@'){
                 TernaryTree.ConstU(listCuant);
                 QList<std::string>::iterator it = listCuant.begin();
                 it++;
                 QQueue<std::string> aux2 = cuant;
                 while(it != listCuant.end())
                 {
                     Formula FormAux(cuant,*it,TernaryTree.ConstantE(),E);
                     FormAux.getFirstOperant(Q1);
                     operantOne = this->_getFormula(Q1);
                     cola.push_front(Q1);
                     TernaryTree.addFormulas(3,operantOne,"Empty",previousFormula,E);
                     Q1.clear();
                     Q2.clear();
                     cuant.clear();
                     cuant = aux2;
                     it++;
                 }
             }
        }
        else { }
        E = false;
    }
    TernaryTree.generateBranches();
}






