/*
 * @<parser.c>::
 */
/*					*
 * Module Developed by Valeria Oliveira *
 *					*/
 
#include <stdio.h>
#include <stdlib.h>

#include <parser.h>	// Import Library parser.h
#include <tokens.h>	// Import Library tokens.h
#include <main.h>	// Import Library main.h

/*
 * lexer-to-parser interface 
 */
int             lookahead;
int             gettoken(FILE *);
/*
 * adopted grammar: LL(1) for simple expressions 
 */
/*************************************************
 * expr -> term rest
 * rest -> '+'term rest | '-'term rest | <empty>
 * term -> fact quoc
 * quoc -> '*'fact quoc | '/'fact quoc | <empty>
 * fact -> ID | DEC | '(' expr ')'
 *************************************************/
/*
 * grammar emulation functions 
 */
void expr(void){
    term();
    rest();
}

/* Absorv '+' '-' in 1 operator */
void rest(void){   
    int oplus = lookahead;
    switch (lookahead) {
    case '+':
    case '-':
        match(lookahead);
        term();
        rest();
//		ação semântica dirigida por sintática *anotação para autora
		/*[*/ fprintf(object,"exec(%c)",oplus);/*]*/ 
        break;
    default:
        ;
    }
}

void term(void){
    fact();
    quoc();
}
/* Auxiliary Function 
Absorv '*' '/' in 1 operator */
void quoc(void){	
	int otimes = lookahead;
    switch (lookahead) {
    case '*':
    case '/':
        match(lookahead);
        fact();
        quoc();
		/*[*/ fprintf(object,"exec(%c)",otimes);/*]*/ 
        break;
    default:
        ;
    }
}

/* Informs in order the factors that are analyzed by returning the **types** */ 
void fact(void){
    switch (lookahead) {
    case UINT:
		/*[*/ fprintf(object," uint ");/*]*/ 
        match(UINT);
        break;
    case FLT:
		/*[*/ fprintf(object," float ");/*]*/ 
        match(FLT);
        break;
    default:
        match('(');
        expr();
        match(')');
    }
}

/*
 * lexer-to-parser interface: 
 */
void match(int expected){
    if (lookahead == expected) {
        lookahead = gettoken(source);
    } else {
        fprintf(stderr,
                "%d seen while %d was expected\n", lookahead, expected);
        exit(-2);
    }
}
