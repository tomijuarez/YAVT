#include "ternarytree.h"

TernaryTree::TernaryTree() {
     this->_constants.enqueue("A");
     this->_constants.enqueue("B");
     this->_constants.enqueue("C");
     this->_constants.enqueue("D");
     this->_constants.enqueue("E");
     this->_constants.enqueue("F");
     this->_constants.enqueue("G");
     this->_ConstE = this->_constants.dequeue();
     this->_ConstU = this->_ConstE;
     this->_CuantConst = NULL;
}

void TernaryTree::setTree(std::string formula)
{
    Coordinate coord;
    coord.setCoord(0,1);
    _treeNode* node = this->_newNode(formula);
    node->coord = coord;
    this->_leaves.enqueue(node);
    this->_tree.enqueue(node);
}


void TernaryTree::_insertOrdered(_treeNode* node,int Y)
{
    QQueue<_treeNode*>::iterator it = this->_tree.begin();
    _treeNode* aux;
    bool found = false;
    //itero hasta encontrar el mismo nivel
    while( (it != this->_tree.end()) && (!found) ) {
        aux = *it;
        if(aux->coord.getCoordY() == Y)
            found = true;
        else
            it++;
    }
    // si no encontre es porque es nuevo en el nivel
    if( found == false){ this->_tree.enqueue(node);}
    else {
        // sino busco la prox posicion en X libre
        while( (it != this->_tree.end()) && ((aux->coord.getCoordY() == Y) ) ){
            it++;
            aux = *it;
        }
        this->_tree.insert(it,node);
    }
}

// Genera un nuevo node del arbol
TernaryTree::_treeNode* TernaryTree::_newNode(std::string formula)
{
    _treeNode* node = new _treeNode;
    node->formula = formula;
    node->father = NULL;
    node->isLiteral = false;
    return node;
}

TernaryTree::_treeNode* TernaryTree::_findSheet(std::string formula)
{
    QQueue<_treeNode*>::iterator it = this->_leaves.begin();
    _treeNode* aux;
    while(it != this->_leaves.end()){
        aux = *it;
        if(aux->formula == formula){ return aux;}
        else { it++;}
    }
    return NULL;
}

void TernaryTree::_deleteSheet(QQueue<_treeNode *> & Leaves, std::string formula,Coordinate coord)
{
    QQueue<_treeNode*>::iterator it = Leaves.begin();
    _treeNode* aux;
    bool found = false;
    while(it != Leaves.end() && (!found)){
        aux = *it;
        if((aux->formula == formula) && (aux->coord.getCoordX() == coord.getCoordX()) && (aux->coord.getCoordY() == coord.getCoordY())) { found = true;}
        else{ it++;}
    }
    Leaves.erase(it);
}

bool TernaryTree::_isLiteral(std::string formula)
{
    int pos = 0;
    int aux = 0;
    bool found = false;
    while(pos <= formula.size() && (found == false)){
        if(formula[pos] == '{'){ aux++;}
        if(formula[pos] == '}'){ aux--;}
        if((formula[pos] == '!' && formula[pos+1] != '=') || formula[pos] == '&' || formula[pos] == '|' || (formula[pos] == '<' && formula[pos+1] == '-' && formula[pos+2] == '>') || (formula[pos] == '-' && formula[pos+1] == '>') )
            found = true;

        pos++;
    }
    if(found == true){return false;}
    else {return true;}
}

void TernaryTree::_insertInSheet(QQueue<_treeNode *> & tree,QQueue<_treeNode *> & Leaves, _treeNode *sheet, std::string formulaOne,std::string formulaTwo,unsigned int type){
    if(type == 3){
        int Y = sheet->coord.getCoordY()+1;
        //creo el nodo
        _treeNode* node = this->_newNode(formulaOne);
        node->father = sheet;
        Coordinate coord;
        coord.setCoord(sheet->coord.getCoordX(),Y);
        node->coord = coord;
        //elimino la "ex" hoja y lo agrego como nueva hoja del arbol
        this->_deleteSheet(Leaves,sheet->formula,sheet->coord);
        Leaves.enqueue(node);
        //lo agrego a los nodos del arbol
        if(this->_isLiteral(formulaOne) ){ node->isLiteral = true;}
        this->_insertOrdered(node,Y);
    }
    else if(type == 2 ||type ==1 ){
        _treeNode* nodeOne = this->_newNode(formulaOne);
        _treeNode* nodeTwo = this->_newNode(formulaTwo);
        int Y = sheet->coord.getCoordY()+1;
        Coordinate coordOne;
        Coordinate coordTwo;
        if(this->_isLiteral(formulaOne) ) {nodeOne->isLiteral = true;}
        if(this->_isLiteral(formulaTwo) ) {nodeTwo->isLiteral = true;}
    if(type == 1){
        nodeOne->father = sheet;
        nodeTwo->father = sheet;
        coordOne.setCoord(sheet->coord.getCoordX()-1,Y);
        coordTwo.setCoord(sheet->coord.getCoordX()+1,Y);
        if(this->isrep(coordOne)){
            coordOne.setCoord(sheet->coord.getCoordX()+1,Y);
            coordTwo.setCoord(sheet->coord.getCoordX()+3,Y);
            sheet->coord.setCoord(sheet->coord.getCoordX()+2,sheet->coord.getCoordY());
        }
        else if(this->isrep(coordTwo)){
            coordOne.setCoord(sheet->coord.getCoordX()-1,Y);
            coordTwo.setCoord(sheet->coord.getCoordX()-3,Y);
            sheet->coord.setCoord(sheet->coord.getCoordX()-2,sheet->coord.getCoordY());
            if(this->isrep(sheet->coord)){
                coordOne.setCoord(sheet->coord.getCoordX()-1,Y);
                coordTwo.setCoord(sheet->coord.getCoordX()-3,Y);
                sheet->coord.setCoord(sheet->coord.getCoordX()-2,sheet->coord.getCoordY());
            }
        }
        nodeOne->coord = coordOne;
        nodeTwo->coord = coordTwo;
        this->_insertOrdered(nodeOne,Y);
        this->_insertOrdered(nodeTwo,Y);
        Leaves.enqueue(nodeOne);
        Leaves.enqueue(nodeTwo);
    }
    else{
        nodeOne->father = sheet;
        nodeTwo->father = nodeOne;
        coordOne.setCoord(sheet->coord.getCoordX(),Y);
        coordTwo.setCoord(sheet->coord.getCoordX(),Y+1);
        nodeOne->coord = coordOne;
        nodeTwo->coord = coordTwo;
        this->_insertOrdered(nodeOne,Y);
        this->_insertOrdered(nodeTwo,Y+1);
        Leaves.enqueue(nodeTwo);
    }
        this->_deleteSheet(Leaves,sheet->formula,sheet->coord);
    }
}

