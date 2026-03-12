#include <stdio.h>
#include <string.h>

#define MAX 50
#define LOW_STOCK 5

struct Product {
    int id;
    char name[50];
    int quantity;
    float price;
};

struct Product inventory[MAX];
int count = 0;
int billNo = 1;

void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getChoice() {
    int choice;
    if (scanf("%d", &choice) != 1) {
        flushInput();
        return -1;
    }
    return choice;
}

void addProduct() {
    printf("\nEnter Product ID: ");
    scanf("%d", &inventory[count].id);
    printf("Enter Product Name: ");
    scanf("%s", inventory[count].name);
    printf("Enter Quantity: ");
    scanf("%d", &inventory[count].quantity);
    printf("Enter Price: ");
    scanf("%f", &inventory[count].price);
    count++;
    printf("Product added successfully!\n");
}

void viewProducts() {
    if (count == 0) {
        printf("\nNo products found.\n");
        return;
    }
    printf("\n--- Product List ---\n");
    printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Qty", "Price");
    printf("----------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %-10d %-10.2f",
               inventory[i].id,
               inventory[i].name,
               inventory[i].quantity,
               inventory[i].price);
        if (inventory[i].quantity < LOW_STOCK)
            printf("  << LOW STOCK");
        printf("\n");
    }
}

void searchProduct() {
    char name[50];
    printf("\nEnter product name to search: ");
    scanf("%s", name);
    int found = 0;
    printf("\n%-5s %-20s %-10s %-10s\n", "ID", "Name", "Qty", "Price");
    printf("----------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        if (strstr(inventory[i].name, name) != NULL) {
            printf("%-5d %-20s %-10d %-10.2f\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].quantity,
                   inventory[i].price);
            found = 1;
        }
    }
    if (!found) printf("No product found with that name.\n");
}

void deleteProduct() {
    int id;
    printf("\nEnter Product ID to delete: ");
    scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (inventory[i].id == id) {
            for (int j = i; j < count - 1; j++)
                inventory[j] = inventory[j + 1];
            count--;
            printf("Product deleted successfully!\n");
            return;
        }
    }
    printf("Product not found!\n");
}

void generateBill() {
    int id, qty;
    float total = 0;
    char choice;
    char billLines[20][100];
    int lineCount = 0;

    printf("\n--- Generate Bill ---\n");

    do {
        printf("Enter Product ID: ");
        scanf("%d", &id);

        int found = -1;
        for (int i = 0; i < count; i++) {
            if (inventory[i].id == id) { found = i; break; }
        }

        if (found == -1) {
            printf("Product not found!\n");
        } else {
            printf("Enter Quantity: ");
            scanf("%d", &qty);
            if (qty > inventory[found].quantity) {
                printf("Not enough stock! Available: %d\n", inventory[found].quantity);
            } else {
                float amount = qty * inventory[found].price;
                total += amount;
                inventory[found].quantity -= qty;
                printf("%s x %d = Rs %.2f\n", inventory[found].name, qty, amount);
                sprintf(billLines[lineCount++], "%-20s x%-3d  Rs %.2f",
                        inventory[found].name, qty, amount);
            }
        }

        printf("Add more items? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    float tax = total * 0.18;
    float grandTotal = total + tax;

    printf("\n---------------------------\n");
    printf("Subtotal : Rs %.2f\n", total);
    printf("Tax(18%%): Rs %.2f\n", tax);
    printf("Total    : Rs %.2f\n", grandTotal);
    printf("---------------------------\n");

    printf("Save bill to file? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        char filename[30];
        sprintf(filename, "bill_%d.txt", billNo);
        FILE *fp = fopen(filename, "w");
        fprintf(fp, "====== BILL No: %d ======\n", billNo);
        for (int i = 0; i < lineCount; i++)
            fprintf(fp, "%s\n", billLines[i]);
        fprintf(fp, "------------------------\n");
        fprintf(fp, "Subtotal : Rs %.2f\n", total);
        fprintf(fp, "Tax(18%%): Rs %.2f\n", tax);
        fprintf(fp, "Total    : Rs %.2f\n", grandTotal);
        fprintf(fp, "========================\n");
        fclose(fp);
        printf("Bill saved as %s\n", filename);
        billNo++;
    }
}

void updateStock() {
    int id, qty;
    printf("\nEnter Product ID: ");
    scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (inventory[i].id == id) {
            printf("Enter quantity to add: ");
            scanf("%d", &qty);
            inventory[i].quantity += qty;
            printf("Stock updated! New quantity: %d\n", inventory[i].quantity);
            return;
        }
    }
    printf("Product not found!\n");
}

int main() {
    int choice;
    printf("=== Billing and Inventory Management ===\n");
    do {
        printf("\n1. Add Product\n");
        printf("2. View Products\n");
        printf("3. Search Product\n");
        printf("4. Generate Bill\n");
        printf("5. Update Stock\n");
        printf("6. Delete Product\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        choice = getChoice();
        switch (choice) {
            case 1: addProduct();    break;
            case 2: viewProducts();  break;
            case 3: searchProduct(); break;
            case 4: generateBill();  break;
            case 5: updateStock();   break;
            case 6: deleteProduct(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice! Enter a number 0-6.\n");
        }
    } while (choice != 0);
    return 0;
}