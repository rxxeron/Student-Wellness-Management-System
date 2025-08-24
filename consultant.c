#include "common.h"

int cpfm() {                                             // Post menu function
    int choice;
    while(1) {
        printf("\n1. Back to Consultant Menu\n2. Logout\nChoice: ");
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        if(choice==1)  return 1; clrscrn(); // Back to menu
        if(choice==2) return 0; clrscrn(); // Logout
        printf("Invalid choice!\n");
    }
}
// Consultant Function
int logconst(char id[], Consultant *c){                     // Consultant Login
    clrscrn();
    FILE *file=fopen(consultfile,"r");
    if(!file){ printf("No consultants registered.\n"); return 0; }

    char username[max_un], pass[max_pass], pass_input[max_pass];
    printf("\n\t\t\t\t--------------- Consultant Login ---------------\nUsername: "); scanf("%29s",id);
    printf("Password: "); scanf("%29s",pass_input);

    int success=0;
    while(fscanf(file,"%29s %29s",username,pass)==2){
        if(strcmp(id,username)==0 && strcmp(pass_input,pass)==0){
            strcpy(c->username,username);  strcpy(c->pass,pass); success=1; break;
        }
    }
    fclose(file);
    if(!success) printf("Login failed!\n");
    return success;
}
int consvrecomm(Consultant c) {                            // Consultant view recommendation
    clrscrn();
    FILE *fp = fopen(recommendfile, "r");
    if (!fp) { printf("No recommendations found.\n"); return cpfm(); }

    Recommendation recs[100]; int count=0; char line[200];
    while(fgets(line,sizeof(line),fp)) {
        sscanf(line,"%s %s %d %s %s", recs[count].std_id, recs[count].faculty_username, &recs[count].mood_lvl, recs[count].type, recs[count].status);
        if(strcmp(recs[count].status,"pending")==0) count++;
    }
    fclose(fp);

    if(count==0){ printf("No pending recommendations.\n"); return cpfm(); }

    printf("\n\t\t\t\t--------------- Pending Recommendations ---------------\n");
    for(int i=0;i<count;i++)
        printf("%d. Student: %s | Recommended by: %s | Mood: %d | Type: %s\n",
               i+1, recs[i].std_id, recs[i].faculty_username, recs[i].mood_lvl, recs[i].type);

    int sel;
    printf("Select a recommendation to schedule appointment (0 to skip): ");
    if(scanf("%d",&sel)==1 && sel>0 && sel<=count) {
        char date[20], time[20];
        printf("Enter appointment date (YYYY-MM-DD): "); scanf("%s",date);
        printf("Enter appointment time (HH:MM): "); scanf("%s",time);

        long new_id=1;
        FILE *fa_check=fopen(apptfile,"r");
        if(fa_check){
            Appointment temp; long max_id=0;
            while(fscanf(fa_check,"%ld %s %s %s %s %s %s",
                         &temp.id,temp.student_id,temp.consultant_un,temp.faculty_un,
                         temp.date,temp.time,temp.status)==7)
                if(temp.id>max_id) max_id=temp.id;
            fclose(fa_check); new_id=max_id+1;
        }

        FILE *fa=fopen(apptfile,"a");
        if(fa){
            fprintf(fa,"%ld %s %s %s %s %s pending\n",new_id,recs[sel-1].std_id,c.username,recs[sel-1].faculty_username,date,time);
            fclose(fa);

            strcpy(recs[sel-1].status,"scheduled");
            fp=fopen(recommendfile,"w");
            for(int i=0;i<count;i++)
                fprintf(fp,"%s %s %d %s %s\n", recs[i].std_id, recs[i].faculty_username,
                        recs[i].mood_lvl, recs[i].type, recs[i].status);
            fclose(fp);

            printf("Appointment scheduled successfully for student %s with ID %ld.\n", recs[sel-1].std_id, new_id);
        }
    }
return cpfm();
}
int consvsche(Consultant c) {                             // Consultant view schedule
    clrscrn();
    FILE *fp=fopen(apptfile,"r");
    if(!fp){ printf("No appointments.\n"); return cpfm(); }

    Appointment apps[100]; int count=0;
    while(fscanf(fp,"%ld %s %s %s %s %s %s",
                 &apps[count].id, apps[count].student_id, apps[count].consultant_un,
                 apps[count].faculty_un, apps[count].date, apps[count].time, apps[count].status)==7){
        if(strcmp(apps[count].consultant_un,c.username)==0 && strcmp(apps[count].status,"accepted")==0)
            count++;
    }
    fclose(fp);

    if(count==0) printf("No scheduled appointments.\n");
    else{
        printf("\n\t\t\t\t--------------- Scheduled Appointments ---------------\nID | Student | Date | Time | Status\n--------------------------------\n");
        for(int i=0;i<count;i++)
            printf("%ld | %s | %s | %s | %s\n", apps[i].id, apps[i].student_id, apps[i].date, apps[i].time, apps[i].status);
    }
return cpfm();
}
int consvreq(Consultant c){                              // Consultant view requests
    clrscrn();
    FILE *fp=fopen(apptfile,"r");
    Appointment apps[100];
    int count=0;

    if(fp){
        while(fscanf(fp,"%ld %s %s %s %s %s %s",
                     &apps[count].id, apps[count].student_id, apps[count].consultant_un,
                     apps[count].faculty_un, apps[count].date, apps[count].time, apps[count].status) == 7){
            if(strcmp(apps[count].consultant_un,c.username)==0 && strcmp(apps[count].status,"pending")==0)
                count++;
        }
        fclose(fp);
    }

    if(count==0){
        printf("No pending appointment requests.\n");
        return cpfm();
    }

    printf("\n\t\t\t\t--------------- Pending Appointments ---------------\n");
    for(int i=0;i<count;i++)
        printf("%d. ID: %ld | Student: %s | Date: %s | Time: %s\n",
               i+1, apps[i].id, apps[i].student_id, apps[i].date, apps[i].time);

    int choice;
    printf("Enter number to accept appointment (0 to skip): ");
    if(scanf("%d",&choice)==1 && choice>0 && choice<=count){
        strcpy(apps[choice-1].status,"accepted");

        FILE *fa=fopen(apptfile,"w");
        if(fa){
            for(int i=0;i<count;i++)
                fprintf(fa,"%ld %s %s %s %s %s %s\n", apps[i].id, apps[i].student_id, apps[i].consultant_un, apps[i].faculty_un, apps[i].date, apps[i].time, apps[i].status);
            fclose(fa);
            printf("Appointment accepted successfully!\n");
        }
    }

return cpfm();
}
void constmenu(Consultant c){                           // Consultant's Menu
    int cont=1;
    while(cont){
        clrscrn();
        int choice;
        printf("\n\t\t\t\t--------------- Consultant Menu ------------------\n");
        printf("1. View Recommendations\n2. View Scheduled Appointments\n3. View Requests\n4. Logout\nChoice: ");
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); continue; }

        switch(choice){
            case 1: cont = consvrecomm(c); break;
            case 2: cont = consvsche(c); break;
            case 3: cont = consvreq(c); break;
            case 4: cont = 0; break;  // logout
            default: printf("Invalid choice!\n"); break;
        }
    }

}
