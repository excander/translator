#include <iostream>
#include <cstring>

using namespace std;

enum type_of_lex {
    LEX_NULL, 								// 0
    LEX_PROGRAM, LEX_INT, LEX_STRING, 		// 1 2 3
    LEX_IF, LEX_ELSE, LEX_WHILE, 			// 4 5 6
    LEX_READ, LEX_WRITE, LEX_NOT, 			// 7 8 9
    LEX_AND, LEX_OR, LEX_BREAK, 			// 10 11 12
    LEX_CASE, LEX_OF, LEX_END, LEX_DO,		// 13 14 15 16

    LEX_LBRACE, LEX_RBRACE, LEX_SEMICOLON, 	// { } ;
    LEX_COMMA, LEX_ASSIGN, LEX_PLUS,		// , = +
    LEX_MINUS, LEX_QUOTE, LEX_TIMES, 		// - " *
    LEX_SLASH, LEX_LSS, LEX_GTR,			// / < >
    LEX_LEQ, LEX_GEQ, LEX_EQ,				// <= >= ==
    LEX_NEQ, LEX_LPAREN, LEX_RPAREN,		// != (  )
    LEX_COLON, LEX_PERCENT,					// : %
    LEX_NUM,
    LEX_ID,
    LEX_EOF,
    LEX_PHRASE
};

string str_lex [100] = {
    "LEX_NULL", 								// 0
    "LEX_PROGRAM", "LEX_INT", "LEX_STRING", 	// 1 2 3
    "LEX_IF", "LEX_ELSE", "LEX_WHILE", 			// 4 5 6
    "LEX_READ", "LEX_WRITE", "LEX_NOT", 		// 7 8 9
    "LEX_AND", "LEX_OR", "LEX_BREAK", 			// 10 11 12
    "LEX_CASE", "LEX_OF", "LEX_END", "LEX_DO",	// 13 14 15 16

    "LEX_LBRACE", "LEX_RBRACE", "LEX_SEMICOLON",// { } ;
    "LEX_COMMA", "LEX_ASSIGN", "LEX_PLUS",		// , = +
    "LEX_MINUS", "LEX_QUOTE", "LEX_TIMES", 		// - " *
    "LEX_SLASH", "LEX_LSS", "LEX_GTR",			// / < >
    "LEX_LEQ", "LEX_GEQ", "LEX_EQ",				// <= >= ==
    "LEX_NEQ", "LEX_LPAREN", "LEX_RPAREN",		// != (  )
    "LEX_COLON", "LEX_PERCENT",					// : %
    "LEX_NUM",
    "LEX_ID",
    "LEX_EOF",
    "LEX_PHRASE"
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
    int get_size(){return size;}
};

int Tabl_ident::put(char *buf){
    for(int j=1; j<top; j++)
        if ( !strcmp(p[j].get_name(), buf) )
            return j;
    p[top].put_name(buf);
    top++;
    return top-1;
}

//////////////////////  Класс Tabl_phrase  ///////////////////////

// class Tabl_phrase{
//     string *p;
//     int size;
//     int top;

// public:
//     Tabl_phrase(int maxsize){
//         p = new string[maxsize];
//         top = 1;
//     }
//     ~Tabl_phrase () { delete []p; }
//     string& operator[] (int k) { return p[k]; }
//     int put (char *);
// };

// int Tabl_phrase::put(char *buf){
//     // for(int j=1; j<top; j++)
//     //     if ( !strcmp(p[j].get_name(), buf) )
//     //         return j;
//     p[top] = string(buf);
//     top++;
//     return top-1;
// }

////////////////////////////////////////////////////////////////////

Tabl_ident TID(100);
// Tabl_phrase TPHR(100);

///////////////////////////////////////////////////////////////////

template < class T, int max_size >
class Stack
{
         T            s [max_size];
         int          top;
public:
                      Stack () { top = 0; }
         void         reset () { top = 0; }
         void         push ( T i );
         T            pop ();
         bool         is_empty () { return top == 0; }
         bool         is_full  () { return top == max_size; }
};
 
template < class T, int max_size >
void Stack < T, max_size > :: push (T i)
{
  if ( !is_full() )
    s [top++] = i;
  else
    throw "Stack_is_full";
}
 
template <class T, int max_size >
T Stack < T, max_size > :: pop ()
{
  if ( !is_empty() )
    return s[--top];
  else
    throw "stack_is_empty";
}

/////////////////////  Класс Scanner  //////////////////////////////

class Scanner {
public:
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
// public:
    Scanner (const char *program){
        fp = fopen(program, "r");
        CS = H;
        clear();
        gc();
    }
    Lex get_lex();
};

