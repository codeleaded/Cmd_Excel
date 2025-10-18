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
    
    Excel e = Excel_New(10,10);

    Excel_Set(&e,0,0,"=1+2");
    Excel_Set(&e,1,0,"=3+4");

    Excel_Set(&e,2,0,"={0,0}+{1,0}");

    Excel_Free(&e);

    return 0;
}