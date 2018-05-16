// #include "common.h"

class Parser{
    int ind, sign = 1; // для инициализации констант
    int loop_flag = 0, plb = -1; // для break 
	Lex curr_lex;
	type_of_lex c_type;
	int c_val;
    Stack < type_of_lex, 100 >  st_lex;
	// string str_val; //// ??
	Scanner scan;

	void Programma();
	void Opisaniya();
	// void Opisanie();
	// void Tip();
	void Peremennaya(Lex);
	void Konstanta(Lex);
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
    Poliz prog;
	Parser(const char *program): scan(program), prog(1000){}
	void analyze();
};