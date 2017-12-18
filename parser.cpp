// purpose: CSE 4713 / 6713 Assignment 2 Skeleton code
//*****************************************************************************
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>

using namespace std;

#include "lexer.h"
#include "parseTree.h"

#define VERBOSE
expressionDictionary combinatorDictionary ;


// Production functions
void P( void );
void S( void );
void J( void );
expressionP K( void );  // **************** K was expressionP (editing) ********************
void L( void );
void M( void );
expressionP A( void );
expressionP F( void );
expressionP I( void );
int E( void );
int T( void );
int R( void );

// Function declarations for checking whether the current token is
// in the first set of each production rule.
bool IsFirstOfP( void );
bool IsFirstOfS( void );
bool IsFirstOfJ( void );
bool IsFirstOfK( void );
bool IsFirstOfL( void );
bool IsFirstOfM( void );
bool IsFirstOfA( void );
bool IsFirstOfF( void );
bool IsFirstOfI( void );
bool IsFirstOfE( void );
bool IsFirstOfT( void );
bool IsFirstOfR( void );


//*****************************************************************************
// Function to help print the tree nesting
string psp( int n ) // Stands for p-space, but I want the name short
{
    string str( n, ' ' );
    return str;
}

// Needed global variables
int iTok;                  // The current token

#ifdef VERBOSE
int parsingDepth = -1 ;
#endif


//*****************************************************************************
// The main processing loop
int main( int argc, char* argv[] )
{
    int token;   // hold each token code
    
    // Set the input and output streams
    yyout = stdout;
    yyin = stdin;
    // Get the first token
    iTok = yylex();
    // Fire up the parser!
    try {
        // Process P Production
        while (IsFirstOfP())
        {
            P();
        }
        
        if( iTok != TOK_EOF )
            throw "end of file expected, but there is more here!";
    }
    catch( char const *errmsg ) {
        cout << endl << "***ERROR (line " << yyLine << "): "<< errmsg << endl;
        
        exit(-1) ;
        while(!feof(yyin)) {
            iTok = yylex() ;
            if(iTok==TOK_SEMICOLON)
                break ;
        }
        iTok = yylex() ;
        
    }
    
    
    cout << "Combinator Dictionary is:" << endl ;
    combinatorDictionary.Print(cout) ;
    
    return 0;
}




//*****************************************************************************
//*****************************************************************************
// <P> --> <S> { <S> }
void P( void )
{
    static int Pcnt = 0; // P Count
    int CurPcnt = Pcnt++;
    char const *Perr =
    "statement does not start with recognized keyword";
    
    
    cout << psp( ++parsingDepth ) << "enter P " << CurPcnt << endl;
    if (!IsFirstOfS())
    {
        throw Perr;
    }
    else
        S();
    while(IsFirstOfS()) // A series of S's
        S();
    
    cout << psp( parsingDepth-- ) << "exit P " << CurPcnt << endl;
    
}




//*****************************************************************************
//*****************************************************************************
// S --> (J | K | L | M) ";"

void S( void )
{
    static int Scnt = 0; // S Count
    int CurScnt = Scnt++;
    char const *Serr =
    "J/K/L/M not found";
    
    cout << psp( ++parsingDepth ) << "enter S " << CurScnt << endl;
    
    
    // (J | K | L | M)
    if (IsFirstOfJ())
    {
        J();
    }
    if (IsFirstOfK())
    {
        K();
    }
    if (IsFirstOfL())
    {
        L();
    }
    if (IsFirstOfM())
    {
        M();
    }
    
    
    // ';'
    if(iTok != TOK_SEMICOLON) // Should end with semicolon
        throw "missing ';' at end of assignment statement";
    
    cout << psp(parsingDepth) << "-->found " << yytext << endl;
    
    // Next
    iTok = yylex();
    cout << psp(parsingDepth--) << "exit S " << CurScnt << endl;
}


//*****************************************************************************
// J --> "combinator" ["evaluate"] ID A