bool TernaryTree::_sameBranch(_treeNode *sheet, std::string formula)
{
    if(sheet != NULL){
         if(sheet->formula == formula){return true;}
         else
         { return this->_sameBranch(sheet->father,formula);} }
    else {return false;}
}

void TernaryTree::_insertInLeaves(QQueue<_treeNode *> & tree, std::string formulaOne, std::string formulaTwo,unsigned int type,std::string previousFormula)
{
    QQueue<_treeNode*> auxQQueue = this->_leaves;
    QQueue<_treeNode*>::iterator it = auxQQueue.begin();
    _treeNode* auxNode;
    while(it != auxQQueue.end()){
        auxNode = *it;
        if( (this->_sameBranch(auxNode,previousFormula) ) == true) {this->_insertInSheet(tree,this->_leaves,auxNode,formulaOne,formulaTwo,type);}
        it++;
    }
    auxQQueue.clear();
}

void TernaryTree::_insertTypeA_cuant(QQueue<_treeNode*> &tree,std::string formula,std::string previousFormula,bool E)
{
    _treeNode* aux = this->_findSheet(previousFormula);
    if(aux == NULL){ this->_insertInLeaves(tree,formula,"empty",3,previousFormula); }
    else
    {
       this->_insertInSheet(tree,this->_leaves,aux,formula,"empty",3);
       aux = this->_findSheet(previousFormula);
       while(aux  != NULL)
       {
             this->_insertInSheet(tree,this->_leaves,aux,formula,"empty",3);
             aux = this->_findSheet(previousFormula);
       }
    }
    this->admisConst(E);
}

void TernaryTree::_insert(QQueue<_treeNode *> &tree, std::string formulaOne, std::string formulaTwo,std::string previousFormula,int type)
{
    _treeNode* aux = this->_findSheet(previousFormula);
    if(aux == NULL) { this->_insertInLeaves(tree,formulaOne,formulaTwo,type,previousFormula); }
    else
    {
       this->_insertInSheet(tree,this->_leaves,aux,formulaOne,formulaTwo,type);
       aux = this->_findSheet(previousFormula);
       while(aux  != NULL) {
             this->_insertInSheet(tree,this->_leaves,aux,formulaOne,formulaTwo,type);
             aux = this->_findSheet(previousFormula);
       }
    }
}

void TernaryTree::addFormulas(unsigned int type, std::string ConclusionOne, std::string ConclusionTwo,std::string previousFormula,bool E)
{
    if(type == 1 || type == 2){ this->_insert(this->_tree,ConclusionOne,ConclusionTwo,previousFormula,type);}
    else
    { this->_insertTypeA_cuant(this->_tree,ConclusionOne,previousFormula,E);}
}

/***************************************************** Funciones de la administracion de las ramas *****************************/

TernaryTree::_Branch* TernaryTree::_newBranch(_treeNode* sheet,unsigned int num)
{
    _Branch* node = new _Branch;
    node->branch = sheet;
    node->num = num;
    node->close = false;
    node->view = false;
    node->nextBranch = NULL;
}

void TernaryTree::_insertBranch(_Branch* & branches, _Branch* branch){
    if(branches == NULL){ branches = branch;}
    else{ this->_insertBranch(branches->nextBranch,branch);}
}

