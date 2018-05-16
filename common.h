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
    LEX_PHRASE,
    POLIZ_LABEL, 
    POLIZ_ADDRESS,
    POLIZ_GO,
    POLIZ_FGO
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
    "LEX_PHRASE",
    "POLIZ_LABEL", 
    "POLIZ_ADDRESS",
    "POLIZ_GO",
    "POLIZ_FGO"
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
    int value;
    // string strval;

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
    int get_value() { return value; }
    void put_value(int v) { value = v; }
    // string get_strval() { return strval; } 		//???????????
    // void put_strval(string s) { strval = s; } 	//???????????
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

class Tabl_phrase{
    string *p;
    // int size;
    int top;

public:
    Tabl_phrase(int maxsize){
        p = new string[maxsize];
        top = 1;
    }
    ~Tabl_phrase () { delete []p; }
    string& operator[] (int k) { return p[k]; }
    int put (string);
    int get_top(){return top;}
};

int Tabl_phrase::put(string buf){
    // for(int j=1; j<top; j++)
    //     if ( !strcmp(p[j].get_name(), buf) )
    //         return j;
    p[top] = buf;
    top++;
    return top-1;
}

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

/////////////////////  Класс Poliz    //////////////////////////////

class Poliz {
    Lex* p;
    int size;
    int free;
public:
    Poliz(int max_size) {
        p = new Lex[size = max_size];
        free = 0;
    }
    ~Poliz() {delete []p;}
    void put_lex(Lex l){
        p[free] = l;
        free++;
    }
    void put_lex(Lex l, int place){
        if (0 < place && place <= size)
            p[place] = l;
    }
    Lex& operator[](int k){
        if ( k > size )
            throw "POLIZ:out of array";
        else if ( k > free )
            throw "POLIZ:indefinite element of array";
        else
            return p[k];
    }
    void blank(){ free++; }
    int get_free(){ return free; }
    void print(){
        for (int i=0; i<free; i++){
            // cout << p[i];
            cout << str_lex[p[i].get_type()] << endl;
        }
    }
};