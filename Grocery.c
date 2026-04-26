#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// ---------------- Data Models ----------------
typedef struct {
    int id;
    char name[50];
    char category[50];
    int quantity;
    char expiryDate[15]; // DD/MM/YYYY
} Food;

typedef struct {
    int id;
    char name[50];
    char role[50];
    float salary;
} Employee;

typedef struct {
    int id;
    char name[50];
    char phone[20];
} Customer;

typedef struct {
    int id;
    char name[50];
    char product[50];
} Supplier;

// ---------------- Global Variables ----------------
Food foods[MAX];            int foodCount = 0;
Employee employees[MAX];    int empCount  = 0;
Customer customers[MAX];    int custCount = 0;
Supplier suppliers[MAX];    int suppCount = 0;
float totalSales = 0.0f;

// ---------------- Assisting Functions ----------------
void parseDate(const char *dateStr, int *d, int *m, int *y) {
    sscanf(dateStr, "%d/%d/%d", d, m, y);
}
int isExpired(const char *expiry, const char *today) {
    int d1, m1, y1, d2, m2, y2;
    parseDate(expiry, &d1, &m1, &y1);
    parseDate(today, &d2, &m2, &y2);
    if (y1 < y2) return 1;
    if (y1 == y2 && m1 < m2) return 1;
    if (y1 == y2 && m1 == m2 && d1 < d2) return 1;
    return 0;
}

// ---------------- Persistence: Foods ----------------

//--------Function Prototypes for food items-----------

void saveFoods(void);
void loadFoods(void);
void foodMenu(void);


void saveFoods(void)
{
    FILE *f = fopen("foods.txt", "w");
    if (!f) { printf("Error: cannot write in foods.txt\n"); return; }
    for (int i = 0; i < foodCount; i++)
        fprintf(f, "%d,%s,%s,%d,%s\n",
                foods[i].id, foods[i].name, foods[i].category,
                foods[i].quantity, foods[i].expiryDate);
    fclose(f);
}
void loadFoods(void) {
    FILE *f = fopen("foods.txt", "r");
    if (!f) return;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        Food x;
        if (sscanf(line, "%d,%49[^,],%49[^,],%d,%14[^\n]",
                   &x.id, x.name, x.category, &x.quantity, x.expiryDate) == 5) {
            if (foodCount < MAX) foods[foodCount++] = x;
        }
    }
    fclose(f);
}

// ---------------- Persistence: Employees ----------------

//--------Function Prototypes for Employees ---------------

void saveEmployees(void);
void loadEmployees(void);
void employeeMenu(void);


void saveEmployees(void)
{
    FILE *f = fopen("employees.txt", "w");
    if (!f) { printf("Error: cannot write employees.txt\n"); return; }
    for (int i = 0; i < empCount; i++)
        fprintf(f, "%d,%s,%s,%.2f\n",
                employees[i].id, employees[i].name, employees[i].role, employees[i].salary);
    fclose(f);
}
void loadEmployees(void)
{
    FILE *f = fopen("employees.txt", "r");
    if (!f) return;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        Employee e;
        if (sscanf(line, "%d,%49[^,],%49[^,],%f",
                   &e.id, e.name, e.role, &e.salary) == 4) {
            if (empCount < MAX) employees[empCount++] = e;
        }
    }
    fclose(f);
}

// ---------------- Persistence: Customers ----------------

//--------Function Prototypes for Employees ---------------

void saveCustomers(void);
void loadCustomers(void);
void customerMenu(void);



void saveCustomers(void) {
    FILE *f = fopen("customers.txt", "w");
    if (!f) { printf("Error: cannot write customers.txt\n"); return; }
    for (int i = 0; i < custCount; i++)
        fprintf(f, "%d,%s,%s\n",
                customers[i].id, customers[i].name, customers[i].phone);
    fclose(f);
}
void loadCustomers(void) {
    FILE *f = fopen("customers.txt", "r");
    if (!f) return;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        Customer c;
        if (sscanf(line, "%d,%49[^,],%19[^\n]",
                   &c.id, c.name, c.phone) == 3) {
            if (custCount < MAX) customers[custCount++] = c;
        }
    }
    fclose(f);
}

// ---------------- Persistence: Suppliers ----------------

