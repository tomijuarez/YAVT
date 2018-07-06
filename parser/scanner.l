%option noyywrap
%{
    #include <iostream>
    #include "parser.tab.c"
    using namespace std;
%}

%%

"if" {
    return IF;
}

"else" {
    return ELSE;
}

"while" {
    return WHILE;
}

[a-zA-Z0-9]+  {
    yylval = strdup(yytext);
    return VAR;
}

[ \t\r\n\s] { /*NADA.*/ }

"=" {
    return EQUAL;
}

";" {
    return END_OPERATION;
}

"{" {
    return LEFT_BRACE;
}

"}" {
    return RIGHT_BRACE;
}

"&&" {
    yylval = strdup(yytext);
    return LOGIC_OPERATOR;
}

"||" {
    yylval = strdup(yytext);
    return LOGIC_OPERATOR;
}

"->"    {
    yylval = strdup(yytext);
    return LOGIC_OPERATOR;
}

"<->" {
    yylval = strdup(yytext);
    return LOGIC_OPERATOR;
}

"!" {
    return NOT;
}

"!(" {
    return DIST;
}

[ \0\0] {
    return END;
}

"("     {
    return LEFT_PAR;
}

")"     {
    return RIGHT_PAR;
}

[\^\*+-><]  {
    yylval = strdup(yytext);
    return ARITH_OPERATOR;
}

">=" {
    yylval = strdup(yytext);
    return ARITH_OPERATOR;
}

"<=" {
    yylval = strdup(yytext);
    return ARITH_OPERATOR;
}

"!=" {
	yylval = strdup(yytext);
	return ARITH_OPERATOR;
}

"@"  {
    yylval = strdup(yytext);
    return CUANTIFIER;
}

"#" {
    yylval = strdup(yytext);
    return CUANTIFIER;
}

%%