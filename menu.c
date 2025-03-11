#include "menu.h"
void showMenu(unsigned int employeeLevel)
{
    PracticionerMenu();  // Default menu for all employee levels
    if (employeeLevel == 1) {
        EmployeeMenu();
        AdminMenu();
    }
    else if (employeeLevel == 2) {
        EmployeeMenu();
    }

}
void PracticionerMenu()
{
    printf("\n============> ToyShop MENU <============= \n");
	printf("Choose your operation:\n\n");
	printf("1  - Search for an item\n");
	printf("2  - Adding a new item to inventory\n");
	printf("3  - Adding a new customer\n");
    printf("4  - Change password\n");// Our addition to the functionality of the project
    printf("5  - Log out and log in as another user\n");
    printf("6  - View a customer's purchase history\n");
}
void EmployeeMenu()
{
    printf("7  - Removing an existing item\n");
    printf("8  - Removing an existing customer\n");
    printf("9  - Update an item\n");
    printf("10 - Sell product to customer\n");
    printf("11 - Returning a product purchased by a customer (within 14 days) to the store\n");
    printf("12 - Registering a customer as a club member\n"); // Our addition to the functionality of the project
}
void AdminMenu()
{
    printf("13 - Adding a new employees\n");
    printf("14 - Upgrading an employee's wage\n");// Our addition to the functionality of the project
}