//--------Function Prototypes for Suppliers---------------

void saveSuppliers(void);
void loadSuppliers(void);
void supplierMenu(void);

void saveSuppliers(void) {
    FILE *f = fopen("suppliers.txt", "w");
    if (!f) { printf("Error: cannot write suppliers.txt\n"); return; }
    for (int i = 0; i < suppCount; i++)
        fprintf(f, "%d,%s,%s\n",
                suppliers[i].id, suppliers[i].name, suppliers[i].product);
    fclose(f);
}
void loadSuppliers(void) {
    FILE *f = fopen("suppliers.txt", "r");
    if (!f) return;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        Supplier s;
        if (sscanf(line, "%d,%49[^,],%49[^\n]",
                   &s.id, s.name, s.product) == 3) {
            if (suppCount < MAX) suppliers[suppCount++] = s;
        }
    }
    fclose(f);
}

// ---------------- Persistence: Sales total ----------------

//--------Function Prototypes for Sales Total ---------------

void saveSales(void);
void loadSales(void);
void salesReport(void);

void saveSales(void) {
    FILE *f = fopen("sales.txt", "w");
    if (!f) { printf("Error: cannot write sales.txt\n"); return; }
    fprintf(f, "%.2f\n", totalSales);
    fclose(f);
}
void loadSales(void) {
    FILE *f = fopen("sales.txt", "r");
    if (!f) return;
    fscanf(f, "%f", &totalSales);
    fclose(f);
}

// ---------------- Bill ID Sequence ----------------
int nextBillId(void) {
    int id = 0;
    FILE *f = fopen("billseq.txt", "r");
    if (f) {
        fscanf(f, "%d", &id);
        fclose(f);
    }
    id += 1;
    f = fopen("billseq.txt", "w");
    if (f) { fprintf(f, "%d\n", id); fclose(f); }
    return id;
}

// ---------------- Bill Index (summary list) ----------------

//--------Function Prototypes for bills ---------------

void appendBillIndex(int billId, const char *date, const char *customer, float total);
void listBills(void);
void viewBillById(void);

void appendBillIndex(int billId, const char *date, const char *customer, float total) {
    FILE *f = fopen("bills_index.txt", "a");
    if (!f) { printf("Error: cannot append bills_index.txt\n"); return; }
    fprintf(f, "%d,%s,%s,%.2f\n", billId, date, customer, total);
    fclose(f);
}

void listBills(void) {
    FILE *f = fopen("bills_index.txt", "r");
    if (!f) { printf("No bills yet.\n"); return; }
    char line[256];
    printf("\n---- Bill History (ID, Date, Customer, Total) ----\n");
    while (fgets(line, sizeof(line), f)) {
        int id; char date[20], customer[60]; float total;
        if (sscanf(line, "%d,%19[^,],%59[^,],%f", &id, date, customer, &total) == 4) {
            printf("%d | %-12s | %-15s | %.2f\n", id, date, customer, total);
        }
    }
    fclose(f);
}

void viewBillById(void) {
    int id;
    printf("Enter Bill ID to view: ");
    scanf("%d", &id);
    char fname[64];
    snprintf(fname, sizeof(fname), "bill_%d.txt", id);
    FILE *f = fopen(fname, "r");
    if (!f) { printf("Bill not found.\n"); return; }
    printf("\n----- BILL #%d -----\n", id);
    char ch;
    while ((ch = (char)fgetc(f)) != EOF) putchar(ch);
    fclose(f);
    printf("\n--------------------\n");
}

