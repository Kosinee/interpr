#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iterator>
#include <string.h>
#include <algorithm>
#include <cstdio>
#include <stack>

using namespace std;

enum type_of_lex
{
    LEX_NULL, /*0*/
    LEX_AND, LEX_BOOL, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_BREAK,
    LEX_GOTO, LEX_FOR, LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE,
    LEX_WHILE, LEX_WRITE, LEX_STRING, LEX_STRUCT, /*18*/
    LEX_FIN, /*19*/
    LEX_SEMICOLON, LEX_COMMA, LEX_LBRAC, LEX_RBRAC,LEX_LCURBRAC,/*24*/
    LEX_RCURBRAC, LEX_COLON, LEX_MOD, LEX_QUOT, LEX_DIV, LEX_EQ,/*30*/
    LEX_LESS, LEX_MORE, LEX_PLUS,LEX_MINUS, LEX_MULT, LEX_DEG, /*36*/
    LEX_LESSEQ, LEX_COMP, LEX_MOREEQ, LEX_NEQ,/*40*/
    LEX_NUM, /*41*/
    LEX_STR,/*42*/
    LEX_ID, /*43*/
    POLIZ_LABEL, /*44*/
    POLIZ_ADDRESS, /*45*/
    POLIZ_GO, /*46*/
    POLIZ_FGO,/*47*/
    POLIZ_STRUCT/*48*/
};

class Lex
{
    type_of_lex t_lex;//тип лексемы
    int v_lex;//номер в таблице либо TW, либо TD, либо TID или значение числа
    int int_lex;
    string str_lex;
    bool bool_lex;

public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0, int i = 0, string str = "", bool bl = false)
    {
        t_lex = t;
        v_lex = v;
        int_lex = i;
        str_lex = str;
        bool_lex = bl;
    }
    type_of_lex get_type()const
    {
        return t_lex;
    }
    int get_value()const
    {
        return v_lex;
    }
    int get_int()const
    {
        return int_lex;
    }
    string get_str()const
    {
        return str_lex;
    }
    bool get_bool()const
    {
        return bool_lex;
    }
    friend ostream& operator << (ostream &s, Lex l)
    {
        s << '(' << l.t_lex << ',' << l.v_lex <<')';
        return s;
    }
};

class Ident
{
    string name;//имя новой переменной
    bool declare;
    type_of_lex type;//тип новой переменной
    bool assign;
    bool mark;
    int mark_num;
    int int_val;//значение числа
    string str_val;//значение строки
    bool bool_val;//значение логической
public:
    vector<int> num_pol;
    Ident()
    {
        declare = false;
        assign = false;
        mark = false;
    }
    Ident(string n)
    {
        name = n;
        declare = false;
        assign = false;
        mark = false;
    }
    bool operator == (const string& s)const
    {
        return name == s;
    }
    void put_name(string s)
    {
        name = s;
    }
    string get_name()
    {
        return name;
    }
    bool get_declare()
    {
        return declare;
    }
    void put_declare()
    {
        declare = true;
    }
    type_of_lex get_type()
    {
        return type;
    }
    void put_type(type_of_lex t)
    {
        type = t;
    }
    bool get_assign()
    {
        return assign;
    }
    void put_assign()
    {
        assign = true;
    }
    bool get_mark()
    {
        return mark;
    }
    void put_mark()
    {
        mark = true;
    }
    void put_mark_num(int v)
    {
        mark_num = v;
    }
    int get_mark_num()
    {
        return mark_num;
    }
    int get_int_val()
    {
        return int_val;
    }
    void put_int_val(int v)
    {
        int_val = v;
    }
    void put_str_val(string v)
    {
        str_val = v;
    }
    string get_str_val()
    {
        return str_val;
    }
    void put_bool_val(bool v)
    {
        bool_val = v;
    }
    bool get_bool_val()
    {
        return bool_val;
    }
};

vector<Ident> TID;
int put(const string & buf)
{
    vector <Ident>::iterator k;
    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end())
    {
        return (k - TID.begin());
    }
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}

struct Struct
{
    string name;
    vector<Ident> TIDS;
};

vector<Struct> TSTRUCT;
void print_TSTRUCT()
{
    for (int i = 0; i < TSTRUCT.size(); i++)
    {
        cout << TSTRUCT[i].name << endl;
        for (int j = 0; j < TSTRUCT[i].TIDS.size(); j++)
        {
            cout<< '\t' << TSTRUCT[i].TIDS[j].get_name() << endl;;
        }
    }
}

int check_struct_name(const string & buf)
{
    for(int i = 0; i < TSTRUCT.size(); i++)
    {
        if(TSTRUCT[i].name == buf)
        {
            return i;
        }
    }
    return -1;
}

int check_TID(const string &buf)
{
    for (int i0 = 0; i0 < TID.size(); i0++)
    {
        if (buf == TID[i0].get_name())
            return i0;
    }
    throw "No such IDent\n";
    return -1;
}

class Scanner
{
    FILE* fp;
    char c;
    int look(const string buf, char** lst)
    {
        int i = 1;
        while(lst[i])
        {
            if(buf == lst[i])
            {
                return i;
            }
            i++;
        }
        return 0;
    }
    void gc()
    {
        c = fgetc(fp);
    }
public:
    static char* TW[],* TD[];
    Scanner(const char* program)
    {
        fp = fopen(program, "r");
        if (fp == NULL)
        {
            throw "can’t open file";
        }
    }
    Lex get_lex();
};

char* Scanner::TW[] =
{
    nullptr, "and", "bool", "else", "if", "false", "int", "break", "goto", "for",
    "not", "or", "program", "read", "true", "while", "write", "string", "struct"
};
char* Scanner::TD[] =
{
     "@", ";", ",", "(", ")", "{", "}", ":", "%", "\"", "/",
     "=", "<", ">", "+", "-", "*", "^", "<=", "==", ">=", "!="
};

