// #include "common.h"
#include "lexer.h"

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
            		j = TPHR.put(string(buf));
            		return Lex(LEX_PHRASE, j);
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
                    c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '%'){
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