// ==========================================================
// FEATURE 1: Food Items Management (CRUD + Expiry Check)
// ==========================================================
void foodMenu(void) {
    int choice;
    do {
        printf("\n--- Food Management ---\n");
        printf("1. Add\n2. Display\n3. Search\n4. Delete\n5. Modify\n6. Check Expired\n7. Back\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Food f;
            printf("ID: "); scanf("%d", &f.id);
            printf("Name: "); scanf(" %[^\n]", f.name);
            printf("Category: "); scanf(" %[^\n]", f.category);
            printf("Quantity: "); scanf("%d", &f.quantity);
            printf("Expiry (DD/MM/YYYY): "); scanf("%14s", f.expiryDate);
            if (foodCount < MAX) foods[foodCount++] = f;
            saveFoods();
            printf("Food Item added.\n");
        }
        else if (choice == 2) {
            if (foodCount == 0) { printf("No foods.\n"); continue; }
            printf("=================================== Available FOOD ITEMS ============================\n");
            printf("\n%-15s | %-20s | %-15s | %-12s | %-12s\n", "ID","Name", "Category", "Quantity", "Expiry Date");
            printf("--------------------------------------------------------------------------------------\n");
            for (int i = 0; i < foodCount ; i++)
                { printf("%-15d | %-20s | %-15s | %-12d | %-12s\n",
                    foods[i].id,foods[i].name, foods[i].category, foods[i].quantity, foods[i].expiryDate); }

        }
        else if (choice == 3) {
            int id; printf("Enter Food ID: "); scanf("%d", &id);
            int found = 0;
            for (int i = 0; i < foodCount; i++)
                if (foods[i].id == id) {
                    printf("\nItem Found !!\n");
                   printf("ID : %-5d | Name : %-15s | Category : %-15s | Quantity : %-5d | Expiry Date : %-12s\n",
                    foods[i].id,foods[i].name, foods[i].category, foods[i].quantity, foods[i].expiryDate);


                    found = 1; break;
                }
            if (!found) printf("Not found.\n");
        }
        else if (choice == 4) {
            int id; printf("Food ID to delete: "); scanf("%d", &id);
            int idx = -1;
            for (int i = 0; i < foodCount; i++) if (foods[i].id == id) { idx = i; break; }
            if (idx >= 0) {
                for (int j = idx; j < foodCount - 1; j++) foods[j] = foods[j + 1];
                foodCount--; saveFoods(); printf("Food Item successfully Deleted!\n");
            } else printf("Not found.\n");
        }
        else if (choice == 5) {
            int id; printf("ID to modify: "); scanf("%d", &id);
            int idx = -1;
            for (int i = 0; i < foodCount; i++) if (foods[i].id == id) { idx = i; break; }
            if (idx >= 0) {
                printf("New Name: ");     scanf(" %[^\n]", foods[idx].name);
                printf("New Category: "); scanf(" %[^\n]", foods[idx].category);
                printf("New Quantity: "); scanf("%d", &foods[idx].quantity);
                printf("New Expiry: ");   scanf("%14s", foods[idx].expiryDate);
                saveFoods(); printf("Updated.\n");
            } else printf("Not found.\n");
        }
        else if (choice == 6) {
            char today[15]; printf("Today's date (DD/MM/YYYY): "); scanf("%14s", today);
            int any = 0;
            for (int i = 0; i < foodCount; i++)
                if (isExpired(foods[i].expiryDate, today)) {
                    printf("Expired: %s (ID %d) on %s\n", foods[i].name, foods[i].id, foods[i].expiryDate);
                    any = 1;
                }
            if (!any) printf("No expired items.\n");
        }
    } while (choice != 7);
}

// ==========================================================
// FEATURE 2: Employee Management (CRUD)
// ==========================================================
void employeeMenu(void) {
    int choice;
    do {
        printf("\n--- Employee Management ---\n");
        printf("1. Add\n2. Display\n3. Search\n4. Delete\n5. Modify\n6. Back\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Employee e;
            printf("ID: "); scanf("%d", &e.id);
            printf("Name: "); scanf(" %[^\n]", e.name);
            printf("Role: "); scanf(" %[^\n]", e.role);
            printf("Salary: "); scanf("%f", &e.salary);
            if (empCount < MAX) employees[empCount++] = e;
            saveEmployees(); printf("Employee added.\n");
        }
        else if (choice == 2) {
            if (empCount == 0) { printf("No employees.\n"); continue; }
            printf("\n%-7s | %-20s | %-15s | %-12s | %-12s\n","ID","Name","Role","Salary");
            printf("--------------------------------------------------------\n");
            for (int i = 0; i < empCount; i++)
                printf("%-7d | %-20s | %-15s | %.2f\n",
                       employees[i].id, employees[i].name, employees[i].role, employees[i].salary);
        }
        else if (choice == 3) {
            int id; printf("Enter ID: "); scanf("%d", &id);
            int found = 0;
            for (int i = 0; i < empCount; i++)
                if (employees[i].id == id) {
                    printf("ID: %-7d | Name : %-20s | Role : %-15s | Salary : %.2f\n",
                           employees[i].id, employees[i].name, employees[i].role, employees[i].salary);
                    found = 1; break;
                }
            if (!found) printf("Not found.\n");
        }
        else if (choice == 4) {
            int id; printf("ID to delete: "); scanf("%d", &id);
            int idx = -1;
            for (int i = 0; i < empCount; i++) if (employees[i].id == id) { idx = i; break; }
            if (idx >= 0) {
                for (int j = idx; j < empCount - 1; j++) employees[j] = employees[j + 1];
                empCount--; saveEmployees(); printf("Deleted.\n");
            } else printf("Not found.\n");
        }
        else if (choice == 5) {
            int id; printf("ID to modify: "); scanf("%d", &id);
            int idx = -1;
            for (int i = 0; i < empCount; i++) if (employees[i].id == id) { idx = i; break; }
            if (idx >= 0) {
                printf("New Name: ");  scanf(" %[^\n]", employees[idx].name);
                printf("New Role: ");  scanf(" %[^\n]", employees[idx].role);
                printf("New Salary: ");scanf("%f", &employees[idx].salary);
                saveEmployees(); printf("Updated.\n");
            } else printf("Not found.\n");
        }
    } while (choice != 6);
}

