// Library list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define place
#define max_un 30
#define max_pass 30
#define max_course 20
#define max_sec 16
#define max_note 260
#define maxcpt 10
#define stdfile "students.txt"
#define teachfile "teachers.txt"
#define consultfile "consultants.txt"
#define moodentryfile "mood_entries.txt"
#define recommendfile "recommendations.txt"
#define apptfile "appointments.txt"

// Function prototypes
void stdmenu(char std_id[]);
int spfm();
// Structure
typedef struct {
    char std_id[max_un];
    char pass[max_pass];
    char course[max_course];
    char sec[max_sec];
} Student;
typedef struct {
    char username[max_un];
    char pass[max_pass];
    char course[max_course];
    char sec[max_sec];
} Teacher;
typedef struct {
    char username[max_un];
    char pass[max_pass];
} Consultant;
typedef struct {
    char std_id[max_un];
    char date[11]; // YYYY-MM-DD
    int mood;
    char note[max_note];
} MoodEntry;
typedef struct {
    long id;
    char std_id[max_un];
    char faculty_username[max_un];
    int mood_lvl;
    char type[20];   // consult/break
    char status[20]; // pending/accepted/rejected/scheduled
} Recommendation;
typedef struct {
    long id;
    char student_id[max_un];
    char consultant_un[max_un];
    char faculty_un[20];   // keep placeholder, can be "-" if not used
    char date[20];
    char time[10];
    char status[20];
} Appointment;
#ifdef _WIN32
#define clrcmd "cls"
#else
#define clrcmd "clear"
#endif

void clrscrn(){         // Clears terminal after menu switch
    system(clrcmd);
}
void paucon() {         // Pause after function finish work
    printf("\nPress Enter to continue...");
    while(getchar()!='\n');
    getchar();
}

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
}
void admin_menu() {                     // Admin menu
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
                FILE *fp = fopen(stdfile,"r");
                if(!fp){ printf("No students registered.\n"); break; }
                char line[200];
                printf("\n\t\t\t\t--------------- Registered Students ---------------\n");
                while(fgets(line,sizeof(line),fp)) printf("%s",line);
                fclose(fp);
                break;
            }
            case 2: {
                FILE *fp = fopen(teachfile,"r");
                if(!fp){ printf("No faculty registered.\n"); break; }
                char line[200];
                printf("\n\t\t\t\t--------------- Registered Faculty ---------------\n");
                while(fgets(line,sizeof(line),fp)) printf("%s",line);
                fclose(fp);
                break;
            }
            case 3: {
                FILE *fp = fopen(consultfile,"r");
                if(!fp){ printf("No consultants registered.\n"); break; }
                char line[200];
                printf("\n\t\t\t\t--------------- Registered Consultants ---------------\n");
                while(fgets(line,sizeof(line),fp)) printf("%s",line);
                fclose(fp);
                break;
            }
            case 4: adminaddfac(); break;
            case 5: adminaddconst(); break;
            case 6: printf("Logging out...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 6);
}

// Student Function
void regstd() {                                            // Student Registration
    clrscrn();
    FILE *file = fopen(stdfile, "a");
    if(!file){ printf("Error! Contact admin.\n"); return; }

    Student s;
    int n_courses;

    printf("\n\t\t\t\t--------------- Student Registration ---------------\n");
    printf("Student ID (xxxx-x-xx-xxx): "); scanf("%29s", s.std_id);
    printf("Password: "); scanf("%29s", s.pass);

    while(1) {
        printf("Number of courses to register: ");
        if(scanf("%d", &n_courses)!=1 || n_courses<1) { while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        break;
    }
    getchar(); 

    for(int i=0;i<n_courses;i++) {
        printf("Course %d: ", i+1); fgets(s.course,sizeof(s.course),stdin); s.course[strcspn(s.course,"\n")]=0;
        printf("Section %d: ", i+1); fgets(s.sec,sizeof(s.sec),stdin); s.sec[strcspn(s.sec,"\n")]=0;
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
    paucon();
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
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n"); pfm=1; break;
        }

        if(pfm==0){ printf("Logging out...\n"); return; }

    } while(1);
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
    if(!fp){ printf("No mood entries found.\n"); goto END; }

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
END:
    printf("\n1. Back to Faculty Menu\n2. Logout\nChoice: ");
    int c; scanf("%d",&c);
    while(getchar()!='\n');
    return (c==2)?0:1;
}
int facvulstd(Teacher t){                                  // Faculty view vulnerable students 
    clrscrn();
    FILE *fp = fopen(moodentryfile, "r");
    if(!fp){ printf("No mood entries found.\n"); goto END; }

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

END:
    printf("\n1. Back to Faculty Menu\n2. Logout\nChoice: ");
    int c; scanf("%d",&c);
    while(getchar()!='\n');
    return (c==2)?0:1;
}
int facrecconst(Teacher t){                               // Faculty recommend to consultant
    clrscrn();
    FILE *fp = fopen(moodentryfile, "r");
    if(!fp){ printf("No mood entries found.\n"); goto END; }

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

    if(sm_count==0){ printf("No students found in your section.\n"); goto END; }

    printf("\n\t\t\t\t--------------- Students in Your Section ---------------\n");
    for(int i=0;i<sm_count;i++){
        double avg = (double)sm[i].avg_mood / sm[i].count;
        printf("%d. %s | Average Mood: %.2f\n", i+1, sm[i].std_id, avg);
    }

    int choice;
    printf("\nEnter student number to recommend for consultation (0 to skip): ");
    if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); goto END; }
    if(choice!=0){
        if(choice<1 || choice>sm_count){ printf("Invalid selection!\n"); goto END; }

        Recommendation rec;
        strcpy(rec.std_id, sm[choice-1].std_id);
        strcpy(rec.faculty_username, t.username);
        rec.mood_lvl = sm[choice-1].avg_mood / sm[choice-1].count;
        strcpy(rec.type, "consult");
        strcpy(rec.status, "pending");

        FILE *rf = fopen(recommendfile, "a");
        if(!rf){ printf("Error saving recommendation.\n"); goto END; }
        fprintf(rf, "%s %s %d %s %s\n", rec.std_id, rec.faculty_username, rec.mood_lvl, rec.type, rec.status);
        fclose(rf);

        printf("Recommendation submitted successfully for student %s.\n", rec.std_id);
    }

