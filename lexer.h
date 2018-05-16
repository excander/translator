// #include "common.h"
pragma ONCE;

class Scanner {
// public:
    enum state { H, IDENT, NUMB, STRN, ALE, NEQ, COMDEL, COM, ZVZ, DELIM};
    static char* TW[];
    static type_of_lex words[];
    static char* TD[];
    static type_of_lex dlms[];
    state CS;
    FILE *fp;
    char c;
    char buf[80];
    int buf_top;
    void clear(){
        buf_top = 0;
        for(int i=0; i<80; i++)
            buf[i] = '\0';
    }
    void add(){
        buf[buf_top++] = c;
    }

    int look(const char *buf, char **list){
        int i = 0;
        while (list[i]){
            if ( !strcmp(buf, list[i]) )
                return i;
            i++;
        }
        return 0;
    }
    void gc(){ c = fgetc(fp); }
public:
    Scanner (const char *program){
        fp = fopen(program, "r");
        CS = H;
        clear();
        gc();
    }
    Lex get_lex();
};

char *
        Scanner::TW[] = {"", "program",  "int", "string", "if", "else", "while", "read", "write", "not", "and", "or", "break", "case", "of", "end", "do", NULL};

type_of_lex
        Scanner::words[] {
        LEX_NULL, 								// 0
        LEX_PROGRAM, LEX_INT, LEX_STRING, 		// 1 2 3
        LEX_IF, LEX_ELSE, LEX_WHILE, 			// 4 5 6
        LEX_READ, LEX_WRITE, LEX_NOT, 			// 7 8 9
        LEX_AND, LEX_OR, LEX_BREAK, 			// 10 11 12
        LEX_CASE, LEX_OF, LEX_END, LEX_DO,		// 13 14 15
};

char *
        Scanner::TD[] = { "", "{", "}", ";", ",", "=", "+", "-", "\"", "*", "/", "<", ">", "<=", ">=", "==", "!=", "(", ")", ":", "%", NULL};
type_of_lex
        Scanner::dlms[] {
        LEX_NULL,
        LEX_LBRACE, LEX_RBRACE, LEX_SEMICOLON, 	// { } ;
        LEX_COMMA, LEX_ASSIGN, LEX_PLUS,		// , = +
        LEX_MINUS, LEX_QUOTE, LEX_TIMES, 		// - " *
        LEX_SLASH, LEX_LSS, LEX_GTR,			// / < >
        LEX_LEQ, LEX_GEQ, LEX_EQ,				// <= >= ==
        LEX_NEQ, LEX_LPAREN, LEX_RPAREN,			// != (  )
        LEX_COLON, LEX_PERCENT					// : %
};