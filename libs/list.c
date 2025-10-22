#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/Excel.h"

Number Implementation_IntOf(Excel* e,Token* a){
    Number n = NUMBER_PARSE_ERROR;
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            n = *(Number*)Variable_Data(a_var);
        }else{
            printf("[Int_Number]: 1. Arg: Variable %s doesn't exist!\n",a->str);
        }
    }else if(a->tt==TOKEN_NUMBER){
        n = Number_Parse(a->str);
    }
    return n;
}
Boolean Implementation_BooleanOf(Excel* e,Token* a){
    Boolean n = 0;
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            n = *(Boolean*)Variable_Data(a_var);
        }else{
            printf("[Int_Boolean]: 1. Arg: Variable %s doesn't exist!\n",a->str);
        }
    }else if(a->tt==TOKEN_BOOL){
        n = Boolean_Parse(a->str);
    }
    return n;
}
Double Implementation_FloatOf(Excel* e,Token* a){
    Double n = DOUBLE_PARSE_ERROR;
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            n = *(Double*)Variable_Data(a_var);
        }else{
            printf("[Int_Double]: 1. Arg: Variable %s doesn't exist!\n",a->str);
        }
    }else if(a->tt==TOKEN_FLOAT){
        n = Double_Parse(a->str,1);
    }
    return n;
}
CStr Implementation_StrOf(Excel* e,Token* a){
    CStr n = NULL;
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            n = *(CStr*)Variable_Data(a_var);
        }else{
            printf("[Int_CStr]: 1. Arg: Variable %s doesn't exist!\n",a->str);
        }
    }else if(a->tt==TOKEN_CONSTSTRING_DOUBLE){
        n = a->str;
    }
    return n;
}


typedef VariableMap Listtype;// Vector<Variable>

void List_Destroyer(Variable* v){
    //printf("List: Destroyer!\n");
    VariableMap_Free(Variable_Data(v));
}
void List_Cpyer(Variable* src,Variable* dst){
    printf("List: Cpyer!\n");
    Listtype* src_str = (Listtype*)Variable_Data(src);
    Listtype* dst_str = (Listtype*)Variable_Data(dst);
    *dst_str = VariableMap_Cpy(src_str);
}

