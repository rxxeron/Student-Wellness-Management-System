#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define max_username 30
#define max_password 30



void registeruser(){
    char username[max_username];
    char password[max_password];
    char existuser[max_username];
    char existpass[max_password];
FILE *file = fopen("user.txt", "r");
    printf("\n--- User Registration ---\n");
    while(1){
    printf("Enter new username: ");
    scanf("%29s", username);
    int exist = 0;
    
    if(file != NULL)
    {
        while(fscanf(file, "%s %s", existuser, existpass) !=EOF)
        {
            if(strcmp(username, existuser) == 0)
            {
                printf("Username already exists. Try a different one.\n");
                exist = 1;
                break;
            }
        }
        fclose(file);
    }
    if (!exist){
        break;
    }
}
    printf("Enter new password: ");
    scanf("%29s", password);

    file = fopen("user.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;        
    }
    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    printf("Congratulations! Registration Successful!\n");
}

int loginuser(char username[])
{
    
    char password[max_password];
    char fileuser[max_username];
    char filepass[max_password];
    int success = 0;
while(!success){
    printf("\n\n\n--- Login to Mental Wellness Journal by EWU! ---\n");
    printf("Username: ");
    scanf("%29s", username);
    printf("Password: ");
    scanf("%29s", password);

    FILE *file= fopen("user.txt", "r");
    if(file == NULL){
        printf("No registered users found.\n");
        return 0;
    }
    int found = 0;
    while (fscanf(file, "%29s %29s", fileuser, filepass) != EOF){
    if(strcmp(username, fileuser) == 0 )
    {
        found = 1;
        if (strcmp(password,filepass) == 0){
            success = 1;
            break;;
        }
        else {
            printf("Wrong Password!\n");
            break;
        }
        
    }
}

fclose (file);
if(!found){
    printf("User not found!\n");
}
if(!success){
    printf("Login failed.Please Try again,\n\n");
}
}
printf("\n\n ------ Welcome to Mental Wellness Journal by EWU, %s\n ",username);
return 1;
}
int dd(const char *date_str){
    int year,month,day;
        struct tm entry_date = {0};
        time_t now = time(NULL);
        struct tm *current_date = localtime(&now);
        if (sscanf(date_str,"%d-%d-%d", &year, &month, &day) !=3){
            return -1;
        }
        entry_date.tm_year = year - 1900;
        entry_date.tm_mon =month - 1;
        entry_date.tm_mday = day;

        time_t entry_time = mktime(&entry_date);
        if(entry_time == -1){
            return -1;
        }
        double diffs= difftime(now, entry_time);
        int days = (int)(diffs/(60*60*24));
        return days;
    }



