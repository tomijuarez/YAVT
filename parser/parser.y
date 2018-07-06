%{
    #include <iostream>
    #include <QList>
    #include <QMap>
    #include "classes/parsingtree.h"


    using namespace std;

    #define YYSTYPE string

    int yylex(void);
    void yyerror(char *);

    bool ifFound = false;
    bool elseFound = false;
    bool whileFound = false;

    ParsingTree tree;

    QMap<string, string> outsideAssignations;
    QMap<string, string> assignationsTrue;
    QMap<string, string> assignationsFalse;

    QMap<string, string> assignations;

    QList<char*> errors;

%}

%token  LEFT_PAR
        RIGHT_PAR
        VAR
        END
        NOT
        DIST
        CUANTIFIER
        BLANK
        LEFT_BRACE
        RIGHT_BRACE
        IF
        ELSE
        WHILE
        EQUAL
        END_OPERATION

%left   LOGIC_OPERATOR
        ARITH_OPERATOR

%start input

%%

input:
        /* empty */
    |   assignations { 
            outsideAssignations = assignations;
            assignations.clear();
        } 
        block_statement
    |   block_statement
;

block_statement:
        if_statement
    |   while_statement
    |   formula
;


else_statement:
        /*BLANK*/
    |   ELSE            /*else*/                               
        LEFT_BRACE      /*{*/ 
        assignations {
            assignationsFalse = assignations;
            assignations.clear();
        }
        RIGHT_BRACE     /*}*/                                   
        {
            elseFound = true;
            if ( whileFound )
                yyerror("No se pueden insertar sentencias anidadas.");
            cout << "SE INSERTÓ UN IF CON ELSE.";
        }
    
;

if_statement:
        /**
         * if sin else.
         */
        IF              /*if*/                      
        LEFT_PAR        /*(*/                       
        formula         /*condiciones compuestas.*/ 
        RIGHT_PAR       /*)*/                       
        LEFT_BRACE      /*{*/                       
        assignations {
            assignationsTrue = assignations;
            assignations.clear();
        }   /*operaciones.*/            
        RIGHT_BRACE     /*}*/  
        else_statement                    
        {
            ifFound = true;
            if ( whileFound )
                yyerror("No se pueden insertar sentencias anidadas.");
            cout << "SE INSERTO UN IF SIN ELSE.";
        }
;

while_statement:
        WHILE           /*while*/                   
        LEFT_PAR        /*(*/                       
        formula         /*condiciones compuestas.*/ 
        RIGHT_PAR       /*)*/                       
        LEFT_BRACE      /*{*/                       
        assignations  {
            assignationsTrue = assignations;
            assignations.clear();
        }  /*asignaciones.*/            
        RIGHT_BRACE     /*}*/                       
        {
            whileFound = true;
            if ( ifFound )
                yyerror("No se pueden insertar sentencias anidadas.");
            cout << "SE INSERTÓ UN WHILE.";
        }
;

assignations:
        VAR EQUAL arith_expr END_OPERATION {
            assignations[$1] = $3;
        }
    |   assignations VAR EQUAL arith_expr END_OPERATION {
            assignations[$2] = $4;
        }
    ;
;

formula:
        arith_expr {
            $$ = $1;
            tree.pushSymbol("{"+$1+"}");
        }
    |	CUANTIFIER formula {
    		$$ = $2;
    		tree.pushLogicOperator($1);
    	}
    |   LEFT_PAR formula RIGHT_PAR {
            $$ = $2;
            tree.pushLogicOperator("()");
        }
    |   NOT formula {
            $$ = $2;
            tree.pushLogicOperator("!");
        }
    |   DIST formula RIGHT_PAR {
            $$ = $2;
            tree.pushLogicOperator("$");
        }
    |   formula LOGIC_OPERATOR formula {
            tree.pushLogicOperator($2);
        }
;

arith_expr:
        VAR                                     { 
            $$ = $1; 
        }
    |   LEFT_PAR arith_expr RIGHT_PAR {
            $$ = "(" + $2 + ")";
        } 
    |   arith_expr ARITH_OPERATOR arith_expr    { 
            $$ = $1 + $2 + $3; 
        }
;

%%

void yyerror(char *s) {
    errors.push_back(s);
}