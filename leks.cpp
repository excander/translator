#include <iostream>
#include <cstring>

using namespace std;

enum type_of_lex {
	LEX_NULL, 								// 0
	LEX_PROGRAM, LEX_INT, LEX_STRING, 		// 1 2 3
	LEX_IF, LEX_ELSE, LEX_WHILE, 			// 4 5 6 
	LEX_READ, LEX_WRITE, LEX_NOT, 			// 7 8 9
	LEX_AND, LEX_OR, LEX_BREAK, 			// 10 11 12
	LEX_CASE, LEX_OF, LEX_END, LEX_DO,		// 13 14 15

	LEX_LBRACE, LEX_RBRACE, LEX_SEMICOLON, 	// { } ;
	LEX_COMMA, LEX_ASSIGN, LEX_PLUS,		// , = +
	LEX_MINUS, LEX_QUOTE, LEX_TIMES, 		// - " *			
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
		s << '(' << l.t_lex << " , " << l.v_lex << "); ";
		return s;
	}
};

/////////////////////  Класс Ident  ////////////////////////////

class Ident {
	char *name;
	bool declare;
	type_of_lex type;
	bool assign;
	int intval;
	string strval;

public:
	Ident() {
		declare = false;
		assign = false;
	}

	char *get_name() { return name; }
	void put_name(const char *n){
		name = new char [strlen(n)+1];
		strcpy(name, n);
	}
	bool get_declare() { return declare; }
	void put_declare() { declare = true; }
	type_of_lex get_type() { return type; }
	void put_type(type_of_lex t) { type = t; }
	bool get_assign() { return assign; }
	void put_assign() { assign = true; }
	int get_intval() { return intval; }
	void put_intval(int v) { intval = v; }
	string get_strval() { return strval; } 		//???????????
	void put_strval(string s) { strval = s; } 	//???????????
};

//////////////////////  Класс Tabl_ident  ///////////////////////

class Tabl_ident{
	Ident *p;
	int size;
	int top;

public:
	Tabl_ident(int maxsize){
		p = new Ident[size = maxsize];
		top = 1;
	}
	~Tabl_ident () { delete []p; }
	Ident& operator[] (int k) { return p[k]; }
	int put (char *name);
};

int Tabl_ident::put(char *buf){
	for(int j=1; j<top; j++)
		if ( !strcmp(p[j].get_name(), buf) )
			return j; 
	p[top].put_name(buf);
	top++;
	return top-1;
}




int main() {
	Lex l;
	Ident id1;
	id1.put_strval("asd");
	cout << id1.get_strval();
}