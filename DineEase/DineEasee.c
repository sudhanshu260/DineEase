#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

// Structure for menu items
struct Item {
    char name[50];
    float rating;
    float price;
    int food_Id_No;
    struct Item *next;
    struct Item *prev;
};

// Structure for order history
struct order_hist {
    int Customer_ID;
    int items[10][2];
    float amount;
    char date[11];
    char customerName[25]; // Store customer name
    int isTeacher; // 1 for teacher, 0 for student
    struct order_hist *next;
    struct order_hist *prev;
};

// Global variables
int cust_id = 1;
struct Item* head = NULL;
struct Item* last = NULL;
int Today_customer = 0;
float total_income = 0;

// Separate lists for teachers and students
struct order_hist* teacherList = NULL; // Head of the teachers' list
struct order_hist* teacherListLast = NULL; // Last of the teachers' list
struct order_hist* studentList = NULL; // Head of the students' list
struct order_hist* studentListLast = NULL; // Last of the students' list

// Function to create a new menu item node
struct Item* getnewnode(char a[], float p, int fin) {
    struct Item* temp = (struct Item*)malloc(sizeof(struct Item));
    temp->food_Id_No = fin;
    strcpy(temp->name, a);
    temp->rating = 4.0;
    temp->price = p;
    temp->next = NULL;
    temp->prev = NULL;
    return temp;
}

// Function to insert a new menu item
void insert(char n[], float p, int fin) {
    struct Item* temp1 = getnewnode(n, p, fin);
    if (head == NULL) {
        head = temp1;
        last = temp1;
    } else {
        temp1->prev = last;
        last->next = temp1;
        last = temp1;
    }
}

// Function to create a new order history node
struct order_hist* getnewNode_hist() {
    struct order_hist* temp = (struct order_hist*)malloc(sizeof(struct order_hist));
    temp->next = temp->prev = NULL;
    temp->isTeacher = 0; // Default to student
    return temp;
}

// Function to display the menu
void Display() {
    printf("                                      -----------------                                                     \n");
    printf("---------------------------------------------------MENU--------------------------------------------------------\n");
    printf("                                             --------------------                                              \n");
    printf("INDEX      ITEM NAME      PRICE        RATING\n");
    struct Item* temp = head;
    if (head == NULL) {
        printf("empty\n");
    }
    while (temp != NULL) {
        printf("%d\t%s\t%f\t%f\n", temp->food_Id_No, temp->name, temp->price, temp->rating);
        temp = temp->next;
    }
    printf("------------------------------------------------------------------------------------------------------------------------------\n");
}

// Function to handle login
int login() {
    char username[20];
    char userpwd[11]; //for storing pwd
    printf("enter your username : ");
    scanf("%s", username);

    printf("enter password: ");
    for (int i = 0; i < 10; i++) {
        userpwd[i] = getch();
        printf("*");
    }
    userpwd[10] = '\0';

    if (!strcmp(username, "Admin") && !strcmp(userpwd, "MCA2SEMPBL")) {
        printf("\n\nLogged in successfully\n");
        return 1;
    } else {
        printf("\n\nIncorrect username and password\n");
        return 0;
    }
}

// Function to add an order to the teachers' list
void addToTeacherList(struct order_hist* order) {
    if (teacherList == NULL) {
        teacherList = teacherListLast = order;
    } else {
        teacherListLast->next = order;
        order->prev = teacherListLast;
        teacherListLast = order;
    }
}

// Function to add an order to the students' list
void addToStudentList(struct order_hist* order) {
    if (studentList == NULL) {
        studentList = studentListLast = order;
    } else {
        studentListLast->next = order;
        order->prev = studentListLast;
        studentListLast = order;
    }
}

// Function to display all orders (teachers first, then students)
void displayAllOrders() {
    printf("----------------------------------------------ALL ORDERS-----------------------------------------------\n");
    printf("TYPE   CUSTOMER NAME      DATE       TOTAL_AMOUNT\n");

    // Display teachers' orders first
    struct order_hist* temp = teacherList;
    while (temp != NULL) {
        printf("Teacher\t%s\t%s\t%f\n", temp->customerName, temp->date, temp->amount);
        temp = temp->next;
    }

    // Display students' orders
    temp = studentList;
    while (temp != NULL) {
        printf("Student\t%s\t%s\t%f\n", temp->customerName, temp->date, temp->amount);
        temp = temp->next;
    }
    printf("-------------------------------------------------------------------------------------------------------------\n");
}

