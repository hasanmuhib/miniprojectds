#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Student structure
typedef struct {
    long long int id;
    char name[100];
    int attendance;
    int classAssessment;
    int midterm;
    int finalExam;
} Student;

// Stack Structure and Functions
typedef struct {
    char** data;
    int top;
    int size;
} Stack;

Stack* createStack(int size) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        fprintf(stderr, "Error allocating memory for stack\n");
        exit(EXIT_FAILURE);
    }
    stack->data = (char**)malloc(size * sizeof(char*));
    if (stack->data == NULL) {
        fprintf(stderr, "Error allocating memory for stack data\n");
        exit(EXIT_FAILURE);
    }
    stack->top = -1;
    stack->size = size;
    return stack;
}

int isStackFull(Stack* stack) {
    return stack->top == stack->size - 1;
}

int isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, const char* action) {
    if (isStackFull(stack)) {
        printf("Warning: Stack is full!\n");
        return;
    }
    stack->data[++stack->top] = strdup(action);
    if (stack->data[stack->top] == NULL) {
        fprintf(stderr, "Error allocating memory for stack action\n");
        exit(EXIT_FAILURE);
    }
}

char* pop(Stack* stack) {
    if (isStackEmpty(stack)) {
        printf("Warning: Stack is empty!\n");
        return NULL;
    }
    return stack->data[stack->top--];
}

void displayRecentActions(Stack* stack) {
    if (isStackEmpty(stack)) {
        printf("No recent actions.\n");
        return;
    }
    printf("Recent Actions:\n");
    for (int i = stack->top; i >= 0; i--) {
        printf("%s\n", stack->data[i]);
    }
}

#define MAX_STUDENTS 100
Student students[MAX_STUDENTS];
int studentCount = 0;

// Function to add a student
void addStudent(Student student) {
    if (studentCount >= MAX_STUDENTS) {
        printf("Error: Student list is full!\n");
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == student.id) {
            printf("Error: Student with ID %lld already exists!\n", student.id);
            return;
        }
    }
    students[studentCount++] = student;
    printf("Student added successfully.\n");
}

// Function to delete a student by ID
void deleteStudent(int id) {
    int found = 0;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            found = 1;
            for (int j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            studentCount--;
            printf("Student deleted successfully.\n");
            break;
        }
    }
    if (!found) {
        printf("Error: Student not found!\n");
    }
}

// Function to mark attendance for a student
void markAttendance(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            students[i].attendance++;
            printf("Attendance marked successfully.\n");
            return;
        }
    }
    printf("Error: Student not found!\n");
}

// Function to enter marks for a student
void enterMarks(int id, int classAssessment, int midterm, int finalExam) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            students[i].classAssessment = classAssessment;
            students[i].midterm = midterm;
            students[i].finalExam = finalExam;
            printf("Marks entered successfully.\n");
            return;
        }
    }
    printf("Error: Student not found!\n");
}

// Function to view results for all students
void viewResults() {
    if (studentCount == 0) {
        printf("No students to display.\n");
        return;
    }
    printf("Results:\n");
    for (int i = 0; i < studentCount; i++) {
        printf("ID: %lld, Name: %s, Attendance: %d, Class Assessment: %d, Midterm: %d, Final Exam: %d\n",
               students[i].id, students[i].name, students[i].attendance,
               students[i].classAssessment, students[i].midterm, students[i].finalExam);
    }
}

// Function to search for a student by ID
void searchStudent(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            printf("ID: %lld, Name: %s, Attendance: %d, Class Assessment: %d, Midterm: %d, Final Exam: %d\n",
                   students[i].id, students[i].name, students[i].attendance,
                   students[i].classAssessment, students[i].midterm, students[i].finalExam);
            return;
        }
    }
    printf("Error: Student not found!\n");
}

// Function to sort students by total marks
void sortStudentsByTotalMarks() {
    if (studentCount < 2) {
        printf("Not enough students to sort.\n");
        return;
    }
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = i + 1; j < studentCount; j++) {
            int totalMarksI = students[i].classAssessment + students[i].midterm + students[i].finalExam;
            int totalMarksJ = students[j].classAssessment + students[j].midterm + students[j].finalExam;
            if (totalMarksI < totalMarksJ) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}

void displaySortedStudents() {
    sortStudentsByTotalMarks();
    if (studentCount == 0) {
        printf("No students to display.\n");
        return;
    }
    printf("Sorted Student List by Total Marks:\n");
    for (int i = 0; i < studentCount; i++) {
        int totalMarks = students[i].classAssessment + students[i].midterm + students[i].finalExam;
        printf("ID: %lld, Name: %s, Total Marks: %d\n", students[i].id, students[i].name, totalMarks);
    }
}