char *
        Scanner::TW[] {"", "program",  "int", "string", "if", "else", "while", "read", "write", "not", "and", "or", "break", "case", "of", "end", "do", NULL};

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
        Scanner::TD[] { "", "{", "}", ";", ",", "=", "+", "-", "\"", "*", "/", "<", ">", "<=", ">=", "==", "!=", "(", ")", ":", "%", NULL};
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

Lex
Scanner::get_lex() {
    int d,j;

    CS = H;
    do {
        switch(CS){
            case H:
                if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
                    gc();
                else if (isalpha(c)){
                    clear();
                    add();
                    gc();
                    CS = IDENT;
                }
                else if (isdigit(c)){
                    d = c - '0';
                    gc();
                    CS = NUMB;
                }
                else if (c == '"'){
                    clear();
                    gc();
                    CS = STRN;
                }
                else if (c == '<' || c == '>' || c == '='){
                    clear();
                    add();
                    gc();
                    CS = ALE;
                }
                else if (c == '!'){
                    clear();
                    add();
                    gc();
                    CS = NEQ;
                }
                else if (c == '/'){
                    clear();
                    add();
                    gc();
                    CS = COMDEL;
                }
                else if (c == EOF){
                    return Lex(LEX_EOF, LEX_EOF); ///////////////////////////////////
                    break;
                }
                else {
                    // cout << "delim";
                    CS = DELIM;}
                break;

            case IDENT:
                if (isalpha(c) || isdigit(c)){
                    add();
                    gc();
                }
                else if (j = look(buf, TW)){
                    return Lex(words[j], j);
                    CS = H;
                }
                else {
                    j = TID.put(buf);
                    return Lex(LEX_ID, j);
                    CS = H;
                }
                break;

            case NUMB:
                if (isdigit(c)){
                    d = d*10 + c-'0';
                    gc();
                }
                else{
                    return Lex(LEX_NUM, d);
                	CS = H;
                }
                break;

            case STRN:
            	if (isprint(c) && c!= '"'){
            		add();
            		gc();
            	}
            	else if  (c == '"'){
            		gc();
            		// j = TPHR.put(buf);
            		return Lex(LEX_PHRASE);
            	}
            	else {
            		gc();
            	}
                break;

            case ALE:
                if (c == '='){
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                    CS = H;
                }
                else {
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                    CS = H;
                }
                break;

            case NEQ:
                if (c == '='){
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                    CS = H;
                }
                else {
                    throw "error";
                }
                break;

            case COMDEL:
                if (c == '*'){
                    gc();
                    CS = COM;
                }
                else {
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                    CS = H;
                }
                break;

            case COM:
                if (c == '*'){
                    gc();
                    CS = ZVZ;
                }
                else if (c == EOF){
                    throw "unexpected EOF (in comment)";
                }
                else
                    gc();
                break;
            case ZVZ:
                if (c == '/'){
                    gc();
                    CS = H;
                }
                else if (c == EOF){
                    throw "unexpected EOF (in comment)";
                }
                else {
                    gc();
                    CS = COM;
                }
                break;
            case DELIM:
                if (c == '*' || c == '+' || c == '-' || c == '/' || c == ',' ||
                    c == ';' || c == '(' || c == ')' || c == '{' || c == '}'){
                    clear();
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                    CS = H;
                }
                else{
                    throw("error");
                }
                break;
        }
    }//end switch
    while(true);

}

class Parser{
	Lex curr_lex;
	type_of_lex c_type;
	int c_val;
    Stack < type_of_lex, 100 >  st_lex;
	string str_val; /////////////////// ??
	Scanner scan;

	void Programma();
	void Opisaniya();
	// void Opisanie();
	// void Tip();
	void Peremennaya(type_of_lex);
	void Konstanta(type_of_lex param_type);
	void Celochislennaya();
	// void Znak();
	void Strokovaya();
	void Operatori();
	void Operator();
	void SostavnoyOperator();
	void OperatorViragenie();
	void Viragenie();

	void E1();
	void T();
	void F();

    void check_id ();
    void check_op ();
    void check_not ();
    void eq_type ();
    void eq_int();
    void check_id_in_read();

	void gl(){
		curr_lex = scan.get_lex();
		c_type = curr_lex.get_type();
		c_val = curr_lex.get_value();
	};

public:
	Parser(const char *program): scan(program){}
	void analyze();
};



void Parser::analyze(){
	gl();
	Programma();
	cout << endl << "Success!!!" << endl;
}

void Parser::Programma(){
	if (c_type == LEX_PROGRAM)
		gl();
	else
		throw curr_lex;
	if (c_type == LEX_LBRACE)
		gl();
	else
		throw curr_lex;
	Opisaniya();
	Operatori();
	if (c_type == LEX_RBRACE)
		gl();
	else
		throw curr_lex;
	if (c_type != LEX_EOF)
		throw curr_lex;
}