void consultExpert() {
   int choice;
   while(1){
    printf("\n------ Consult Expert ------\n");
    printf("1. EWU Counselling Appointment Info\n");
    printf("2. Kan Pete Roi Contact Number\n");
    printf("3. Back to Main Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice){
        case 1: printf("\n------ EWU Counseling Appointment Information ------\n");
                printf("East West University has taken initiative to provide psychological counseling services to the students for mental well-being.\n");
                printf("For this purpose, EWU has appointed full-time counselors to help with psychological stress, drug addiction prevention, family difficulties, and social disorders.\n\n");
                printf("Days  :  Sunday to Thursday\n");
                printf("Time  :  10:00AM – 4:00PM\n");
                printf("Venue :  Room No: 306, Socio-Psyche Consultation Center\n");
                printf("         Career Counseling Center, East West University\n\n");
                printf("Please register and CCC will confirm your counseling date and time.\n");
                printf("Registration link: https://docs.google.com/forms/d/e/1FAIpQLSd4Yp1y_i1MN1QnSlJhP4ZgEe3WYaawLt6-K1Pb_J1opQLisw/viewform\n\n");
                break;
                case 2:
                printf("\n------ Kan Pete Roi Contact ------\n");
                printf("For support from Kan Pete Roi, please contact:\n");
                printf("+880 9612-119911\n\n");
                break;

            case 3:
                return;  
            default:
                printf("Invalid choice. Please try again.\n");
    }
   }
}
void getsuggestion(){
    int choice;
    
    const char *stress[]={
        "Take a short walk in nature.",
        "Try deep breathing for 2 minutes.",
        "Talk to a friend about your day.",
        "Avoid Multitasking, focus on one task.",
        "Write down what's bothering you."
    };
    const char *sleep[]={
        "Avoid caffeine after 6 PM.",
        "Try a warm shower before bed.",
        "Use a sleep tracker to understand your patterns.",
        "Make your bedroom a screen-free zone.",
        "Go to bed at the same time every night."
    };
    const char *moodt[]={
        "Listen to your favourite music.",
        "Practice gratitude by writing 3 things you're thankful for.",
        "Watch a funny video.",
        "Do one kind thing for some else.",
        "Stretch your body or dance a little."
    };
    int sc = sizeof(stress)/sizeof(stress[0]);
    int sc2 = sizeof(sleep)/sizeof(sleep[0]);
    int mc = sizeof(moodt)/sizeof(moodt[0]);
    
    while(1){
        printf("\n----- Get Suggestions -----\n");
        printf("1. Tips for Reducing Stress\n");
        printf("2. Tips for Improving Sleep\n");
        printf("3. Tips for Boosting Mood\n");
        printf("4. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1: 
            printf("\nStress Tip: %s\n\n", stress[rand()% sc]);
            break;
            case 2: 
            printf("\nSleep Tip: %s\n\n", sleep[rand()% sc2]);
            break;
            case 3: 
            printf("\nMood Tips: %s\n\n", moodt[rand()%mc]);
            break;
            case 4:
            return;
            default:
            printf("Invalid Choice.Please try again.\n");
        }

    }
}
void addmoodentry(char username[]){
    int mood;
    char notes[250];
    char date[11];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon +1, tm.tm_mday);
    printf("\n ------ Add Mood Entry for %s on %s ------\n", username, date);
    while(1){
    printf("Select your current mood: \n");
    printf("1. Extremely Sad\n");
    printf("2. Sad \n");
    printf("3. Down \n");
    printf("4. Low \n");
    printf("5. Neutral\n");
    printf("6. Okay \n");
    printf("7. Good \n");
    printf("8. Happy \n");
    printf("9. Very Happy\n");
    printf("10. Excited/Excellent \n");
    printf("Enter your choice: \n");
    if (scanf("%d", &mood) != 1){
        printf("Invalid mood choice. Please try again.\n");
        while(getchar() != '\n');
        continue;
    }

    if (mood < 1 || mood > 10)
    {
        printf("Invalid Mood Choice. Please try again.\n\n");
        continue;
    }
    break;
}

    getchar();
    printf("Any notes for today ? (max 250 chars): ");
    fgets(notes, sizeof(notes), stdin);
    size_t len = strlen(notes);
    if(len > 0 && notes [len-1] == '\n')
    {
        notes[len - 1] = '\0';
    }

    FILE *file = fopen("mood_entries.txt", "a");
    if(file == NULL){
        printf("Error Opening mood entries file.\n");
        return;
    }

    fprintf(file, "%s %s %d %s\n", username, date,mood,notes);
    fclose(file);

    printf("Mood entry saved successfully!\n");

    if (mood < 4)
    {
        printf("\nYour mood is quite low today.\n");
        consultExpert();
    }
    else {
        getsuggestion();
    }
    }



