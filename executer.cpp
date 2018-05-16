// #include "common.h"
#include "executer.h"

void ExecStack:: push (int i)
{
  if ( !is_full() )
    s [top++] = Lex(LEX_INT, i);
  else
    throw "Stack_is_full";
}

void ExecStack:: push_str (int i)
{
  if ( !is_full() )
    s [top++] = Lex(LEX_STRING, i);
  else
    throw "Stack_is_full";
}
 

int ExecStack:: pop ()
{
  if ( !is_empty() )
    return s[--top].get_value();
  else
    throw "stack_is_empty";
}

type_of_lex ExecStack:: get_type ()
{
  if ( !is_empty() )
    return s[top-1].get_type();
  else
    throw "stack_is_empty";
}

/////////////////////////////////////////////////////////

void Executer::execute ( Poliz & prog )
{
  ExecStack args;
  int i, j, index = 0, size = prog.get_free();
  while ( index < size )
  {
    pc_el = prog [ index ];
    switch ( pc_el.get_type () )
    {
      case LEX_NUM: case POLIZ_LABEL: // +
            args.push ( pc_el.get_value () );
            break;
      case POLIZ_ADDRESS:
            if (TID[pc_el.get_value()].get_type() == LEX_STRING){
                args.push_str( pc_el.get_value () );
                break;
            }
            else if (TID[pc_el.get_value()].get_type() == LEX_INT){
                args.push( pc_el.get_value () );
                break;
            }

      case LEX_PHRASE: // +
            args.push_str( pc_el.get_value () );
            break;
      case LEX_ID: // +
            i = pc_el.get_value ();
            if (TID[i].get_assign()){
                if (TID[i].get_type() == LEX_INT){
                    args.push ( TID[i].get_value () );
                    break;
                }
                else if (TID[i].get_type() == LEX_STRING){
                    args.push_str ( TID[i].get_value () );
                    break;
                }
            }
            else
              throw "(Poliz) Error: indefinite identifier!";
      case LEX_NOT: // +
            args.push( !args.pop() );
            break;
      case LEX_OR: // +
        i = args.pop();
        args.push ( args.pop() || i );
        break;
      case LEX_AND: // +
        i = args.pop();
        args.push ( args.pop() && i );
        break;
      case POLIZ_GO:
        index = args.pop() - 1;
        break;
      case POLIZ_FGO:
        i = args.pop();
        if ( !args.pop() ) index = i-1;
        break;
      case LEX_WRITE: // +
            if (args.get_type() == LEX_INT){
                cout << args.pop() << endl;
                break;}
            else if (args.get_type() == LEX_STRING){
                cout << TPHR[args.pop()] << endl;
                break;
            }
      case LEX_READ:
            if (args.get_type() == LEX_INT){
                i = args.pop ();
                int k;
                cout << "Input int value for " << TID[i].get_name () << endl;
                cin >> k;
                TID[i].put_value (k);
                TID[i].put_assign ();
                break;}
            else if (args.get_type() == LEX_STRING){
                i = args.pop();
                int k;
                string input_str;
                cout << "Input string value for " << TID[i].get_name () << endl;
                cin >> input_str;
                TPHR[TID[i].get_value()] = input_str;
                // TID[i].put_value (k);
                TID[i].put_assign ();
                break;
            }
        {
          int k;
          i = args.pop ();
          if ( TID[i].get_type () == LEX_INT )
          {
            cout << "Input int value for" << TID[i].get_name () << endl;
            cin >> k;
          }
          else
          {
            char j[20];
            rep:
            cout << "Input boolean value (true or false) for" << TID[i].get_name() << endl;
            cin >> j;
            if (!strcmp(j, "true"))
              k = 1;
            else
              if (!strcmp(j, "false"))
                k = 0;
              else
              {
                cout << "Error in input:true/false" << endl;
                goto rep;
              }
          }
          TID[i].put_value (k);
          TID[i].put_assign ();
          break;
        }
      case LEX_PLUS: // +
            if (args.get_type() == LEX_INT){
                args.push ( args.pop() + args.pop() );
                break;}
            else if (args.get_type() == LEX_STRING){ 
                args.push_str(TPHR.put( TPHR[args.pop()] +TPHR[args.pop()] ));
                break;
            }
      case LEX_TIMES: // +
        args.push ( args.pop() * args.pop() );
        break;
      case LEX_MINUS: // +
        i = args.pop();
        args.push ( args.pop() - i );
        break;
      case LEX_SLASH: // +
        i = args.pop();
        if (i)
        {
          args.push(args.pop() / i);
          break;
        }
        else
          throw "(Poliz) Error: divizion by zero!";
      case LEX_PERCENT: // +
        i = args.pop();
        if (i)
        {
          args.push(args.pop() % i);
          break;
        }
        else
          throw "(Poliz) Error: divizion by zero!";
      case LEX_EQ: // +
            if (args.get_type() == LEX_INT){
                args.push ( args.pop() == args.pop() );
                break;}
            else if (args.get_type() == LEX_STRING){  
                args.push(TPHR[args.pop()]  == TPHR[args.pop()]);
                break;
            }
      case LEX_LSS: // +
            if (args.get_type() == LEX_INT){
                i = args.pop();
                args.push ( args.pop() < i);
                break;}
            else if (args.get_type() == LEX_STRING){  
                string ts = TPHR[args.pop()];
                args.push(TPHR[args.pop()]  < ts);
                break;
            }
      case LEX_GTR: // +
            if (args.get_type() == LEX_INT){
                i = args.pop();
                args.push ( args.pop() > i);
                break;}
            else if (args.get_type() == LEX_STRING){  
                string ts = TPHR[args.pop()];
                args.push(TPHR[args.pop()]  > ts);
                break;
            }
      case LEX_LEQ: // +
        i = args.pop();
        args.push ( args.pop() <= i );
        break;
      case LEX_GEQ: // +
        i = args.pop();
        args.push ( args.pop() >= i );
        break;
      case LEX_NEQ: // +
        if (args.get_type() == LEX_INT){
                i = args.pop();
                args.push ( args.pop() != i);
                break;}
            else if (args.get_type() == LEX_STRING){  
                string ts = TPHR[args.pop()];
                args.push(TPHR[args.pop()]  != ts);
                break;
            }
      case LEX_ASSIGN: // +
            i = args.pop();
            j = args.pop();
            TID[j].put_value(i);
            TID[j].put_assign(); break;
      default:
        throw string("(Poliz) Error: unexpected elem ") + string(str_lex[pc_el.get_type ()]);
    }//end of switch
    ++index;
  };//end of while
  cout << "Finish of executing!!!" << endl;
}