Lex Scanner::get_lex()
{
    enum state{H,IDENT, NUMB, QUOT, ALE, NEQ, COMM, SLASH};
    state CS = H;
    string buf;
    int d, j;
    do
    {
        gc();
        switch(CS)
        {
        case H:
            if(c == ' ' || c == '\n' || c== '\r' || c == '\t')
            {
            }
            else if(isalpha(c))
            {
                buf.push_back(c);
                CS = IDENT;
            }
            else if(isdigit(c))
            {
                d = c - '0';
                CS = NUMB;
            }
            else if(c == '=' || c == '<' || c == '>')
            {
                buf.push_back(c);
                CS = ALE;
            }
            else if(c == '@')
            {
                return Lex(LEX_FIN, 0);
            }
            else if(c == '!')
            {
                buf.push_back(c);
                CS = NEQ;
            }
            else if(c == '"')
            {
                CS = QUOT;
            }
            else if(c == '/')
            {
                buf.push_back(c);
                CS = SLASH;
            }
            else
            {
                buf.push_back(c);
                if ((j = look(buf, TD)))
                {
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                else
                {
                    throw c;
                }
            }
            break;

        case IDENT:
            if(isalpha(c) || isdigit(c) || c == '.')
            {
                buf.push_back(c);
            }
            else
            {
                ungetc(c, fp);
                if(buf == "true")
                {
                    return Lex(LEX_TRUE, 0, 1, "", true);
                }
                else if(buf == "false")
                {
                    return Lex(LEX_FALSE, 0, 0, "", false);
                }
                else if (j = look(buf, TW))
                {
                    return Lex((type_of_lex)j, j);
                }
                else
                {
                     j = put(buf);
                     return Lex(LEX_ID, j);
                }
            }
            break;

        case NUMB:
            if (isdigit(c))
            {
                d = d * 10 + (c - '0');
            }
            else
            {
                ungetc(c,fp);
                return Lex(LEX_NUM, 0, d);
            }
            break;

        case QUOT:
            if (c != '"')
            {
                buf.push_back(c);
            }
            else
            {
                return Lex(LEX_STR, 0, 0, buf);
            }
            break;

        case ALE:
            if (c == '=')
            {
                buf.push_back(c);
                j = look(buf, TD);
                return Lex ((type_of_lex)(j+(int) LEX_FIN), j);
            }
            else
            {
                ungetc(c, fp);
                j = look (buf, TD);
                return Lex((type_of_lex) (j+(int) LEX_FIN),j);
            }
            break;

        case NEQ:
            if (c == '=')
            {
                buf.push_back(c);
                j = look(buf, TD);
                return Lex(LEX_NEQ, j);
            }
            else
            {
                throw '!';
            }
            break;

        case COMM:
            if(c == '*')
            {
                gc();
                if (c == '/')
                {
                    CS = H;
                }
                else
                {
                    ungetc(c, fp);
                }
            }
            else if(c == '@')
            {
                throw c;
            }
            break;

        case SLASH:
            if(c == '*')
            {
                buf.pop_back();
                CS = COMM;
            }
            else if(j = look("/", TD))
            {
                ungetc(c, fp);
                return Lex(LEX_DIV, j);
            }
            break;

        }
    }while (true);
}

class Parser
{
    Lex curr_lex;//текущая лексема
    type_of_lex c_type;//тип текущей лексемы
    int c_val;//номер в таблице лексем
    int c_int;
    string c_str;//строка введенная
    bool c_bool;
    Scanner scan;
    type_of_lex var_type;
    int num_tid;
    stack<type_of_lex> st_lex;
    vector<int> v_break;
    int cycle_flag;
    int minus_flag;
    int if_flag;
    int for_flag = 0;
    void P();void D();void G();void C();void OPS();
    void OP();void V();void OR(); void AND();
    void COMP();void PM();void MD();void NOT();void S();
    void dec(type_of_lex);
    void check_id();
    void check_op();
    void check_not();
    void eq_bool();
    void check_id_in_read();
    void check_in_cycle();
    void gl()
    {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
        c_int = curr_lex.get_int();
        c_str = curr_lex.get_str();
        c_bool = curr_lex.get_bool();
    }
public:
    vector<Lex> poliz;
    Parser(const char* program):scan(program){}
    void analyze();
    void srchmark();
};

template<class T, class T_EL>
void from_st(T& t, T_EL& x)
{
    x = t.top();
    t.pop();
}

void Parser::dec(type_of_lex type)
{
    if(TID[num_tid].get_declare())
    {
        throw "twice";
    }
    else
    {
        TID[num_tid].put_type(type);
        TID[num_tid].put_declare();
    }
}

void Parser::check_in_cycle()
{
    if(cycle_flag == 0)
    {
        throw curr_lex;
    }
}

void Parser::check_id_in_read()
{
    if (!TID [c_val].get_declare())
    {
        throw "not declared";
    }
}

void Parser::eq_bool()
{
    if (st_lex.top() != LEX_BOOL) {
        throw "expression is not boolean";
    }
    st_lex.pop();
}

void Parser::check_id()
{
    if(TID[c_val].get_declare())
    {
        st_lex.push(TID[c_val].get_type());
    }
    else
    {
        TID[c_val].put_mark_num(poliz.size());
        TID[c_val].put_assign();
        int u = TID[c_val].num_pol.size() - 1;
        while(0 < TID[c_val].num_pol.size())
        {
            poliz[TID[c_val].num_pol[u]] = Lex(POLIZ_LABEL, 0, TID[c_val].get_mark_num());
            TID[c_val].num_pol.pop_back();
            u--;
        }
        gl();
        if(c_type != LEX_COLON)
        {
            throw "not declarated";
        }
    }
}

void Parser::check_op()
{
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;
    t1 = st_lex.top();
    st_lex.pop();
    op = st_lex.top();
    st_lex.pop();
    t2 = st_lex.top();
    st_lex.pop();
    if (op == LEX_MINUS || op == LEX_MULT || op == LEX_DIV)
    {
        r = LEX_INT;
    }
    if (op == LEX_PLUS)
    {
        if (t1 == LEX_STRING || t1 == LEX_INT)
        {
            t = t1;
            r = t1;
        }
        else
        {
            throw "wrong operand";
        }
    }
    if (op == LEX_OR || op == LEX_AND)
    {
        r = t = LEX_BOOL;
    }
    if (op == LEX_EQ)
    {
        if (t1 != t2)
        {
            throw "wrong operand";
        }
        t = t1;
        r = t1;
    }
    if (op == LEX_COMP || op == LEX_NEQ
    || op == LEX_LESSEQ || op == LEX_MOREEQ
    || op == LEX_NEQ || op == LEX_MORE
    || op == LEX_LESS)
    {
        t = t1;
        r = LEX_BOOL;
    }
    if (t1 == t2  &&  t1 == t)
    {
        st_lex.push(r);
    }
    else
    {
        throw "wrong types are in operation";
    }
    poliz.push_back(Lex(op));
    cout<<"op ";
}

void Parser::check_not()
{
    if (st_lex.top() != LEX_BOOL)
    {
        throw "wrong type is in not";
    }
    else
    {
        cout<<"not";
        poliz.push_back(Lex(LEX_NOT));
    }
}

void Parser::srchmark()
{
    int k;
    gl();
    while(c_type != LEX_FIN)
    {
        if(c_type == LEX_ID)
        {
            k = c_val;
            gl();
            if(c_type == LEX_COLON)
            {
                if(TID[k].get_mark())
                {
                    throw "mark error";
                }
                else
                TID[k].put_mark();
            }
        }
        if(c_type != LEX_FIN)
        {
            gl();
        }
    }
}

void Parser::analyze()
{
    cout<<"POLIZ :"<<endl;
    gl();
    P();
    gl();
    if(c_type != LEX_FIN)
    {
        cout<<"NOOK";
        throw curr_lex;
    }
    else
    {
        cout<<endl<<"OK"<<endl;
    }

}

void Parser::P()//программа
{
    if (c_type == LEX_PROGRAM)
    {
        minus_flag = 0;
        cycle_flag = 0;
        gl();
        //cout<<"program";
        if (c_type == LEX_LCURBRAC)
        {
            //cout<<endl<<"{"<<endl;
            gl();
            while (c_type == LEX_STRUCT)
            {
                S();
                gl();
            }
            D();
            OPS();
            if (c_type != LEX_RCURBRAC)
            {
                throw curr_lex;
            }
            else
            {
                //cout<<endl<<"}"<<endl;
            }
        }
    }
}

void Parser::S()//описания структуры
{
    type_of_lex typefield;
    Ident idnt;
    Struct strct;
    gl();
    if (c_type != LEX_ID)
    {
        throw curr_lex;
    }
    if(check_struct_name(TID[c_val].get_name()) >= 0)
    {
        throw "struct error";
    }
    strct.name = TID[c_val].get_name();
    TID[c_val].put_type(LEX_STRUCT);
    gl();
    if (c_type != LEX_LCURBRAC)
    {
        throw curr_lex;
    }
    gl();
    while(c_type != LEX_RCURBRAC)
    {
        if(c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING)
        {
            typefield = c_type;
        }
        else
        {
            throw curr_lex;
        }
        gl();
        while (c_type != LEX_SEMICOLON)
        {
            if (c_type != LEX_ID)
            {
                throw curr_lex;
            }
            idnt.put_name(TID[c_val].get_name());
            idnt.put_type(typefield);
            strct.TIDS.push_back(idnt);
            gl();
            if (c_type == LEX_COMMA)
            {
                gl();
            }
            else if(c_type != LEX_SEMICOLON)
            {
                throw curr_lex;
            }
        }
        gl();
    }
    TSTRUCT.push_back(strct);
}

void Parser::D()//описания
{
    if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL)
    {
        //cout<<"type";
        var_type = c_type;
        G();
        while (c_type == LEX_COMMA)
        {
            //cout<<",";
            G();
        }
        if (c_type != LEX_SEMICOLON)
        {
            throw curr_lex;
        }
        //cout<<";"<<endl;
        gl();
        D();
    }
    else if(TID[c_val].get_type() == LEX_STRUCT)
    {
        string name;
        int k = check_struct_name(TID[c_val].get_name());// номер в таблице структур
        name = TID[c_val].get_name();
        do
        {
            gl();
            TID[c_val].put_type(LEX_STRUCT);
            TID[c_val].put_int_val(k);//номер в таблице структур
            TID[c_val].put_str_val(name);
            for(int i = 0; i < TSTRUCT[k].TIDS.size(); i++)
            {
                name = TID[c_val].get_name() + "." + TSTRUCT[k].TIDS[i].get_name();
                int q = put(name);
                TID[q].put_type(TSTRUCT[k].TIDS[i].get_type());
                TID[q].put_declare();
            }
            gl();
        }while(c_type == LEX_COMMA);
        if (c_type != LEX_SEMICOLON)
        {
            throw curr_lex;
        }
        gl();
        D();
    }
}

