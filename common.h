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

// Common utility functions
void clrscrn();
void paucon();

// Function prototypes
// Student functions
void regstd();
int logstd(char std_id[]);
int spfm();
int ame(char std_id[]);
int view_report(char std_id[]);
void sba(char student_id[]);
int sva(char std_id[]);
int vrecomm(char std_id[]);
void stdmenu(char student_id[]);

// Faculty functions
int logfac(char id[], Teacher *t);
int fvam(Teacher t);
int facvulstd(Teacher t);
int facrecconst(Teacher t);
int facvrecommsta(Teacher t);
void teachmenu(Teacher t);

// Consultant functions
int logconst(char id[], Consultant *c);
int consvrecomm(Consultant c);
int consvsche(Consultant c);
int consvreq(Consultant c);
void constmenu(Consultant c);

// Admin functions
int login_admin();
void adminaddfac();
void adminaddconst();
void admin_menu();
