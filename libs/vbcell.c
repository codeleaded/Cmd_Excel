#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/Excel.h"

void VBCell_Destroyer(Variable* v){
    //printf("VBCell: Destroyer!\n");
    CStr* str = (CStr*)Variable_Data(v);
    CStr_Free(str);
}
void VBCell_Cpyer(Variable* src,Variable* dst){
    //printf("VBCell: Cpyer!\n");
    CStr* src_str = (CStr*)Variable_Data(src);
    CStr* dst_str = (CStr*)Variable_Data(dst);
    *dst_str = CStr_Cpy(*src_str);
}

Token VBCell_VBCell_Handler_Ass(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);
    
    /*
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(a_var){
            Variable_PrepairFor(a_var,sizeof(CStr),"str",VBCell_Destroyer,VBCell_Cpyer);
            Variable_SetTo(a_var,(CStr[]){ CStr_Cpy(n2) });
        }else{
            Scope_BuildInitVariableRange(s,a->str,"str",s->range-1,(CStr[]){ CStr_Cpy(n2) });
        }
    }else{
        printf("[VBCell_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
    }
    */

    return Token_By(TOKEN_CONSTSTRING_DOUBLE,NULL);
}
Token VBCell_VBCell_Handler_Add(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ADD: %s + %s\n",a->str,b->str);

    CStr n1 = Implementation_VBCellOf(s,a);
    CStr n2 = Implementation_VBCellOf(s,b);
    int s1 = CStr_Size(n1);
    int s2 = CStr_Size(n2);
    
    CStr res = malloc(s1+s2+1);
    memcpy(res,n1,s1);
    memcpy(res+s1,n2,s2+1);

    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,res);
}
Token VBCell_VBCell_Handler_Equ(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("EQU: %s == %s\n",a->str,b->str);

    CStr n1 = Implementation_VBCellOf(s,a);
    CStr n2 = Implementation_VBCellOf(s,b);
    Boolean res = CStr_Cmp(n1,n2);

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token VBCell_Handler_Cast(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("CAST: %s\n",a->str);

    CStr n1 = Implementation_VBCellOf(s,a);
    
    CStr res = n1;
    char* resstr = CStr_Cpy(res);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,resstr);
}
Token VBCell_Handler_Destroy(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("DESTROY: %s\n",a->str);

    Variable* a_var = Scope_FindVariable(s,a->str);
    if(a_var){
        a_var->destroy(a_var);
    }
    
    return Token_Null();
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("str",8,OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))VBCell_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,NULL),
                OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "str",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_VBCell_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))VBCell_VBCell_Handler_Add),
                OperatorDefiner_New(TOKEN_VBLIKE_EQU,(Token(*)(void*,Token*,Vector*))VBCell_VBCell_Handler_Equ),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),VBCell_Destroyer,VBCell_Cpyer)
    );
}