// ============================================================
// FEATURE 3: Customer (Membership Owners) Management (CRUD)
// ============================================================
void customerMenu(void) {
    int choice;
    do {
        printf("\n--- Customer (Membership Owners) Management ---\n");
        printf("1. Add\n2. Display\n3. Search\n4. Delete\n5. Modify\n6. Back\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Customer c;
            printf("ID: "); scanf("%d", &c.id);
            printf("Name: "); scanf(" %[^\n]", c.name);
            printf("Phone: "); scanf("%19s", c.phone);
            if (custCount < MAX) customers[custCount++] = c;
            saveCustomers(); printf("Customer added.\n");
        }
        else if (choice == 2) {
            if (custCount == 0) { printf("No customers.\n"); continue; }
            printf("\n%-7s | %-20s | %-15s\n","ID","Name","Phone");
            printf("-----------------------------------------------\n");
            for (int i = 0; i < custCount; i++)
                printf("%-7d | %-20s | %-15s\n",customers[i].id, customers[i].name, customers[i].phone);
        }
        else if (choice == 3) {
            int id; printf("Enter ID: "); scanf("%d", &id);
            int found = 0;
            for (int i = 0; i < custCount; i++)
                if (customers[i].id == id) {
                    printf("Id : %-5d | Name : %-20s | Phone no : %-15s\n",
                           customers[i].id, customers[i].name, customers[i].phone);
                    found = 1; break;
                }
            if (!found) printf("Not found.\n");
        }
        else if (choice == 4) {
            int id; printf("ID to delete: "); scanf("%d", &id);
            int idx = -1;
            for (int i = 0; i < custCount; i++) if (customers[i].id == id) { idx = i; break; }
            if (idx >= 0) {
                for (int j = idx; j < custCount - 1; j++) customers[j] = customers[j + 1];
                custCount--; saveCustomers(); printf("Deleted.\n");
            } else printf("Not found.\n");
        }
        else if (choice == 5) {
            int id; printf("ID to modify: "); scanf("%d", &id);
            int idx = -1;
            for (int i = 0; i < custCount; i++) if (customers[i].id == id) { idx = i; break; }
            if (idx >= 0) {
                printf("New Name: ");  scanf(" %[^\n]", customers[idx].name);
                printf("New Phone: "); scanf("%19s", customers[idx].phone);
                saveCustomers(); printf("Updated.\n");
            } else printf("Not found.\n");
        }
    } while (choice != 6);
}

