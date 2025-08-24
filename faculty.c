#include "common.h"


int fpfm() {                                             // Post menu function
    int choice;
    while(1) {
        printf("\n1. Back to Faculty Menu\n2. Logout\nChoice: ");
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        if(choice==1)  return 1; clrscrn(); // Back to menu
        if(choice==2) return 0; clrscrn(); // Logout
        printf("Invalid choice!\n");
    }
}
// Faculty functions
int logfac(char id[], Teacher *t) {                          // Faculty login
    clrscrn();
    FILE *file = fopen(teachfile, "r");
    if (!file) {
        printf("No registered faculty.\n");    return 0;
    }

    char username[max_un], pass[max_pass], course[max_course], sec[max_sec];
    char input_pass[max_pass];

    printf("\n\t\t\t\t--------------- Faculty Login ---------------\nUsername: ");    scanf("%29s", id);
    printf("Password: ");  scanf("%29s", input_pass);
    int success = 0;
    while(fscanf(file, "%29s %29s %19s %15s", username, pass, course, sec) == 4){
        if(strcmp(id, username) == 0 && strcmp(input_pass, pass) == 0){
            success = 1;
            strcpy(t->username, username); strcpy(t->pass, pass); strcpy(t->course, course); strcpy(t->sec, sec); break;
        }
    }

    fclose(file);
    if (!success) {
        printf("Login failed! Check username and password.\n"); return 0;
    }
    return 1;
}
int fvam(Teacher t){                                        // Faculty view average mood
    clrscrn();
    FILE *fp = fopen(moodentryfile, "r");
    if(!fp){ printf("No mood entries found.\n"); return fpfm(); }

    char line[300];
    int total_mood = 0, count = 0;

    while(fgets(line,sizeof(line),fp)){
        MoodEntry e;
        sscanf(line, "%s %s %d %[^\n]", e.std_id, e.date, &e.mood, e.note);

        FILE *sf = fopen(stdfile,"r");
        if(!sf) continue;

        char std_id[max_un], pass[max_pass], course[max_course], sec[max_sec];
        int found = 0;
        while(fscanf(sf,"%s %s %s %s", std_id, pass, course, sec)==4){
            if(strcmp(std_id,e.std_id)==0 && strcmp(course,t.course)==0 && strcmp(sec,t.sec)==0){
                found=1; break;
            }
        }
        fclose(sf);

        if(found){
            total_mood += e.mood;
            count++;
        }
    }
    fclose(fp);
    if(count==0) printf("No mood entries found for your section.\n");
    else printf("Average mood for %s (%s): %.2f\n", t.course, t.sec, (double)total_mood/count);
return fpfm();
}
int facvulstd(Teacher t){                                  // Faculty view vulnerable students
    clrscrn();
    FILE *fp = fopen(moodentryfile, "r");
    if(!fp){ printf("No mood entries found.\n"); return fpfm(); }

    typedef struct {
        char std_id[max_un];
        int total_mood;
        int entry_count;
    } StudentMood;

    StudentMood sm[100];
    int sm_count = 0;

    char line[300];
    while(fgets(line,sizeof(line),fp)){
        MoodEntry e;
        sscanf(line,"%s %s %d %[^\n]", e.std_id, e.date, &e.mood, e.note);

        FILE *sf = fopen(stdfile,"r");
        if(!sf) continue;

        char std_id[max_un], pass[max_pass], course[max_course], sec[max_sec];
        int found = 0;
        while(fscanf(sf,"%s %s %s %s", std_id, pass, course, sec)==4){
            if(strcmp(std_id,e.std_id)==0 && strcmp(course,t.course)==0 && strcmp(sec,t.sec)==0){
                found = 1; break;
            }
        }
        fclose(sf);

        if(found){
            int idx = -1;
            for(int i=0;i<sm_count;i++) if(strcmp(sm[i].std_id,e.std_id)==0){ idx=i; break; }

            if(idx==-1){
                strcpy(sm[sm_count].std_id,e.std_id);
                sm[sm_count].total_mood = e.mood;
                sm[sm_count].entry_count = 1;
                sm_count++;
            } else {
                sm[idx].total_mood += e.mood;
                sm[idx].entry_count++;
            }
        }
    }
    fclose(fp);

    printf("\n\t\t\t\t--------------- Vulnerable Students (Avg mood < 5) ---------------\n");
    int found_vuln = 0;
    for(int i=0;i<sm_count;i++){
        double avg = (double)sm[i].total_mood / sm[i].entry_count;
        if(avg < 5.0){
            printf("%s | Average Mood: %.2f\n", sm[i].std_id, avg);
            found_vuln = 1;
        }
    }
    if(!found_vuln) printf("No vulnerable students found in your section.\n");
return fpfm();
}
int facrecconst(Teacher t){                               // Faculty recommend to consultant
    clrscrn();
    FILE *fp = fopen(moodentryfile, "r");
    if(!fp){ printf("No mood entries found.\n"); return fpfm(); }

    typedef struct {
        char std_id[max_un];
        int avg_mood;
        int count;
    } StudentMood;

    StudentMood sm[100];
    int sm_count = 0;

    char line[300];
    while(fgets(line,sizeof(line),fp)){
        MoodEntry e;
        sscanf(line,"%s %s %d %[^\n]", e.std_id, e.date, &e.mood, e.note);

        FILE *sf = fopen(stdfile,"r");
        if(!sf) continue;

        char std_id[max_un], pass[max_pass], course[max_course], sec[max_sec];
        int found = 0;
        while(fscanf(sf,"%s %s %s %s", std_id, pass, course, sec)==4){
            if(strcmp(std_id,e.std_id)==0 && strcmp(course,t.course)==0 && strcmp(sec,t.sec)==0){
                found = 1; break;
            }
        }
        fclose(sf);

        if(found){
            int idx = -1;
            for(int i=0;i<sm_count;i++) if(strcmp(sm[i].std_id,e.std_id)==0){ idx=i; break; }

            if(idx==-1){
                strcpy(sm[sm_count].std_id,e.std_id);
                sm[sm_count].avg_mood = e.mood;
                sm[sm_count].count = 1;
                sm_count++;
            } else {
                sm[idx].avg_mood += e.mood;
                sm[idx].count++;
            }
        }
    }
    fclose(fp);

    if(sm_count==0){ printf("No students found in your section.\n"); return fpfm(); }

    printf("\n\t\t\t\t--------------- Students in Your Section ---------------\n");
    for(int i=0;i<sm_count;i++){
        double avg = (double)sm[i].avg_mood / sm[i].count;
        printf("%d. %s | Average Mood: %.2f\n", i+1, sm[i].std_id, avg);
    }

    int choice;
    printf("\nEnter student number to recommend for consultation (0 to skip): ");
    if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); return fpfm(); }
    if(choice!=0){
        if(choice<1 || choice>sm_count){ printf("Invalid selection!\n"); return fpfm(); }

        Recommendation rec;
        strcpy(rec.std_id, sm[choice-1].std_id);
        strcpy(rec.faculty_username, t.username);
        rec.mood_lvl = sm[choice-1].avg_mood / sm[choice-1].count;
        strcpy(rec.type, "consult");
        strcpy(rec.status, "pending");

        FILE *rf = fopen(recommendfile, "a");
        if(!rf){ printf("Error saving recommendation.\n"); return fpfm(); }
        fprintf(rf, "%s %s %d %s %s\n", rec.std_id, rec.faculty_username, rec.mood_lvl, rec.type, rec.status);
        fclose(rf);

        printf("Recommendation submitted successfully for student %s.\n", rec.std_id);
    }
