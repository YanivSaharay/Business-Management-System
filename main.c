#include "customers.h"
#include "employees.h"
#include "items.h"
#include "menu.h"
#include "log.h"

void WhichOperationToDo(Employee emp);

int main() 
{

    if (!checkIfEmployeeFileExists())   
        createDefaultAdmin();
   
    Employee currentEmployee = login();
    if (currentEmployee.level == 0) {
        printf("Login failed. Try again later..\n");
        return 0;
    }

    printf("\nYou have successfully logged in to the system you are logged on as:\n");
    for(int i =0; i< 20; i++)
        printf("%c", currentEmployee.username[i]);
    printf("\n");
    printf("with a permission level of % d\n", currentEmployee.level);
   
    showMenu(currentEmployee.level);
    WhichOperationToDo(currentEmployee);
    return 0;
}

/* A function that detects which operation is selected to do and performs it*/
void WhichOperationToDo(Employee emp)
{ 
 
    FILE* employee_file = NULL;
    node* root_items = NULL;
    node_Customer* root_customer = NULL;

    employee_file = fopen("Employees.txt", "r+");
    assert(employee_file);
    int size;
    int id;
    int OperationChoice;
    Reload_Existing_Items_To_The_Tree(&root_items);
    Reload_Existing_Customers_To_The_Tree(&root_customer);
    printf("Your choice:");
    scanf("%d", &OperationChoice);
    do
    {
      //  Reload_Existing_Items_To_The_Tree(&root_items);
      //  Reload_Existing_Customers_To_The_Tree(&root_customer);
    switch (OperationChoice)
    {
        case 1: //Search for an item
            SearchItem(root_items);
            break;

        case 2: //Adding a new item to inventory
            AddNewItem(&root_items);
            printf("Id             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
            print_inorder(root_items);
            break;

        case 3: //Adding a new customer
            AddNewCustomer(&root_customer);
            printf("Id             Name                Total purchases amount        Club member?       Birthday   Join date\n");
            print_customer_inorder(root_customer);
            break;

        case 4: //Change password
            ChangePass(emp);
            break;

        case 5: //Log out and log in as another user
            getchar();
            Employee currentEmployee = login(); 
            if (currentEmployee.level == 0) {
                printf("Login failed. Try again later..\n");
                return 0;
            }
            printf("You have successfully logged in to the system you are logged on as:\n");
            for (int i = 0; i < 20; i++)
                printf("%c", currentEmployee.username[i]);
            printf("\n");
            printf("with a permission level of % d\n", currentEmployee.level);
            showMenu(currentEmployee.level);
            emp.level = currentEmployee.level;
            break;
        
        case 6://View a customer's purchase history
            node_Customer* TEMP6 = NULL;
            printf("Please enter the customer ID number for which you would like to view the purchases history\n");
            scanf("%d", &id);
            TEMP6 = SearchCustomerID(&root_customer, id);
            if (TEMP6)
            {
                if (Number_Of_Purchases(TEMP6->data.purchased_items) == 0)
                    printf("There is no purchases history for this customer\n");
                Print_Purchases_History(TEMP6->data.purchased_items);
            }
            else
                printf("The ID number dost not exist.. Please try again later\n\n");
            break;

        case 7: //Removing an existing item
            if (emp.level == 1 || emp.level == 2)
            {
                printf("Please enter an ID number for the item you would like to remove\n");
                scanf("%d", &id);
                root_items = RemoveItem(root_items, id);
                printf("Id             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
                print_inorder(root_items);
            }
            else 
                printf("\n\n==> Choice is not on the list.. Try Again..!\n");
            break;
    
        case 8: //Removing an existing customer
            if (emp.level == 1 || emp.level == 2)
            {
                node_Customer* TEMP8 = NULL;
                printf("Please enter an ID number for the customer you would like to remove\n");
                scanf("%d", &id);
                TEMP8 = SearchCustomerID(&root_customer, id);
                if (TEMP8)   
                    root_customer = RemoveCustomer(root_customer, TEMP8->data.join_date);
                else
                    printf("The ID number dost not exist.. Please try again later\n\n");
                printf("Id             Name                Total purchases amount        Club member?       Birthday   Join date\n");
                print_customer_inorder(root_customer);
            }
           else
                printf("\n\n==> Choice is not on the list.. Try Again..!\n");

            break;
        case 9: //Update an item
            if (emp.level == 1 || emp.level == 2)
            { 
                root_items = UpdateItem(root_items);
                printf("Id             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
                print_inorder(root_items);
            }
            else
                printf("\n\n==> Choice is not on the list.. Try Again..!\n");
            break;
        
        case 10: //Sell product to customer
            if (emp.level == 1 || emp.level == 2)
                SellItemToCustomer(root_customer, root_items);    
            else
                printf("\n\n==> Choice is not on the list.. Try Again..!\n");
            break;

        case 11: // Returning a product purchased by a customer to the store
            node_Customer * TEMP11 = NULL;
            printf("Please enter the ID number of the customer who wants to return the product to the store\n");
            scanf("%d", &id);
            TEMP11 = SearchCustomerID(&root_customer, id);
            if (TEMP11)
            {
                if (Number_Of_Purchases(TEMP11->data.purchased_items) == 0)
                    printf("There is no purchases history for this customer\nReturning the item to the store is not relevant\n");
                else
                {
                    ReturnItems(id, TEMP11->data.purchased_items, root_items);
                    printf("Id             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
                    print_inorder(root_items);
                }
            }
            else
                printf("The ID number dost not exist.. Please try again later\n\n");
            break;

        case 12: //Registering a customer as a club member (existing customer)
            if (emp.level == 1 || emp.level == 2)
            {
                node_Customer* TEMP12 = NULL;
                printf("please enter the customer's id you would like to register as a club member\n");
                scanf("%d", &id);
                TEMP12 = SearchCustomerID(&root_customer, id);
                if (TEMP12 && !TEMP12->data.club_member)
                {
                    TEMP12->data.club_member = true;
                    DocChangeToClubMember(TEMP12->data);
                }
                else if (TEMP12 && TEMP12->data.club_member)
                    printf("The customer is already registered as a club member\n\n");
                else
                    printf("The ID number dost not exist.. Please try again later\n\n");
                printf("Id             Name                Total purchases amount        Club member?       Birthday     Join date\n");
                print_customer_inorder(root_customer);
            }
            else
                printf("\n\n==> Choice is not on the list.. Try Again..!\n");
            break;

        case 13: //Adding a new employees 
                /*Adding the customers to the file is performed at the end,
                so duplicate usernames will not be handled in such a case (A built-in solution exists for the rest of the cases)*/
            if (emp.level == 1)
            {
                printf("\nHow many new employees do you want to update?\n");
                scanf("%d", &size);
                AddNewEmployee(size);
            }
            else
                printf("\n\n==> Choice is not on the list.. Try Again..!\n");
            break;

        case 14: //Upgrading an employee's wage
            if (emp.level == 1)
                UpgradeWage(emp);
            else
                printf("\n\n==> Choice is not on the list.. Try Again..!\n");
            break;
        
        default:
            printf("\n\n==> Choice is not on the list.. Try Again..!\n");
            break;
    }
        printf("\n\n==> To log out and close the system press -1\n");
        printf("Your choice:");
        scanf("%d", &OperationChoice);
    }while (OperationChoice != -1);

    /* Before the program ends, we load the data into files*/
    WriteToItemsFile(root_items);
    FILE* Transaction_File = fopen("Transactions.txt", "w");
    fclose(Transaction_File);
    WriteToCustomerssFile(root_customer);
 //   TransactionsDocumentations(root_customer, root_customer->data.purchased_items);
}
