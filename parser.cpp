// #include "common.h"
#include "parser.h"

void Parser::analyze(){
	gl();
	Programma();
	// cout << endl << "Success!!!" << endl;
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
        Lex param_lex = curr_lex;
		gl();
		Peremennaya(param_lex);
		while (c_type == LEX_COMMA){
			gl();
			Peremennaya(param_lex);
		}
		if (c_type == LEX_SEMICOLON)
			gl();
		else 
			throw curr_lex;
	}
}

// void Parser::Opisanie(){}
// void Parser::Tip(){}
void Parser::Peremennaya(Lex param_lex){
	if (c_type == LEX_ID){
		ind=curr_lex.get_value();
        if (TID[ind].get_declare())
            throw string("Error: Variable ")+ string(TID[ind].get_name()) + string(" declared twice!");
        else { 
            TID[ind].put_declare();
            TID[ind].put_type(param_lex.get_type());
            gl();
        }
    }
	else
		throw curr_lex;
	if (c_type == LEX_ASSIGN){
		gl();
		Konstanta(param_lex);
	}
}
void Parser::Konstanta(Lex param_lex){
	if (c_type == LEX_PLUS || c_type == LEX_MINUS){
        (c_type == LEX_MINUS)?sign=-1:sign=1;
		gl();
        if (param_lex.get_type() == LEX_INT)
		  Celochislennaya();
        else
            throw string("Error: Incorrect initialization of variable with type ") + string(str_lex[param_lex.get_type()]);
	}
	else if (c_type == LEX_NUM){
		if (param_lex.get_type() == LEX_INT)
          Celochislennaya();
        else
            throw string("Error: Incorrect initialization of variable with type ") + string(str_lex[param_lex.get_type()]);
	}
	// else if (c_type == LEX_QUOTE){
	// }
	else if (c_type == LEX_PHRASE){
		if (param_lex.get_type() == LEX_STRING)
          Strokovaya();
        else
            throw string("Error: Incorrect initialization of variable with type ") + string(str_lex[param_lex.get_type()]);
	}
	else
		throw curr_lex;
}

void Parser::Celochislennaya(){
	if (c_type == LEX_NUM){
        TID[ind].put_value(sign*curr_lex.get_value());
        TID[ind].put_assign();
        gl();
    }
	else 
		throw curr_lex;
}

// void Parser::Znak(){}
void Parser::Strokovaya(){
	if (c_type == LEX_PHRASE){
        TID[ind].put_value(curr_lex.get_value());
        TID[ind].put_assign();
		gl();
    }
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
    int pl0, pl1, pl2, pl3;

	if (c_type == LEX_IF){
		gl();
		if (c_type == LEX_LPAREN)
			gl();
		else
			throw curr_lex;
		Viragenie();
        eq_int(); // проверка типов в операторах
        pl2 = prog.get_free(); // внутр предст
        prog.blank(); // внутр предст
        prog.put_lex(Lex(POLIZ_FGO)); // внутр предст
		if (c_type == LEX_RPAREN)
			gl();
		else
			throw curr_lex;
		Operator();
        pl3 = prog.get_free(); prog.blank(); // внутр предст
        prog.put_lex(Lex(POLIZ_GO)); // внутр предст
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()),pl2); // внутр предст
		if (c_type == LEX_ELSE){
			gl();
            Operator();
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()),pl3); // внутр предст
		}
        else
			throw curr_lex;
	}
	else if (c_type == LEX_WHILE){
        pl0=prog.get_free(); // внутр предст
		gl();
		if (c_type == LEX_LPAREN)
			gl();
		else
			throw curr_lex;
		Viragenie();
        eq_int(); // проверка типов в операторах
        pl1=prog.get_free(); prog.blank(); // внутр предст
        prog.put_lex(Lex(POLIZ_FGO)); // внутр предст
		if (c_type == LEX_RPAREN){
			gl();
            loop_flag=1;
            Operator();
            loop_flag=0;
            prog.put_lex(Lex(POLIZ_LABEL, pl0)); // внутр предст
            prog.put_lex(Lex(POLIZ_GO)); // внутр предст
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1); // внутр предст
            if (plb != -1){  // для break
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), plb);
                plb = -1;
            }
        }
		else
			throw curr_lex;

	}
	else if (c_type == LEX_READ){
		gl();
		if (c_type == LEX_LPAREN)
			gl();
		else
			throw curr_lex;
		if (c_type == LEX_ID){
            check_id_in_read(); // проверка типов в операторах
            prog.put_lex(Lex(POLIZ_ADDRESS, c_val)); // внутр предст
			gl();
        }
		else
			throw curr_lex;
		if (c_type == LEX_RPAREN){
			gl();
            prog.put_lex(Lex(LEX_READ)); // внутр предст
        }
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
        prog.put_lex(Lex(LEX_WRITE)); // внутр предст
		while (c_type == LEX_COMMA){
			gl();
			Viragenie();
            prog.put_lex(Lex(LEX_WRITE)); // внутр предст
		}
		if (c_type == LEX_RPAREN){
			gl();
        }
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
        prog.put_lex(Lex(POLIZ_ADDRESS, c_val)); // внутр предст
		gl();
		if (c_type == LEX_ASSIGN){
			gl();
			OperatorViragenie();
            eq_type(); // проверка типов в операторах
            prog.put_lex(Lex(LEX_ASSIGN)); // внутр предст
		}
		else
			throw curr_lex;
	}
    else if (c_type == LEX_BREAK){
        if (loop_flag == 1) {
            gl();
            plb=prog.get_free(); 
            prog.blank();
            prog.put_lex(Lex(POLIZ_GO));
            if (c_type == LEX_SEMICOLON)
                gl();
            else
                throw curr_lex;
        }
        else
            throw "Error: break statement non within loop!";
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
  while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND || c_type == LEX_PERCENT) {
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
    prog.put_lex(curr_lex); // внутр предст
    gl();
  }
  else if ( c_type == LEX_NUM ){
    st_lex.push(LEX_INT); // проверка типов в выражении
    prog.put_lex(curr_lex); // внутр предст
    gl();
  }
  else if (c_type == LEX_NOT){
    gl(); 
    F(); 
    check_not(); // проверка типов в выражении
  }
  else if (c_type == LEX_PHRASE){
    st_lex.push(LEX_STRING); // проверка типов в выражении
    prog.put_lex(curr_lex); // внутр предст
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
  prog.put_lex(Lex(op)); // внутр предст
}

void Parser::check_not () 
{
  if (st_lex.pop() != LEX_INT)
    throw "wrong type is in not";
  else 
  {
    st_lex.push (LEX_INT);
    prog.put_lex(Lex(LEX_NOT)); // внутр предст
  }
}

////////////////проверка типов в операторах////////////////////

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
       throw string("Error: Variable ") + string(TID[c_val].get_name()) + string(" is not declared in read operator!");
}