// Function to place an order
void order() {
    int a[10][2];
    int n, j = 0, i = 0;

    // Ask if the user is a student or teacher
    int isTeacher;
    printf("Are you a student or teacher?\n1. Student\n2. Teacher\n");
    scanf("%d", &isTeacher);

    do {
        printf("Please enter food id number of item and its quantity: ");
        for (i = 0; i < 2; i++) {
            scanf("%d", &a[j][i]);
        }
        j++;
        printf("Do you want more items?\n1. Yes\n2. No: ");
        scanf("%d", &n);
    } while (n == 1);

    float total_amount = 0.0;
    char name[25];
    char Date[10];
    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter today's date: ");
    scanf("%s", Date);

    printf("\n------------------------------------------------------------------------------------------------------------------------\n");
    printf("                                           BILL                                       \n");
    printf("Name: %s\n", name);
    printf("Date: %s\n", Date);

    for (int k = 0; k < j; k++) {
        struct Item* temp = head;
        while (temp->food_Id_No != a[k][0]) {
            temp = temp->next;
        }
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        printf("%d\t%s\t%d\t%f\n", temp->food_Id_No, temp->name, a[k][1], (a[k][1] * (temp->price)));
        total_amount += (a[k][1] * (temp->price));
    }

    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("Total payable amount is: %f\n", total_amount);
    printf("-------------------------------------------------------------------------------------------------------------\n");

    // Create a new order
    struct order_hist* temp2 = getnewNode_hist();
    temp2->amount = total_amount;
    temp2->Customer_ID = cust_id++;
    temp2->isTeacher = (isTeacher == 2) ? 1 : 0; // Set isTeacher flag
    strcpy(temp2->customerName, name); // Store customer name
    strcpy(temp2->date, Date);

    // Add to the appropriate list
    if (isTeacher == 2) {
        addToTeacherList(temp2); // Add to teachers' list
    } else {
        addToStudentList(temp2); // Add to students' list
    }

    Today_customer++;
    total_income += total_amount;

    // Display all orders (teachers first, then students)
    displayAllOrders();
}

// Main function
int main() {
    head = NULL;
    last = NULL;
    insert("  Burger      ", 70.23, 1);
    insert("  Pizza       ", 237.26, 6);
    insert("  HOT CAKE    ", 750.23, 10);
    insert("  Coffee      ", 70.23, 2);
    insert("  Ice-cream    ", 70.23, 3);
    insert("  Sandwich     ", 60.23, 4);
    insert("  Momo        ", 70.3, 8);
    insert("  Bread       ", 35.13, 5);
    insert("  Cold Drinks  ", 20.13, 9);

    int choice;
    do {
        printf("---------------------------------------------------------------\n");
        printf("1..... Food Part\n");
        printf("2..... Admin Panel\n");
        printf("3..... Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int ex;
                do {
                    printf("1. FOOD LIST\n");
                    printf("2. ORDER\n");
                    int n;
                    scanf("%d", &n);
                    switch (n) {
                        case 1:
                            Display();
                            break;
                        case 2:
                            order();
                            break;
                    }
                    printf("1. FOOD PART\n");
                    printf("2. MAIN MENU\n");
                    scanf("%d", &ex);
                } while (ex == 1);
                break;
            }
            case 2: {
                int k = login();
                if (k == 1) {
                    int ch;
                    do {
                        printf("1. Enter new dish\n");
                        printf("2. Today's Total Income\n");
                        printf("3. Today total number of customers\n");
                        printf("4. Display All Orders\n");
                        printf("5. List of items\n");
                        printf("6. Exit\n");
                        printf("Enter your choice: ");
                        scanf("%d", &ch);

                        switch (ch) {
                            case 1: {
                                float p;
                                int fin;
                                char n[50];
                                printf("Enter the name of item: ");
                                scanf("%s", n);
                                printf("Enter the price of item: ");
                                scanf("%f", &p);
                                printf("Enter the food_id_no of item: ");
                                scanf("%d", &fin);
                                insert(n, p, fin);
                                printf("New dish added successfully.\n");
                                break;
                            }
                            case 2:
                                printf("Today's total income is: %f\n", total_income);
                                break;
                            case 3:
                                printf("Today total number of customers: %d\n", Today_customer);
                                break;
                            case 4:
                                displayAllOrders();
                                break;
                            case 5:
                                Display();
                                break;
                        }
                    } while (ch != 6);
                }
                break;
            }
        }
    } while (choice != 3);
    return 0;
}
