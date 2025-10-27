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

Token VBCell_VBCell_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASS: %s = %s\n",a->str,b->str);

    ExcelCell* ec = Excel_VBCell_Get(e,a);

    if(ec){
        if(CStr_Cmp(ec->type,"int")){
            if(ec->output) free(ec->output);
            CStr_Set(&ec->type,"int");
            ec->output = malloc(sizeof(Number));
            *(Number*)ec->output = Excel_Int_Get(e,b);
        }else if(CStr_Cmp(ec->type,"float")){
            if(ec->output) free(ec->output);
            CStr_Set(&ec->type,"float");
            ec->output = malloc(sizeof(Double));
            *(Double*)ec->output = Excel_Float_Get(e,b);
        }else if(CStr_Cmp(ec->type,"bool")){
            if(ec->output) free(ec->output);
            CStr_Set(&ec->type,"bool");
            ec->output = malloc(sizeof(Boolean));
            *(Boolean*)ec->output = Excel_Bool_Get(e,b);
        }else if(CStr_Cmp(ec->type,"str")){
            if(ec->output) free(ec->output);
            CStr_Set(&ec->type,"str");
            ec->output = CStr_Cpy(Excel_Str_Get(e,b));
        }else if(CStr_Cmp(ec->type,"func")){
            if(ec->output) free(ec->output);
            CStr_Set(&ec->type,"func");
            ec->output = CStr_Cpy(Excel_Str_Get(e,b));
        }else{
            if(ec->output) free(ec->output);
            CStr_Free(&ec->type);
            ec->type = NULL;

            ExcelCell* ec_b = Excel_VBCell_Get(e,b);
            ec->output = CStr_Cpy((CStr)ec_b->output);
        }
    }else if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(&e->vbl.ev.sc,a->str);
        const Vic2 pos = VBLike_ExtractCoords(&e->vbl,b);
        
        if(pos.x>=0 && pos.x<e->countx && pos.y>=0 && pos.y<e->county){
            if(a_var){
                Variable_PrepairFor(a_var,sizeof(CStr),"vbcell",VBCell_Destroyer,VBCell_Cpyer);
                Variable_SetTo(a_var,(CStr[]){ CStr_Format("%d,%d",pos.x,pos.y) });
            }else{
                Scope_BuildInitVariableRange(
                    &e->vbl.ev.sc,
                    a->str,
                    "vbcell",
                    e->vbl.ev.sc.range - 1,
                    (CStr[]){ CStr_Format("%d,%d",pos.x,pos.y) }
                );
            }
        }else
            Interpreter_ErrorHandler(&e->vbl.ev,"[VBCell_VBCell_Ass] Cell %d,%d is invalid!\n",pos.x,pos.y);
    }else{
        Interpreter_ErrorHandler(&e->vbl.ev,"[VBCell_VBCell_Ass] Operator %s undefined for: %s and %s!\n",op->str,a->str,b->str);
    }

    return Token_Cpy(a);
}
Token VBCell_VBCell_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ADD: %s + %s\n",a->str,b->str);

    ExcelCell* ec = Excel_VBCell_Get(e,a);

    if(ec){
        if(CStr_Cmp(ec->type,"int")){
            return Token_Move(TOKEN_NUMBER,I64_Get_D(Excel_Int_Get(e,a) + Excel_Int_Get(e,b)));
        }else if(CStr_Cmp(ec->type,"float")){
            return Token_Move(TOKEN_FLOAT,F64_Get_Dc(Excel_Float_Get(e,a) + Excel_Float_Get(e,b)));
        }else if(CStr_Cmp(ec->type,"str")){
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,CStr_Concat(Excel_Str_Get(e,a),Excel_Str_Get(e,b)));
        }else if(!ec->type){
            return Token_Move(TOKEN_CONSTSTRING_DOUBLE,CStr_Concat(Excel_Str_Get(e,a),Excel_Str_Get(e,b)));
        }
    }

    Interpreter_ErrorHandler(&e->vbl.ev,"[VBCell_VBCell_Add] Operator %s undefined for: %s and %s!\n",op->str,a->str,b->str);
    return Token_Null();
}

Token VBCell_Int_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASS: %s = %s\n",a->str,b->str);

    //const Vic2 pos = VBLike_ExtractCoords(&e->vbl,a);
    ExcelCell* vbcell_a = Excel_VBCell_Get(e,a);

    if(vbcell_a->output) free(vbcell_a->output);
    CStr_Set(&vbcell_a->type,"int");
    vbcell_a->output = malloc(sizeof(Number));
    *(Number*)vbcell_a->output = Excel_Int_Get(e,b);

    return Token_Cpy(a);
}
Token VBCell_Int_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ADD: %s + %s\n",a->str,b->str);
    
    return Token_Move(TOKEN_NUMBER,I64_Get_D(Excel_Int_Get(e,a) + Excel_Int_Get(e,b)));
}

Token VBCell_Float_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASS: %s = %s\n",a->str,b->str);

    //const Vic2 pos = VBLike_ExtractCoords(&e->vbl,a);
    ExcelCell* vbcell_a = Excel_VBCell_Get(e,a);

    if(vbcell_a->output) free(vbcell_a->output);
    CStr_Set(&vbcell_a->type,"float");
    vbcell_a->output = malloc(sizeof(Double));
    *(Double*)vbcell_a->output = Excel_Float_Get(e,b);

    return Token_Cpy(a);
}
Token VBCell_Float_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ADD: %s + %s\n",a->str,b->str);
    
    return Token_Move(TOKEN_FLOAT,F64_Get_Dc(Excel_Float_Get(e,a) + Excel_Float_Get(e,b)));
}