void J( void )
{
    float rValue1 = 0;   // The value to return
    float rValue2;
    static int Jcnt = 0; // J Count
    int CurJcnt = Jcnt++;
    
    cout << psp( ++parsingDepth ) << "enter J " << CurJcnt << endl;
    
    // Combinator
    cout << psp(parsingDepth) << "-->found " << yytext << endl;
    
    // Next
    iTok = yylex();
    
    // Check Evaluate
    if (iTok == TOK_EVALUATE)
    {
        cout << psp(parsingDepth) << "-->found " << yytext << endl;
        // Next
        iTok = yylex();
    }
    else
        // Next
        iTok = yylex();
    
    // Check if !A()
    if (!IsFirstOfA())
    {
        throw "A not found in J";
    }
    else
        // Else call A
        A();
    cout << "Combinator Dictionary is:" << endl ;
    //combinatorDictionary.insertItem() ;
    cout << psp( parsingDepth-- ) << "exit J " << CurJcnt << endl;
}

//*****************************************************************************
//*****************************************************************************
// K --> "evaluate" A

expressionP K( void )
{
    float rValue1 = 0;   // The value to return
    float rValue2;
    static int Kcnt = 0; // K Count
    int CurKcnt = Kcnt++;
    
    cout << psp( ++parsingDepth ) << "enter K " << CurKcnt << endl;
    cout << psp(parsingDepth) << "-->found " << yytext << endl;
    expressionP  term = 0;
    // Next
    iTok = yylex();
    
    // Check if A()
    if (IsFirstOfA())
    {
        term = A();
        
    }
    cout << "Eval expression: " << term->String() << endl;
    cout << psp( parsingDepth-- ) << "exit K " << CurKcnt << endl;
    return term;
}


//*****************************************************************************
//*****************************************************************************
// L --> "dictionary"

void L( void )
{
    float rValue1 = 0;   // The value to return
    float rValue2;
    static int Lcnt = 0; // L Count
    int CurLcnt = Lcnt++;
    
    cout << psp( ++parsingDepth ) << "enter L " << CurLcnt << endl;
    // Check if diction.
    if( iTok == TOK_DICTIONARY)
    {
        cout << "Combinator Dictionary is: " <<  combinatorDictionary.Print(cout);
        // Next
        iTok = yylex();
    }
    
    cout << psp( parsingDepth-- ) << "exit L " << CurLcnt << endl;
    
}


//*****************************************************************************
//*****************************************************************************
// M --> "set" ID E

void M( void )
{
    float rValue;        // Value returned from expression
    static int Mcnt = 0; // M Count
    int CurMcnt = Mcnt++;
    
    cout << psp( ++parsingDepth ) << "enter M " << CurMcnt << endl;
    
    // We know we have found a 'set' token
    cout << psp(parsingDepth) << "-->found " << yytext << endl;
    
    // Next
    iTok = yylex();
    
    
    // Check if not ID
    if( iTok != TOK_IDENTIFIER ) // Check for ID
        throw "missing identifier in assignment statement";
    else
        cout << psp(parsingDepth) << "-->found ID: " << yytext << endl;
    
    // Next
    iTok = yylex();
    
    // Check if !E()
    if(!IsFirstOfE() )
        throw "E not found in M";
    else
        // Else call E()
        E();
    
    cout << psp( parsingDepth-- ) << "exit M " << CurMcnt << endl;
    
}
//*****************************************************************************
//*****************************************************************************
// F --> "\" ID "." A | "{" F "}"

expressionP F( void )
{
    
    float rValue = 0;     // the value to return
    float rValue2;
    static int Fcnt = 0;  // F Count
    int CurFcnt = Fcnt++;
    char const *Ferr =
    "Error with F";
    cout << psp( ++parsingDepth ) << endl; // << "enter F " << CurFcnt << endl;
    // Found Slash
    cout << psp(parsingDepth) << "-->found " << yytext << endl;
    
    expressionP term2 = 0;
    // Next
    iTok = yylex();
    // Check if ID
    if (iTok == TOK_IDENTIFIER)
    {
        cout << psp(parsingDepth) << "-->found ID: " << yytext << endl;
        // Next
        iTok = yylex();
    }
    // Check if "."
    if (iTok == TOK_DOT)
    {
        cout << psp(parsingDepth) << "-->found: " << yytext << endl;
        // Next
        iTok = yylex();
    }
    // Check if !A()
    if (!IsFirstOfA())
    {
        throw "No A in F";
    }
    else
    {
        // else call A()
        A();
    }
    
    
    // Check if "{"
    if (iTok == TOK_OPENBRACE)
    {
        cout << psp(parsingDepth) << "-->found: " << yytext << endl;
        // Next
        iTok = yylex();
    }
    // Check if F();
    if (IsFirstOfF())
    {
        // Call F()
        F();
        // Next
        iTok = yylex();
    }
    // Check if "}"
    if (iTok == TOK_CLOSEBRACE)
    {
        cout << psp(parsingDepth) << "-->found: " << yytext << endl;
        // Next
        iTok = yylex();
    }
    
    cout << psp( parsingDepth-- ) << endl; // << "exit F " << CurFcnt << endl;
    return term2;
}

