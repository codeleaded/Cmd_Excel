#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/Excel.h"

/*
Token VBCell_Any_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASS: %s = %s\n",a->str,b->str);

    ExcelCell* vbcell_a = Excel_VBCell_Get(e,a);

    if(b->tt == TOKEN_VBLIKE_CELL){
        ExcelCell* vbcell_b = Excel_VBCell_Get(e,b);
        if(vbcell_b){
            vbcell_a->type = CStr_Cpy(vbcell_b);

            const int size = TypeMap_SizeOf(&e->vbl.ev.sc.types,vbcell_b->type);
            vbcell_a->output = malloc(size);
            memcpy(vbcell_a->output,vbcell_b->output,size);
        }
    }else{
        if(b->tt == TOKEN_STRING){
            const Vic2 pos = VBLike_ExtractCoords(a->str);
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
            *(I64*)vbcell_a->output = Excel_Int_Get(e,b);
            CStr_Set(&vbcell_a->type,"int");
        }else if(b->tt == TOKEN_FLOAT){
            vbcell_a->output = malloc(sizeof(F64));
            *(F64*)vbcell_a->output = Excel_Float_Get(e,b);
            CStr_Set(&vbcell_a->type,"float");
        }else if(b->tt == TOKEN_BOOL){
            vbcell_a->output = malloc(sizeof(Boolean));
            *(Boolean*)vbcell_a->output = Excel_Bool_Get(e,b);
            CStr_Set(&vbcell_a->type,"bool");
        }else if(b->tt == TOKEN_CONSTSTRING_DOUBLE){
            vbcell_a->output = malloc(sizeof(CStr));
            *(CStr*)vbcell_a->output = CStr_Cpy(Excel_Str_Get(e,b));
            CStr_Set(&vbcell_a->type,"str");
        }else if(b->tt == TOKEN_FUNCTIONPOINTER){
            vbcell_a->output = malloc(sizeof(CStr));
            *(CStr*)vbcell_a->output = CStr_Cpy(Excel_Str_Get(e,b));
            CStr_Set(&vbcell_a->type,"func");
        }
    }

    return Token_Cpy(a);
}
Token VBCell_Any_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ADD: %s + %s\n",a->str,b->str);

    ExcelCell* vbcell_a = Excel_VBCell_Get(e,a);

    if(b->tt == TOKEN_VBLIKE_CELL){
        ExcelCell* vbcell_b = Excel_VBCell_Get(e,b);
        if(vbcell_b){
            vbcell_a->output,vbcell_b->output
        }
    }else{
        if(b->tt == TOKEN_STRING){
            const Vic2 pos = VBLike_ExtractCoords(a->str);
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
            *(I64*)vbcell_a->output = Excel_Int_Get(e,b);
            CStr_Set(&vbcell_a->type,"int");
        }else if(b->tt == TOKEN_FLOAT){
            vbcell_a->output = malloc(sizeof(F64));
            *(F64*)vbcell_a->output = Excel_Float_Get(e,b);
            CStr_Set(&vbcell_a->type,"float");
        }else if(b->tt == TOKEN_BOOL){
            vbcell_a->output = malloc(sizeof(Boolean));
            *(Boolean*)vbcell_a->output = Excel_Bool_Get(e,b);
            CStr_Set(&vbcell_a->type,"bool");
        }else if(b->tt == TOKEN_CONSTSTRING_DOUBLE){
            vbcell_a->output = malloc(sizeof(CStr));
            *(CStr*)vbcell_a->output = CStr_Cpy(Excel_Str_Get(e,b));
            CStr_Set(&vbcell_a->type,"str");
        }else if(b->tt == TOKEN_FUNCTIONPOINTER){
            vbcell_a->output = malloc(sizeof(CStr));
            *(CStr*)vbcell_a->output = CStr_Cpy(Excel_Str_Get(e,b));
            CStr_Set(&vbcell_a->type,"func");
        }
    }

    return Token_Move(TOKEN_NUMBER,NULL);
}
*/

Token VBCell_VBCell_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASS: %s = %s\n",a->str,b->str);

    ExcelCell* ec = Excel_VBCell_Get(e,a);

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

    return Token_Cpy(a);
}
Token VBCell_VBCell_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ADD: %s + %s\n",a->str,b->str);

    ExcelCell* vbcell_a = Excel_VBCell_Get(e,a);



    return Token_Move(TOKEN_NONE,NULL);
}

Token VBCell_Int_Handler_Ass(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ASS: %s = %s\n",a->str,b->str);

    const Vic2 pos = VBLike_ExtractCoords(a->str);
    ExcelCell* vbcell_a = Excel_VBCell_Get(e,a);

    if(vbcell_a->output) free(vbcell_a->output);
    CStr_Set(&vbcell_a->type,"int");
    vbcell_a->output = malloc(sizeof(Number));
    *(Number*)vbcell_a->output = Excel_Int_Get(e,b);

    return Token_Move(TOKEN_NONE,NULL);
}
Token VBCell_Int_Handler_Add(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ADD: %s + %s\n",a->str,b->str);
    
    return Token_Move(TOKEN_NUMBER,I64_Get_D(Excel_Int_Get(e,a) + Excel_Int_Get(e,b)));
}

Token VBCell_Handler_Cast(Excel* e,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //ExcelCell* ec = Excel_VBCell_Get(e,a);
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
                OperatorDefiner_New(TOKEN_VBLIKE_ASS,(Token(*)(void*,Token*,Vector*))VBCell_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_VBLIKE_ADD,(Token(*)(void*,Token*,Vector*))VBCell_Int_Handler_Add),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),NULL,NULL)
    );
}