return fpfm();
}
int facvrecommsta(Teacher t){                            // Faculty view his/her recommendation status
    clrscrn();
    FILE *fp = fopen(recommendfile, "r");
    if(!fp){ printf("No recommendations found.\n"); return fpfm(); }

    Recommendation recs[100];
    int count = 0;

    while(fscanf(fp,"%s %s %d %s %s", recs[count].std_id, recs[count].faculty_username, &recs[count].mood_lvl, recs[count].type, recs[count].status) == 5){
        if(strcmp(recs[count].faculty_username, t.username) == 0) count++;
    }
    fclose(fp);

    if(count == 0){ printf("No recommendations made by you.\n"); return fpfm(); }

    printf("\n\t\t\t\t--------------- Your Recommendations Progress ---------------\n");
    printf("Student | Mood | Type | Status\n");
    printf("-------------------------------\n");
    for(int i = 0; i < count; i++)
        printf("%s | %d | %s | %s\n",recs[i].std_id,recs[i].mood_lvl,recs[i].type,recs[i].status);
return fpfm();
}
void teachmenu(Teacher t){                              // Teacher Menu
    int choice;
    while(1){
        clrscrn();
        printf("\n\t\t\t\t--------------- Faculty Menu ---------------\n1. View Average Mood of Section\n2. View Vulnerable Students\n3. Recommend Student for Consultation\n4. View Recommendation Status\n5. Logout\nChoice: ");
        if(scanf("%d",&choice)!=1){
            while(getchar()!='\n');
            printf("Invalid input!\n");
            continue;
        }

        int res = 1; // default = back to menu
        switch(choice){
            case 1: res = fvam(t); break;
            case 2: res = facvulstd(t); break;
            case 3: res = facrecconst(t); break;
            case 4: res = facvrecommsta(t); break;
            case 5: res = 0; clrscrn(); break;  // logout chosen from menu
            default: printf("Invalid choice!\n"); break;
        }

        if(res == 0) break; // logout chosen either from a function or menu
    }

}