END:
    printf("\n1. Back to Faculty Menu\n2. Logout\nChoice: ");
    int c; scanf("%d",&c);
    while(getchar()!='\n');
    return (c==2)?0:1;
}
int facvrecommsta(Teacher t){                            // Faculty view his/her recommendation status
    clrscrn();
    FILE *fp = fopen(recommendfile, "r");
    if(!fp){ printf("No recommendations found.\n"); goto END; }

    Recommendation recs[100];
    int count = 0;

    while(fscanf(fp,"%s %s %d %s %s", recs[count].std_id, recs[count].faculty_username, &recs[count].mood_lvl, recs[count].type, recs[count].status) == 5){
        if(strcmp(recs[count].faculty_username, t.username) == 0) count++;
    }
    fclose(fp);

    if(count == 0){ printf("No recommendations made by you.\n"); goto END; }

    printf("\n\t\t\t\t--------------- Your Recommendations Progress ---------------\n");
    printf("Student | Mood | Type | Status\n");
    printf("-------------------------------\n");
    for(int i = 0; i < count; i++)
        printf("%s | %d | %s | %s\n",
               recs[i].std_id,
               recs[i].mood_lvl,
               recs[i].type,
               recs[i].status);

END:
    printf("\n1. Back to Faculty Menu\n2. Logout\nChoice: ");
    int c; scanf("%d",&c);
    while(getchar()!='\n');
    return (c==2)?0:1;
}
void teachmenu(Teacher t){                              // Teacher Menu
    int choice;
    while(1){
        clrscrn();
        printf("\n\t\t\t\t--------------- Faculty Menu ---------------\n");
        printf("1. View Average Mood of Section\n");
        printf("2. View Vulnerable Students\n");
        printf("3. Recommend Student for Consultation\n");
        printf("4. View Recommendation Status\n");
        printf("5. Logout\nChoice: ");

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
            case 5: res = 0; break;   // logout chosen from menu
            default: printf("Invalid choice!\n"); break;
        }

        if(res == 0) break; // logout chosen either from a function or menu
    }
   
}


// Consultant Function
int logconst(char id[], Consultant *c){                     // Consultant Login
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
    if (!fp) { printf("No recommendations found.\n"); goto end_option; }

    Recommendation recs[100]; int count=0; char line[200];
    while(fgets(line,sizeof(line),fp)) {
        sscanf(line,"%s %s %d %s %s", recs[count].std_id, recs[count].faculty_username, &recs[count].mood_lvl, recs[count].type, recs[count].status);
        if(strcmp(recs[count].status,"pending")==0) count++;
    }
    fclose(fp);

    if(count==0){ printf("No pending recommendations.\n"); goto end_option; }

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

end_option:
    while(1){
        int opt;
        printf("\n1. Back to Consultant Menu\n2. Logout\nChoice: ");
        if(scanf("%d",&opt)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        if(opt==1) return 1;  // back to consultant menu
        if(opt==2) return 0;  // logout -> main menu
        printf("Invalid choice!\n");
    }
}
int consvsche(Consultant c) {                             // Consultant view schedule
    clrscrn();
    FILE *fp=fopen(apptfile,"r");
    if(!fp){ printf("No appointments.\n"); goto end_option; }

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

end_option:
    while(1){
        int opt;
        printf("\n1. Back to Consultant Menu\n2. Logout\nChoice: ");
        if(scanf("%d",&opt)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        if(opt==1) return 1;
        if(opt==2) return 0;
        printf("Invalid choice!\n");
    }
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
        goto end_option; 
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

end_option:
    while(1){
        int opt;
        printf("\n1. Back to Consultant Menu\n2. Logout\nChoice: ");
        if(scanf("%d",&opt)!=1){ while(getchar()!='\n'); printf("Invalid input!\n"); continue; }
        if(opt==1) return 1; // back to menu
        if(opt==2) return 0; // logout
        printf("Invalid choice!\n");
    }
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

// Main Function
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
            case 3: {
                Teacher t; if(logfac(id,&t)) teachmenu(t); break;
            }
            case 4: {
                Consultant c; if(logconst(id,&c)) constmenu(c); break;
            }
            case 5: if(login_admin()) admin_menu(); break;
            case 6: printf("Goodbye!\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