void Parser::G()//присваивание
{
    gl();
    if(c_type != LEX_ID)
    {
        throw curr_lex;
    }
    else
    {
        //cout<<"elem";
        num_tid = c_val;
        dec(var_type);
        gl();
        if(c_type == LEX_EQ)
        {
            //cout<<"=";
            poliz.push_back(Lex(POLIZ_ADDRESS, num_tid));
            cout<<"&"<<TID[num_tid].get_name()<<" ";
            C();
            poliz.push_back(Lex(LEX_EQ));
            cout<<"= ";
        }
    }
}

void Parser::C()//сопоставление
{
    gl();
    if (c_type == LEX_PLUS || c_type == LEX_MINUS)
    {
        //cout<<" znak ";
        int m = 2 * (c_type == LEX_MINUS);
        gl();
        if (c_type == LEX_NUM && var_type == LEX_INT)
        {
            cout<<"num ";
            TID[num_tid].put_assign();
            c_int = (1 - m) * c_int;
            poliz.push_back(Lex(c_type, c_val, c_int));
            gl();
        }
        else
        {
            throw curr_lex;
        }
    }
    else if (var_type == LEX_INT)
    {
        if (c_type == LEX_NUM)
        {
            cout<<"num ";
            TID[num_tid].put_assign();
            //TID[num_tid].put_int_val(c_int);
            poliz.push_back(Lex(c_type, c_val, c_int));
            gl();
        }
        else
        {
            throw "invalid assign";
        }
    }
    else if(var_type == LEX_STRING)
    {
        if (c_type == LEX_STR)
        {
            cout<<"str ";
            TID[num_tid].put_assign();
            //TID[num_tid].put_str_val(c_str);
            poliz.push_back(Lex(c_type, c_val, c_int, c_str));
            gl();
        }
        else
        {
            throw "invalid assign";
        }
    }
    else if(var_type == LEX_BOOL)
    {
        if (c_type == LEX_TRUE || c_type == LEX_FALSE)
        {
            cout<<"bool ";
            TID[num_tid].put_assign();
            //TID[num_tid].put_bool_val(c_bool);
            poliz.push_back(Lex(c_type, c_val, c_int, c_str, c_bool));
            gl();
        }
        else
        {
            throw "invalid assign";
        }
    }
    else
    {
        throw curr_lex;
    }
}

