#include "common.h"


int main(){
    srand(time(NULL));
    int choice;
    char id[max_un];

    while(1){
        
        void clrscrn();

        printf("\n\t\t\t\t--------------- Student Wellness Management System ---------------\n");
        printf("1. Student Register\n2. Student Login\n3. Faculty Login\n4. Consultant Login\n5. Admin Login\n6. Exit\nChoice: ");
        if(scanf("%d",&choice)!=1){
            while(getchar()!='\n');
            printf("Invalid input!\n");
            continue;
        }
        switch(choice){
            case 1: regstd(); break;
            case 2: if(logstd(id)) stdmenu(id); break;
            case 3: {   Teacher t; if(logfac(id,&t)) teachmenu(t); break; }
            case 4: {   Consultant c; if(logconst(id,&c)) constmenu(c); break; }
            case 5: if(login_admin()) admin_menu(); break;
            case 6: printf("Goodbye!\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}
