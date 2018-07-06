#ifndef TERNARYTREE_H
#define TERNARYTREE_H

#include <iostream>
#include <string>
#include <QString>
#include <QQueue>
#include <QMap>
#include <QList>
#include "coordinate.h"
#include "data.h"


class TernaryTree
{
public:
    TernaryTree();
    ~TernaryTree();
    void setTree(std::string formula);
    void addFormulas(unsigned int type,std::string ConclusionOne, std::string ConclusionTwo,std::string previousFormula,bool E);
    void generateBranches();
    void getTree(QQueue<data> & Output);
    void getBranches(QList<QString> & branches);
    unsigned int getNumBranch();
    void closeBranch();
    bool closeTree();
    void ConstU(QList<std::string> & L);
    std::string ConstantE();
    std::string ConstantU();

    void printCoordenates();

private:

    struct _treeNode{
        std::string formula;
        Coordinate coord;
        _treeNode* father;
        bool isLiteral;
    };

   //Nuevo para administrar las ramas
    struct _Branch {
        unsigned int num;
        _treeNode* branch;
        bool view;
        bool close;
        _Branch* nextBranch;

    };

    //contiene los nodos del arbol
    QQueue<_treeNode*> _tree;
    //contiene las hojas del mismo
    QQueue<_treeNode*> _leaves;
    _Branch* _branches;

    /***Administracion de los cuantificadores***/
    QQueue<std::string> _constants;
    std::string _ConstU;
    std::string _ConstE;
    void admisConst(bool E);

    /**Nuevo**/
    struct _constCuant{
        std::string constant;
        _constCuant* nextConst;
    };
    _constCuant* _CuantConst;
    void _insertConst(_constCuant* & constants, _constCuant* constant);
    _constCuant* _newConst(std::string constant);
    void _copyConst(QList<std::string> & list,_constCuant* constants);


    //Nuevo para administrar las ramas
    _Branch* _newBranch(_treeNode* sheet,unsigned int num);
    void _insertBranch(_Branch* & branches,_Branch* branch);
    void _generateBranches();
    void _copy(_treeNode* sheet,QList<QString> & branches);
    bool _isLiteral(std::string formula);
    _Branch* _nextBranch;
    _Branch* _currentBranch;
    bool _endBranches;
    bool _closeTree(_Branch* branches);


    _treeNode* _newNode(std::string formula);//Nuevo nodo del arbol
    _treeNode* _findSheet(std::string formula);// me devuelve la rama del arbol que voy a derivar, en caso contrario me devuelve NULL

    void _deleteSheet(QQueue<_treeNode*> & Leaves,std::string formula,Coordinate coord);//elimina la ultima hoja a la que se le agrego nodos
    void _insertInLeaves(QQueue<_treeNode*> & tree,std::string formulaOne,std::string formulaTwo,unsigned int type,std::string previousFormula);// inserta los nodos en cada una de las hojas del arbol
    void _insertInSheet(QQueue<_treeNode*> & tree,QQueue<_treeNode*> & Leaves,_treeNode* sheet,std::string formulaOne,std::string formulaTwo,unsigned int type);//inserta el nodo en una huja especifica y luego la elimina de la cola de hojas
    bool _sameBranch(_treeNode* sheet,std::string formula);//Me devuelve un valor booleano diciendome si la hoja se encuentra en la misma rama de la formula a la que voy a derivar, ya que solo en esas hojas voy a agregar los nodos

    void _insertOrdered(_treeNode* node,int Y);
    void _insert(QQueue<_treeNode*> & tree,std::string formulaOne,std::string formulaTwo,std::string previousFormula,int type);
    void _insertTypeA_cuant(QQueue<_treeNode*> & tree,std::string formula,std::string previousFormula,bool E);

    void _conversion(char output[],std::string input);

    bool isrep(Coordinate coord);

};

#endif // TERNARYTREE_H
