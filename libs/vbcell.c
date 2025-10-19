#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/Excel.h"

Token VBCell_Any_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);

    const Vic2 pos = VBLike_ExtractCoords(a->str);
    ExcelCell* vbcell_a = VBCell_Implementation(e,a);

    if(b->tt == TOKEN_VBLIKE_CELL){
        ExcelCell* vbcell_b = VBCell_Implementation(e,b);
    }else{
        if(b->tt == TOKEN_STRING){
            Variable* v_b = Scope_FindVariable(&e->vbl.ev.sc,b->str);
            
            char buffer[32];
            sprintf(buffer,"$(%d,%d)",pos.x,pos.y);
            Variable v_vbcell = Variable_New(buffer,NULL,-1,-1,NULL,NULL);

            Variable_Set(&v_vbcell,v_b);

            vbcell_a->output = v_vbcell.data;
            vbcell_a->type = v_vbcell.typename;
            v_vbcell.data = NULL;
            v_vbcell.typename = NULL;

            Variable_Free(&v_vbcell);
        }else if(b->tt == TOKEN_NUMBER){
            vbcell_a->output = malloc(sizeof(I64));
            *(I64*)vbcell_a->output = I64_Parse(b->str);
            CStr_Set(&vbcell_a->type,"int");
        }else if(b->tt == TOKEN_FLOAT){
            vbcell_a->output = malloc(sizeof(F64));
            *(F64*)vbcell_a->output = F64_Parse(b->str);
            CStr_Set(&vbcell_a->type,"float");
        }else if(b->tt == TOKEN_CONSTSTRING_DOUBLE){
            vbcell_a->output = malloc(sizeof(CStr));
            *(CStr*)vbcell_a->output = CStr_Cpy(b->str);
            CStr_Set(&vbcell_a->type,"str");
        }else if(b->tt == TOKEN_BOOL){
            vbcell_a->output = malloc(sizeof(Boolean));
            *(Boolean*)vbcell_a->output = Boolean_Parse(b->str);
            CStr_Set(&vbcell_a->type,"bool");
        }else if(b->tt == TOKEN_FUNCTIONPOINTER){
            vbcell_a->output = malloc(sizeof(CStr));
            *(CStr*)vbcell_a->output = CStr_Cpy(b->str);
            CStr_Set(&vbcell_a->type,"func");
        }
    }

    return Token_Move(TOKEN_NONE,NULL);
}
Token VBCell_Any_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return Token_Move(TOKEN_NONE,NULL);
}

Token VBCell_VBCell_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);

    const Vic2 pos = VBLike_ExtractCoords(a->str);
    ExcelCell* vbcell_a = VBCell_Implementation(e,a);
    ExcelCell* vbcell_b = VBCell_Implementation(e,b);

    if(vbcell_a && vbcell_b){
        if(vbcell_b->type){
            CStr_Set(&vbcell_a->type,vbcell_b->type);

            if(vbcell_b->output){
                if(vbcell_a->output) free(vbcell_a->output);
                const int size = TypeMap_SizeOf(&e->vbl.ev.sc.types,vbcell_b->type);
                vbcell_a->output = malloc(size);
                memcpy(vbcell_a->output,vbcell_b->output,size);
            }else{
                Interpreter_ErrorHandler(&e->vbl.ev,"%s is null!",b->str);
                return Token_Null();
            }
        }else{
            Interpreter_ErrorHandler(&e->vbl.ev,"%s is typeless!",b->str);
            return Token_Null();
        }
    }else{
        Interpreter_ErrorHandler(&e->vbl.ev,"%s or %s is not a VB Cell!",a->str,b->str);
        return Token_Null();
    }

    return Token_Cpy(a);
}
Token VBCell_VBCell_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);

    const Vic2 pos = VBLike_ExtractCoords(a->str);
    ExcelCell* vbcell_a = VBCell_Implementation(e,a);
    ExcelCell* vbcell_b = VBCell_Implementation(e,b);

    if(CStr_Cmp(vbcell_a->type,"int")){
        I64 a_value = *(I64*)vbcell_a->output;

        if(CStr_Cmp(vbcell_b->type,"int")){
            I64 b_value = *(I64*)vbcell_b->output;
            return Token_Move(TOKEN_NUMBER,I64_Get_D(a_value + b_value));
        }else if(CStr_Cmp(vbcell_b->type,"float")){
            F64 b_value = *(F64*)vbcell_b->output;
            return Token_Move(TOKEN_FLOAT,F64_Get_Dc((F64)a_value + b_value));
        }else if(!vbcell_b->type){
            CStr a_str = I64_Get_D(a_value);
            CStr ret = CStr_Concat(a_str,(CStr)vbcell_b->output);
            CStr_Free(a_str);
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,ret);
        }

        return Token_Null();
    }else if(CStr_Cmp(vbcell_a->type,"float")){
        F64 a_value = *(F64*)vbcell_a->output;

        if(CStr_Cmp(vbcell_b->type,"int")){
            I64 b_value = *(I64*)vbcell_b->output;
            return Token_Move(TOKEN_FLOAT,F64_Get_Dc(a_value + (F64)b_value));
        }else if(CStr_Cmp(vbcell_b->type,"float")){
            F64 b_value = *(F64*)vbcell_b->output;
            return Token_Move(TOKEN_FLOAT,F64_Get_Dc(a_value + b_value));
        }else if(!vbcell_b->type){
            CStr a_str = F64_Get_Dc(a_value);
            CStr ret = CStr_Concat(a_str,(CStr)vbcell_b->output);
            CStr_Free(a_str);
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,ret);
        }
        
        return Token_Null();
    }else if(!vbcell_a->type){
        CStr a_value = (CStr)vbcell_a->output;

        if(CStr_Cmp(vbcell_b->type,"int")){
            I64 b_value = *(I64*)vbcell_b->output;
            CStr b_str = I64_Get_D(b_value);
            CStr ret = CStr_Concat(a_value,b_str);
            CStr_Free(b_str);
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,ret);
        }else if(CStr_Cmp(vbcell_b->type,"float")){
            F64 b_value = *(F64*)vbcell_b->output;
            CStr b_str = F64_Get_Dc(b_value);
            CStr ret = CStr_Concat(a_value,b_str);
            CStr_Free(b_str);
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,ret);
        }else if(!vbcell_b->type){
            CStr ret = CStr_Concat(a_value,(CStr)vbcell_b->output);
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,ret);
        }
        
        return Token_Null();
    }

    return Token_Null();
}

