#ifndef LEXER_H_
#define LEXER_H_

//单词种别
enum symbol
{
    nul,ident,number,plus,minus,times,slash,oddsym,eql,neq,lss,leq,gtr,geq,
    lparen,rparen,comma,semicolon,period,becomes,beginsym,endsym,ifsym,thensym, whilesym,writesym,readsym,
    dosym,callsym,constsym,varsym,procsym
};
// nul表示识别不了的单词
// becomes 赋值
// comma 逗号 semicolon分号 period 句号


enum error_code
{
    NO_P,
    ID_LEN_TOO_MAX,
    SHOULD_EQUAL,
    CANNOT_IDENTIFY,
    SHOULD_BE_NUM,

};
const int ID_MAXLEN = 10; //标识符的最大长度

extern enum symbol sym;        //单词种别
extern char id[ID_MAXLEN + 1]; //传递标识符单词自身的值 即名字
extern int num;     

extern char ch;


#endif 