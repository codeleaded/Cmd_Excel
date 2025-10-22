#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/Excel.h"

void Func_Destroyer(Variable* v){
    //printf("Str: Destroyer!\n");
    CStr* func = (CStr*)Variable_Data(v);
    CStr_Free(func);
}
void Func_Cpyer(Variable* src,Variable* dst){
    //printf("Str: Cpyer!\n");
    CStr* src_str = (CStr*)Variable_Data(src);
    CStr* dst_str = (CStr*)Variable_Data(dst);
    *dst_str = CStr_Cpy(*src_str);
}

Token Func_Func_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);

    CStr n2 = Excel_Func_Get(e,b);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            Variable_PrepairFor(a_var,sizeof(CStr),"func",Func_Destroyer,Func_Cpyer);
            Variable_SetTo(a_var,(CStr[]){ CStr_Cpy(n2) });
        }else{
            Scope_BuildInitVariableRange(&e->vbl.ev.sc,a->str,"func",e->vbl.ev.sc.range-1,(CStr[]){ CStr_Cpy(n2) });
        }
    }else{
        printf("[Func_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
    }

    return Token_By(TOKEN_CONSTSTRING_DOUBLE,n2);
}
Token Func_Func_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ADD: %s + %s\n",a->str,b->str);

    CStr n1 = Excel_Func_Get(e,a);
    CStr n2 = Excel_Func_Get(e,b);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,CStr_Concat(n1,n2));
}
Token Func_Func_Handler_Equ(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("EQU: %s == %s\n",a->str,b->str);

    CStr n1 = Excel_Func_Get(e,a);
    CStr n2 = Excel_Func_Get(e,b);
    Boolean res = CStr_Cmp(n1,n2);

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Func_Handler_Cast(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("CAST: %s\n",a->str);

    CStr n1 = Excel_Func_Get(e,a);
    
    CStr res = n1;
    char* resstr = CStr_Cpy(res);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,resstr);
}
Token Func_Handler_Destroy(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("DESTROY: %s\n",a->str);

    Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
    if(a_var){
        a_var->destroy(a_var);
    }
    
    return Token_Null();
}

Token Func_Int_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ADD: %s + %s\n",a->str,b->str);

    CStr n1 = Excel_Func_Get(e,a);
    Number n2 = Excel_Int_Get(e,b);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,CStr_Format("%s%d",n1,n2));
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("func",sizeof(CStr),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))Func_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,NULL),
                OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "func",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))Func_Func_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))Func_Func_Handler_Add),
                OperatorDefiner_New(TOKEN_VBLIKE_EQU,(Token(*)(void*,Token*,Vector*))Func_Func_Handler_Equ),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "int",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))Func_Int_Handler_Add),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),Func_Destroyer,Func_Cpyer)
    );
}