void Parser::Opisaniya(){
	while (c_type == LEX_INT || c_type == LEX_STRING){
        type_of_lex temp_type = c_type;
		gl();
		Peremennaya(temp_type);
		while (c_type == LEX_COMMA){
			gl();
			Peremennaya(temp_type);
		}
		if (c_type == LEX_SEMICOLON)
			gl();
		else 
			throw curr_lex;
	}
}

// void Parser::Opisanie(){}
// void Parser::Tip(){}
void Parser::Peremennaya(type_of_lex param_type){
	if (c_type == LEX_ID){
		int i=curr_lex.get_value();
        if (TID[i].get_declare())
            throw string("Error: Variable ")+ string(TID[i].get_name()) + string(" declared twice!");
        else { 
            TID[i].put_declare();
            TID[i].put_type(param_type);
            gl();
        }
    }
	else
		throw curr_lex;
	if (c_type == LEX_ASSIGN){
		gl();
		Konstanta(param_type);
	}
}
void Parser::Konstanta(type_of_lex param_type){
	if (c_type == LEX_PLUS || c_type == LEX_MINUS){
		gl();
        if (param_type == LEX_INT)
		  Celochislennaya();
        else
            throw string("Error: Incorrect initialization of variable with type ") + string(str_lex[param_type]);
	}
	else if (c_type == LEX_NUM){
		if (param_type == LEX_INT)
          Celochislennaya();
        else
            throw string("Error: Incorrect initialization of variable with type ") + string(str_lex[param_type]);
	}
	// else if (c_type == LEX_QUOTE){
	// }
	else if (c_type == LEX_PHRASE){
		if (param_type == LEX_STRING)
          Strokovaya();
        else
            throw string("Error: Incorrect initialization of variable with type ") + string(str_lex[param_type]);
	}
	else
		throw curr_lex;
}

void Parser::Celochislennaya(){
	if (c_type == LEX_NUM)
		gl();
	else 
		throw curr_lex;
}

// void Parser::Znak(){}
void Parser::Strokovaya(){
	if (c_type == LEX_PHRASE)
		gl();
	else 
		throw curr_lex;
}
void Parser::Operatori(){
	while (c_type == LEX_IF || 
		c_type == LEX_WHILE || 
		c_type == LEX_READ || 
		c_type == LEX_WRITE || 
		c_type == LEX_LBRACE ||
		c_type == LEX_ID){
			Operator();
	}
}

void Parser::Operator(){
	if (c_type == LEX_IF){
		gl();
		if (c_type == LEX_LPAREN)
			gl();
		else
			throw curr_lex;
		Viragenie();
        eq_int(); // проверка типов в операторах
		if (c_type == LEX_RPAREN)
			gl();
		else
			throw curr_lex;
		Operator();
		if (c_type == LEX_ELSE)
			gl();
		else
			throw curr_lex;
		Operator();
	}
	else if (c_type == LEX_WHILE){
		gl();
		if (c_type == LEX_LPAREN)
			gl();
		else
			throw curr_lex;
		Viragenie();
        eq_int(); // проверка типов в операторах
		if (c_type == LEX_RPAREN)
			gl();
		else
			throw curr_lex;
		Operator();
	}
	else if (c_type == LEX_READ){
		gl();
		if (c_type == LEX_LPAREN)
			gl();
		else
			throw curr_lex;
		if (c_type == LEX_ID){
            check_id_in_read(); // проверка типов в операторах
			gl();
        }
		else
			throw curr_lex;
		if (c_type == LEX_RPAREN)
			gl();
		else
			throw curr_lex;
		if (c_type == LEX_SEMICOLON)
			gl();
		else
			throw curr_lex;
	}
	else if (c_type == LEX_WRITE){
		gl();
		if (c_type == LEX_LPAREN)
			gl();
		else
			throw curr_lex;
		Viragenie();
		while (c_type == LEX_COMMA){
			gl();
			Viragenie();
		}
		if (c_type == LEX_RPAREN)
			gl();
		else
			throw curr_lex;
		if (c_type == LEX_SEMICOLON)
			gl();
		else
			throw curr_lex;
	}
	else if (c_type == LEX_LBRACE){   // SostavnoyOperator()
		gl();
		Operatori();
		if (c_type == LEX_RBRACE)
			gl();
		else
			throw curr_lex;
	}
	else if (c_type == LEX_ID){
        check_id (); // проверка типов в операторах
		gl();
		if (c_type == LEX_ASSIGN){
			gl();
			OperatorViragenie();
            eq_type(); // проверка типов в операторах
		}
		else
			throw curr_lex;
	}
	else
		OperatorViragenie();
}

