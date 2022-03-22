/*
 * @Author: liangzichao 
 * @Date: 2022-03-22 14:36:09 
 * @Last Modified by: liangzichao
 * @Last Modified time: 2022-03-22 14:37:24
 * 
 * 这是最直接的版本，没有用到DFA
 * todo:error处理 标识符长度限制
 */
#include <cstdio>
#include <cstring>
#include <cctype>

//单词种别
enum symbol
{
    nul,ident,number,plus,minus,times,slash,oddsym,eql,neq,lss,leq,gtr,geq,lparen,rparen,comma,semicolon,
    period,becomes,beginsym,endsym,ifsym,thensym,whilesym,writesym,readsym,dosym,callsym,constsym,varsym,procsym,comment
};
//nul表示识别不了
//becomes 赋值
//comma 逗号 semicolon分号 period 句号


const int al=10;//标识符的最大长度

//func
int lexer(FILE *fp);
//函数返回值
enum symbol sym;//单词种别
char id[al+1];//传递标识符单词自身的值 即名字
int num;//传递无符号整数单词的值 




const int WORD_CNT=13;
const int SINGAL_CH_CNT=11;//+ - * /  = # ( ) , . ;

char * WORD[WORD_CNT]={"const","var","procedure","begin","end","odd","if","then","call","while","do","read","write"};
int WSYM[WORD_CNT]={constsym,varsym,procsym,beginsym,endsym,oddsym,ifsym,thensym,callsym,whilesym,dosym,readsym,writesym};


char SINGAL_CH[SINGAL_CH_CNT]={'+','-','*','/','=','#','(',')',',','.',';'};
int CSYM[SINGAL_CH_CNT]={plus,minus,times,slash,eql,neq,lparen,rparen,comma,period,semicolon};
//word symbol 单词（关键字）对应的种别



int main()
{
    FILE *fp=fopen("source.txt","r");

    printf("------------lexer begin!------------\n");
    lexer(fp);    
    printf("------------lexer over!------------\n");


    fclose(fp);
}
//liangtodo 注释yet done
int lexer(FILE * fp)
{
    char ch=fgetc(fp);
    while(ch!=EOF)
    {
        //忽略空格 回车 换行 tab
        while(ch==' '||ch=='\r'||ch=='\n'||ch=='\t')
            ch=fgetc(fp);
        //这会不会是EOF...

        if(isalpha(ch))//标识符或者关键字
        {
            //后面可能是数字或者字母
            int i=0;
            while(isalnum(ch))
            {
                //liangtodo 没有限制大小 error
                id[i++]=ch;
                ch=fgetc(fp);
            }
            id[i]='\0';
            bool is_reserved=0;
            //判断是关键字还是标识符
            for(int i=0;i<WORD_CNT;++i)
            {
                if(strcmp(id,WORD[i])==0)
                {
                    sym=(enum symbol)WSYM[i];
                    is_reserved=1;
                    break;
                }
            }
            if(!is_reserved)
            {
                sym=ident;
            }
            //返回
            printf("(%2d,'%s')\n",sym,id);

            //现在的ch是下一个单词的开头了
        }
        else if(isdigit(ch))//如果是数字
        {
            sym=number;
            num=0;
            while(isdigit(ch))
            {
                num=num*10+ch-'0';
                ch=fgetc(fp);
            }
            printf("(%2d,%d)\n",sym,num);
        }
        else if(ch==':') //:=
        {
            ch=fgetc(fp);
            if(ch=='=')
            {
                sym=becomes;
                printf("(%2d,:=)\n",sym);
                ch=fgetc(fp);
            }
            else 
            {
                sym=nul;
                //error
            }
        }
        else if(ch=='>')//> >=
        {
            ch=fgetc(fp);
            if(ch=='=')
            {
                sym=geq;
                printf("(%2d,>=)\n",sym);
                ch=fgetc(fp);
            }
            else 
            {
                sym=gtr;
                printf("%2d,>)\n",sym);
            }
        }
        else if(ch=='<')//< <=
        {
            ch=fgetc(fp);
            if(ch=='=')
            {
                sym=leq;
                printf("(%2d,<=)\n",sym);
                ch=fgetc(fp);
            }
            else 
            {
                sym=lss;
                printf("(%2d,<)\n",sym);
            }
        }  
        else if(ch=='{')//注释
        {
            sym=comment;
            while(ch=fgetc(fp)!='}')
                ;
            //ch现在就是}
            ch=fgetc(fp);
            printf("(%d,{})\n",sym);
        }
        else//单个字符的 + - * / = #
        {
            for(int i=0;i<SINGAL_CH_CNT;++i)
            {
                if(ch==SINGAL_CH[i])
                {
                    sym=(enum symbol)CSYM[i];
                    printf("(%2d,%c)\n",sym,SINGAL_CH[i]);
                    break;
                }
            }
            ch=fgetc(fp);
        }
    }
}