void Parser::OPS()//операторы
{
    if (c_type == LEX_IF || c_type == LEX_FOR || c_type == LEX_WHILE
    || c_type == LEX_BREAK || c_type == LEX_GOTO || c_type == LEX_READ
    || c_type == LEX_WRITE || c_type == LEX_LCURBRAC || c_type == LEX_ID
    || c_type == LEX_NUM || c_type == LEX_STR || c_type == LEX_NOT
    || c_type == LEX_LBRAC || c_type == LEX_FALSE || c_type == LEX_TRUE)
    {
        OP();
        OPS();
    }
}

void Parser::OP() //оператор
{
    int pl[8];
    if(c_type == LEX_IF)//если
    {
        //cout<<"if";
        gl();
        if (c_type == LEX_LBRAC)
        {
            //cout<<"(";
            gl();
            V();
            eq_bool();
            pl[0] = poliz.size();
            poliz.push_back(Lex());
            cout<<"mark ";
            poliz.push_back(Lex(POLIZ_FGO));
            cout<<"!F ";
            if (c_type == LEX_RBRAC)
            {
                //cout<<")";
                gl();
                if_flag++;
                OP();
                if_flag--;
                pl[1] = poliz.size();
                poliz.push_back(Lex());
                cout<<"mark ";
                poliz.push_back(Lex(POLIZ_GO));
                cout<<"! ";
                poliz[pl[0]] = Lex(POLIZ_LABEL, 0, poliz.size());
                if (c_type == LEX_ELSE)
                {
                    //cout<<"else";
                    gl();
                    if_flag++;
                    OP();
                    if_flag--;
                    poliz[pl[1]] = Lex(POLIZ_LABEL, 0, poliz.size());

                }
                else
                {
                    throw curr_lex;
                }
            }
            else
            {
                throw curr_lex;
            }
        }
        else
        {
            throw curr_lex;
        }
    }
    else if(c_type == LEX_FOR)
    {
        //cout<<"for";
        gl();
        if (c_type == LEX_LBRAC)
        {
            //cout<<"(";
            gl();
            for_flag++;
            V();
            if (c_type == LEX_SEMICOLON)
            {
                //cout<<";";
                pl[2] = poliz.size();
                gl();
                V();
                if (c_type == LEX_SEMICOLON)
                {
                    //cout<<";";
                    pl[3] = poliz.size();
                    poliz.push_back(Lex());
                    cout<<"mark ";
                    poliz.push_back(Lex(POLIZ_FGO));
                    cout<<"!F ";
                    pl[4] = poliz.size();
                    poliz.push_back(Lex());
                    cout<<"mark ";
                    poliz.push_back(Lex(POLIZ_GO));
                    cout<<"! ";
                    pl[5] = poliz.size();
                    gl();
                    for_flag++;
                    V();
                    pl[1] = poliz.size();
                    poliz.push_back(Lex(POLIZ_LABEL, 0, pl[2]));
                    cout<<"mark ";
                    poliz.push_back(Lex(POLIZ_GO));
                    cout<<"! ";
                    if (c_type == LEX_RBRAC)
                    {
                        //cout<<")";
                        gl();
                        poliz[pl[4]] = Lex(POLIZ_LABEL, 0, poliz.size());
                        cycle_flag++;
                        OP();
                        cycle_flag--;
                        poliz.push_back(Lex(POLIZ_LABEL, 0, pl[5]));
                        cout<<"mark ";
                        poliz.push_back(Lex(POLIZ_GO));
                        cout<<"! ";
                        poliz[pl[3]] = Lex(POLIZ_LABEL, 0, poliz.size());
                        while (!v_break.empty())
                        {
                            poliz[v_break[v_break.size() - 1]] = Lex(POLIZ_LABEL, 0, poliz.size());
                            v_break.pop_back();
                        }
                    }
                    else
                    {
                        throw curr_lex;
                    }
                }
                else
                {
                    throw curr_lex;
                }
            }
            else
            {
                throw curr_lex;
            }
        }
        else
        {
            throw curr_lex;
        }
    }
    else if(c_type == LEX_WHILE)
    {
        //cout<<"while";
        gl();
        if (c_type == LEX_LBRAC)
        {
            //cout<<"(";
            pl[6] = poliz.size();
            gl();
            V();
            pl[7] = poliz.size();
            poliz.push_back(Lex());
            cout<<"mark ";
            poliz.push_back(Lex(POLIZ_FGO));
            cout<<"!F ";
            eq_bool();
            if (c_type == LEX_RBRAC)
            {
                //cout<<")";
                gl();
                cycle_flag++;
                OP();
                cycle_flag--;
                poliz.push_back(Lex(POLIZ_LABEL, 0, pl[6]));
                cout<<"mark ";
                poliz.push_back(Lex(POLIZ_GO));
                cout<<"! ";
                poliz[pl[7]] = Lex(POLIZ_LABEL, 0, poliz.size());
                while (!v_break.empty())
                {
                    poliz[v_break[v_break.size() - 1]] = Lex(POLIZ_LABEL, 0, poliz.size());
                    v_break.pop_back();
                }
            }
            else
            {
                throw curr_lex;
            }
        }
        else
        {
            throw curr_lex;
        }
    }
    else if(c_type == LEX_BREAK)
    {
        //cout<<"break";
        gl();
        if(c_type != LEX_SEMICOLON)
        {
            throw curr_lex;
        }
        //cout<<";"<<endl;
        gl();
        check_in_cycle();
        v_break.push_back(poliz.size());
        poliz.push_back(Lex());
        cout<<"mark ";
        poliz.push_back(Lex(POLIZ_GO));
        cout<<"! ";
    }
    else if(c_type == LEX_GOTO)
    {
        //cout<<"goto ";
        gl();
        if (c_type == LEX_ID && TID[c_val].get_mark())
        {
            //cout<<"elem";
            poliz.push_back(Lex(POLIZ_LABEL, 0, TID[c_val].get_mark_num()));
            if(!TID[c_val].get_assign())
            {
                TID[c_val].num_pol.push_back(poliz.size() - 1);
            }
            cout<<"mark ";
            poliz.push_back(Lex(POLIZ_GO));
            cout<<"! ";
            gl();
            if (c_type != LEX_SEMICOLON)
            {
                throw curr_lex;
            }
            //cout<<";"<<endl;
            gl();
        }
        else
        {
            throw "err metka";
        }
    }
    else if(c_type == LEX_READ)
    {
        //cout<<"read";
        gl();
        if (c_type == LEX_LBRAC)
        {
            //cout<<"(";
            gl();
            if(c_type == LEX_ID)
            {
                //cout<<"elem";
                check_id_in_read();
                cout << TID[c_val].get_name()<<" ";
                poliz.push_back(Lex(POLIZ_ADDRESS, c_val, 0, c_str));
                gl();
                if (c_type == LEX_RBRAC)
                {
                    //cout<<")";
                    gl();
                    poliz.push_back(Lex(LEX_READ));
                    cout<<"read ";
                    if (c_type != LEX_SEMICOLON)
                    {
                        throw curr_lex;
                    }
                    gl();
                    //cout<<";"<<endl;
                }
                else
                {
                    throw curr_lex;
                }
            }
            else
            {
                throw curr_lex;
            }
        }
        else
        {
            throw curr_lex;
        }
    }
    else if(c_type == LEX_WRITE)
    {
        //cout<<"write";
        int w_args = 0;
        gl();
        if (c_type == LEX_LBRAC)
        {
            //cout<<"(";
            gl();
            V();
            w_args++;
            while (c_type == LEX_COMMA)
            {
                //cout<<",";
                gl();
                V();
                w_args++;
            }
            if (c_type == LEX_RBRAC)
            {
                //cout<<")";
                gl();
                poliz.push_back(Lex(LEX_WRITE, 0, w_args));
                cout<<"write ";
                if (c_type == LEX_SEMICOLON)
                {
                    //cout<<";"<<endl;
                    gl();
                }
                else
                {
                    throw curr_lex;
                }
            }
            else
            {
                throw curr_lex;
            }
        }
        else
        {
            throw curr_lex;
        }
    }
    else if(c_type == LEX_LCURBRAC)//фигурные скобки
    {
        //cout<<endl<<"{"<<endl;
        gl();
        OPS();
        if (c_type == LEX_RCURBRAC)
        {
            //cout<<endl<<"}"<<endl;
            gl();
        }
        else
        {
            throw curr_lex;
        }
    }
    else if(c_type == LEX_NUM || c_type ==  LEX_STR || c_type == LEX_NOT || c_type == LEX_LBRAC || c_type == LEX_FALSE || c_type == LEX_TRUE)
    {
        V();
        if (c_type == LEX_SEMICOLON)
        {
            //cout<<";";
            gl();
        }
        else
        {
            throw curr_lex;
        }
    }
    else if(c_type == LEX_ID)
    {
        if(TID[c_val].get_type() == LEX_STRUCT)
        {
            int type = TID[c_val].get_int_val();
            poliz.push_back(Lex(POLIZ_STRUCT, c_val, c_int));
            cout<<TID[c_val].get_name();
            gl();
            if(c_type == LEX_EQ)
            {
                gl();
                if(TID[c_val].get_type() == LEX_STRUCT)
                {
                    if(type != TID[c_val].get_int_val())
                    {
                        throw "Error struct";
                    }
                    poliz.push_back(Lex(POLIZ_STRUCT, c_val, c_int));
                    cout<<TID[c_val].get_name();
                    poliz.push_back(Lex(LEX_EQ, c_val));
                    cout<<"= ";
                    gl();
                    if(c_type != LEX_SEMICOLON)
                    {
                        throw "Error struct";
                    }
                    gl();
                }
                else
                {
                    throw "Erro struct";
                }
            }
            else
            {
                throw "Erro struct";
            }
        }
        else
        {
            check_id();
            if(c_type != LEX_COLON)
            {
                cout <<"&"<<TID[c_val].get_name()<<" ";
                poliz.push_back(Lex(POLIZ_ADDRESS, c_val, c_int, c_str, c_bool));
                gl();
            }
            if(c_type == LEX_EQ || c_type == LEX_OR || c_type == LEX_AND
            || c_type == LEX_COMP || c_type == LEX_NEQ ||c_type == LEX_LESSEQ
            || c_type == LEX_LESS || c_type == LEX_PLUS|| c_type == LEX_MINUS
            || c_type == LEX_MULT || c_type == LEX_DIV)
            {
                V();
                if (c_type == LEX_SEMICOLON)
                {
                    //cout<<";"<<endl;
                    gl();
                }
                else
                {
                    throw curr_lex;
                }
            }
            else if(c_type == LEX_COLON)
            {
                //cout<<":"<<endl;
                gl();
            }
            else
            {
                throw curr_lex;
            }
        }
    }
    else
    {
        throw curr_lex;
    }
}

