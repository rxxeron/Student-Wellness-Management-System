/* mental_full_upgraded.c
   Upgraded Mental Wellness Journal with:
   - Proper input flushing after scanf
   - Safer fgets usage and trimming
   - No malloc/free for role in login (static buffer)
   - Password input masking (basic)
   - Improved error messages and code clarity
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <conio.h>  // For Windows getch()
#else
#include <termios.h>
#include <unistd.h>
#endif

#define MAX_USER 64
#define MAX_NAME 128
#define MAX_DEPT 16
#define MAX_COURSES 256
#define MAX_PASS 64
#define MAX_LINE 1024
#define USERS_FILE "users.txt"
#define MOOD_FILE "mood_entries.txt"
#define REC_FILE "recommendations.txt"
#define USERS_BAK  "users_backup.txt"

typedef struct {
    char id[MAX_USER];
    char name[MAX_NAME];
    char dept[MAX_DEPT];
    char courses[MAX_COURSES];
    char pass[MAX_PASS];
    char role[20]; // Student, Faculty, Counselor, Authority
} U;

// ---------- Utilities ----------

int bak() {
    FILE *src = fopen(USERS_FILE, "r");
    if (!src) return 1; // no file, treat as success
    FILE *dst = fopen(USERS_BAK, "w");
    if (!dst) { fclose(src); return 0; }
    char buf[4096];
    while (fgets(buf, sizeof(buf), src)) fputs(buf, dst);
    fclose(src); fclose(dst);
    return 1;
}

void trim(char *s) {
    // Trim trailing whitespace including \n \r and spaces
    size_t n = strlen(s);
    while (n && (s[n-1] == '\n' || s[n-1] == '\r' || s[n-1] == ' ' || s[n-1] == '\t')) s[--n] = '\0';
}

int dd(const char *date_str) {
    int y,m,d;
    if (sscanf(date_str, "%d-%d-%d", &y, &m, &d) != 3) return -1;
    struct tm entry = {0};
    entry.tm_year = y - 1900;
    entry.tm_mon = m - 1;
    entry.tm_mday = d;
    entry.tm_hour = 12;
    time_t tentry = mktime(&entry);
    if (tentry == (time_t)-1) return -1;
    time_t now = time(NULL);
    double diff = difftime(now, tentry);
    return (int)(diff / (60*60*24));
}

void bar(double value, double maxval) {
    int len = 40;
    int filled = (maxval>0) ? (int)( (value / maxval) * len ) : 0;
    if (filled < 0) filled = 0;
    if (filled > len) filled = len;
    for (int i=0;i<filled;i++) putchar('*');
    for (int i=filled;i<len;i++) putchar(' ');
}

// ---------- User file handling ----------

int parse_user_line(const char *line, U *u) {
    char tmp[MAX_LINE];
    strncpy(tmp, line, MAX_LINE-1); tmp[MAX_LINE-1] = '\0';
    trim(tmp);
    int pipe_count = 0;
    for (char *p = tmp; *p; ++p) if (*p == '|') pipe_count++;
    char *tok;
    if (pipe_count == 4) {
        // old format: no role
        tok = strtok(tmp, "|"); if (!tok) return 0; strncpy(u->id, tok, MAX_USER);
        tok = strtok(NULL, "|"); if (!tok) return 0; strncpy(u->name, tok, MAX_NAME);
        tok = strtok(NULL, "|"); if (!tok) return 0; strncpy(u->dept, tok, MAX_DEPT);
        tok = strtok(NULL, "|"); if (!tok) return 0; strncpy(u->courses, tok, MAX_COURSES);
        tok = strtok(NULL, "|"); if (!tok) return 0; strncpy(u->pass, tok, MAX_PASS);
        strncpy(u->role, "Student", sizeof(u->role));
        return 1;
    } else if (pipe_count >= 5) {
        tok = strtok(tmp, "|"); if (!tok) return 0; strncpy(u->id, tok, MAX_USER);
        tok = strtok(NULL, "|"); if (!tok) return 0; strncpy(u->name, tok, MAX_NAME);
        tok = strtok(NULL, "|"); if (!tok) return 0; strncpy(u->dept, tok, MAX_DEPT);
        tok = strtok(NULL, "|"); if (!tok) return 0; strncpy(u->courses, tok, MAX_COURSES);
        tok = strtok(NULL, "|"); if (!tok) return 0; strncpy(u->pass, tok, MAX_PASS);
        tok = strtok(NULL, "|"); if (!tok) return 0; trim(tok); strncpy(u->role, tok, sizeof(u->role));
        return 1;
    } else {
        return 0;
    }
}

int write_user_line(const U *u) {
    FILE *f = fopen(USERS_FILE, "a");
    if (!f) return 0;
    fprintf(f, "%s|%s|%s|%s|%s|%s\n", u->id, u->name, u->dept, u->courses, u->pass, u->role);
    fclose(f);
    return 1;
}

int load_user(const char *id, U *u) {
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) return 0;
    char line[MAX_LINE];
    int ok = 0;
    while (fgets(line, sizeof(line), f)) {
        if (parse_user_line(line, u)) {
            if (strcmp(u->id, id) == 0) { ok = 1; break; }
        }
    }
    fclose(f);
    return ok;
}

void migrate_users() {
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) return;
    char line[MAX_LINE];
    if (!fgets(line, sizeof(line), f)) { fclose(f); return; }
    int pipes = 0; for (char *p = line; *p; ++p) if (*p == '|') pipes++;
    fclose(f);
    if (pipes >= 5) return;
    if (!bak()) { printf("Warning: Could not backup users file. Migration aborted.\n"); return; }
    f = fopen(USERS_FILE, "r");
    if (!f) return;
    FILE *tmp = fopen("users_tmp.txt", "w");
    if (!tmp) { fclose(f); return; }
    while (fgets(line, sizeof(line), f)) {
        U u;
        if (parse_user_line(line, &u)) {
            fprintf(tmp, "%s|%s|%s|%s|%s|%s\n", u.id, u.name, u.dept, u.courses, u.pass, u.role);
        }
    }
    fclose(f); fclose(tmp);
    remove(USERS_FILE);
    rename("users_tmp.txt", USERS_FILE);
}

// ---------- Input flushing utility ----------

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------- Password input masking ----------

void input_password(char *buf, size_t size) {
    // Simple password input with '*' echo
    size_t i = 0;
#ifdef _WIN32
    while (1) {
        int c = _getch();
        if (c == '\r' || c == '\n') { putchar('\n'); break; }
        if (c == '\b' && i > 0) {
            i--;
            printf("\b \b");
        } else if (c >= 32 && i < size-1) {
            buf[i++] = c;
            putchar('*');
        }
    }
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int c;
    while ((c = getchar()) != '\n' && c != EOF && i < size-1) {
        if (c == 127 || c == '\b') { // backspace
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
        } else {
            buf[i++] = (char)c;
            printf("*");
            fflush(stdout);
        }
    }
    buf[i] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    putchar('\n');
#endif
    buf[i] = '\0';
}

// ---------- Registration ----------

void reg() {
    U u;
    printf("\n--- Registration ---\n");
    printf("Enter role (Student/Faculty/Counselor/Authority) [Student]: ");
    char rolebuf[32];
    fgets(rolebuf, sizeof(rolebuf), stdin); trim(rolebuf);
    if (strlen(rolebuf) == 0) strncpy(u.role, "Student", sizeof(u.role));
    else strncpy(u.role, rolebuf, sizeof(u.role));

    while (1) {
        printf("Enter ID (e.g. 2025-2-50-009): ");
        fgets(u.id, sizeof(u.id), stdin); trim(u.id);
        if (strlen(u.id) == 0) continue;
        U tmp;
        if (load_user(u.id, &tmp)) {
            printf("ID exists. Try another.\n");
            continue;
        }
        break;
    }
    printf("Enter Full Name: ");
    fgets(u.name, sizeof(u.name), stdin); trim(u.name);
    printf("Enter Department (CSE/ICE/EEE): ");
    fgets(u.dept, sizeof(u.dept), stdin); trim(u.dept);
    printf("Enter Courses (comma separated): ");
    fgets(u.courses, sizeof(u.courses), stdin); trim(u.courses);
    printf("Enter Password: ");
    input_password(u.pass, sizeof(u.pass));

    if (!write_user_line(&u)) {
        printf("Error writing user.\n");
        return;
    }
    printf("Registration successful for %s (%s)\n", u.id, u.role);
}

// ---------- Login ----------

// logn returns pointer to static buffer with role (no malloc/free needed)
const char* logn(char *userid_out) {
    static char role_buf[20];
    char id[MAX_USER], pass[MAX_PASS], line[MAX_LINE];
    printf("\n--- Login ---\n");
    printf("Username: ");
    fgets(id, sizeof(id), stdin); trim(id);
    printf("Password: ");
    input_password(pass, sizeof(pass));
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) { printf("No users registered.\n"); return NULL; }
    while (fgets(line, sizeof(line), f)) {
        U u; if (!parse_user_line(line, &u)) continue;
        if (strcmp(u.id, id) == 0) {
            if (strcmp(u.pass, pass) == 0) {
                strcpy(userid_out, u.id);
                strncpy(role_buf, u.role, sizeof(role_buf));
                role_buf[sizeof(role_buf)-1] = '\0';
                fclose(f);
                return role_buf;
            } else {
                fclose(f); printf("Wrong password!\n"); return NULL;
            }
        }
    }
    fclose(f);
    printf("User not found!\n");
    return NULL;
}

// ---------- Mood entries ----------

void addm(const char *userid) {
    int mood;
    char notes[512];
    time_t t = time(NULL); struct tm tm = *localtime(&t);
    char date[16]; sprintf(date, "%04d-%02d-%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday);
    printf("\n--- Add Mood Entry for %s on %s ---\n", userid, date);
    while (1) {
        printf("Mood (1-10): ");
        if (scanf("%d", &mood) != 1) {
            printf("Invalid\n");
            flush_input();
            continue;
        }
        flush_input();
        if (mood < 1 || mood > 10) {
            printf("Range 1-10\n");
            continue;
        }
        break;
    }
    printf("Notes (optional): ");
    fgets(notes, sizeof(notes), stdin); trim(notes);
    FILE *f = fopen(MOOD_FILE, "a");
    if (!f) { printf("Cannot open mood file.\n"); return; }
    fprintf(f, "%s %s %d %s\n", userid, date, mood, notes);
    fclose(f);
    printf("Saved.\n");
}

void viewm(const char *userid) {
    int d,m,y;
    printf("Enter date (day month year): ");
    if (scanf("%d %d %d", &d, &m, &y) != 3) {
        printf("Invalid date\n");
        flush_input();
        return;
    }
    flush_input();
    char date[16]; sprintf(date, "%04d-%02d-%02d", y, m, d);
    FILE *f = fopen(MOOD_FILE, "r");
    if (!f) { printf("No mood data\n"); return; }
    char line[MAX_LINE], id[MAX_USER], datef[16], notes[512];
    int mood;
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%s %s %d %[^\n]", id, datef, &mood, notes) >= 3) {
            if (strcmp(id, userid) == 0 && strcmp(datef, date) == 0) {
                printf("Mood %d (%s)\nNotes: %s\n", mood,
                    (mood<=2?"Very Low":mood<=4?"Low":mood==5?"Neutral":mood<=7?"High":"Very High"),
                    (strlen(notes)?notes:"(no notes)"));
                found = 1; break;
            }
        }
    }
    fclose(f);
    if (!found) printf("No entry for %s on %s\n", userid, date);
}

void wkr(const char *userid) {
    FILE *f = fopen(MOOD_FILE, "r");
    if (!f) { printf("No mood data\n"); return; }
    char line[MAX_LINE], id[MAX_USER], date[16], notes[512];
    int mood;
    int any = 0, sum=0, cnt=0;
    printf("\n--- Weekly mood (last 7 days) ---\n");
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%s %s %d %[^\n]", id, date, &mood, notes) >= 3) {
            if (strcmp(id, userid) == 0) {
                int days = dd(date);
                if (days >= 0 && days <= 7) {
                    printf("%s mood:%d notes:%s\n", date, mood, (strlen(notes)?notes:"(no notes)"));
                    any = 1; sum += mood; cnt++;
                }
            }
        }
    }
    fclose(f);
    if (!any) printf("No recent entries.\n");
    else printf("Average: %.2f (%s)\n", cnt?((double)sum/cnt):0.0,
                ((sum/cnt)<=2?"Very Low":(sum/cnt)<=4?"Low":(sum/cnt)==5?"Neutral":(sum/cnt)<=7?"High":"Very High"));
}

// ---------- Student notifications ----------

void checkRecForStudent(const char *userid) {
    FILE *f = fopen(REC_FILE, "r");
    if (!f) return;
    char line[MAX_LINE], sid[MAX_USER], fid[MAX_USER], status[32];
    while (fgets(line, sizeof(line), f)) {
        trim(line);
        char tmp[MAX_LINE]; strncpy(tmp, line, MAX_LINE);
        char *p = strtok(tmp, "|"); if (!p) continue; strcpy(sid, p);
        p = strtok(NULL, "|"); if (!p) continue; strcpy(fid, p);
        p = strtok(NULL, "|"); if (!p) continue; strcpy(status, p);
        if (strcmp(sid, userid) == 0 && strcmp(status, "pending") == 0) {
            printf("\n*** You have been recommended for counseling by %s (pending) ***\n", fid);
            break;
        }
    }
    fclose(f);
}

// ---------- Faculty ----------

void fac_view(const char *faculty_id) {
    U u; if (!load_user(faculty_id, &u)) { printf("Faculty not found.\n"); return; }
    char dept[MAX_DEPT]; strcpy(dept, u.dept);
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) { printf("No users file\n"); return; }
    char line[MAX_LINE];
    printf("\nStudents in dept %s (avg mood 7d):\n", dept);
    int any = 0;
    while (fgets(line, sizeof(line), f)) {
        U s; if (!parse_user_line(line, &s)) continue;
        if (strcmp(s.role, "Student") == 0 && strcmp(s.dept, dept) == 0) {
            FILE *mf = fopen(MOOD_FILE, "r");
            if (!mf) continue;
            char mline[MAX_LINE], id[MAX_USER], date[16], notes[512]; int mood;
            int sum=0,cnt=0;
            while (fgets(mline, sizeof(mline), mf)) {
                if (sscanf(mline, "%s %s %d %[^\n]", id, date, &mood, notes) >= 3) {
                    if (strcmp(id, s.id) == 0) {
                        int days = dd(date);
                        if (days >= 0 && days <= 7) { sum += mood; cnt++; }
                    }
                }
            }
            fclose(mf);
            if (cnt>0) {
                printf("Student %s avg mood: %.2f\n", s.id, (double)sum/cnt);
                any=1;
            }
        }
    }
    fclose(f);
    if (!any) printf("No students or no recent mood data.\n");
}

void fac_recommend() {
    char sid[MAX_USER], fid[MAX_USER];
    printf("Your faculty ID: ");
    fgets(fid, sizeof(fid), stdin); trim(fid);
    printf("Student ID to recommend for counseling: ");
    fgets(sid, sizeof(sid), stdin); trim(sid);
    // Validate both users exist and roles
    U su, fu;
    if (!load_user(fid, &fu) || strcmp(fu.role, "Faculty") != 0) {
        printf("Invalid faculty ID\n"); return;
    }
    if (!load_user(sid, &su) || strcmp(su.role, "Student") != 0) {
        printf("Invalid student ID\n"); return;
    }
    FILE *f = fopen(REC_FILE, "a");
    if (!f) { printf("Error opening recommendations file\n"); return; }
    fprintf(f, "%s|%s|pending\n", sid, fid);
    fclose(f);
    printf("Recommendation sent.\n");
}

// ---------- Counselor ----------

void coun_view_recommendations(const char *counselor_id) {
    (void)counselor_id; // counselor_id not used currently but could filter by assigned
    FILE *f = fopen(REC_FILE, "r");
    if (!f) { printf("No recommendations.\n"); return; }
    char line[MAX_LINE], sid[MAX_USER], fid[MAX_USER], status[32];
    int any = 0;
    printf("\nPending recommendations:\n");
    while (fgets(line, sizeof(line), f)) {
        trim(line);
        char tmp[MAX_LINE]; strncpy(tmp, line, MAX_LINE);
        char *p = strtok(tmp, "|"); if (!p) continue; strcpy(sid, p);
        p = strtok(NULL, "|"); if (!p) continue; strcpy(fid, p);
        p = strtok(NULL, "|"); if (!p) continue; strcpy(status, p);
        if (strcmp(status, "pending") == 0) {
            printf("Student %s recommended by %s (pending)\n", sid, fid);
            any = 1;
        }
    }
    fclose(f);
    if (!any) printf("No pending recommendations.\n");
}

void coun_mark_handled() {
    char sid[MAX_USER];
    printf("Enter student ID to mark recommendation as handled: ");
    fgets(sid, sizeof(sid), stdin); trim(sid);
    FILE *f = fopen(REC_FILE, "r");
    if (!f) { printf("No recommendations.\n"); return; }
    char lines[1000][MAX_LINE];
    int n = 0;
    int found = 0;
    while (fgets(lines[n], sizeof(lines[n]), f)) {
        n++;
        if (n >= 1000) break;
    }
    fclose(f);
    for (int i=0; i<n; i++) {
        char tmp[MAX_LINE]; strncpy(tmp, lines[i], MAX_LINE);
        char *p = strtok(tmp, "|"); if (!p) continue;
        char *sid_r = p;
        p = strtok(NULL, "|"); if (!p) continue;
        char *fid_r = p;
        p = strtok(NULL, "|"); if (!p) continue;
        char *status = p;
        trim(status);
        if (strcmp(sid_r, sid) == 0 && strcmp(status, "pending") == 0) {
            snprintf(lines[i], MAX_LINE, "%s|%s|handled\n", sid_r, fid_r);
            found = 1;
        }
    }
    if (!found) {
        printf("No pending recommendation found for student %s\n", sid);
        return;
    }
    f = fopen(REC_FILE, "w");
    if (!f) { printf("Cannot open recommendation file for writing\n"); return; }
    for (int i=0; i<n; i++) fputs(lines[i], f);
    fclose(f);
    printf("Marked handled.\n");
}

// ---------- Authority ----------

void auth_show_stats() {
    FILE *f = fopen(MOOD_FILE, "r");
    if (!f) { printf("No mood data\n"); return; }
    char line[MAX_LINE], id[MAX_USER], date[16], notes[512];
    int mood;
    int days_arr[31] = {0};
    int counts[31] = {0};
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%s %s %d %[^\n]", id, date, &mood, notes) >= 3) {
            int days = dd(date);
            if (days >= 0 && days < 31) {
                days_arr[days] += mood;
                counts[days]++;
            }
        }
    }
    fclose(f);
    printf("\nLast 30 days mood averages:\nDay  Mood Avg\n");
    for (int i = 0; i < 31; i++) {
        if (counts[i] > 0) {
            double avg = (double)days_arr[i] / counts[i];
            printf("%2d : %5.2f ", i, avg);
            bar(avg, 10);
            printf("\n");
        }
    }
}

// ---------- Main menu handlers ----------

void student_menu(const char *userid) {
    int ch;
    while (1) {
        printf("\n--- Student Menu ---\n");
        checkRecForStudent(userid);
        printf("1. Add Mood Entry\n2. View Mood by Date\n3. Weekly Mood Report\n4. Logout\nChoice: ");
        if (scanf("%d", &ch) != 1) { printf("Invalid input\n"); flush_input(); continue; }
        flush_input();
        switch(ch) {
            case 1: addm(userid); break;
            case 2: viewm(userid); break;
            case 3: wkr(userid); break;
            case 4: return;
            default: printf("Invalid choice\n");
        }
    }
}

void faculty_menu(const char *userid) {
    int ch;
    while (1) {
        printf("\n--- Faculty Menu ---\n");
        printf("1. View Students' Weekly Avg Mood\n2. Recommend Student for Counseling\n3. Logout\nChoice: ");
        if (scanf("%d", &ch) != 1) { printf("Invalid input\n"); flush_input(); continue; }
        flush_input();
        switch(ch) {
            case 1: fac_view(userid); break;
            case 2: fac_recommend(); break;
            case 3: return;
            default: printf("Invalid choice\n");
        }
    }
}

void counselor_menu(const char *userid) {
    int ch;
    while (1) {
        printf("\n--- Counselor Menu ---\n");
        printf("1. View Pending Recommendations\n2. Mark Recommendation Handled\n3. Logout\nChoice: ");
        if (scanf("%d", &ch) != 1) { printf("Invalid input\n"); flush_input(); continue; }
        flush_input();
        switch(ch) {
            case 1: coun_view_recommendations(userid); break;
            case 2: coun_mark_handled(); break;
            case 3: return;
            default: printf("Invalid choice\n");
        }
    }
}

void authority_menu() {
    int ch;
    while (1) {
        printf("\n--- Authority Menu ---\n");
        printf("1. Show Last 30 Days Mood Stats\n2. Logout\nChoice: ");
        if (scanf("%d", &ch) != 1) { printf("Invalid input\n"); flush_input(); continue; }
        flush_input();
        switch(ch) {
            case 1: auth_show_stats(); break;
            case 2: return;
            default: printf("Invalid choice\n");
        }
    }
}

// ---------- Main ----------

int main() {
    migrate_users();
    printf("Mental Wellness Journal\n");
    while (1) {
        printf("\n1. Register\n2. Login\n3. Exit\nChoice: ");
        int ch;
        if (scanf("%d", &ch) != 1) {
            printf("Invalid input\n");
            flush_input();
            continue;
        }
        flush_input();
        if (ch == 1) {
            reg();
        } else if (ch == 2) {
            char userid[MAX_USER];
            const char *role = logn(userid);
            if (!role) continue;
            printf("Welcome %s (%s)\n", userid, role);
            if (strcmp(role, "Student") == 0) {
                student_menu(userid);
            } else if (strcmp(role, "Faculty") == 0) {
                faculty_menu(userid);
            } else if (strcmp(role, "Counselor") == 0) {
                counselor_menu(userid);
            } else if (strcmp(role, "Authority") == 0) {
                authority_menu();
            } else {
                printf("Unknown role: %s\n", role);
            }
        } else if (ch == 3) {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid choice\n");
        }
    }
    return 0;
}