//*****************************************************************************
//*****************************************************************************
// A --> I {I} left associative

expressionP A( void )

{
    static int Acnt = 0; // A Count
    int CurAcnt = Acnt++;
    cout << psp( ++parsingDepth ) << "enter A " << CurAcnt << endl;
    char const *Aerr =
    "I not found";
    expressionP term = 0 ;
    // While series of I's
    
    if(IsFirstOfI())
        
        I();
    
    
    
    while (IsFirstOfI())
    {
        I();
        
    }
    
    term = makeVariable(string(yytext)) ;
    // term = makeApplication(makeApplication(makeVariable(string(yytext));
    
    
    
    
    cout << psp( parsingDepth-- ) << "exit A " << CurAcnt << endl;
    return term;
}



//*****************************************************************************
//****************************************************************************
// I --> CID | ID | "(" A ")" | F

expressionP I( void )
{
    
    float rValue = 0;
    float rValue2;
    static int Icnt = 0;   // I Count
    int CurIcnt = Icnt++;
    cout << psp( ++parsingDepth ) << "enter I " << CurIcnt << endl;
    expressionP term = 0;
    
    // Check if CID
    if (iTok == TOK_CIDENTIFIER)
    {
        cout << psp(parsingDepth) << "-->found CID: " << yytext << endl;
        term = makeVariable(string(yytext)) ;
        // Next
        iTok = yylex();
    }
    
    // Check if ID
    if (iTok == TOK_IDENTIFIER)
    {
        cout << psp(parsingDepth) << "-->found ID: " << yytext << endl;
        term = makeVariable(string(yytext)) ;
        // Next
        iTok = yylex();
    }
    
    // Check if "("
    if (iTok == TOK_OPENPAREN)
    {
        cout << psp(parsingDepth) << "-->found "<< yytext  << endl;
        // Next
        iTok = yylex();
        // Check if !A();
        if (!IsFirstOfA())
        {
            throw "No A";
        }
        
        else
            // Call A()
            term = A();
        // Check if ")"
        if( iTok == TOK_CLOSEPAREN )
        {
            cout << psp(parsingDepth) << "-->found "<< yytext  << endl;
            // Next
            iTok = yylex();
            
        }
    }
    
    // Check if F()
    if (IsFirstOfF())
    {
        cout << "!" << endl;
        // Call F()
        term = F();
        
    }
    cout << psp( parsingDepth-- ) << "exit I " << CurIcnt << endl;
    return term;
}


//*****************************************************************************
//*****************************************************************************
// E --> T { ( + | - ) T }

int E( void )
{
    float rValue1 = 0;   // The value to return
    float rValue2;
    static int Ecnt = 0; // E Count
    int CurEcnt = Ecnt++;
    char const *Terr =
    "term does not start with T ";
    cout << psp( ++parsingDepth ) << "enter E " << CurEcnt << endl;
    
    // Check if T();
    if( IsFirstOfT() )
        rValue1 = T();
    else
        throw "Error Message T";
    
    // As long as the next token is + or -, keep parsing T's
    while( iTok == TOK_PLUS || iTok == TOK_MINUS )
    {
        cout << psp(parsingDepth) << "-->found " << yytext << endl;
        int iTokLast = iTok;
        // Next
        iTok = yylex();
        if( IsFirstOfT() )
            rValue2 = T();
        else
            throw "Error Message T";
        
        // Perform the operation to update rValue1 acording to rValue2
        switch( iTokLast )
        {
            case TOK_PLUS:
                rValue1 = rValue1 + rValue2;
                break;
                
            case TOK_MINUS:
                rValue1 = rValue1 - rValue2;
        }
    }
    
    cout << psp( parsingDepth-- ) << "exit E " << CurEcnt << endl;
    return rValue1;
}



//*****************************************************************************
//*****************************************************************************
// T --> R { ( * | / ) R }