void Parser::V()//выражение
{
    OR();
    while (c_type == LEX_EQ)
    {
        //cout<<" = ";
        st_lex.push(c_type);
        gl();
        OR();
        check_op();
    }
}

void Parser::OR()//разбор выражения или
{
    AND();
    while (c_type == LEX_OR)
    {
        //cout<<" | ";
        st_lex.push(c_type);
        gl();
        AND();
        check_op();
    }
}

void Parser::AND()//разбор выражения и
{
    COMP();
    while (c_type == LEX_AND)
    {
        //cout<<" and ";
        st_lex.push(c_type);
        gl();
        COMP();
        check_op();
    }
}

void Parser::COMP() //разбор выражения сравнение
{
    PM();
    while (c_type == LEX_COMP || c_type == LEX_NEQ
    || c_type == LEX_LESSEQ || c_type == LEX_MOREEQ
    || c_type == LEX_NEQ
    || c_type == LEX_MORE || c_type == LEX_LESS)
    {
        st_lex.push(c_type);
        gl();
        //cout << " ? ";
        PM();
        check_op();
    }
}

void Parser::PM() //разбор выражения плюс минус
{
    MD();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS)
    {
        st_lex.push(c_type);
        //cout << " znak ";
        gl();
        MD();
        check_op();
    }
}
void Parser::MD() //разбор выражения умнождел
{
    NOT();
    while (c_type == LEX_MULT || c_type == LEX_DIV) {
        st_lex.push(c_type);
        gl();
        //cout <<  " sznak ";
        NOT();
        check_op();
    }
}
void Parser::NOT() //разбор выражения не
{
    if (c_type == LEX_NOT)
    {
        //cout << " not ";
        gl();
        NOT();
        check_not();
    }
    else if (c_type == LEX_MINUS)
    {
        //cout << " - ";
        gl();
        minus_flag = 1;
        NOT();
    }
    else if (c_type == LEX_LBRAC)
    {
        //cout << "(";
        gl();
        V();
        if (c_type == LEX_RBRAC)
        {
            //cout<<")";
            gl();
        }
        else
        {
            throw curr_lex;
        }
    }
    else if (c_type == LEX_ID)
    {
        check_id();
        if(!TID[c_val].get_mark())
        {
            if(for_flag == 0)
            {
                cout << TID[c_val].get_name()<<" ";
                if(minus_flag)
                {
                    poliz.push_back(Lex(LEX_NUM, 0, 0));
                }
                poliz.push_back(Lex(c_type, c_val, c_int, c_str, c_bool));
                if(minus_flag)
                {
                    poliz.push_back(Lex(LEX_MINUS));
                    minus_flag = 0;
                }
            }
            else
            {
                for_flag = 0;
                cout <<"&"<< TID[c_val].get_name()<<" ";
                if(minus_flag)
                {
                    poliz.push_back(Lex(LEX_NUM, 0, 0));
                }
                poliz.push_back(Lex(POLIZ_ADDRESS, c_val, c_int, c_str, c_bool));
                if(minus_flag)
                {
                    poliz.push_back(Lex(LEX_MINUS));
                    minus_flag = 0;
                }
            }
        }
        gl();
    }
    else if (c_type == LEX_STR)
    {
        cout << "str ";
        st_lex.push(LEX_STRING);
        poliz.push_back(Lex(c_type, 0, 0, c_str));
        gl();
    }
    else if (c_type == LEX_NUM)
    {
        cout << "num ";
        st_lex.push(LEX_INT);
        if(minus_flag)
        {
            c_int = 0 - c_int;
            minus_flag = 0;
        }
        poliz.push_back(Lex(c_type, 0, c_int));
        gl();
    }
    else if (c_type == LEX_FALSE || c_type == LEX_TRUE)
    {
        cout << "bool ";
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(c_type, 0, 0, "", c_bool));
        gl();
    }
}

