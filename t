That's a great idea. Adding an **admin-type functionality** will help the university authority (or counselors) monitor and analyze students’ moods more effectively. You already have mood data stored in `mood_entries.txt`, so we can build an admin dashboard around that.

---

### ✅ Here's What You Can Add:

**New Features for Admin:**

1. **Admin Login (simple username/password check)**
2. **View all mood entries**
3. **View average mood of all users**
4. **View individual user mood history**
5. **List users with consistently low mood**
6. **Logout**

---

### 🛠 Step-by-Step Changes

---

#### 1. **Add Admin Login Function**

Add this function:

```c
int adminLogin() {
    char username[30], password[30];

    printf("\n--- Admin Login ---\n");
    printf("Username: ");
    scanf("%29s", username);
    printf("Password: ");
    scanf("%29s", password);

    // For simplicity, hardcode admin credentials
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        printf("Admin login successful!\n");
        return 1;
    } else {
        printf("Invalid admin credentials.\n");
        return 0;
    }
}
```

---

#### 2. **Admin Menu**

Add this function:

```c
void adminMenu() {
    int choice;

    while (1) {
        printf("\n--- Admin Panel ---\n");
        printf("1. View All Mood Entries\n");
        printf("2. View Average Mood of All Users\n");
        printf("3. View Individual User's Mood History\n");
        printf("4. List Users with Low Moods\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewAllEntries(); break;
            case 2: viewAverageMood(); break;
            case 3: viewUserHistory(); break;
            case 4: listLowMoodUsers(); break;
            case 5: printf("Logging out from admin...\n"); return;
            default: printf("Invalid choice.\n");
        }
    }
}
```

---

#### 3. **Helper Functions for Admin**

Add the following:

```c
void viewAllEntries() {
    FILE *file = fopen("mood_entries.txt", "r");
    if (file == NULL) {
        printf("No mood entries found.\n");
        return;
    }

    char username[30], date[11], notes[250];
    int mood;

    printf("\n--- All Mood Entries ---\n");

    while (fscanf(file, "%s %s %d %[^\n]", username, date, &mood, notes) == 4) {
        printf("User: %s | Date: %s | Mood: %d | Notes: %s\n", username, date, mood, notes);
    }

    fclose(file);
}
```

```c
void viewAverageMood() {
    FILE *file = fopen("mood_entries.txt", "r");
    if (file == NULL) {
        printf("No mood entries found.\n");
        return;
    }

    int mood, count = 0, total = 0;
    char username[30], date[11], notes[250];

    while (fscanf(file, "%s %s %d %[^\n]", username, date, &mood, notes) == 4) {
        total += mood;
        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("No entries to calculate average.\n");
        return;
    }

    double average = (double) total / count;
    printf("Average Mood of All Users: %.2f (%s)\n", average, getMoodLabel((int)(average + 0.5)));
}
```

```c
void viewUserHistory() {
    char target[30];
    printf("Enter username to view history: ");
    scanf("%29s", target);

    FILE *file = fopen("mood_entries.txt", "r");
    if (file == NULL) {
        printf("No mood entries found.\n");
        return;
    }

    int mood, found = 0;
    char username[30], date[11], notes[250];

    while (fscanf(file, "%s %s %d %[^\n]", username, date, &mood, notes) == 4) {
        if (strcmp(username, target) == 0) {
            printf("Date: %s | Mood: %d | Notes: %s\n", date, mood, notes);
            found = 1;
        }
    }

    if (!found) {
        printf("No entries found for user '%s'.\n", target);
    }

    fclose(file);
}
```

```c
void listLowMoodUsers() {
    FILE *file = fopen("mood_entries.txt", "r");
    if (file == NULL) {
        printf("No mood entries found.\n");
        return;
    }

    struct {
        char username[30];
        int mood_total;
        int mood_count;
    } users[100];

    int user_count = 0;

    char username[30], date[11], notes[250];
    int mood;

    while (fscanf(file, "%s %s %d %[^\n]", username, date, &mood, notes) == 4) {
        int found = 0;
        for (int i = 0; i < user_count; i++) {
            if (strcmp(users[i].username, username) == 0) {
                users[i].mood_total += mood;
                users[i].mood_count++;
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(users[user_count].username, username);
            users[user_count].mood_total = mood;
            users[user_count].mood_count = 1;
            user_count++;
        }
    }

    fclose(file);

    printf("\n--- Users with Consistently Low Mood (avg < 5) ---\n");
    for (int i = 0; i < user_count; i++) {
        double avg = (double) users[i].mood_total / users[i].mood_count;
        if (avg < 5.0) {
            printf("User: %s | Average Mood: %.2f\n", users[i].username, avg);
        }
    }
}
```

---

### 🔄 Modify `main()`

Update your main menu in `main()`:

```c
printf("1. Register\n");
printf("2. Login\n");
printf("3. Admin Login\n"); // New option
printf("4. Exit\n");

switch (choice) {
    case 1: registeruser(); break;
    case 2:
        while (!loginuser(loggeduser)) {
            printf("Login failed. Please try again.\n");
        }
        usermenu(loggeduser);
        break;
    case 3:
        if (adminLogin()) {
            adminMenu();
        }
        break;
    case 4:
        printf("Have a safe and sound day. Goodbye!\n");
        exit(0);
    default:
        printf("Invalid Choice.\n");
}
```

---

### ✅ Summary of Changes

| Feature              | Description                       |
| -------------------- | --------------------------------- |
| `adminLogin()`       | Secure admin access               |
| `adminMenu()`        | Admin dashboard                   |
| `viewAllEntries()`   | See all students’ mood logs       |
| `viewAverageMood()`  | Track campus-wide mood average    |
| `viewUserHistory()`  | Analyze individual students       |
| `listLowMoodUsers()` | Identify students needing support |

---

Would you like me to help you modularize this into separate files (`admin.c`, `main.c`, etc.) for better code organization?