int T( void )
{
    float rValue1 = 0;   // The value to return
    float rValue2;
    static int Tcnt = 0; // T Count
    int CurTcnt = Tcnt++;
    char const *Rerr =
    "factor does not start with R";
    cout << psp( ++parsingDepth ) << "enter T " << CurTcnt << endl;
    if( IsFirstOfR() )
        rValue1 = R();
    else
        throw "Error Message R";
    
    // As long as the next token is * or /, keep parsing F's
    while( iTok == TOK_MULTIPLY || iTok == TOK_DIVIDE )
    {
        cout << psp(parsingDepth) << "-->found " << yytext << endl;
        int iTokLast = iTok;
        // Next
        iTok = yylex();
        if( IsFirstOfR() )
            rValue2 = R();
        else
            throw "Error Message R";
        
        // Perform the operation to update rValue1 acording to rValue2
        switch( iTokLast )
        {
            case TOK_MULTIPLY:
                rValue1 = rValue1 * rValue2;
                break;
                
            case TOK_DIVIDE:
                rValue1 = rValue1 / rValue2;
        }
    }
    
    cout << psp( parsingDepth-- ) << "exit T " << CurTcnt << endl;
    return rValue1;
}


//*****************************************************************************
//*****************************************************************************
// R -->  INTLIT | (
int R( void )
{
    float rValue = 0;           // the value to return
    
    static int Rcnt = 0;        // R Count
    int CurRcnt = Rcnt++;
    
    cout << psp( ++parsingDepth ) << "enter R " << CurRcnt << endl;
    // Determine what token we have
    switch( iTok )
    {
        case TOK_INTLIT:
            cout << psp(parsingDepth) << "-->found INTLIT: " << yytext << endl;
            rValue = (float)atof( yytext );
            // Next
            iTok = yylex();
            break;
            
        case TOK_OPENPAREN:
            cout << psp(parsingDepth) << "-->found (" << endl;
            // Next
            iTok = yylex();
            rValue = E();
            if( iTok == TOK_CLOSEPAREN ) {
                cout << psp(parsingDepth) << "-->found )" << endl;
                iTok = yylex();
            }
            else
                throw "E does not end with )";
            break;
            
        default:
            // If we made it to here, syntax error
            throw "factor does not start with 'INTLIT' | '('";
    }
    cout << psp( parsingDepth-- ) << "exit R " << CurRcnt << endl;
    
    return rValue;
}






//*****************************************************************************
bool IsFirstOfP( void )
{
    return iTok == TOK_COMBINATOR || iTok == TOK_EVALUATE || iTok == TOK_DICTIONARY || iTok == TOK_SET;
}


//*****************************************************************************
bool IsFirstOfS( void )
{
    return iTok == TOK_COMBINATOR || iTok == TOK_EVALUATE || iTok == TOK_DICTIONARY || iTok == TOK_SET;
}


//*****************************************************************************
bool IsFirstOfJ( void )
{
    return iTok == TOK_COMBINATOR;
}


//*****************************************************************************
bool IsFirstOfK( void )
{
    return iTok == TOK_EVALUATE;
}


//*****************************************************************************
bool IsFirstOfL( void )
{
    return iTok == TOK_DICTIONARY;
}


//*****************************************************************************
bool IsFirstOfM( void )
{
    return iTok == TOK_SET;
}


//*****************************************************************************
bool IsFirstOfA( void )
{
    return iTok == TOK_IDENTIFIER || iTok == TOK_CIDENTIFIER || iTok == TOK_SLASH || iTok == TOK_OPENBRACE || iTok == TOK_OPENPAREN;
}


//*****************************************************************************
bool IsFirstOfF( void )
{
    return iTok == TOK_SLASH || iTok == TOK_OPENBRACE;
}


//*****************************************************************************
bool IsFirstOfI( void )
{
    return iTok == TOK_IDENTIFIER || iTok == TOK_CIDENTIFIER || iTok == TOK_SLASH || iTok == TOK_OPENPAREN || iTok == TOK_OPENBRACE;
}


//*****************************************************************************
bool IsFirstOfE( void )
{
    return iTok == TOK_INTLIT || iTok == TOK_OPENPAREN;
}


//*****************************************************************************
bool IsFirstOfT( void )
{
    return iTok == TOK_INTLIT || iTok == TOK_OPENPAREN;
}


//*****************************************************************************
bool IsFirstOfR( void )
{
    return iTok == TOK_INTLIT || iTok == TOK_OPENPAREN;
}
