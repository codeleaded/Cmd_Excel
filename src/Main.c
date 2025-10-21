#if defined(__linux__) && !defined(_WIN32)
    #include "/home/codeleaded/System/Static/Library/Excel.h"
#elif defined(_WIN32) || defined(_WIN64)
    #include "/home/codeleaded/System/Static/Library/Excel.h"
#elif defined(__APPLE__)
    #error "Apple not supported!"
#else
    #error "Platform not supported!"
#endif


int main(int argc,const char *argv[]){
    
    //Excel e = Excel_New("./bin",10,10);
    Excel e = Excel_Load("./bin","./data/SpreadSheet0.vblss");

    // Excel_Set(&e,0,0,"=1");
    // Excel_Set(&e,1,0,"=2");

    // Excel_Set(&e,2,0,"=1+2");
    // Excel_Set(&e,3,0,"=3+4");
    // Excel_Set(&e,4,0,"=$(0,0)+$(1,0)+$(2,0)+$(3,0)");

    //Excel_Script(&e.vbl.ev,"./code/Main.vbl");
    
    //Variable ret = Excel_Function(&e.vbl.ev,"main",(Variable[]){
    //    Variable_Null()
    //});
    //Variable_Free(&ret);

    Excel_Print(&e);
    Excel_Print_O(&e);

    //Excel_Save(&e,"./data/SpreadSheet0.vblss");

    Excel_Free(&e);

    return 0;
}