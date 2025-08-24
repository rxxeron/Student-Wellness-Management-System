#include "common.h"


// Admin Function
int login_admin() {             // Admin login
    clrscrn();
    const char admin_user[] = "admin_swms";
    const char admin_pass[] = "swmsewu2025";

    char input_user[max_un], input_pass[max_pass];

    printf("\n\t\t\t\t--------------- Admin Login --------------\n");
    printf("Username: ");
    scanf("%29s", input_user);
    while(getchar()!='\n');
    printf("Password: ");
    scanf("%29s", input_pass);
    while(getchar()!='\n');

    if(strcmp(input_user, admin_user) == 0 && strcmp(input_pass, admin_pass) == 0){
        printf("Admin login successful!\n");
        return 1;
    } else {
        printf("Login failed! Check username and password.\n");
        return 0;
    }
}
void adminaddfac() {                    // Admin add faculty

    clrscrn();
    FILE *fp = fopen(teachfile, "a");
    if (!fp) { printf("Error opening faculty file.\n"); return; }

    char username[max_un], password[max_pass], course[max_course], section[max_sec];

    printf("\n\t\t\t\t--------------- Add New Faculty ---------------\n");
    printf("Username: "); scanf("%29s", username);
    printf("Password: "); scanf("%29s", password);
    getchar(); // clear newline
    printf("Course: "); fgets(course, sizeof(course), stdin);
    course[strcspn(course, "\n")] = '\0';
    printf("Section: "); fgets(section, sizeof(section), stdin);
    section[strcspn(section, "\n")] = '\0';

    fprintf(fp, "%s %s %s %s\n", username, password, course, section);
    fclose(fp);
    printf("Faculty added successfully!\n");
    paucon();
}
void adminaddconst() {                  // Admin add consultant
    FILE *fp = fopen(consultfile, "a");
    if (!fp) { printf("Error opening consultant file.\n"); return; }

    char username[max_un], password[max_pass];

    printf("\n\t\t\t\t--------------- Add New Consultant ---------------\n");
    printf("Username: "); scanf("%29s", username);
    printf("Password: "); scanf("%29s", password);

    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);
    printf("Consultant added successfully!\n");
    paucon();
}
void admin_menu() {                     // Admin menu
    clrscrn();
    int choice;
    do {
        printf("\n\t\t\t\t--------------- Admin Menu ---------------\n");
        printf("1. View registered students\n");
        printf("2. View registered faculty\n");
        printf("3. View registered consultants\n");
        printf("4. Add faculty\n");
        printf("5. Add consultant\n");
        printf("6. Logout\n");
        printf("Choice: ");
        if(scanf("%d", &choice) != 1) { while(getchar()!='\n'); printf("Invalid input!\n"); continue; }

        switch(choice){
            case 1: {
                clrscrn();
                FILE *fp = fopen(stdfile,"r");
                if(!fp){ printf("No students registered.\n"); break; }
                char line[200];
                printf("\n\t\t\t\t--------------- Registered Students ---------------\n");
                while(fgets(line,sizeof(line),fp)) printf("%s",line);
                fclose(fp);
                paucon();
                break;
            }
            case 2: {
                clrscrn();
                FILE *fp = fopen(teachfile,"r");
                if(!fp){ printf("No faculty registered.\n"); break; }
                char line[200];
                printf("\n\t\t\t\t--------------- Registered Faculty ---------------\n");
                while(fgets(line,sizeof(line),fp)) printf("%s",line);
                fclose(fp);
                paucon();
                break;
            }
            case 3: {
                clrscrn();
                FILE *fp = fopen(consultfile,"r");
                if(!fp){ printf("No consultants registered.\n"); break; }
                char line[200];
                printf("\n\t\t\t\t--------------- Registered Consultants ---------------\n");
                while(fgets(line,sizeof(line),fp)) printf("%s",line);
                fclose(fp);
                paucon();
                break;
            }
            case 4: adminaddfac(); break;
            case 5: adminaddconst(); break;
            case 6: printf("Logging out...\n"); clrscrn(); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 6);
}