Token List_List_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASS: %s = %s\n",a->str,b->str);

    Variable* b_var;
    if(b->tt==TOKEN_STRING){
        b_var = Scope_FindVariable(&e->vbl.ev.sc,b->str);
        if(!b_var){
            printf("[List_Ass]: 1. Arg: Variable %s doesn't exist!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Ass]: 1. Arg: %s is not a List type!\n",a->str);
        return Token_Null();
    }
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(!a_var){
            Scope_BuildVariableRange(&e->vbl.ev.sc,a->str,"list",e->vbl.ev.sc.range-1);
            a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
            if(a_var->data) free(a_var->data);
            a_var->data = NULL;
        }

        Variable_Set(a_var,b_var);
    }else{
        printf("[List_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}

Token List_Int_Handler_Asd(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASD: %s += %s\n",a->str,b->str);

    Number cn = Excel_Int_Get(e,b);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Acs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else if(cn!=NUMBER_PARSE_ERROR){
                VariableMap* members = Variable_Data(a_var);
                VariableMap_PPush(members,(Variable[]){ Variable_Make("NL","int",(Number[]){ cn },sizeof(Number),-1,NULL,NULL) });
            }else{
                printf("[List_Asd]: 2. Arg: %s is not a int!\n",b->str);
                return Token_Null();
            }
        }else{
            printf("[List_Asd]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Asd]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token List_Str_Handler_Asd(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASD: %s += %s\n",a->str,b->str);

    CStr cs = Excel_Str_Get(e,b);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Acs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else if(cs!=NULL){
                VariableMap* members = Variable_Data(a_var);
                VariableMap_PPush(members,(Variable[]){ 
                    Variable_Make("NL","str",(CStr[]){ CStr_Cpy(cs) },
                    sizeof(CStr),-1,Scope_DestroyerOfType(&e->vbl.ev.sc,"str"),Scope_CpyerOfType(&e->vbl.ev.sc,"str"))
                });
            }else{
                printf("[List_Asd]: 2. Arg: %s is not a str!\n",b->str);
                return Token_Null();
            }
        }else{
            printf("[List_Asd]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Asd]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token List_Boolean_Handler_Asd(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASD: %s += %s\n",a->str,b->str);

    Boolean cb = Excel_Bool_Get(e,b);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Acs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else if(cb){
                VariableMap* members = Variable_Data(a_var);
                VariableMap_PPush(members,(Variable[]){ Variable_Make("NL","Boolean",(Boolean[]){ cb },sizeof(Boolean),-1,NULL,NULL) });
            }else{
                printf("[List_Asd]: 2. Arg: %s is not a Boolean!\n",b->str);
                return Token_Null();
            }
        }else{
            printf("[List_Asd]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Asd]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token List_Float_Handler_Asd(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASD: %s += %s\n",a->str,b->str);

    Double cf = Excel_Float_Get(e,b);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Acs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else if(cf!=DOUBLE_PARSE_ERROR){
                VariableMap* members = Variable_Data(a_var);
                VariableMap_PPush(members,(Variable[]){ Variable_Make("NL","float",(Double[]){ cf },sizeof(Double),-1,NULL,NULL) });
            }else{
                printf("[List_Asd]: 2. Arg: %s is not a float!\n",b->str);
                return Token_Null();
            }
        }else{
            printf("[List_Asd]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Asd]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token List_Obj_Handler_Asd(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASD: %s += %s\n",a->str,b->str);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Acs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else if(b->tt==TOKEN_STRING){
                VariableMap* members = Variable_Data(a_var);
                Variable* b_var = Scope_FindVariable(&e->vbl.ev.sc,b->str);
                if(b_var){
                    if(!Variable_Data(b_var)){
                        printf("[List_Asd]: 2. Arg: %s is not inited!\n",b->str);
                        return Token_Null();
                    }else if(CStr_Cmp(b_var->typename,"obj")){
                        void (*Cpyer)(Variable*,Variable*) = Scope_CpyerOfType(&e->vbl.ev.sc,b_var->typename);
                        
                        Variable var = Variable_Make("NL",b_var->typename,NULL,sizeof(CStr),-1,Scope_DestroyerOfType(&e->vbl.ev.sc,b_var->typename),Scope_CpyerOfType(&e->vbl.ev.sc,b_var->typename));
                        Cpyer(b_var,&var);

                        VariableMap_PPush(members,&var);
                    }else{
                        printf("[List_Asd]: 2. Arg: %s is from a not defined type!\n",b->str);
                        return Token_Null();
                    }
                }else{
                    printf("[List_Asd]: 2. Arg: %s is not a variable!\n",b->str);
                    return Token_Null();
                }
            }else{
                printf("[List_Asd]: 2. Arg: %s is not a obj!\n",a->str);
                return Token_Null();
            }
        }else{
            printf("[List_Asd]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Asd]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token List_List_Handler_Asd(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASD: %s += %s\n",a->str,b->str);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Acs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else if(b->tt==TOKEN_STRING){
                VariableMap* members = Variable_Data(a_var);
                Variable* b_var = Scope_FindVariable(&e->vbl.ev.sc,b->str);
                if(b_var){
                    if(!Variable_Data(b_var)){
                        printf("[List_Asd]: 2. Arg: %s is not inited!\n",b->str);
                        return Token_Null();
                    }else if(CStr_Cmp(b_var->typename,"list")){
                        void (*Cpyer)(Variable*,Variable*) = Scope_CpyerOfType(&e->vbl.ev.sc,b_var->typename);
                        
                        Variable var = Variable_Make("NL",b_var->typename,NULL,sizeof(CStr),-1,Scope_DestroyerOfType(&e->vbl.ev.sc,b_var->typename),Scope_CpyerOfType(&e->vbl.ev.sc,b_var->typename));
                        Cpyer(b_var,&var);

                        VariableMap_PPush(members,&var);
                    }else{
                        printf("[List_Asd]: 2. Arg: %s is from a not defined type!\n",b->str);
                        return Token_Null();
                    }
                }else{
                    printf("[List_Asd]: 2. Arg: %s is not a variable!\n",b->str);
                    return Token_Null();
                }
            }else{
                printf("[List_Asd]: 2. Arg: %s is not a obj!\n",a->str);
                return Token_Null();
            }
        }else{
            printf("[List_Asd]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Asd]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}

Token List_Int_Handler_Asu(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASU: %s -= %s\n",a->str,b->str);

    Number cn = Excel_Int_Get(e,b);
    CStr cs = Excel_Str_Get(e,b);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Acs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else if(cn!=NUMBER_PARSE_ERROR){
                VariableMap* members = Variable_Data(a_var);
                if(cn>=0 && cn<members->size){
                    Variable* var = (Variable*)PVector_Get(members,cn);
                    Variable_Free(var);
                    PVector_Remove(members,cn);
                }else{
                    printf("[List_Asu]: index %ld out of bounds!\n",cn);
                    return Token_Null();
                }
            }
        }else{
            printf("[List_Asu]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Asu]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token List_Str_Handler_Asu(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASU: %s -= %s\n",a->str,b->str);

    CStr cs = Excel_Str_Get(e,b);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Acs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else if(cs!=NULL){
                VariableMap* members = Variable_Data(a_var);
                VariableMap_Remove(members,cs);
            }else{
                printf("[List_Asu]: 2. Arg: %s is not a str!\n",b->str);
                return Token_Null();
            }
        }else{
            printf("[List_Asu]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Asu]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}

Token List_Str_Handler_Asm(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASM: %s *= %s\n",a->str,b->str);

    CStr cs = Excel_Str_Get(e,b);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Acs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else if(cs!=NULL){
                VariableMap* members = Variable_Data(a_var);
                VariableMap_PPush(members,(Variable[]){ 
                    Variable_Make(cs,"str",(CStr[]){ "" },
                    sizeof(CStr),-1,Scope_DestroyerOfType(&e->vbl.ev.sc,"str"),Scope_CpyerOfType(&e->vbl.ev.sc,"str"))
                });
            }else{
                printf("[List_Asd]: 2. Arg: %s is not a str!\n",b->str);
                return Token_Null();
            }
        }else{
            printf("[List_Asd]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Asd]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}

Token List_Int_Handler_Subs(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("SUBS: %s[%s]\n",a->str,b->str);

    Number n2 = Excel_Int_Get(e,b);
    
    CStr name = NULL;
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Subs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else{
                VariableMap* members = Variable_Data(a_var);
                if(n2>=0 && n2<members->size){
                    Variable* member = (Variable*)PVector_Get(members,n2);

                    String strbuilder = String_Make(".LISTSUBS");
                
                    Variable* temp = Scope_FindVariableLike(&e->vbl.ev.sc,".LISTSUBS*",'*');

                    if(temp){
                        CStr retnumberstr = temp->name + 9;// CStr_Size(".LISTSUBS") -> 9
                        CStr strdepth = Number_Get(Number_Parse(retnumberstr) + 1);
                        String_Append(&strbuilder,strdepth); 
                        name = String_CStr(&strbuilder);
                        String_Free(&strbuilder);
                        CStr_Free(&strdepth);
                    }else{
                        name = CStr_Cpy(".LISTSUBS0");
                    }

                    Scope_BuildRefVariableRange(&e->vbl.ev.sc,name,member->typename,e->vbl.ev.sc.range+1,member);
                }else{
                    printf("[List_Subs]: in list %s index %ld is out of bounds!\n",a_var->name,n2);
                    return Token_Null();
                }
            }
        }else{
            printf("[List_Subs]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Subs]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Move(TOKEN_STRING,name);
}
Token List_Str_Handler_Subs(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("SUBS: %s[%s]\n",a->str,b->str);

    CStr n2 = Excel_Str_Get(e,b);
    
    CStr name = NULL;
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(a_var){
            if(!Variable_Data(a_var) || !CStr_Cmp(a_var->typename,"list")){
                printf("[List_Subs]: 1. Arg: %s is not a list type!\n",a->str);
                return Token_Null();
            }else{
                VariableMap* members = Variable_Data(a_var);
                Variable* member = VariableMap_Find(members,n2);
                if(member){
                    String strbuilder = String_Make(".LISTSUBS");
                
                    Variable* temp = Scope_FindVariableLike(&e->vbl.ev.sc,".LISTSUBS*",'*');

                    if(temp){
                        CStr retnumberstr = temp->name + 9;// CStr_Size(".LISTSUBS") -> 9
                        CStr strdepth = Number_Get(Number_Parse(retnumberstr) + 1);
                        String_Append(&strbuilder,strdepth); 
                        name = String_CStr(&strbuilder);
                        String_Free(&strbuilder);
                        CStr_Free(&strdepth);
                    }else{
                        name = CStr_Cpy(".LISTSUBS0");
                    }

                    Scope_BuildRefVariableRange(&e->vbl.ev.sc,name,member->typename,e->vbl.ev.sc.range+1,member);
                }else{
                    printf("[List_Subs]: in list %s field \"%s\" doesn't exist!\n",a_var->name,n2);
                    return Token_Null();
                }
            }
        }else{
            printf("[List_Subs]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Subs]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Move(TOKEN_STRING,name);
}
Token List_Handler_Cast(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("CAST: %s\n",a->str);

    Variable* a_var;
    if(a->tt==TOKEN_STRING){
        a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        if(!a_var){
            printf("[List_Ass]: 1. Arg: Variable %s doesn't exist!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[List_Ass]: 1. Arg: %s is not a List type!\n",a->str);
        return Token_Null();
    }

    Listtype* list = (Listtype*)Variable_Data(a_var);

    String builder = String_Make("{ ");
    for(int i = 0;i<list->size;i++){
        Variable* v = (Variable*)PVector_Get(list,i);
        CStr num = Number_Get(i);
        String_Append(&builder,num);
        CStr_Free(&num);
        String_Append(&builder," \"");
        String_Append(&builder,v->name);
        String_Append(&builder,"\":");
        String_Append(&builder,v->typename);
        
        String_Append(&builder," = ");

        CStr content = Scope_VariableContentStr(&e->vbl.ev.sc,v);
        if(content){
            String_Append(&builder,content);
            CStr_Free(&content);
        }else{
            String_Append(&builder,"None");
        }
        
        if(i<list->size-1) String_Append(&builder,", ");
    }
    String_Append(&builder," }");
    char* resstr = String_CStr(&builder);
    String_Free(&builder);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,resstr);
}
Token List_Handler_Destroy(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    printf("DESTROY: %s\n",a->str);

    Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
    if(a_var){
        a_var->destroy(a_var);
    }
    
    return Token_Null();
}

Variable List_Make(Excel* e,CStr name,Variable* args){
    Variable ret = Variable_Make(
        "Listtype","list",(VariableMap[]){ VariableMap_New() },
        sizeof(VariableMap),e->vbl.ev.sc.range-1,
        Scope_DestroyerOfType(&e->vbl.ev.sc,"list"),
        Scope_CpyerOfType(&e->vbl.ev.sc,"list")
    );
    return ret;
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("list",sizeof(Listtype),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))List_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,NULL),
                OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "list",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))List_List_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ASD,(Token(*)(void*,Token*,Vector*))List_List_Handler_Asd),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "int",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_SUBS,(Token(*)(void*,Token*,Vector*))List_Int_Handler_Subs),
                OperatorDefiner_New(TOKEN_VBLIKE_ASD,(Token(*)(void*,Token*,Vector*))List_Int_Handler_Asd),
                OperatorDefiner_New(TOKEN_VBLIKE_ASU,(Token(*)(void*,Token*,Vector*))List_Int_Handler_Asu),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "str",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_SUBS,(Token(*)(void*,Token*,Vector*))List_Str_Handler_Subs),
                OperatorDefiner_New(TOKEN_VBLIKE_ASD,(Token(*)(void*,Token*,Vector*))List_Str_Handler_Asd),
                OperatorDefiner_New(TOKEN_VBLIKE_ASU,(Token(*)(void*,Token*,Vector*))List_Str_Handler_Asu),
                OperatorDefiner_New(TOKEN_VBLIKE_ASM,(Token(*)(void*,Token*,Vector*))List_Str_Handler_Asm),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "Boolean",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASD,(Token(*)(void*,Token*,Vector*))List_Boolean_Handler_Asd),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "float",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASD,(Token(*)(void*,Token*,Vector*))List_Float_Handler_Asd),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "obj",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASD,(Token(*)(void*,Token*,Vector*))List_Obj_Handler_Asd),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),List_Destroyer,List_Cpyer)
    );

    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("new",NULL,(Member[]){ 
        MEMBER_END 
    },(void*)List_Make));
}
