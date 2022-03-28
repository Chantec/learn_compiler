/*
 * @Author: liangzichao
 * @Date: 2022-03-22 14:36:09
 * @Last Modified by: liangzichao
 * @Last Modified time: 2022-03-22 14:42:57
 *
 * 这是最直接的版本，没有用到DFA
 * todo:error处理 标识符长度限制
 */
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>

//单词种别
enum symbol
{
    nul,
    ident,
    number,
    plus,
    minus,
    times,
    slash,
    oddsym,
    eql,
    neq,
    lss,
    leq,
    gtr,
    geq,
    lparen,
    rparen,
    comma,
    semicolon,
    period,
    becomes,
    beginsym,
    endsym,
    ifsym,
    thensym,
    whilesym,
    writesym,
    readsym,
    dosym,
    callsym,
    constsym,
    varsym,
    procsym,
    comment
};
// nul表示识别不了
// becomes 赋值
// comma 逗号 semicolon分号 period 句号

enum error_code
{
    ID_LEN_TOO_MAX,
    SHOULD_EQUAL,
    CANNOT_IDENTIFY,
    SHOULD_BE_NUM
};

const int ID_MAXLEN = 10; //标识符的最大长度

// func
int lexer(FILE *fp);
//函数返回值
enum symbol sym;        //单词种别
char id[ID_MAXLEN + 1]; //传递标识符单词自身的值 即名字
int num;                //传递无符号整数单词的值

const int WORD_CNT = 13;
const int SINGAL_CH_CNT = 11; //+ - * /  = # ( ) , . ;

char *WORD[WORD_CNT] = {"const", "var", "procedure", "begin", "end", "odd", "if", "then", "call", "while", "do", "read", "write"};
int WSYM[WORD_CNT] = {constsym, varsym, procsym, beginsym, endsym, oddsym, ifsym, thensym, callsym, whilesym, dosym, readsym, writesym};

char SINGAL_CH[SINGAL_CH_CNT] = {'+', '-', '*', '/', '=', '#', '(', ')', ',', '.', ';'};
int CSYM[SINGAL_CH_CNT] = {plus, minus, times, slash, eql, neq, lparen, rparen, comma, period, semicolon};
// word symbol 单词（关键字）对应的种别

void error(int i);

//用来管理 每一行 因为要产生报错信息
// 以下变量便于报错时知道是哪一行哪一列出错的
// 在getch()中用的到
int cc = 0;    // character count (当前已读的字符个数)
int ll = 0;    // line length
int lc = 1;    // line count
char line[80]; //输入缓冲

char ch;

FILE *fp;

void getch()
{
    if (cc == ll)
    {
        ll = cc = 0;
        if (feof(fp))
        {
            ch=EOF;
            exit(0);
        }
        while (!feof(fp) && (ch = getc(fp)) != '\n')
            line[ll++] = ch;
        lc++;
        line[ll++] = ' ';
    }
    ch = line[cc++];
}

int getSym() //每次得到一个符号 二元组
{
    if(ch==EOF)
        return 0;
    //忽略空格 回车 换行 tab
    while (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t')
        getch();
    //这会不会是EOF...

    if (isalpha(ch)) //标识符或者关键字
    {
        //后面可能是数字或者字母
        int i = 0;
        while (isalnum(ch))
        {
            // liangtodo 没有限制大小 error
            id[i++] = ch;
            getch();

            if (i > ID_MAXLEN)
            {
                error(ID_LEN_TOO_MAX);
            }
        }
        id[i] = '\0';
        bool is_reserved = 0;
        //判断是关键字还是标识符
        for (int i = 0; i < WORD_CNT; ++i)
        {
            if (strcmp(id, WORD[i]) == 0)
            {
                sym = (enum symbol)WSYM[i];
                is_reserved = 1;
                break;
            }
        }
        if (!is_reserved)
        {
            sym = ident;
        }
        //返回
        printf("(%2d,'%s')\n", sym, id);

        //现在的ch是下一个单词的开头了
    }
    else if (isdigit(ch)) //如果是数字
    {
        sym = number;
        num = 0;
        while (isdigit(ch))
        {
            num = num * 10 + ch - '0';
            getch();
        }
        if(isalpha(ch))
        {
            error(SHOULD_BE_NUM);
        }
        printf("(%2d,%d)\n", sym, num);
    }
    else if (ch == ':') //:=
    {
        getch();
        if (ch == '=')
        {
            sym = becomes;
            printf("(%2d,:=)\n", sym);
            getch();
        }
        else
        {
            sym = nul;
            error(SHOULD_EQUAL);
        }
    }
    else if (ch == '>') //> >=
    {
        getch();
        if (ch == '=')
        {
            sym = geq;
            printf("(%2d,>=)\n", sym);
            getch();
        }
        else
        {
            sym = gtr;
            printf("%2d,>)\n", sym);
        }
    }
    else if (ch == '<') //< <=
    {
        getch();
        if (ch == '=')
        {
            sym = leq;
            printf("(%2d,<=)\n", sym);
            getch();
        }
        else
        {
            sym = lss;
            printf("(%2d,<)\n", sym);
        }
    }
    else if (ch == '{') //注释
    {
        sym = comment;
        getch();
        while (ch != '}')
            getch();
        // ch现在就是}
        getch();
        printf("(%d,{})\n", sym);
    }
    else //单个字符的 + - * / = #
    {
        bool canfind = false;
        for (int i = 0; i < SINGAL_CH_CNT; ++i)
        {
            if (ch == SINGAL_CH[i])
            {
                sym = (enum symbol)CSYM[i];
                printf("(%2d,%c)\n", sym, SINGAL_CH[i]);
                canfind = true;
                break;
            }
        }

        if (!canfind)
        {
            error(CANNOT_IDENTIFY);
        }
        getch();
    }
    return 1;
}

int main()
{
    fp = fopen("source.txt", "r");

    printf("------------lexer begin!------------\n");
    lexer(fp);
    printf("------------lexer over!------------\n");

    fclose(fp);
}

int lexer(FILE *fp)
{
    getch();
    while (getSym())
        ;
}


void error(int i)
{
    
    for(int i=0;i<ll;++i) printf("%c",line[i]);//打印本行
    printf("\n");
    for(int i=0;i<cc-1;++i)//这个减2 liangtodo
        printf(" ");//打印空格
    printf("^\n");//打印指出错误的地方

    switch (i)
    {
    case ID_LEN_TOO_MAX:
        printf("id len too long");
        break;

    case SHOULD_EQUAL:
        printf("there should be =");
        break;

    case SHOULD_BE_NUM:
        printf("there should be number!");
    
    default:
        break;
    }

    exit(0);
}
