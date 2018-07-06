#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QQuickItem>
#include <QDebug>
#include <QVector>
#include <QString>
#include <QQueue>
#include <QList>
#include <iostream>
#include <string>
#include <QListIterator>
#include "parsingtree.h"
#include "parser.h"
#include "refutationtree.h"
#include "formula.h"
#include "test.h"
#include "ternarytree.h"
#include "data.h"
#include "parseddata.h"

using namespace std;

class Receiver : public QObject {
    Q_OBJECT

    signals:
        void treeNode(int x, int y, QString formula, bool isLiteral, int parentX, int parentY );
        void debugNode(QString formula, QString croppedFormula);
        void verificationReady(int numTests);
        void testReady(unsigned int numTest);
        void debugBranchReady();
        void debugTestEnd();
        void notify(unsigned int id, QString receivedFormula);
        void clickedEndedTest(unsigned int requestedTest);
        void algorithmError();
        void preconditionError();
        void postconditionError();
        void boundError();
        void invariantError();
        void results(bool first, bool second, bool third, bool fourth, bool fifth);

    public slots:
        void requestTest(unsigned int test);
        void setData(QString _algorithm, QString _preCondition, QString _postCondition, QString _invariant, QString _bound );
        void setClickedDebugNode(unsigned int id, QString formula);
        void skipBranch();
        void closeBranch(QString firstFormula, QString secondFormula, bool twoNodes);
        void endedTest(unsigned int requestedTest);
        void requestResults();

    private:
        void _sendTreeNodes(QQueue<data> node);
        void _sendDebugNodes(TernaryTree tree);
        void _sendBranch(QList<QString> branch);
        bool _testCharged(int test, QQueue<data> & _test) const;
        void _isClosed(unsigned int requestedTest, bool isClosed);
        QString _cropFormula(QString formula, unsigned int length);
        void _cleanData();

        Test _tests;
        TernaryTree _tree;
        QMap<int,QQueue<QString> > _result;

        QQueue<data> _firstTest;
        QQueue<data> _secondTest;
        QQueue<data> _thirdTest;
        QQueue<data> _fourthTest;
        QQueue<data> _fifthTest;

        bool isClosedFirstTree;
        bool isClosedSecondTree;
        bool isClosedThirdTree;
        bool isClosedFourthTree;
        bool isClosedFifthTree;

        QList<unsigned int> _endedTests;
};

#endif // RECEIVER_H