void TernaryTree::_generateBranches()
{
    QQueue<_treeNode*>::iterator it = this->_leaves.begin();
    _treeNode* aux;
    _Branch* node;
    unsigned int branch = 1;
    while(it != this->_leaves.end()){
         aux = *it;
         node = this->_newBranch(aux,branch);
        this->_insertBranch(this->_branches,node);
        branch++;
         it++;
    }
}

void TernaryTree::generateBranches()
{
    this->_branches = NULL;
    this->_generateBranches();
    this->_nextBranch = this->_branches;
    this->_endBranches = false;
}

void TernaryTree::_conversion(char output[], std::string input){
    unsigned int pos = 0;
    while(pos <= input.size()){
        output[pos] = input[pos];
        pos++;
    }
}

void TernaryTree::_copy(_treeNode* sheet, QList<QString> &branches)
{
    if(sheet != NULL)
    {
        if(sheet->isLiteral == true)
        {
            char output[sheet->formula.size()];
            this->_conversion(output,sheet->formula);
            QString formula(output);
            branches.push_back(formula);
        }
        this->_copy(sheet->father,branches); }
}

void TernaryTree::getBranches(QList<QString> & branches)
{
   if(this->_endBranches == false)
   {
      this->_currentBranch = this->_nextBranch;
      this->_currentBranch->view = true;
      this->_copy(this->_nextBranch->branch,branches);
      if(this->_nextBranch->nextBranch != NULL) {this->_nextBranch = this->_nextBranch->nextBranch;}
      else {this->_endBranches = true;}
   }
}

void TernaryTree::closeBranch(){ this->_currentBranch->close = true; }
bool TernaryTree::closeTree(){ return this->_closeTree(this->_branches); }
bool TernaryTree::_closeTree(_Branch * branches){ if(branches != NULL){ return ((branches->close) && (this->_closeTree(branches->nextBranch)));} }

unsigned int TernaryTree::getNumBranch(){ return this->_currentBranch->num; }

/************************************************************** Sector de adminsitracion de constantes ******************************************/
//Duda de como usar los cuantificadores en el arbol, es decir, la idea de como resolverlo esta, solo falta definir la duda teoria de como manejar esas situaciones
void TernaryTree::_copyConst(QList<std::string> &list, _constCuant *constants){
    if(constants != NULL){
        list.push_back(constants->constant);
        this->_copyConst(list,constants->nextConst);
    }
}

void TernaryTree::ConstU(QList<std::string> & L){ this->_copyConst(L,this->_CuantConst); }

void TernaryTree::admisConst(bool E){
    if(E == true){
            std::string aux = this->_constants.dequeue();
            _constCuant* node = this->_newConst(this->_ConstE);
            this->_insertConst(this->_CuantConst,node);
            this->_ConstE = aux;
       }
}

void TernaryTree::_insertConst(_constCuant* & constants, _constCuant* constant)
{
    if(constants == NULL){ constants = constant;}
    else {this->_insertConst(constants->nextConst,constant);}
}

TernaryTree::_constCuant* TernaryTree::_newConst(std::string constant)
{
    _constCuant* node = new _constCuant;
    node->constant = constant;
    node->nextConst = NULL;
    return node;
}

std::string TernaryTree::ConstantE(){ return this->_ConstE;}
std::string TernaryTree::ConstantU(){return this->_ConstU;}

void TernaryTree::getTree(QQueue<data> & Output)
{
    int first = 1;
    QQueue<_treeNode*>::iterator it = this->_tree.begin();
    while(it != this->_tree.end())
    {
        if(first == 1){
            _treeNode* auxData= *it;
            data _node;
            _node.setData(auxData->formula,auxData->coord.getCoordX(),auxData->coord.getCoordY(),auxData->isLiteral,0,0);
            Output.enqueue(_node);
            it++;
            first = 0;
        }
        else{
        _treeNode* auxData= *it;
        data _node;
        _node.setData(auxData->formula,auxData->coord.getCoordX(),auxData->coord.getCoordY(),auxData->isLiteral,auxData->father->coord.getCoordX(),auxData->father->coord.getCoordY());
        Output.enqueue(_node);
        it++;
        }
    }
}

/************************************************************** Sector de Impresiones ******************************************/
void TernaryTree::printCoordenates()
{
    QQueue<_treeNode*>::iterator it = this->_tree.begin();
    _treeNode* aux;
    std::cout<<"****************************************************"<<std::endl;
    std::cout<<"Este es el árbol generado: "<<std::endl;
    while(it != this->_tree.end())
    {
     aux = *it;
     std::cout<<"Formula: "<<aux->formula<<" "<<"Coordenada: "<<"("<<aux->coord.getCoordX()<<","<<aux->coord.getCoordY()<<")"<<"   Este literal: "<<aux->isLiteral<<std::endl;
     it++;
    }
}


bool TernaryTree::isrep(Coordinate coord){
    QQueue<_treeNode*>::iterator it = this->_tree.begin();

    while(it != this->_tree.end()){
        _treeNode *aux = *it;
        if(aux->coord.getCoordX() == coord.getCoordX() && aux->coord.getCoordY() == coord.getCoordY()){
            return true;
        }
        it++;
    }

    return false;
}

TernaryTree::~TernaryTree(){ }


