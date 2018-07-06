#ifndef REFUTATIONTREE_H
#define REFUTATIONTREE_H

#include <iostream>
#include "formula.h"
#include <string>
#include <QQueue>
#include "ternarytree.h"

class RefutationTree {
    public:
        RefutationTree();
        ~RefutationTree();
        void generateTree(QQueue<std::string> tokens,TernaryTree & TernaryTree);

    private:

        void _printQQueue( QQueue<std::string> Q);
        QQueue<QQueue<std::string> > _subFormulas;
        std::string _getFormula(QQueue<std::string> Q);

};


#endif // REFUTATIONTREE_H