// ==========================================================
// FEATURE 4: Billing (multi-line items, saves each bill file)
// also updates foods stock and totalSales
// ==========================================================
void billingSystem(void) {
    char date[20], customer[60];
    printf("\n--- New Bill ---\n");
    printf("Bill Date (DD/MM/YYYY): "); scanf("%19s", date);
    printf("Customer Name: ");          scanf(" %[^\n]", customer);

    float total = 0.0f;
    while (1) {
        int fid, qty;
        char more;
        printf("\nEnter Food ID (or -1 to finish): ");
        scanf("%d", &fid);
        if (fid == -1) break;

        int idx = -1;
        for (int i = 0; i < foodCount; i++) if (foods[i].id == fid) { idx = i; break; }
        if (idx < 0) { printf("Food not found.\n"); continue; }

        printf("Quantity to buy: ");
        scanf("%d", &qty);
        if (qty <= 0) { printf("Invalid quantity.\n"); continue; }
        if (foods[idx].quantity < qty) { printf("Not enough stock.\n"); continue; }

        float price;
        printf("Price per unit: ");
        scanf("%f", &price);
        if (price < 0) { printf("Invalid price.\n"); continue; }

        float line = qty * price;
        total += line;
        foods[idx].quantity -= qty;
        printf("Added: %s x%d @ %.2f = %.2f (Remaining stock: %d)\n",
               foods[idx].name, qty, price, line, foods[idx].quantity);
        saveFoods(); // persist stock change asap
        printf("Add another item? (y/n): ");
        scanf(" %c", &more);
        if (more != 'y' && more != 'Y') break;
    }

    if (total <= 0.0f) {
        printf("No items added. Bill cancelled.\n");
        return;
    }

    int billId = nextBillId();

    // Save full bill to its own file
    char fname[64];
    snprintf(fname, sizeof(fname), "bill_%d.txt", billId);
    FILE *bf = fopen(fname, "w");
    if (!bf) { printf("Error: cannot write bill file.\n"); return; }
    fprintf(bf, "BILL ID: %d\nDATE: %s\nCUSTOMER: %s\n", billId, date, customer);
    fprintf(bf, "----------------------------------------\n");
    fprintf(bf, "TOTAL: %.2f\n", total);
    fclose(bf);

    // Append to bill index
    appendBillIndex(billId, date, customer, total);

    // Update total sales
    totalSales += total;
    saveSales();

    printf("\nBill saved as %s\n", fname);
}

// ==========================================================
// FEATURE 5: Stock Overview
// ==========================================================
void stockManagement(void) {
    printf("\n-------------- Stock Overview -------------\n");
    if (foodCount == 0) { printf("No foods.\n"); return; }
    printf("%-7s | %-20s | %-10s\n","ID","Name","Quantity");
    printf("--------------------------------------------\n");
    for (int i = 0; i < foodCount; i++)
        printf("%-7d | %-20s | %-7d\n", foods[i].id, foods[i].name, foods[i].quantity);
}

// ==========================================================
// FEATURE 6: Sales Report
// ==========================================================
void salesReport(void) {
    printf("\n--- Sales Report ---\n");
    printf("Total Sales: %.2f\n", totalSales);
    printf("For individual bills, open Bill History (option 8).\n");
}


// ==========================================================
// FEATURE 7: Supplier Management
// ==========================================================
void supplierMenu(void) {
    int choice;
    do {
        printf("\n--- Supplier Management ---\n");
        printf("1. Add\n2. Display\n3. Search\n4. Delete\n5. Modify\n6. Back\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Supplier s;
            printf("ID: "); scanf("%d", &s.id);
            printf("Name: "); scanf(" %[^\n]", s.name);
            printf("Product: "); scanf(" %[^\n]", s.product);
            if (suppCount < MAX) suppliers[suppCount++] = s;
            saveSuppliers(); printf("Supplier added.\n");
        }
        else if (choice == 2) {
            if (suppCount == 0) { printf("No suppliers.\n"); continue; }
            printf("\n%-7s | %-20s | %-15s\n","ID","Name","Product");
            printf("------------------------------------------\n");
            for (int i = 0; i < suppCount; i++)
                printf("%-7d | %-20s | %-15s\n", suppliers[i].id, suppliers[i].name, suppliers[i].product);
        }
        else if (choice == 3) {
            int id; printf("Enter ID: "); scanf("%d", &id);
            int found = 0;
            for (int i = 0; i < suppCount; i++)
                if (suppliers[i].id == id) {
                    printf("ID : %-7d | Name : %-20s | Product : %-15s\n", suppliers[i].id, suppliers[i].name, suppliers[i].product);
                    found = 1; break;
                }
            if (!found) printf("Not found.\n");
        }
        else if (choice == 4) {
            int id; printf("ID to delete: "); scanf("%d", &id);
            int idx = -1;
            for (int i = 0; i < suppCount; i++) if (suppliers[i].id == id) { idx = i; break; }
            if (idx >= 0) {
                for (int j = idx; j < suppCount - 1; j++) suppliers[j] = suppliers[j + 1];
                suppCount--; saveSuppliers(); printf("Deleted.\n");
            } else printf("Not found.\n");
        }
        else if (choice == 5) {
            int id; printf("ID to modify: "); scanf("%d", &id);
            int idx = -1;
            for (int i = 0; i < suppCount; i++) if (suppliers[i].id == id) { idx = i; break; }
            if (idx >= 0) {
                printf("New Name: ");     scanf(" %[^\n]", suppliers[idx].name);
                printf("New Product: ");  scanf(" %[^\n]", suppliers[idx].product);
                saveSuppliers(); printf("Updated.\n");
            } else printf("Not found.\n");
        }
    } while (choice != 6);
}