class Executer
{
public:
    void execute(vector<Lex> & poliz);
};

void Executer::execute (vector<Lex>& poliz)
{
    Lex pc_el;
    stack <Lex> args;
    stack <Lex> write_st;
    Lex l1, l2;
    int i, index = 0, size = poliz.size();
    bool log;
    while (index < size)
    {
        pc_el = poliz[index];
        switch (pc_el.get_type())
        {
            case LEX_TRUE: case LEX_FALSE:
                //cout<<"log ";
                args.push(Lex(LEX_BOOL, pc_el.get_value(), pc_el.get_int(), pc_el.get_str(), pc_el.get_bool()));
                break;

            case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
                //cout<<"num ";
                args.push(Lex(LEX_INT, pc_el.get_value(), pc_el.get_int()));
                //cout<<pc_el.get_int();
                break;

            case LEX_STR:
               // cout<<"str ";
                args.push(Lex(LEX_STRING, pc_el.get_value(), pc_el.get_int(), pc_el.get_str()));
                break;

            case LEX_ID:
                //cout<<"elem ";
                i = pc_el.get_value();
                if (TID[i].get_assign())
                {
                    if (TID[i].get_type() == LEX_INT)
                    {
                        args.push(Lex(LEX_INT, 0, TID[i].get_int_val()));
                    }
                    else if (TID[i].get_type() == LEX_STRING)
                    {
                        args.push(Lex(LEX_STRING, 0, 0, TID[i].get_str_val()));
                    }
                    else if (TID[i].get_type() == LEX_BOOL)
                    {
                        args.push(Lex(LEX_BOOL, 0, 0, "", TID[i].get_bool_val()));
                    }
                    else
                    {
                        throw "wrong type";
                    }
                }
                else
                {
                    throw "not assign";
                }
                break;

            case POLIZ_STRUCT:{
                //cout<<"elem ";
                string name1, name2, name;
                int m1, m2;
                int k = TID[pc_el.get_value()].get_int_val();// номер в таблице структур
                name1 = TID[pc_el.get_value()].get_name();
                index++;
                name2 = TID[poliz[index].get_value()].get_name();
                for(int j = 0; j < TSTRUCT[k].TIDS.size(); j++)
                {
                    name =  TSTRUCT[k].TIDS[j].get_name();
                    m1 = check_TID(name1 + "." + name);
                    m2 = check_TID(name2 + "." + name);
                    TID[m1].put_int_val(TID[m2].get_int_val());
                    TID[m1].put_str_val(TID[m2].get_str_val());
                    TID[m1].put_bool_val(TID[m2].get_bool_val());
                    TID[m1].put_assign();
                }
                index++;
                break;
            }

            case LEX_NOT :
                from_st(args, l1);
                args.push(Lex(LEX_BOOL, 0, 0, "", !l1.get_bool()));
                break;

            case LEX_OR:
                from_st(args, l1);
                from_st(args, l2);
                log = (l1.get_bool() || l2.get_bool());
                args.push(Lex(LEX_BOOL, 0, 0, "", log));
                break;

            case LEX_AND:
                from_st(args, l1);
                from_st(args, l2);
                log = (l1.get_bool() && l2.get_bool());
                args.push(Lex(LEX_BOOL, 0, 0, "", log));
                break;

            case POLIZ_GO:
                from_st(args, l1);
                index = l1.get_int() - 1;
                break;

            case POLIZ_FGO:
                from_st(args, l1);
                from_st(args, l2);
                if (!l2.get_bool())
                {
                    index = l1.get_int() - 1;
                }
                break;

            case LEX_WRITE:
                i = pc_el.get_int();
                while (i > 0){
                    from_st(args, l1);
                    write_st.push(l1);
                    i--;
                }
                i = pc_el.get_int();
                while (i > 0){
                    from_st(write_st, l1);
                    if(l1.get_type() == LEX_INT)
                    {
                        cout << l1.get_int();
                    }
                    else if(l1.get_type() == LEX_STRING)
                    {
                        cout << l1.get_str();
                    }
                    else
                    {
                        cout << l1.get_bool();
                    }
                    i--;
                }
                cout << endl;
                break;

            case LEX_READ:
                int k;
                from_st(args, l1);
                if (TID[l1.get_value()].get_type() == LEX_INT)
                {
                    cout << "Input int " << TID[l1.get_value()].get_name() << endl;
                    cin >> k;
                    TID[l1.get_value()].put_int_val(k);
                }
                else if (TID[l1.get_value()].get_type() == LEX_BOOL)
                {
                    string j;
                    while(1)
                    {
                        cout << "Input boolean true | false " << TID[l1.get_value()].get_name() << endl;
                        cin >> j;
                        if (j != "true" && j != "false")
                        {
                            cout << "Error in input : true | false" << endl;
                            continue;
                        }
                        k = (j == "true");
                        break;
                    }
                    TID[l1.get_value()].put_bool_val(k);
                }
                else if (TID[l1.get_value()].get_type() == LEX_STRING)
                {
                    string s;
                    cout << "Input string " << TID[l1.get_value()].get_name() << endl;
                    cin >> s;
                    TID[l1.get_value()].put_str_val(s);
                }
                TID[l1.get_value()].put_assign();
                break;

            case LEX_PLUS:
                //cout<<"+ ";
                from_st(args, l1);
                from_st(args, l2);
                if (l1.get_type() == LEX_INT){
                    int k = l2.get_int() + l1.get_int();
                    args.push(Lex(LEX_INT, 0, k));
                }
                else
                {
                    string s = l2.get_str() + l1.get_str();
                    args.push(Lex(LEX_STRING, 0, 0,s));
                }
                break;

            case LEX_MULT:
                //cout<<"* ";
                from_st(args, l1);
                from_st(args, l2);
                i = l2.get_int() * l1.get_int();
                args.push(Lex(LEX_INT, 0, i));
                break;

            case LEX_MINUS:
                //cout<<"- ";
                from_st(args, l1);
                from_st(args, l2);
                i = l2.get_int() - l1.get_int();
                args.push(Lex(LEX_INT, 0, i));
                break;

            case LEX_DIV:
               // cout<<"/ ";
                from_st(args, l1);
                from_st(args, l2);
                i = l2.get_int() / l1.get_int();
                args.push(Lex(LEX_INT, 0, i));
                break;

            case LEX_COMP:
                //cout<<"== ";
                from_st(args, l1);
                from_st(args, l2);
                if (l1.get_type() == LEX_INT)
                {
                    log = (l1.get_int() == l2.get_int());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else if (l1.get_type() == LEX_STR)
                {
                    log = (l1.get_str() == l2.get_str());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else
                {
                    log = (l1.get_bool() == l2.get_bool());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                break;

            case LEX_LESS:
                //cout<<"< ";
                from_st(args, l1);
                from_st(args, l2);
                if (l1.get_type() == LEX_INT)
                {
                    log = (l1.get_int() > l2.get_int());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else if (l1.get_type() == LEX_STR)
                {
                    log = (l1.get_str() > l2.get_str());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else
                {
                    log = (l1.get_bool() > l2.get_bool());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                break;

            case LEX_MORE:
                //cout<<"> ";
                from_st(args, l1);
                from_st(args, l2);
                if (l1.get_type() == LEX_INT)
                {
                    log = (l1.get_int() < l2.get_int());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else if (l1.get_type() == LEX_STR)
                {
                    log = (l1.get_str() < l2.get_str());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else
                {
                    log = (l1.get_bool() < l2.get_bool());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                break;

            case LEX_LESSEQ:
                //cout<<"<= ";
                from_st(args, l1);
                from_st(args, l2);
                if (l1.get_type() == LEX_INT)
                {
                    log = (l1.get_int() >= l2.get_int());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else if (l1.get_type() == LEX_STR)
                {
                    log = (l1.get_str() >= l2.get_str());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else
                {
                    log = (l1.get_bool() >= l2.get_bool());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                break;

            case LEX_MOREEQ:
               // cout<<">= ";
                from_st(args, l1);
                from_st(args, l2);
                if (l1.get_type() == LEX_INT)
                {
                    log = (l1.get_int() <= l2.get_int());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else if (l1.get_type() == LEX_STR)
                {
                    log = (l1.get_str() <= l2.get_str());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else
                {
                    log = (l1.get_bool() <= l2.get_bool());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                break;

            case LEX_NEQ:
                //cout<<"!= ";
                from_st(args, l1);
                from_st(args, l2);
                if (l1.get_type() == LEX_INT)
                {
                    log = (l1.get_int() != l2.get_int());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else if (l1.get_type() == LEX_STR)
                {
                    log = (l1.get_str() != l2.get_str());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                else
                {
                    log = (l1.get_bool() != l2.get_bool());
                    args.push(Lex(LEX_BOOL, 0, 0, "", log));
                }
                break;

            case LEX_EQ:
                i = 0;
                do {
                    i++;
                    index++;//cout<<"= ";
                } while (poliz[index].get_type() == LEX_EQ);
                index--;

                while (i > 0)
                {
                    from_st(args, l1);
                    from_st(args, l2);
                    if (l1.get_type() == LEX_INT)
                    {
                        //cout<<" "<<l2.get_value()<<" "<<l1.get_int()<<" ";
                        TID[l2.get_value()].put_int_val(l1.get_int());
                        TID[l2.get_value()].put_assign();
                        args.push(Lex(LEX_INT, l1.get_value(), l1.get_int()));
                        //cout<<args.top().get_int();
                    }
                    else if (l1.get_type() == LEX_BOOL)
                    {
                        //cout<<" "<<l2.get_value()<<" "<<l1.get_int()<<" ";
                        TID[l2.get_value()].put_bool_val(l1.get_bool());
                        TID[l2.get_value()].put_assign();
                        args.push(Lex(LEX_BOOL, l1.get_value(), l1.get_bool()));
                        //cout<<args.top().get_int();
                    }
                    else
                    {
                        TID[l2.get_value()].put_str_val(l1.get_str());
                        TID[l2.get_value()].put_assign();
                        args.push(Lex(LEX_STRING, 0, 0, l1.get_str()));
                    }
                    i--;
                }
                break;
            default:
                throw "Poliz error";
        }
        index++;
    }
    cout << endl << "Right completion!" << endl;
}

class Interpretator {
    Parser   pars, markp;
    Executer E;
public:
    Interpretator( const char* program ): pars (program), markp(program) {}
    void     interpretation ();
};

void Interpretator::interpretation()
{
    markp.srchmark();
    pars.analyze();
    int siz = TID.size(), i = 0;
    /*while(i < siz)
    {
        cout<<TID[i].get_name()<<" = ";
        if(TID[i].get_type() == LEX_INT)
        {
            cout<<TID[i].get_int_val();
        }
        else if(TID[i].get_type() == LEX_BOOL)
        {
            cout<<TID[i].get_bool_val();
        }
        else if(TID[i].get_type() == LEX_STRING)
        {
            cout<<TID[i].get_str_val();
        }
        cout<<" "<<TID[i].get_declare()<<" "<<TID[i].get_assign()<<" "<<TID[i].get_type()<<endl;
        i++;
    }
    cout<<endl;
    vector <Lex>::size_type iter = 0;
    int u = 0;
    while (iter < pars.poliz.size()) {
        cout << u << ": ";
        cout << pars.poliz[iter];
        iter++; u++;
    }
    cout<<"fin"<<endl;*/
    E.execute(pars.poliz);
    /*siz = TID.size(), i = 0;
    cout<<endl;
    while(i < siz)
    {
        cout<<TID[i].get_name()<<" = ";
        if(TID[i].get_type() == LEX_INT)
        {
            cout<<TID[i].get_int_val();
        }
        else if(TID[i].get_type() == LEX_BOOL)
        {
            cout<<TID[i].get_bool_val();
        }
        else if(TID[i].get_type() == LEX_STRING)
        {
            cout<<TID[i].get_str_val();
        }
        cout<<" "<<TID[i].get_declare()<<" "<<TID[i].get_assign()<<endl;
        i++;
    }*/
}

int main()
{ try
    {
        Interpretator I("test.txt");
        I.interpretation();
        return 0;
    }
    catch(const char* c)
    {
        cout<<endl<<c<<endl;
    }
    catch(Lex l)
    {
        cout<<endl<<"invalid lexem "<< l;
    }
    catch(char c)
    {
        cout<<endl<<"err with symbol: "<<c<<endl;
    }
    catch(...)
    {
        cout<<endl<<"undefined err"<<endl;
    }
    return 0;
}
