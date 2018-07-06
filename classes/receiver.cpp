#include "receiver.h"


/**
 * La clase TREE estaba mal hecha, me daba hasta errores de sintaxis, eliminé todo el contenido
 * pero dejé los archivos tree.h y tree.cpp.
 * Luego reemplazalos con el contenido nuevo.
 */

void Receiver::setData(
        QString _algorithm,
        QString _preCondition,
        QString _postCondition,
        QString _invariant,
        QString _bound ) {

    /**
     * LOS PARÁMETROS SON LOS DATOS INGRESADOS EN EL FORMULARIO,
     * USALOS, CUANDO SE EJECUTE ESTA FUNCIÓN YA VAN A ESTAR SETEADOS
     * ESOS VALORES.
     */

    Parser parser;
    QList<char*> errors;
    unsigned int tests;

    this->_cleanData();

    /**
     * Algorithm
     */
    ParsedData algorithm;
    parser.parse(_algorithm);
    parser.getAlgorithmData(algorithm, errors, tests);

    if ( errors.size() > 0 ) {
        this->algorithmError();
    }

    /**
     * precondition
     */
    QQueue<string> precondition;
    parser.parse(_preCondition);
    parser.getData(precondition, errors);

    if ( errors.size() > 0 ) {
        this->preconditionError();
    }

    /**
     * postcondition
     */
    QQueue<string> postcondition;
    parser.parse(_postCondition);
    parser.getData(postcondition, errors);

    if ( errors.size() > 0 ) {
        this->postconditionError();
    }

    /**
     * invariant
     */
    QQueue<string> invariant;
    parser.parse(_invariant);
    parser.getData(invariant, errors);

    if ( errors.size() > 0 ) {
        this->invariantError();
    }

    /**
     * bound
     */
    QQueue<string> bound;
    parser.parse(_bound);
    parser.getData(bound, errors);

    if ( errors.size() > 0 ) {
        this->boundError();
    }

    this->_tests.setData(precondition,postcondition,invariant,bound,algorithm);
    this->verificationReady(tests);
}

void Receiver::requestTest(unsigned int numTest) {
    if ( ! this->_endedTests.contains(numTest) ) {
        QQueue<data> GUIData;
        TernaryTree ternaryTree;
        if ( this->_testCharged(numTest, GUIData) ) {
            bool endProof = false; //KÉ.
            QQueue<std::string> test;
            this->_tests.getTest(numTest,test,endProof);
            RefutationTree Refutation;
            Refutation.generateTree(test,ternaryTree);
            ternaryTree.printCoordenates();
            ternaryTree.getTree(GUIData);
        }

        this->testReady(numTest);
        this->_sendTreeNodes(GUIData);
        this->_sendDebugNodes(ternaryTree);
    }
    else
        this->clickedEndedTest(numTest);
}

void Receiver::_sendDebugNodes(TernaryTree tree) {
    this->_tree = tree;
    QList<QString> branch;
    this->_tree.getBranches(branch);
    this->_sendBranch(branch);
}

void Receiver::_sendBranch(QList<QString> branch) {
    debugBranchReady(); //Con esto le digo a la interfaz que borre la rama anterior así puedo graficar la nueva.
    QList<QString>::iterator it = branch.begin();
    while ( it != branch.end() ) {
        this->debugNode(*it, this->_cropFormula(*it, 8));
        it++;
    }
}

bool Receiver::_testCharged(int test, QQueue<data> & _test) const {
    bool charged = false;
    switch ( test ) {
        case 1:
            charged = !this->_firstTest.empty();
            _test = this->_firstTest;
        break;
        case 2:
            charged = !this->_secondTest.empty();
            _test = this->_secondTest;
        break;
        case 3:
            charged = !this->_thirdTest.empty();
            _test = this->_thirdTest;
        break;
        case 4:
            charged = !this->_fourthTest.empty();
            _test = this->_fourthTest;
        break;
        case 5:
            charged = !this->_fifthTest.empty();
            _test = this->_fifthTest;
        break;
        default: exit(1); break;
        return charged;
    }
}

void Receiver::_sendTreeNodes(QQueue<data> nodes) {
    data nodeData;
    while ( ! nodes.empty() ) {
        nodeData = nodes.dequeue();
        this->treeNode(
                nodeData.getCoordX(),
                nodeData.getCoordY(),
                nodeData.getFormula(),
                nodeData.isLiteral(),
                nodeData.getFatherCoordX(),
                nodeData.getFatherCoordY()
        );
    }
}

void Receiver::setClickedDebugNode(unsigned int id, QString formula) {
    this->notify(id,formula);
}

void Receiver::skipBranch() {
    QList<QString> branch;
    this->_tree.getBranches(branch);
    if(!branch.empty())
        this->_sendBranch(branch);
    else
       this->debugTestEnd();
}

void Receiver::closeBranch(QString firstFormula, QString secondFormula, bool twoNodes) {
    //Cuando se llama a esta función desde la interfaz, se cierra la rama, pero en ningún lugar le decimos al árbol que esa rama se cierra.
    //entonces cuando vos preguntas this->_tree.isClosed() siempre te va a devolver false. Tendría que haber un método que haga eso.

    this->_tree.closeBranch();

    QQueue<QString> formulas;
    if ( twoNodes ) {
        formulas.enqueue(firstFormula);
        formulas.enqueue(secondFormula);
        this->_result[this->_tree.getNumBranch()] = formulas;
    }
    else {
        formulas.enqueue(secondFormula);
        this->_result[this->_tree.getNumBranch()] = formulas;
    }

    QList<QString> branch;
    this->_tree.getBranches(branch);
    if(!branch.empty())
        this->_sendBranch(branch);
    else {
       this->debugTestEnd();
    }
}

void Receiver::endedTest(unsigned int requestedTest) {
    this->_endedTests.push_back(requestedTest); //finalizó.
    this->_isClosed(requestedTest, this->_tree.closeTree());
}

void Receiver::_isClosed(unsigned int requestedTest, bool isClosed) {
    switch ( requestedTest ) {
        case 1: this->isClosedFirstTree = isClosed; break;
        case 2: this->isClosedSecondTree = isClosed; break;
        case 3: this->isClosedThirdTree = isClosed; break;
        case 4: this->isClosedFourthTree = isClosed; break;
        case 5: this->isClosedFifthTree = isClosed; break;
    }
}

void Receiver::requestResults() {
    this->results(this->isClosedFirstTree, this->isClosedSecondTree, this->isClosedThirdTree, this->isClosedFourthTree, this->isClosedFifthTree);
}

QString Receiver::_cropFormula(QString formula, unsigned int length) {
    return (formula.length() > length) ? formula.mid(0, length) + "..." : formula;
}

void Receiver::_cleanData() {
    this->_firstTest.clear();
    this->_secondTest.clear();
    this->_thirdTest.clear();
    this->_fourthTest.clear();
    this->_fifthTest.clear();
    this->_endedTests.clear();
}