// ==========================================================
// FEATURE 8: Bill History (list & view)
// ==========================================================
void billHistoryMenu(void) {
    int choice;
    do {
        printf("\n--- Bill History ---\n");
        printf("1. List All Bills\n2. View Bill by ID\n3. Back\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) listBills();
        else if (choice == 2) viewBillById();
    } while (choice != 3);
}


// ==========================================================
// FEATURE 9: Adding Feedback
// ==========================================================
// ---------------- Feedback System ----------------
void feedbackSystem() {
    FILE *fp = fopen("feedback.txt", "a");
    if (fp == NULL) {
        printf("Error opening feedback file!\n");
        return;
    }

    char name[50], feedback[200];
    int rating;

    printf("\nEnter your name: ");
    scanf(" %[^\n]", name);

    printf("Enter your feedback: ");
    scanf(" %[^\n]", feedback);

    printf("Rate us (1-5): ");
    scanf("%d", &rating);

    fprintf(fp, "Name: %s | Rating: %d | Feedback: %s\n", name, rating, feedback);
    fclose(fp);

    printf("\nThank you for your feedback!\n");
}


// ==========================================================
// FEATURE 10: Viewing Feedbacks
// ==========================================================
void viewfeedback() {
    FILE *fp = fopen("feedback.txt", "r");
    if (fp == NULL) {
        printf("\nNo feedbacks available yet!\n");
        return;
    }

    char line[300];
    printf("\n===== Customer Feedbacks =====\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}



// ==========================================================
// Login
// ==========================================================
int login(void) {
    char u[32], p[32];
    printf("\n==== User Login ====\nUsername: ");
    scanf("%31s", u);
    printf("Password: ");
    scanf("%31s", p);
    if (strcmp(u,"TheDebuggers") == 0 && strcmp(p, "4321") == 0)
    {
        printf("Login successful.\n");
        return 1;
    }
    printf("Invalid credentials.\n");
    return 0;
}

// ==========================================================
// Main Menu (10 options)
// ==========================================================
int main(void) {
    loadFoods(); loadEmployees(); loadCustomers(); loadSuppliers(); loadSales();

    if (!login()) return 0;

    int choice;
    do {
        printf("\n===== Grocery Shop Management System =====\n");
        printf("1.  Food Items Management\n");
        printf("2.  Employee Management\n");
        printf("3.  Customer (Membership Owners) Management\n");
        printf("4.  Billing System \n");
        printf("5.  Stock Management\n");
        printf("6.  Sales Report\n");
        printf("7.  Supplier Management\n");
        printf("8.  Bill History (list/view)\n");
        printf("9.  Add Feedback\n");
        printf("10. View Feedback\n");
        printf("11. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:  foodMenu();        break;
            case 2:  employeeMenu();    break;
            case 3:  customerMenu();    break;
            case 4:  billingSystem();   break;
            case 5:  stockManagement(); break;
            case 6:  salesReport();     break;
            case 7:  supplierMenu();    break;
            case 8:  billHistoryMenu(); break;
            case 9:  feedbackSystem();  break;
            case 10: viewfeedback();    break;
            case 11: printf("Exiting... Bye!\n");
                      return 0;
                       break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 20);

    return 0;
}
