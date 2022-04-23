#include "lexer.cpp"

//****** 语法分析阶段 ******//
void Block();
void ConstDeclaration();
void VarDeclaration();
void Statement();
void Condition();
void Expression();
void Term();
void Factor();

int main()
{
    fp = fopen("s1.txt", "r");
    if(fp==0)
    {
        errors("no file");
    }


    getSym();
    Block();
    if(sym!=period)
    {
       errors("should be .");
    }
    else 
    {
        printf("语法分析通过!");
    }

    fclose(fp);
}

void Block()
{
    while(1)
    {
        //常量声明的情况
        if(sym==constsym)
        {
            getSym();
            ConstDeclaration();

            while(sym==comma)
            {
                getSym();
                ConstDeclaration();
            }

            if(sym==semicolon)
            {
                getSym();
            }
            else 
            {
                errors("should be ; here");
            }

        }
        else if(sym==varsym)
        {
            getSym();
            VarDeclaration();

            while(sym==comma)
            {
                getSym();
                VarDeclaration();
            }

            if(sym==semicolon)
            {
                getSym();
            }
            else 
            {
                errors("should be ; here");
                //应该是分号
            }
        }
        else if(sym==procsym)
        {
            getSym();
            if(sym==ident)
            {
                //将其注册
                getSym();
            }
            else 
            {
                errors("should be id here");
            }

            if(sym==semicolon)//分号
            {
                getSym();
            }
            else 
            {
                errors("should be ; here");
            }


            Block();

            if(sym==semicolon)
            {
                getSym();
            }
            else 
            {
                errors("should be ; here");
            }
        }
        else 
        {
            break;
        }

        Statement();
    }
}

void ConstDeclaration()
{
    if(sym==ident)
    {
        getSym();
        if(sym==eql)
        {
            getSym();

            if(sym==number)
            {
                //注册这个常量
                getSym();
            }
            else 
            {
                errors("shold be number here");
            }
        }
        else 
        {
            errors("should be =");
        }
    }
    else 
    {
        errors("should be id");
    }

    
}

void VarDeclaration()
{
    if(sym==ident)
    {
        //注册
        getSym();
    }
    else 
    {
        //errorsor
        errors("should be id");
    }
}

void Statement()
{
    if(sym==ident)
    {
        getSym();//
        if(sym!=becomes)
            errors("should be :=");
        getSym();//to expression
        Expression();
    }
    else if(sym==ifsym)
    {
        getSym();
        Condition();

        if(sym==thensym)
        {
            getSym();
            Statement();
        }
        else 
        {
            //should be then
            errors("should be then");
        }
        
    }
    else if(sym==whilesym)
    {
        getSym();
        Condition();

        if(sym==dosym)
        {
            getSym();
            Statement();
        }
        else 
        {
            //should be do
            errors("should be do ");
        }
    }
    else if(sym==callsym)
    {
        getSym();
        if(sym==ident)
        {
            //执行call
            getSym();
        }
        else 
        {
            //should be ident
            errors("should be id");
        }
    }
    else if(sym==readsym)
    {
        //todo
    }
    else if(sym==writesym)
    {
        //todo
    }
    else if(sym==beginsym)
    {
        getSym();
        Statement();
        while(sym==semicolon)
        {
            getSym();
            Statement();
        }
        if(sym==endsym)
        {
            getSym();
        }
        else 
        {
            //should be end
            errors("should be end");
        }

    }
    else 
    {
        //空的情况
    }
}

void Condition()
{
    if(sym==oddsym)
    {
        getSym();
        Expression();
    }
    else 
    {
        Expression();
        if(sym==eql||sym==neq ||sym==lss||sym==leq||sym==gtr||sym==geq)
        {
            getSym();
            Expression();
        }
        else 
        {
            //should be = #
            errors("should be fuhao");
        }
    }
    
}

void Expression()
{
    if(sym==plus||sym==minus)
    {
        getSym();
    }
    Term();
    while(sym==plus||sym==minus)
    {
        getSym();
        Term();
    }
}

void Term()
{
    Factor();
    while(sym==times||sym==slash)
    {
        getSym();
        Factor();
    }
}

void Factor()
{
    if(sym==ident)
    {
        getSym();
    }
    else if(sym==number)
    {
        getSym();
    }
    else if(sym==lparen)
    {
        Expression();
        if(sym==rparen)
        {
            getSym();
        }
        else 
        {
            //should be )
            errors("should be )");
        }
    }
    else 
    {
        errors("factor error should be id or num or (");
    }
}