void viewmoodentry(char username[]){
    int day,month,year;
    char normaldate[11];
while(1){
    printf("\nEnter date to view mood entry (day/month/year): ");
    if(scanf("%d %d %d", &day, &month, &year) !=3)
    {
    printf("Invalid input.Please try again.\n");
    while(getchar()!= '\n');
    continue;
    }
    if ( day < 1 || day > 31 || month < 1 || month > 12){
        printf("Invalid date. Please try again.\n\n");
        continue;
    }
    if (year < 100){
        year += 2000;
    }    
    break;
}
    sprintf(normaldate, "%04d-%02d-%02d", year, month,day);
   
    FILE *file= fopen("mood_entries.txt", "r");
    if( file == NULL)
    {
    printf("No mood entries found.\n");
    return;
    }
    char fileuser[50],date[11],notes[250];
    int mood;
    int found = 0;

    while(fscanf(file, "%s %s %d %[^\n]", fileuser, date, &mood, notes) == 4)
    {
        if (strcmp(username, fileuser) == 0 && strcmp(normaldate, date) == 0)
        {
            found = 1;

            char *moodlabel;
            switch(mood){
                case 1: moodlabel = "Extremely Sad"; break;
                case 2: moodlabel = "Sad"; break;
                case 3: moodlabel = "Down"; break;
                case 4: moodlabel = "Low"; break;
                case 5: moodlabel = "Neutral"; break;
                case 6: moodlabel = "Okay"; break;
                case 7: moodlabel = "Good"; break;
                case 8: moodlabel = "Happy"; break;
                case 9: moodlabel = "Very Happy"; break;
                case 10: moodlabel = "Excited/Excellent"; break;
                default: moodlabel = "Unknown"; break;
            }
            printf("\n Mood Entry on %s:\n Mood:%s\nNotes: %s\n", normaldate, moodlabel, notes);
            break; 
        }
    }
    fclose(file);
    if(!found){
        printf("No mood entry found for %s on %s.\n", username, normaldate);
    }
}

const char* getMoodLabel(int mood) {
    if (mood <= 2) return "Very Low";
    else if (mood <= 4) return "Low";
    else if (mood == 5) return "Neutral";
    else if (mood <= 7) return "High";
    else return "Very High";
}


void weekreport(char username[]){
    FILE *file = fopen("mood_entries.txt", "r");
    if(file == NULL){
        printf("No mood entries found.\n");
        return;
    }

    char line [256];
    char fileuser[30], date[11], notes[250];
    int mood, found = 0;
    int tm = 0, count = 0;

    printf("\n------Weekly Mood Report for %s-----\n", username);

    while(fgets(line,sizeof(line), file)){
        sscanf(line, "%s %s %d %[^\n]", fileuser,date,&mood, notes);

        if(strcmp(fileuser, username) == 0){
            int days = dd(date);
            if(days >=0 && days <=7){
                found =1;
                tm += mood;
                count++;
                printf("%s | mood: %d | Notes: %s\n", date, mood, notes);
            }
        }
    }
    if (!found){
        printf("No mood entries in the last 7 days.\n");
    }
    else {
        double average = (double)tm/count;
        printf("\nAverage mood score: %.2f (%s)\n", average, getMoodLabel((int)(average+0.5)));

    }
    fclose(file);
}
void usermenu(char username[]){
    int choice;
    while(1){
        printf("\n------ Welcome %s ------\n", username);
        printf("1. Add Mood Entry\n");
        printf("2. View Previous Entries\n");
        printf("3. Get Suggestion\n");
        printf("4. Consult Expert\n");
        printf("5. Weekly Mood Report\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1: 
            addmoodentry(username);
            break;
            case 2: 
            viewmoodentry(username);
            break;
            case 3: 
            getsuggestion();
            break;
            case 4:
            consultExpert();
            break;
            case 5: 
            weekreport(username);
            break;
            case 6:
            printf("Logging out......\n");
            return;
            default:
            printf("Invalid Choice. \n");
        }
    }
}
int main()
{
    srand(time(NULL));
    char loggeduser[max_username];
    int choice;
    int isloggedin = 0;
    printf("\n ------ Welcome to the Mental Wellness Journal by EWU! ------\n");
    while(1){
        printf("\nMenu\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
            registeruser();
            break;
            case 2: 
            while (!loginuser(loggeduser)){
                printf("Login failed. Please try again.\n");
            }
            usermenu(loggeduser);
            break;
            case 3:
            printf("Have a safe and sound day. Goodbye! \n");
            exit(0);
            default: 
            printf("Invalid Choice.\n");
        }
    }
    return 0;
}