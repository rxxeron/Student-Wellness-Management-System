#include "common.h"

void clrscrn(){         // Clears terminal after menu switch
    system(clrcmd);
}
void paucon() {         // Pause after function finish work
    printf("\nPress Enter to continue...");
    while(getchar()!='\n');
    getchar();
    clrscrn();
}

// Student Function
void regstd() {                                            // Student Registration
    clrscrn();
    FILE *file = fopen(stdfile, "a");
    if(!file){ printf("Error! Contact admin.\n"); return; }

    Student s;
    int n_courses;

    printf("\n\t\t\t\t--------------- Student Registration ---------------\n");
    printf("Student ID (xxxx-x-xx-xxx): ");
    scanf("%29s", s.std_id);

    // Check if ID already exists
    {
        FILE *check = fopen(stdfile, "r");
        if(check) {
            char id[30], pass[30], course[50], sec[10];
            while(fscanf(check, "%29s %29s %49s %9s", id, pass, course, sec) == 4) {
                if(strcmp(id, s.std_id) == 0) {
                    printf("Error: Student ID already exists!\n");
                    fclose(check);
                    fclose(file);
                    paucon();
                    return;
                }
            }
            fclose(check);
        }
    }

    printf("Password: "); scanf("%29s", s.pass);

    while(1) {
        printf("Number of courses to register: ");
        if(scanf("%d", &n_courses)!=1 || n_courses<1) { while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        break;
    }
    getchar();

    for(int i=0;i<n_courses;i++) {
        printf("Course %d: ", i+1);
        fgets(s.course,sizeof(s.course),stdin);
        s.course[strcspn(s.course,"\n")]=0;

        printf("Section %d: ", i+1);
        fgets(s.sec,sizeof(s.sec),stdin);
        s.sec[strcspn(s.sec,"\n")]=0;

        fprintf(file,"%s %s %s %s\n", s.std_id, s.pass, s.course, s.sec);
    }

    fclose(file);
    printf("Registration successful!\n");
    paucon();
}

int logstd(char std_id[]) {                               // Student Login
    clrscrn();
    FILE *file = fopen(stdfile,"r");
    if(!file){ printf("No registered students.\n"); return 0; }

    char file_id[max_un], file_pass[max_pass], file_course[max_course], file_sec[max_sec];
    char pass[max_pass];
    printf("\n\t\t\t\t--------------- Student Login ---------------\n");
    printf("Student ID: "); scanf("%29s", std_id);
    printf("Password: "); scanf("%29s", pass);

    int success = 0;
    while(fscanf(file,"%29s %29s %19s %15s", file_id, file_pass, file_course, file_sec)==4) {
        if(strcmp(std_id,file_id)==0 && strcmp(pass,file_pass)==0) { success=1; break; }
    }
    fclose(file);

    if(!success) printf("Login failed! Check ID and password.\n");
    return success;
}
int spfm() {                                             // Post menu function
    int choice;
    while(1) {
        printf("\n1. Back to Student Menu\n2. Logout\nChoice: ");
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        if(choice==1) return 1;  // Back to menu
        if(choice==2) return 0;  // Logout
        printf("Invalid choice!\n");
    }
}
int ame(char std_id[]) {                                // Student mood entry
    clrscrn();
    FILE *file = fopen(moodentryfile,"a");
    if(!file){ printf("Error! Contact admin.\n"); return 1; }

    int mood;    char notes[max_note], date[11];
    time_t t = time(NULL);    struct tm tm = *localtime(&t);
    sprintf(date,"%04d-%02d-%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday);
    printf("\n\t\t\t\t--------------- Add Mood Entry for %s (%s) ---------------\n", std_id, date);
    const char *mood_labels[] = {
        "1.Extremely Sad","2.Sad","3.Down","4.Low","5.Neutral","6.Okay","7.Good","8.Happy","9.Very Happy","10.Excited/Excellent" };
    for(int i=0;i<10;i++) printf("%s\n", mood_labels[i]);

    while(1) {
        printf("Enter mood (1-10): ");
        if(scanf("%d",&mood)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        if(mood<1 || mood>10){ printf("Invalid mood! Try again.\n"); continue; }        break;
    }

    getchar();    printf("Enter notes (Press Enter to skip): ");
    fgets(notes,sizeof(notes),stdin); if(notes[strlen(notes)-1]=='\n') notes[strlen(notes)-1]=0;

    fprintf(file,"%s %s %d %s\n", std_id, date, mood, notes);   fclose(file);
    printf("Mood entry saved successfully.\n");

    if(mood<5) {
        printf("Low mood detected. Automatically recommended for consultation.\n");
        Recommendation rec;
        strcpy(rec.std_id,std_id); strcpy(rec.faculty_username,"auto"); rec.mood_lvl=mood; strcpy(rec.type,"consult"); strcpy(rec.status,"pending");

        FILE *rfile=fopen(recommendfile,"a");
        if(!rfile){ printf("Error saving recommendation.\n"); return 1; }
        fprintf(rfile,"%s %s %d %s %s\n", rec.std_id, rec.faculty_username, rec.mood_lvl, rec.type, rec.status);
        fclose(rfile);
    }

    return spfm();
}
int view_report(char std_id[]) {                       // View student report
    clrscrn();
    FILE *file = fopen(moodentryfile,"r");
    if(!file){ printf("No mood entries found.\n"); return spfm(); }

    int choice;
    while(1) {
        printf("\nSelect Report:\n\n");
        printf("1. Today's entry\n2. Weekly report\n3. Monthly report\n4. Tri-monthly report\n5. Return to menu\nEnter choice: ");
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        if(choice>=1 && choice<=4) break;        if(choice==5){ fclose(file); return 1; }
        printf("Invalid choice! Try again.\n");
    }

    time_t now = time(NULL);
    struct tm *today = localtime(&now);
    char line[300]; MoodEntry entries[100]; int count=0;

    while(fgets(line,sizeof(line),file)) {
        MoodEntry e; sscanf(line,"%s %s %d %[^\n]", e.std_id, e.date, &e.mood, e.note);
        if(strcmp(e.std_id,std_id)!=0) continue;

        int y,m,d; sscanf(e.date,"%d-%d-%d",&y,&m,&d);
        struct tm entry_tm={0}; entry_tm.tm_year=y-1900; entry_tm.tm_mon=m-1; entry_tm.tm_mday=d;
        time_t entry_time = mktime(&entry_tm);    double diff_days = difftime(now,entry_time)/(60*60*24);

        int show=0;
        switch(choice) {
            case 1: if(y==today->tm_year+1900 && m==today->tm_mon+1 && d==today->tm_mday) show=1; break;
            case 2: if(diff_days<=7) show=1; break;
            case 3: if(diff_days<=30) show=1; break;
            case 4: if(diff_days<=90) show=1; break;
        }
        if(show) entries[count++]=e;
    }
    fclose(file);

    if(count==0){ printf("No entries found for this period.\n"); return spfm(); }

    printf("\nDate       | Mood | Note\n-----------------------------\n");
    int mood_sum=0;
    for(int i=0;i<count;i++){ printf("%s | %-4d | %s\n", entries[i].date, entries[i].mood, entries[i].note); mood_sum+=entries[i].mood; }
    printf("\nAverage mood for selected period: %.2f\n",(double)mood_sum/count);

    return spfm();
}
void sba(char student_id[]) {                         // Student books appoint
    clrscrn();
    char consultant[max_un], date[20], time[10];

    printf("\n\t\t\t\t--------------- Book Appointment with Consultant ---------------\n");
    printf("Enter Consultant Username: "); scanf("%s", consultant);
    printf("Enter Appointment Date (YYYY-MM-DD): "); scanf("%s", date);
    printf("Enter Appointment Time (HH:MM): "); scanf("%s", time);

    long new_id = 1;
    FILE *fa_check = fopen(apptfile, "r");
    if(fa_check) {
        Appointment temp; long max_id=0;
        while(fscanf(fa_check,"%ld %s %s %s %s %s %s",  &temp.id, temp.student_id, temp.consultant_un, temp.faculty_un,  temp.date, temp.time, temp.status)==7) {
            if(temp.id>max_id) max_id=temp.id;
        }
        fclose(fa_check);
        new_id=max_id+1;
    }

    FILE *fa = fopen(apptfile,"a");
    if(!fa){ printf("Error saving appointment.\n"); return; }
    fprintf(fa,"%ld %s %s - %s %s pending\n", new_id, student_id, consultant, date, time);
    fclose(fa);

    printf("Appointment request sent successfully!\n");
    return ;
}
int sva(char std_id[]) {                             // Student view appointments
    clrscrn();
    FILE *fp=fopen(apptfile,"r");
    if(!fp){ printf("No appointments found.\n"); return spfm(); }

    Appointment apps[100]; int count=0;
    while(fscanf(fp,"%ld %s %s %s %s %s %s",
        &apps[count].id, apps[count].student_id, apps[count].consultant_un, apps[count].faculty_un, apps[count].date, apps[count].time, apps[count].status)==7)
        count++;
        fclose(fp);

    int shown=0; for(int i=0;i<count;i++) if(strcmp(apps[i].student_id,std_id)==0) shown++;
    if(shown==0){ printf("No appointments assigned.\n"); return spfm(); }

    printf("\n\t\t\t\t--------------- Your Appointments ---------------\n");
    printf("ID | Consultant | Recommended by | Date       | Time  | Status\n");
    printf("---------------------------------------------------------------\n");
    for(int i=0;i<count;i++) if(strcmp(apps[i].student_id,std_id)==0)
        printf("%ld | %s | %s | %s | %s | %s\n", apps[i].id, apps[i].consultant_un, apps[i].faculty_un, apps[i].date, apps[i].time, apps[i].status);

    long choice_id;
    while(1) {
        printf("\nEnter appointment ID to accept/reject (0 to skip): ");
        if(scanf("%ld",&choice_id)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        if(choice_id==0) return spfm();
        int idx=-1;
        for(int i=0;i<count;i++) if(apps[i].id==choice_id && strcmp(apps[i].student_id,std_id)==0){ idx=i; break; }
        if(idx==-1){ printf("Invalid ID! Try again.\n"); continue; }
        int opt;
        while(1) {
            printf("1. Accept\n2. Reject\nChoice: "); if(scanf("%d",&opt)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
            if(opt==1){ strcpy(apps[idx].status,"accepted"); break; }
            if(opt==2){ strcpy(apps[idx].status,"rejected"); break; }
            printf("Invalid choice!\n");
        }

        fp=fopen(apptfile,"w");
        for(int i=0;i<count;i++)
            fprintf(fp,"%ld %s %s %s %s %s %s\n", apps[i].id, apps[i].student_id, apps[i].consultant_un, apps[i].faculty_un, apps[i].date, apps[i].time, apps[i].status);
        fclose(fp);

        printf("Appointment updated successfully.\n");
        break;
    }
    return spfm();
}
int vrecomm(char std_id[]) {                        // Student view recommentdation
    clrscrn();
    FILE *file = fopen(recommendfile, "r");
    if(!file){ printf("No recommendations found.\n"); return spfm(); }

    Recommendation recs[100]; int count=0;    char line[200];
    while(fgets(line,sizeof(line),file)) {
        Recommendation r; sscanf(line,"%s %s %d %s %s", r.std_id, r.faculty_username, &r.mood_lvl, r.type, r.status);
        if(strcmp(r.std_id,std_id)==0 && strcmp(r.status,"pending")==0) recs[count++]=r;
    }
    fclose(file);
    if(count==0){ printf("No pending recommendations.\n"); return spfm(); }
    printf("\n\t\t\t\t --------------- Your Recommendations ---------------\n");
    for(int i=0;i<count;i++)
        printf("%d. From: %s | Mood level: %d | Type: %s | Status: %s\n", i+1, recs[i].faculty_username, recs[i].mood_lvl, recs[i].type, recs[i].status);

    int choice;
    while(1) {
        printf("\nEnter number to act on recommendation (0 to skip): ");
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        if(choice==0) return spfm();
        if(choice<1 || choice>count){ printf("Invalid selection!\n"); continue; }        break;
    }
    Recommendation *r = &recs[choice-1];
    if(strcmp(r->type,"consult")==0) {
        printf("\nBooking appointment for consultation...\n");
        sba(std_id);        strcpy(r->status,"done");

        FILE *fout=fopen(recommendfile,"w");
        if(!fout){ printf("Error updating recommendations.\n"); return spfm(); }
        for(int i=0;i<count;i++)
            fprintf(fout,"%s %s %d %s %s\n", recs[i].std_id, recs[i].faculty_username, recs[i].mood_lvl, recs[i].type, recs[i].status);
        fclose(fout);

        printf("Recommendation processed successfully!\n");
    } else printf("This recommendation cannot be acted on directly.\n");

    return spfm();
}
void stdmenu(char student_id[]) {                  // Student Menu
    int choice, pfm;
    do {
        clrscrn();
        printf("\n\t\t\t\t --------------- Student Menu ---------------\n");
        printf("1. Enter Mood\n2. View Mood History\n3. View Recommendations\n4. Book Appointment with Consultant\n5. View My Appointments\n6. Logout\nChoice: ");
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); choice=0; continue; }

        switch(choice){
            case 1: pfm = ame(student_id); break;
            case 2: pfm = view_report(student_id); break;
            case 3: pfm = vrecomm(student_id); break;
            case 4: sba(student_id); pfm=1; break;
            case 5: pfm = sva(student_id); break;
            case 6: printf("Logging out...\n"); clrscrn(); return;
            default: printf("Invalid choice!\n"); pfm=1; break;
        }

        if(pfm==0){ printf("Logging out...\n"); return; }

    } while(1);
}