Token VBCell_Str_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASS: %s = %s\n",a->str,b->str);

    //const Vic2 pos = VBLike_ExtractCoords(&e->vbl,a);
    ExcelCell* vbcell_a = Excel_VBCell_Get(e,a);

    if(vbcell_a->output) free(vbcell_a->output);
    CStr_Set(&vbcell_a->type,"str");
    
    vbcell_a->output = malloc(sizeof(CStr));
    *(CStr*)vbcell_a->output = CStr_Cpy(Excel_Str_Get(e,b));

    return Token_Cpy(a);
}
Token VBCell_Str_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ADD: %s + %s\n",a->str,b->str);
    
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,CStr_Concat(Excel_Str_Get(e,a),Excel_Str_Get(e,b)));
}

Token VBCell_Bool_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASS: %s = %s\n",a->str,b->str);

    ExcelCell* vbcell_a = Excel_VBCell_Get(e,a);

    if(vbcell_a->output) free(vbcell_a->output);
    CStr_Set(&vbcell_a->type,"bool");
    vbcell_a->output = malloc(sizeof(Boolean));
    *(Boolean*)vbcell_a->output = Excel_Bool_Get(e,b);

    return Token_Cpy(a);
}

Token VBCell_Handler_Cast(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //ExcelCell* ec = Excel_VBCell_Get(e,a);
    const Vic2 pos = VBLike_ExtractCoords(&e->vbl,a);
    
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,CStr_Format("$(%d,%d)",pos.x,pos.y));
}

Variable VBCell_CellGet(Excel* e,CStr name,Variable* args){
    Variable* a = &args[0];
    Variable* b = &args[1];
    
    return Variable_Make(
        "OUT",
        "vbcell",
        (CStr[]){ CStr_Format("%ld,%ld",*(Number*)a->data,*(Number*)b->data) },
        sizeof(CStr),
        0,
        NULL,
        NULL
    );
}
Variable VBCell_CellSet(Excel* e,CStr name,Variable* args){
    Variable* a = &args[0];
    Variable* b = &args[1];
    Variable* c = &args[2];

    Token tok = Token_Move(TOKEN_VBLIKE_CELL,CStr_Format("%d,%d",*(Number*)a->data,*(Number*)b->data));
    ExcelCell* ec = Excel_VBCell_Get(e,&tok);
    Token_Free(&tok);

    if(CStr_Cmp(c->typename,"int")){
        if(ec->output) free(ec->output);
        CStr_Set(&ec->type,"int");
        ec->output = malloc(sizeof(Number));
        *(Number*)ec->output = Excel_Int_Get_V(e,c);
    }else if(CStr_Cmp(c->typename,"float")){
        if(ec->output) free(ec->output);
        CStr_Set(&ec->type,"float");
        ec->output = malloc(sizeof(Double));
        *(Double*)ec->output = Excel_Float_Get_V(e,c);
    }else if(CStr_Cmp(c->typename,"bool")){
        if(ec->output) free(ec->output);
        CStr_Set(&ec->type,"bool");
        ec->output = malloc(sizeof(Boolean));
        *(Boolean*)ec->output = Excel_Bool_Get_V(e,c);
    }else if(CStr_Cmp(c->typename,"str")){
        if(ec->output) free(ec->output);
        CStr_Set(&ec->type,"str");
        ec->output = malloc(sizeof(CStr));
        *(CStr*)ec->output = CStr_Cpy(Excel_Str_Get_V(e,c));
    }else if(CStr_Cmp(c->typename,"func")){
        if(ec->output) free(ec->output);
        CStr_Set(&ec->type,"func");
        ec->output = malloc(sizeof(CStr));
        *(CStr*)ec->output = CStr_Cpy(Excel_Func_Get_V(e,c));
    }else{
        if(ec->output) free(ec->output);
        CStr_Free(&ec->type);
        ec->type = NULL;
        
        ExcelCell* ec_b = Excel_VBCell_Get_V(e,c);
        ec->output = CStr_Cpy((CStr)ec_b->output);
    }
    
    return Variable_Null();
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("vbcell",sizeof(CStr),OperatorInterationMap_Make((OperatorInterater[]){
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
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))VBCell_Int_Handler_Add),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "float",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_Float_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))VBCell_Float_Handler_Add),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "str",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_Str_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))VBCell_Str_Handler_Add),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "bool",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_Bool_Handler_Ass),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),NULL,NULL)
    );
    ExternFunctionMap_PushContained_C(Extern_Functions,funcs,(ExternFunction[]){
        ExternFunction_New("get","vbcell",(Member[]){ 
            Member_New("int","x"),
            Member_New("int","y"),
            MEMBER_END
        },(void*)VBCell_CellGet),
        ExternFunction_New("set",NULL,(Member[]){ 
            Member_New("int","x"),
            Member_New("int","y"),
            Member_New(NULL,"content"),
            MEMBER_END
        },(void*)VBCell_CellSet),
        ExternFunction_Null()
    });
}
