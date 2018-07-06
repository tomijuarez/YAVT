#include "debug.h"

Debug::Debug(TreeAux Tree) {
    this->_Tree = Tree;
}

void Debug::StardDebug(QMap<int, QQueue<std::string> > & result){
    QList<QString> branch;
    this->_Tree.getBranches(branch);
    unsigned int num = 1;
    bool close;
    std::string formulaOne;
    std::string formulaTwo;
    QQueue<std::string> OpForm;
    std::cout<<std::endl<<"***********Verificacion formal del algoritmo****************"<<std::endl;
    while( !branch.empty() ) {


        if(close == true)
        {

            OpForm.enqueue(formulaOne);
            OpForm.enqueue(formulaTwo);
            result[num] = OpForm;

            this->_Tree.closeBranch();

        }
        else
        {
            OpForm.enqueue("Rama abierta");
            result[num] = OpForm;
        }

        num++;
        branch.clear();
        OpForm.clear();
        this->_Tree.getBranches(branch);

    }


}


void Debug::printResult(QMap<int,QQueue<std::string> > result)
{
    QMap<int,QQueue<std::string> >::iterator it = result.begin();
    std::string aux1;
    std::string aux2;
    std::cout<<std::endl<<"****************RESULTADOS***************"<<std::endl;
    while(it != result.end())
    {
        std::cout<<"Rama numero: "<<it.key()<<std::endl;

        if(it.value().size() == 2)
        {
            std::cout<<"Rama cerrada"<<std::endl;

            aux1 = it.value().dequeue();
            std::cout<<aux1<<std::endl;
            aux2 = it.value().dequeue();
            if(aux2 != "empty")
            {
                std::cout<<aux2<<std::endl;
            }

            std::cout<<std::endl;
        }
        else
        {
            aux1 = it.value().dequeue();
            std::cout<<aux1<<std::endl;
            std::cout<<std::endl;
        }
      it++;
   }

    if(this->_Tree.closeTree() == true)
    {
        std::cout<<"El arbol es CERRADO, por lo que podemos afirmar que la prueba correspondiente es satisfactoria"<<std::endl;
    }
    else
    {
        std::cout<<"El arbol es ABIERTO, por lo que podemos afirmar que la prueba correspondiente es NO satisfactoria"<<std::endl;
    }

}




Debug::~Debug()
{


}

