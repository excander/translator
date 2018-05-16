// #include "common.h"

class ExecStack
{
         Lex            s [1000];
         int          top;
public:
                      ExecStack () { top = 0; }
         void         reset () { top = 0; }
         void         push ( int i );
         void         push_str ( int i );
         int          pop ();
         type_of_lex  get_type();  
         bool         is_empty () { return top == 0; }
         bool         is_full  () { return top == 1000; }
};

class Executer
{
         Lex          pc_el;
public:
         void         execute ( Poliz & prog );
};

