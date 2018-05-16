#include <iostream>
#include <cstring>

using namespace std;

#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "executer.h"

Tabl_ident TID(100);
Tabl_phrase TPHR(100);

class Interpretator
{
  Parser   pars;
  Executer E;
public:
           Interpretator  (const char* program): pars (program) {}
  void     interpretation ();
};
 
void Interpretator::interpretation ()
{
  pars.analyze ();
  cout << endl;
  pars.prog.print();
  cout << endl;
  E.execute ( pars.prog );
}

int main() {
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

 // // Проверка работы лексич+семантич анализа и генерации полиза
	// Parser pars1("input.txt");
	// try {
	// 	pars1.analyze();
 //        pars1.prog.print();
	// }



  try
  {
    Interpretator I ("input.txt");
    I.interpretation ();
  }

  catch (char c){
    cout << "unexpected symbol " << c << endl;
    return 1;
  }
	catch (char const* str){
    	cout << str << endl;
        return 1;
    }
    catch (Lex l)
  {
    cout << "unexpected lexeme";
    cout << l << " <=> " << str_lex[l.get_type()] << endl;
    return 1;
  }
    catch (string s){
        cout << s << endl;
        return 1;
    }


// // Вывод всех идентификаторов из таблицы TID, с типом и флагом описания
// int i=1;
//  while (TID[i].get_name()){
//     cout << TID[i].get_name() << "  " << str_lex[TID[i].get_type()]<< " -> "<< TID[i].get_declare()<< "  " << TID[i].get_value() << endl;
//     i++;
// }

// // Вывод всех фраз из таблицы TPHR
// int j=1;
//  while (j < TPHR.get_top()){
//     cout << j<<"-\""<< TPHR[j] << "\""<< endl;
//     j++;
// }

}