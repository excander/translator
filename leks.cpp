#inclide <iostream>

using namespace std;

enum type_of_lex {
	LEX_NULL, 							// 0
	LEX_PROGRAM, LEX_INT, LEX_STRING, 	// 1 2 3
	LEX_IF, LEX_ELSE, LEX_WHILE, 		// 4 5 6 
	LEX_READ, LEX_WRITE, LEX_NOT, 		// 7 8 9
	LEX_AND, LEX_OR, LEX_BREAK, 		// 10 11 12
	LEX_CASE, LEX_OF, LEX_END, LEX_DO,	// 13 14 15

	LEX_LBRACE, LEX_RBRACE, LEX_SEMICOLON, 	// { } ;
	LEX_COMMA, LEX_ASSIGN, LEX_PLUS,		// , = +
	LEX_ MINUS, LEX_QUOTE, LEX_TIMES, 		// - " *			
	LEX_SLASH, LEX_LSS, LEX_GTR,			// / < >
	LEX_LEQ, LEX_GEQ, LEX_EQ,				// <= >= ==
	LEX_NEQ, LEX_LPAREN, RPAREN,			// != (  )
	LEX_COLON, LEX_PERCENT					// : %
};


/////////////////////////  Класс Lex  //////////////////////////

class Lex {
	type_of_lex	t_lex;
	int 		v_lex;
public:
	Lex (type_of_lex t = LEX_NULL, int v = 0){
		t_lex = t;
		v_lex = v;
	}
	type_of_lex get_type(){ return t_lex; }
	int get_value() { return v_lex; }
	friend ostream& operator<< (ostream &s, Lex l){
		s << '(' << l.t_lex << ' , ' << l.v_lex << '); ';
		return s;
	}
};