// void Parser::SostavnoyOperator(){}
void Parser::OperatorViragenie(){
	Viragenie();
	if (c_type == LEX_SEMICOLON)
		gl();
	else
		throw curr_lex;
}

void Parser::Viragenie(){
	E1();
	if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
		c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ){
            st_lex.push(c_type); // проверка типов в выражении
			gl();
			E1();
            check_op(); // проверка типов в выражении
	}
}

void Parser::E1 ()
{
  T();
  while ( c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR){
    st_lex.push(c_type); // проверка типов в выражении
    gl();
    T();
    check_op(); // проверка типов в выражении
  }
}
 
void Parser::T ()
{
  F();
  while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
    st_lex.push(c_type); // проверка типов в выражении
    gl();
    F();
    check_op(); // проверка типов в выражении
  }
}
 
void Parser::F () 
{
  if ( c_type == LEX_ID ){
    check_id(); // проверка типов в выражении
    gl();
  }
  else if ( c_type == LEX_NUM ){
    st_lex.push(LEX_INT); // проверка типов в выражении
    gl();
  }
  else if (c_type == LEX_NOT){
    gl(); 
    F(); 
    check_not(); // проверка типов в выражении
  }
  else if (c_type == LEX_PHRASE){
    st_lex.push(LEX_STRING); // проверка типов в выражении
    gl();
  }
  else if ( c_type == LEX_LPAREN ) 
  {
    gl(); 
	Viragenie();
    if ( c_type == LEX_RPAREN)
      gl();
    else 
      throw curr_lex;
  }
  else 
    throw curr_lex;
}

////////////////проверка типов в выражении////////////////////////

void Parser::check_id () 
{
  if ( TID[c_val].get_declare() )
    st_lex.push ( TID[c_val].get_type() );
  else 
    throw  string("Error: Variable ") + string(TID[c_val].get_name()) + string(" is not declared!");
}

void Parser::check_op () 
{
  type_of_lex t1, t2, op;
 
  t2 = st_lex.pop();
  op = st_lex.pop();
  t1 = st_lex.pop();
  if (t1 == t2){
    if (op == LEX_PLUS && t1 == LEX_STRING)
        st_lex.push(LEX_STRING);
    else if (t1 == LEX_STRING && (op == LEX_MINUS || op == LEX_TIMES|| op == LEX_SLASH || op == LEX_PERCENT ||
             op == LEX_LEQ || op == LEX_GEQ || op == LEX_AND || op == LEX_OR))
        throw "Error: Incorrect operation for string type!";
    else
        st_lex.push(LEX_INT);
    // st_lex.push(r);
  }
  else
    throw "Error: Unequal types in expression!";
  // prog.put_lex (Lex (op) );
}

void Parser::check_not () 
{
  if (st_lex.pop() != LEX_INT)
    throw "wrong type is in not";
  else 
  {
    st_lex.push (LEX_INT);
    // prog.put_lex (Lex (LEX_NOT));
  }
}

////////////////проверка типов в операторах////////////////////////

void Parser::eq_type (){
    if (st_lex.pop() != st_lex.pop())
        throw "Error: Wrong types in assign operator!";
} 

void Parser::eq_int (){
    if (st_lex.pop() != LEX_INT)
        throw "Error: Type of conditional expression must be integer!";
}

void Parser::check_id_in_read (){
    if (!TID[c_val].get_declare())
        string("Error: Variable ") + string(TID[c_val].get_name()) + string(" is not declared in read operator!");
}

int main() {
    // Lex l;
    // Ident id1;
    // id1.put_strval("asd");
    // cout << id1.get_strval() << endl; 
 

	// Scanner scan1("input.txt");
 //    try {
	//     Lex lex1;
	//     int i = 1;
	//     lex1 = scan1.get_lex();
	//     while (lex1.get_type() != LEX_EOF){
	//     	cout << lex1 << " <=> " << str_lex[lex1.get_type()] << endl;
	//     	lex1 = scan1.get_lex();
	//     	i++;
	//     }
 //    }
 //    catch (char const* str){
 //    	cout << str << endl;
	// }

	Parser pars1("input.txt");
	try {
		pars1.analyze();
	}
	catch (char const* str){
    	cout << str << endl;
    }
    catch (Lex l)
  {
    cout << "unexpected lexeme";
    cout << l << " <=> " << str_lex[l.get_type()] << endl;
    return 1;
  }
    catch (string s){
        cout << s << endl;
    }

// // Вывод всех идентификаторов из таблицы TID, с типом и флагом описания
// Ident cid;
// int i=1;
//  while (TID[i].get_name()){
//     cout << TID[i].get_name() << "  " << str_lex[TID[i].get_type()]<< " -> " << TID[i].get_declare() << endl;
//     i++;
// }

}