Token VBCell_Int_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);

    const Vic2 pos = VBLike_ExtractCoords(a->str);
    ExcelCell* vbcell_a = VBCell_Implementation(e,a);

    if(vbcell_a){
        I64 b_value = *(I64*)I64_Parse(b->str);
        return Token_Move(TOKEN_NUMBER,I64_Get_D(a_value + b_value));
    }else{
        Interpreter_ErrorHandler(&e->vbl.ev,"%s is not a VB Cell!",a->str);
        return Token_Null();
    }

    return Token_Cpy(a);
}
Token VBCell_Int_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);

    const Vic2 pos = VBLike_ExtractCoords(a->str);
    ExcelCell* vbcell_a = VBCell_Implementation(e,a);
    ExcelCell* vbcell_b = VBCell_Implementation(e,b);

    if(CStr_Cmp(vbcell_a->type,"int")){
        I64 a_value = *(I64*)vbcell_a->output;

        if(CStr_Cmp(vbcell_b->type,"int")){
            I64 b_value = *(I64*)vbcell_b->output;
            return Token_Move(TOKEN_NUMBER,I64_Get_D(a_value + b_value));
        }else if(CStr_Cmp(vbcell_b->type,"float")){
            F64 b_value = *(F64*)vbcell_b->output;
            return Token_Move(TOKEN_FLOAT,F64_Get_Dc((F64)a_value + b_value));
        }else if(!vbcell_b->type){
            CStr a_str = I64_Get_D(a_value);
            CStr ret = CStr_Concat(a_str,(CStr)vbcell_b->output);
            CStr_Free(a_str);
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,ret);
        }

        return Token_Null();
    }else if(CStr_Cmp(vbcell_a->type,"float")){
        F64 a_value = *(F64*)vbcell_a->output;

        if(CStr_Cmp(vbcell_b->type,"int")){
            I64 b_value = *(I64*)vbcell_b->output;
            return Token_Move(TOKEN_FLOAT,F64_Get_Dc(a_value + (F64)b_value));
        }else if(CStr_Cmp(vbcell_b->type,"float")){
            F64 b_value = *(F64*)vbcell_b->output;
            return Token_Move(TOKEN_FLOAT,F64_Get_Dc(a_value + b_value));
        }else if(!vbcell_b->type){
            CStr a_str = F64_Get_Dc(a_value);
            CStr ret = CStr_Concat(a_str,(CStr)vbcell_b->output);
            CStr_Free(a_str);
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,ret);
        }
        
        return Token_Null();
    }else if(!vbcell_a->type){
        CStr a_value = (CStr)vbcell_a->output;

        if(CStr_Cmp(vbcell_b->type,"int")){
            I64 b_value = *(I64*)vbcell_b->output;
            CStr b_str = I64_Get_D(b_value);
            CStr ret = CStr_Concat(a_value,b_str);
            CStr_Free(b_str);
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,ret);
        }else if(CStr_Cmp(vbcell_b->type,"float")){
            F64 b_value = *(F64*)vbcell_b->output;
            CStr b_str = F64_Get_Dc(b_value);
            CStr ret = CStr_Concat(a_value,b_str);
            CStr_Free(b_str);
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,ret);
        }else if(!vbcell_b->type){
            CStr ret = CStr_Concat(a_value,(CStr)vbcell_b->output);
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,ret);
        }
        
        return Token_Null();
    }

    return Token_Null();
}



Token VBCell_Handler_Cast(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //ExcelCell* ec = VBCell_Implementation(e,a);
    const Vic2 pos = VBLike_ExtractCoords(a->str);

    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,CStr_Format("$(%d,%d)",pos.x,pos.y));
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("vbcell",sizeof(void*),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))VBCell_Handler_Cast),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "vbcell",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_VBCell_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))VBCell_VBCell_Handler_Add),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "int",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_Any_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))VBCell_Any_Handler_Add),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "float",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_Any_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))VBCell_Any_Handler_Add),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "bool",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_Any_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))VBCell_Any_Handler_Add),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "str",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_Any_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))VBCell_Any_Handler_Add),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),NULL,NULL)
    );
}