#define FILE_NAME "students.dat"

// Function to save students to a file
void saveStudentsToFile() {
    FILE* file = fopen(FILE_NAME, "wb");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    size_t written = fwrite(students, sizeof(Student), studentCount, file);
    if (written != studentCount) {
        printf("Error writing to file!\n");
    }
    fclose(file);
    printf("Students saved to file successfully.\n");
}

// Function to load students from a file
void loadStudentsFromFile() {
    FILE* file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("Error opening file for reading!\n");
        return;
    }
    size_t read = fread(students, sizeof(Student), MAX_STUDENTS, file);
    if (read == 0 && ferror(file)) {
        printf("Error reading from file!\n");
    }
    studentCount = (int)read;
    fclose(file);
    printf("Students loaded from file successfully.\n");
}

// Function to display the menu
void displayMenu() {
    printf("CSE242.13 Data Structures.\nClass Management System(CMS)\n");
    printf("Developed by:Group-6\n");
    printf("1. Add Student\n");
    printf("2. Delete Student\n");
    printf("3. Mark Attendance\n");
    printf("4. Enter Marks\n");
    printf("5. View Attendance Report\n");
    printf("6. View Results\n");
    printf("7. Search Student\n");
    printf("8. Sort Students by Total Marks\n");
    printf("9. Display Recent Actions\n");
    printf("10. Exit\n");
}

// Main function
int main() {
    int choice, id;
    char name[100];
    int classAssessment, midterm, finalExam;
    Stack* actionsStack = createStack(100);

    // Load students from file at the start
    loadStudentsFromFile();

    while (1) {
        displayMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid input! Please enter a number.\n");
                    while (getchar() != '\n'); // Clear the input buffer
                    continue;
                }
                printf("Enter Name: ");
                if (scanf("%s", name) != 1) {
                    printf("Invalid input! Please enter a valid name.\n");
                    while (getchar() != '\n'); // Clear the input buffer
                    continue;
                }
                Student student = {id, "", 0, 0, 0, 0};
                strcpy(student.name, name);
                addStudent(student);
                push(actionsStack, "Add Student");
                break;
            case 2:
                printf("Enter ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid input! Please enter a number.\n");
                    while (getchar() != '\n'); // Clear the input buffer
                    continue;
                }
                deleteStudent(id);
                push(actionsStack, "Delete Student");
                break;
            case 3:
                printf("Enter ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid input! Please enter a number.\n");
                    while (getchar() != '\n'); // Clear the input buffer
                    continue;
                }
                markAttendance(id);
                push(actionsStack, "Mark Attendance");
                break;
            case 4:
                printf("Enter ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid input! Please enter a number.\n");
                    while (getchar() != '\n'); // Clear the input buffer
                    continue;
                }
                printf("Enter Class Assessment: ");
                if (scanf("%d", &classAssessment) != 1) {
                    printf("Invalid input! Please enter a number.\n");
                    while (getchar() != '\n'); // Clear the input buffer
                    continue;
                }
                printf("Enter Midterm: ");
                if (scanf("%d", &midterm) != 1) {
                    printf("Invalid input! Please enter a number.\n");
                    while (getchar() != '\n'); // Clear the input buffer
                    continue;
                }
                printf("Enter Final Exam: ");
                if (scanf("%d", &finalExam) != 1) {
                    printf("Invalid input! Please enter a number.\n");
                    while (getchar() != '\n'); // Clear the input buffer
                    continue;
                }
                enterMarks(id, classAssessment, midterm, finalExam);
                push(actionsStack, "Enter Marks");
                break;
            case 5:
                if (studentCount == 0) {
                    printf("No students to display.\n");
                    break;
                }
                printf("Attendance Report:\n");
                for (int i = 0; i < studentCount; i++) {
                    printf("ID: %lld, Name: %s, Attendance: %d\n",
                           students[i].id, students[i].name, students[i].attendance);
                }
                break;
            case 6:
                viewResults();
                break;
            case 7:
                printf("Enter ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid input! Please enter a number.\n");
                    while (getchar() != '\n'); // Clear the input buffer
                    continue;
                }
                searchStudent(id);
                break;
            case 8:
                displaySortedStudents();
                break;
            case 9:
                displayRecentActions(actionsStack);
                break;
            case 10:
                // Save students to file before exiting
                saveStudentsToFile();
                // Free stack memory
                while (!isStackEmpty(actionsStack)) {
                    free(pop(actionsStack));
                }
                free(actionsStack->data);
                free(actionsStack);
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}