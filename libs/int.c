#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/Excel.h"

Token Int_Any_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("(int,int) ASS: %s = %s\n",a->str,b->str);

    Number n2 = Excel_Int_Get(e,b);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            Variable_PrepairFor(a_var,sizeof(Number),"int",NULL,NULL);
            Variable_SetTo(a_var,(Number[]){ n2 });
        }else{
            Scope_BuildInitVariableRange(
                &e->vbl.ev.sc,
                a->str,
                "int",
                e->vbl.ev.sc.range-1,
                (Number[]){ n2 }
            );
        }
    }else{
        printf("[Int_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
    }

    Number res = n2;
    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Int_Any_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ADD: %s + %s\n",a->str,b->str);

    Number n1 = Excel_Int_Get(e,a);
    Number n2 = Excel_Int_Get(e,b);
    Number res = n1 + n2;

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Int_Any_Handler_Sub(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("SUB: %s - %s\n",a->str,b->str);

    Number n1 = Excel_Int_Get(e,a);
    Number n2 = Excel_Int_Get(e,b);
    Number res = n1 - n2;

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Int_Any_Handler_Mul(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("MUL: %s * %s\n",a->str,b->str);

    Number n1 = Excel_Int_Get(e,a);
    Number n2 = Excel_Int_Get(e,b);
    Number res = n1 * n2;

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Int_Any_Handler_Div(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("DIV: %s / %s\n",a->str,b->str);

    Number n1 = Excel_Int_Get(e,a);
    Number n2 = Excel_Int_Get(e,b);

    Number res = 0;
    if(n2!=0) res = n1 / n2;
    else{
        printf("[Enviroment]: Error: DIV by Zero\n");
    }

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Int_Any_Handler_Equ(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("EQU: %s == %s\n",a->str,b->str);

    Number n1 = Excel_Int_Get(e,a);
    Number n2 = Excel_Int_Get(e,b);
    Boolean res = n1 == n2;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Int_Any_Handler_Les(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("LES: %s < %s\n",a->str,b->str);

    Number n1 = Excel_Int_Get(e,a);
    Number n2 = Excel_Int_Get(e,b);
    Boolean res = n1 < n2;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Int_Any_Handler_Grt(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("GRT: %s > %s\n",a->str,b->str);

    Number n1 = Excel_Int_Get(e,a);
    Number n2 = Excel_Int_Get(e,b);
    Boolean res = n1 > n2;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Int_Any_Handler_Leq(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("LEQ: %s <= %s\n",a->str,b->str);

    Number n1 = Excel_Int_Get(e,a);
    Number n2 = Excel_Int_Get(e,b);
    Boolean res = n1 <= n2;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Int_Any_Handler_Grq(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("GRQ: %s >= %s\n",a->str,b->str);

    Number n1 = Excel_Int_Get(e,a);
    Number n2 = Excel_Int_Get(e,b);
    Boolean res = n1 >= n2;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}

Token Int_Handler_Neg(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("NEG: -%s\n",a->str);

    Number n1 = Excel_Int_Get(e,a);
    Number res = -n1;

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Int_Handler_Cast(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("CAST: %s\n",a->str);

    Number res = Excel_Int_Get(e,a);
    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,resstr);
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("int",sizeof(Number),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_NEG,(Token(*)(void*,Token*,Vector*))Int_Handler_Neg),
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))Int_Handler_Cast),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ OPERATORINTERATER_DONTCARE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))Int_Any_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))Int_Any_Handler_Add),
                OperatorDefiner_New(TOKEN_VBLIKE_SUB,(Token(*)(void*,Token*,Vector*))Int_Any_Handler_Sub),
                OperatorDefiner_New(TOKEN_VBLIKE_MUL,(Token(*)(void*,Token*,Vector*))Int_Any_Handler_Mul),
                OperatorDefiner_New(TOKEN_VBLIKE_DIV,(Token(*)(void*,Token*,Vector*))Int_Any_Handler_Div),
                OperatorDefiner_New(TOKEN_VBLIKE_EQU,(Token(*)(void*,Token*,Vector*))Int_Any_Handler_Equ),
                OperatorDefiner_New(TOKEN_VBLIKE_LES,(Token(*)(void*,Token*,Vector*))Int_Any_Handler_Les),
                OperatorDefiner_New(TOKEN_VBLIKE_GRT,(Token(*)(void*,Token*,Vector*))Int_Any_Handler_Grt),
                OperatorDefiner_New(TOKEN_VBLIKE_LEQ,(Token(*)(void*,Token*,Vector*))Int_Any_Handler_Leq),
                OperatorDefiner_New(TOKEN_VBLIKE_GRQ,(Token(*)(void*,Token*,Vector*))Int_Any_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),NULL,NULL)
    );
}