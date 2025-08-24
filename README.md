# Student Wellness Management System (SWMS)


A comprehensive C application for managing student wellness, mood tracking, faculty recommendations, and counseling appointments.

## Project Structure

The code has been organized into separate files based on functionality:

### Files Overview

- **`common.h`** - Header file containing all shared structures, constants, and function prototypes
- **`student.c`** - All student-related functions (registration, login, mood entry, reports, appointments)
- **`faculty.c`** - All faculty/teacher-related functions (login, view mood data, recommend students)
- **`consultant.c`** - All consultant-related functions (login, view recommendations, manage appointments)
- **`admin.c`** - All administrator functions (login, add faculty/consultants, view registrations)
- **`main.c`** - Main function with the primary menu system

### Features

#### Student Features
- Registration and login
- Mood entry with automatic consultation recommendations for low moods
- View mood history (daily, weekly, monthly, tri-monthly reports)
- Book appointments with consultants
- View and respond to recommendations
- View appointment status

#### Faculty Features
- Login and authentication
- View average mood of their section
- Identify vulnerable students (average mood < 5)
- Recommend students for consultation
- Track recommendation status

#### Consultant Features
- Login and authentication
- View pending recommendations
- Schedule appointments for recommended students
- View scheduled appointments
- Accept/reject appointment requests

#### Administrator Features
- Secure admin login
- View all registered students, faculty, and consultants
- Add new faculty members
- Add new consultants

### Data Files
- `students.txt` - Student registration data
- `teachers.txt` - Faculty registration data
- `consultants.txt` - Consultant registration data
- `mood_entries.txt` - Student mood entries
- `recommendations.txt` - Faculty recommendations
- `appointments.txt` - Appointment data

## Compilation

To compile the project, you can either:

1. **Compile all files together:**
   ```bash
   gcc -o swms main.c student.c faculty.c consultant.c admin.c
   ```

2. **Compile separately and link:**
   ```bash
   gcc -c main.c student.c faculty.c consultant.c admin.c
   gcc -o swms main.o student.o faculty.o consultant.o admin.o
   ```

## Usage

Run the compiled executable:
```bash
./swms
```

### Default Admin Credentials
- Username: `admin_swms`
- Password: `swmsewu2025`

## Code Organization Benefits

- **Modularity**: Each file handles specific functionality
- **Maintainability**: Easier to locate and modify specific features
- **Readability**: Cleaner code structure with logical separation
- **Collaboration**: Multiple developers can work on different modules
- **Debugging**: Issues can be isolated to specific modules
