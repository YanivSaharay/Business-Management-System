#include "customers.h"
#include "log.h"

const name_lim = 20;
const birthday_lim = 11;
const date_lim = 80;
const customer_line = 120;
const transaction_line = 100;

/* A function to add new customer to data structure - "Save function" */
void AddNewCustomer(node_Customer** root)
{
	Customer cstm;
	node_Customer* element = root;
	getchar(); // To handling with enter key
	printf("Adding a new customer\n");
	printf("Please enter the customer name\n");
	PaddingCustomerInfo(&cstm.name, name_lim);

	printf("Please enter customer id and press enter\n");
	scanf("%d", &cstm.customer_id);
	element = SearchCustomerID(element, cstm.customer_id);
	while (element)
	{
		printf("Sorry, but the id is already exist please assign a new id\n");
		scanf("%d", &cstm.customer_id);
		element = SearchCustomerID(root, cstm.customer_id);
	}
	cstm.purchases_amount = 0;

	char IsClubMemeber;
	getchar();
	printf("Enter Y if the customer wants to register as a club member, otherwise type N\n");
	scanf("%c", &IsClubMemeber);
	while (IsClubMemeber != 'Y' && IsClubMemeber != 'N')
	{
		getchar();
		printf("Invalid choice.. Please try again\n");
		scanf("%c", &IsClubMemeber);
	}
	if (IsClubMemeber == 'Y')
		cstm.club_member = true;
	else if (IsClubMemeber == 'N')
		cstm.club_member = false;

	getchar();
	printf("Please enter the date of the customer's birthday (in the following format: DD-MM-YYYY)\n");
	for (int i = 0; i < birthday_lim - 1; i++)
	{
		scanf("%c", &cstm.birthday[i]);
	}
	cstm.birthday[10] = ' ';
	/* ASSUMPTION: The entered date is correct we will only check whether it matches the request format*/
	while (cstm.birthday[2] != '-' || cstm.birthday[5] != '-')
	{
		getchar();
		printf("Invalid date.. Please try again\n");
		for (int i = 0; i < birthday_lim - 1; i++)
			scanf("%c", &cstm.birthday[i]);
		cstm.birthday[10] = ' ';
	}

	time_t currentTime;
	struct tm* timeInfo;
	time(&currentTime); // Get the current epoch time
	timeInfo = localtime(&currentTime);
	strftime(cstm.join_date, sizeof(cstm.join_date), "%d-%m-%Y %H:%M:%S", timeInfo);
	cstm.purchased_items = NULL;

	insertHead_Customer(root, cstm);
	DocNewCustomer(cstm);

}

/*A function to receive the customer data and pass them by reference.*/
void PaddingCustomerInfo(char* arr, int len)
{
	char c;
	int index = 0;
	while (index < len)
	{
		scanf("%c", &c);
		if (c == '\n')
			break;
		*(arr + index) = c;
		index++;
	}
	while (index < len)
		arr[index++] = ' ';
	return arr;
}

/*Auxiliary function for writing customers to a file.*/
void WriteToCustomerssFile(node_Customer* root)
{
	FILE* Customer_File = NULL;
	Customer_File = fopen("Customers.txt", "r");
	if (!Customer_File) // for the first time
	{
		Customer_File = fopen("Customers.txt", "a");
		assert(Customer_File);
	}
	else
	{
		fclose(Customer_File);
		Customer_File = fopen("Customers.txt", "w");
		assert(Customer_File);
	}
	CreateCustomersTitle(Customer_File);
	Inorder_Writing_Customers(root, Customer_File);
	fclose(Customer_File);
}

/* A function to create title for customers file.*/
void CreateCustomersTitle(FILE* CustomersFile)
{
	int pos1, pos2;

	pos1 = ftell(CustomersFile);

	fprintf(CustomersFile, "ID");
	pos2 = ftell(CustomersFile);
	CreateCustomersPadding(CustomersFile, pos2, pos1 + 10);
	pos1 = ftell(CustomersFile);

	fprintf(CustomersFile, "Name");
	pos2 = ftell(CustomersFile);
	CreateCustomersPadding(CustomersFile, pos2, pos1 + name_lim + 10);
	pos1 = ftell(CustomersFile);

	fprintf(CustomersFile, " Total purchases amount");
	pos2 = ftell(CustomersFile);
	CreateCustomersPadding(CustomersFile, pos2, pos1 + 25);
	pos1 = ftell(CustomersFile);

	fprintf(CustomersFile, " Club member?");
	pos2 = ftell(CustomersFile);
	CreateCustomersPadding(CustomersFile, pos2, pos1 + 18);
	pos1 = ftell(CustomersFile);

	fprintf(CustomersFile, " Birthday");
	pos2 = ftell(CustomersFile);
	CreateCustomersPadding(CustomersFile, pos2, pos1 + birthday_lim + 2);
	pos1 = ftell(CustomersFile);

	fprintf(CustomersFile, " Join date");
	pos2 = ftell(CustomersFile);
	CreateCustomersPadding(CustomersFile, pos2, pos1 + 20 + 2);
	pos1 = ftell(CustomersFile);

	fprintf(CustomersFile, "\n");
	/*Total chars in line = 119 + /n = 120*/
}

/* A function to create padding for customers file.*/
void CreateCustomersPadding(FILE* CustomersFile, int current_pos, int Pad_Len)
{

	for (int i = 0; i < Pad_Len - current_pos; i++)
		fputc(' ', CustomersFile);

}

/* Writing the customers to a file in sorted order(by join date)*/
void Inorder_Writing_Customers(node_Customer* root, FILE* CustomersFile)
{
	int pos1, pos2;
	if (root) // if (root != NULL)
	{
		Inorder_Writing_Customers(root->left, CustomersFile);

		fseek(CustomersFile, 0, SEEK_END);
		pos1 = ftell(CustomersFile);
		fprintf(CustomersFile, "%d", root->data.customer_id);
		pos2 = ftell(CustomersFile);
		CreateCustomersPadding(CustomersFile, pos2, pos1 + 10);

		pos1 = ftell(CustomersFile);
		for (int i = 0; i < name_lim - 1; i++)
			fputc(root->data.name[i], CustomersFile);
		pos2 = ftell(CustomersFile);
		CreateCustomersPadding(CustomersFile, pos2, pos1 + name_lim + 11);

		pos1 = ftell(CustomersFile);
		fprintf(CustomersFile, "%0.2f₪", root->data.purchases_amount);
		pos2 = ftell(CustomersFile);
		CreateCustomersPadding(CustomersFile, pos2, pos1 + 25);

		pos1 = ftell(CustomersFile);
		fprintf(CustomersFile, "%s", root->data.club_member ? "Club member" : "Not a club member");
		pos2 = ftell(CustomersFile);
		CreateCustomersPadding(CustomersFile, pos2, pos1 + 18);


		pos1 = ftell(CustomersFile);
		for (int i = 0; i < birthday_lim - 1; i++)
			fputc(root->data.birthday[i], CustomersFile);
		pos2 = ftell(CustomersFile);
		CreateCustomersPadding(CustomersFile, pos2, pos1 + birthday_lim + 2);

		pos1 = ftell(CustomersFile);
		for (int i = 0; i < 19; i++)
			fputc(root->data.join_date[i], CustomersFile);
		pos2 = ftell(CustomersFile);
		//CreateCustomersPadding(CustomersFile, pos2, pos1 + 2);

		TransactionsDocumentations(root, root->data.purchased_items);
		fprintf(CustomersFile, "  \n");

		Inorder_Writing_Customers(root->right, CustomersFile);
	}
}

/* A function to reload existing customers to the tree - "Loading fucntion*/
void Reload_Existing_Customers_To_The_Tree(node_Customer** root)
{
	FILE* Customer_File = NULL;
	Customer cst;
	int line = 1;
	Customer_File = fopen("Customers.txt", "r+");
	if (!Customer_File)
		return;
	while (!feof(Customer_File))
	{
		fseek(Customer_File, line * customer_line, SEEK_SET);
		fscanf(Customer_File, "%d", &cst.customer_id);
		if (cst.customer_id == -858993460) //For the first time
			return;

		fseek(Customer_File, line * customer_line + 10, SEEK_SET);
		for (int i = 0; i < name_lim; i++)
			fscanf(Customer_File, "%c", &cst.name[i]);

		fseek(Customer_File, line * customer_line + name_lim + 21, SEEK_SET);
		fscanf(Customer_File, "%f", &cst.purchases_amount);

		fseek(Customer_File, line * customer_line + name_lim + 46, SEEK_SET);
		if (getc(Customer_File) == 'C')
			cst.club_member = true;
		else 
			cst.club_member = false;

		fseek(Customer_File, line * customer_line + name_lim + 64, SEEK_SET);
		for (int i = 0; i < birthday_lim - 1; i++)
			fscanf(Customer_File, "%c", &cst.birthday[i]);

		cst.birthday[10] = ' ';

		fseek(Customer_File, line * customer_line + name_lim + 77, SEEK_SET);
		for (int i = 0; i < 19; i++)
			fscanf(Customer_File, "%c", &cst.join_date[i]);
	
		cst.purchased_items = NULL;
		FILE* Transactions_File = fopen("Transactions.txt", "r");
		if (!Transactions_File) // for the first time
			insertHead_Customer(root, cst);
		else
		{
			Reload_Existing_Transactions_To_The_Tree(cst.customer_id, &cst.purchased_items);
			insertHead_Customer(root, cst);
		}
		line++;
	}

}

/* A function to compare between two join dates.
   If the first argument is greater than the second - return 0.
   If the second argument is greater than the first - return 1.
   If they are equal - return -1*/
int CompareJoinDates(char JoinDate1[80], char DateToCheck[80])
{
	//Years
	if ((JoinDate1[6] * 1000 + JoinDate1[7] * 100 + JoinDate1[8] * 10 + JoinDate1[9]) > (DateToCheck[6] * 1000 + DateToCheck[7] * 100 + DateToCheck[8] * 10 + DateToCheck[9]))
		return 0;
	else if ((JoinDate1[6] * 1000 + JoinDate1[7] * 100 + JoinDate1[8] * 10 + JoinDate1[9]) < (DateToCheck[6] * 1000 + DateToCheck[7] * 100 + DateToCheck[8] * 10 + DateToCheck[9]))
		return 1;
	
	//Months
	if ((JoinDate1[3] * 10 + JoinDate1[4]) > (DateToCheck[3] * 10 + DateToCheck[4]))
		return 0;
	else if ((JoinDate1[3] * 10 + JoinDate1[4]) < (DateToCheck[3] * 10 + DateToCheck[4]))
		return 1;

	//Days
	if ((JoinDate1[0] * 10 + JoinDate1[1]) > (DateToCheck[0] * 10 + DateToCheck[1]))
		return 0;
	else if ((JoinDate1[0] * 10 + JoinDate1[1]) < (DateToCheck[0] * 10 + DateToCheck[1]))
		return 1;

	//Hours
	if ((JoinDate1[11] * 10 + JoinDate1[12]) > (DateToCheck[11] * 10 + DateToCheck[12]))
		return 0;
	else if ((JoinDate1[11] * 10 + JoinDate1[12]) < (DateToCheck[11] * 10 + DateToCheck[12]))
		return 1;

	//Minutes
	if ((JoinDate1[14] * 10 + JoinDate1[15]) > (DateToCheck[14] * 10 + DateToCheck[15]))
		return 0;
	else if ((JoinDate1[14] * 10 + JoinDate1[15]) < (DateToCheck[14] * 10 + DateToCheck[15]))
		return 1;

	//Seconds
	if ((JoinDate1[17] * 10 + JoinDate1[18]) > (DateToCheck[17] * 10 + DateToCheck[18]))
		return 0;
	else if ((JoinDate1[17] * 10 + JoinDate1[18]) < (DateToCheck[17] * 10 + DateToCheck[18]))
		return 1;

	return -1;
}

void print_customer_inorder(node_Customer* tree)
{
	if (tree) 
	{
		print_customer_inorder(tree->left);
		PrintCustomer(tree);
		print_customer_inorder(tree->right);
	}
}

/* A function for search customer by his id.
the function scans the entire tree (because it is sorted by dates and not ID)*/
node_Customer* SearchCustomerID(node_Customer** tree, int id)
{

	if (!(*tree) || (*tree)->data.customer_id == id) 
		return *tree;
	node_Customer* L = SearchCustomerID(&((*tree)->left), id);
	if (L)
		return L;
	node_Customer* R = SearchCustomerID(&((*tree)->right), id);
	if (R)
		return R;

}

/*Based on an algorithm learned in the Algorithm and Data Structure course*/
node_Customer* RemoveCustomer(node_Customer* root, char val[80])
{
	node_Customer* TempDel = root;
	node_Customer* Temp = NULL;
	node_Customer* NewTemp = NULL;
	node_PurchasedItems* Del = NULL;

	while (TempDel != NULL &&  (CompareJoinDates(val, TempDel->data.join_date)) != -1)
	{
	
		Temp = TempDel;
		if ((CompareJoinDates(val, TempDel->data.join_date)) == 1)
			TempDel = TempDel->left;
		else 
			TempDel = TempDel->right;
	}

	if (TempDel->left == NULL || TempDel->right == NULL)
	{

		if (TempDel->left == NULL)
			NewTemp = TempDel->right;
		else
			NewTemp = TempDel->left;

		if (Temp == NULL)
			return NewTemp;

		if (TempDel == Temp->left)
			Temp->left = NewTemp;
		else
			Temp->right = NewTemp;
	
		Del = TempDel->data.purchased_items;
		DeleteTree(Del);
		DocRemovingCustomer(TempDel->data);
		free(TempDel);
		
	}
	else
	{
		Temp = TempDel->right;
		while (Temp->left != NULL)
		{
			NewTemp = Temp;
			Temp = Temp->left;
		}

		if (NewTemp != NULL)
			NewTemp->left = Temp->right;
		else
			TempDel->right = Temp->right;
		TempDel->data = Temp->data;

		Del = Temp->data.purchased_items;
		DeleteTree(Del);
		DocRemovingCustomer(Temp->data);
		free(Temp);
	}
	return root;
}

//Registering a customer as a club member
void ChangeToClubMember(node_Customer* root)
{
	Customer cstm;
	node_Customer* element = root;

	for (int i = 0; i < name_lim; i++)
		cstm.name[i] = root->data.name[i];
	
	cstm.customer_id = root->data.customer_id;

	cstm.purchases_amount = root->data.purchases_amount;

	cstm.club_member = true;

	for (int i = 0; i < birthday_lim - 1; i++)
		cstm.birthday[i] = root->data.birthday[i];
	

	for (int i = 0; i < date_lim; i++)
		cstm.join_date[i] = root->data.join_date[i];

	cstm.purchased_items = root->data.purchased_items;

	insertHead_Customer(root, cstm);
}

/* a Function That Recieves an Empty Characters Array and Fill it With the Current Date.*/
void getTime(char* cur_time_adr, unsigned int Size) 
{
	struct tm* FullTime;
	time_t TimeinSec;
	time(&TimeinSec);
	FullTime = localtime(&TimeinSec);
	strftime(cur_time_adr, Size, "%d-%m-%Y %H:%M:%S", FullTime);
}

/* A function to sell an item to a customer.*/
void SellItemToCustomer(node_Customer* root_customer, node* root_items)
{
	node_Customer* buyer;
	node* RequestedItem = NULL;
	PurchasedItems PI;
	int NumberOfPurchases = 0;
	printf("\nWe Are Glad You Chose To Buy At Our Toy Store, Please Enter The Customer's ID:");
	int inputID;
	scanf("%d", &inputID);
	printf("\n");
	buyer = SearchCustomerID(&root_customer, inputID);

	if (buyer == NULL)
	{
		printf("There are no customers with the given ID\nWould You Like To Register?(If You Are A Customer Check Your Spelling And Try Again)\n");
		printf("Y - yes\nN - no\n");
		getchar();
		char choice;
		scanf("%c", &choice);
		while (choice != 'Y' && choice !='N')
		{
			getchar();
			printf("Invalid input.. Please try again\n");
			scanf("%c", &choice);
		}

		if (choice == 'Y')
		{
			printf("Please register the new customer and then select this operation(10) again\n");
			AddNewCustomer(&root_customer);
			printf("Id             Name                Total purchases amount        Club member?       Birthday   Join date\n");
			print_customer_inorder(root_customer);
			return;
		}
		else if (choice == 'N')
		{
			printf("Sorry, You Cannot Make A Purchase Without Register As A Customer\nHope You'll Change Your Mind In The Future, Have A Nice Day\n");
			return;
		}
	}

	int SoldID;
	bool SuccessfullMatch;
	char More_Purchases = 'Y';
	int Number_Of_Purchases_After = 0;
	int IsDiscount = 0;
	while (NumberOfPurchases < 3 && More_Purchases ==  'Y')
	{
		//Number_Of_Purchases_Before = Number_Of_Purchases(buyer->data.purchased_items);
		do 
		{
			SuccessfullMatch = false;
			printf("Please Enter The ID Number of the required Item(Type -1 To Go To The Main Menu): ");
			scanf("%d", &SoldID);
			if (SoldID == -1)
				break;

			RequestedItem = IDSearch(&root_items, SoldID);
			if (RequestedItem == NULL)
				printf("There Are No Items With The Given ID, Try again or press -1 to exit\n");
			else if (RequestedItem->data.inStock == false)
				printf("Sorry, The Item Is Out Of Stock, Please Look For Another Item, or Type -1 To Exit\n");
			else
				SuccessfullMatch = true;

		} while ((SoldID != -1) && (SuccessfullMatch == false));

		if (SoldID == -1)
			break;
		

		printf("good news, the item in stock, the bill is: %.2f\n", RequestedItem->data.price);

		printf("press Y to pay or N to go to the main menu if you changed your mind:");
		getchar();
		char action;
		scanf("%c", &action);
		while ((action != 'Y') && (action != 'N'))
		{
			getchar();
			printf("invalid input, try again\n");
			scanf("%c", &action);
		}

		if (action == 'N')
			break;

		printf("Thank You For Your Purchase\n");



		RequestedItem->data.inventory -= 1;
		if (RequestedItem->data.inventory == 0)
			RequestedItem->data.inStock = false;

		PI.id = RequestedItem->data.id;
		getTime(PI.Purchased_Date, sizeof(PI.Purchased_Date));
		PI.IsReturn = false;

		InsertHead_PurchasesHistory(&buyer->data.purchased_items, PI);
		DocSellToCustomer(buyer->data, RequestedItem->data);
		Number_Of_Purchases_After = Number_Of_Purchases(buyer->data.purchased_items);
		printf("Number of purchses = %d\n",Number_Of_Purchases_After);
		if(buyer->data.club_member)
			IsDiscount = CalculateDiscount(Number_Of_Purchases_After, buyer, PI.Purchased_Date);
		if (IsDiscount == 1)
		{
			printf("Your updated price is: %.2f\n", RequestedItem->data.price / 2);
			buyer->data.purchases_amount += RequestedItem->data.price / 2;
		}
		else if (IsDiscount == 2)
		{
			printf("Your updated price is: %.2f\n", RequestedItem->data.price * 0.75);
			buyer->data.purchases_amount += RequestedItem->data.price * 0.75;
		}
		else if (IsDiscount == 0)
			buyer->data.purchases_amount += RequestedItem->data.price;

		NumberOfPurchases++;
		printf("If you want to continue for more purchases, please enter Y, otherwise enter N\n");
		getchar();
		scanf("%c", &More_Purchases);
		while (More_Purchases != 'Y' && More_Purchases != 'N')
		{
			getchar();
			printf("Invalid input.. Please try again\n");
			scanf("%c", &More_Purchases);
		}
		getchar();
	}
	
	//buyer->data.purchased_items = root_purchases;
	if (NumberOfPurchases == 3)
		printf("Sorry, but you have reached the limit of the number of possible purchases\n");
	printf("Thank you for visiting the store we look forward to seeing you again\n");
	Print_Purchases_History(buyer->data.purchased_items);
	//TransactionsDocumentations(buyer, buyer->data.purchased_items);
}


/*A function that counts the number of purchases by the customer.
  IMPORTANT NOTE: Includes the purchases of the items returned to the store for the purpose of calculating discounts for club members.
  This means that items returned to the store are not deleted from the customer's purchase history.*/
int Number_Of_Purchases(node_PurchasedItems* tree)
{
	if (!tree)
		return 0;
	else
		return 1 + Number_Of_Purchases(tree->left) + Number_Of_Purchases(tree->right);
}

/*A function that calculates and returns the difference in months between two dates*/
int CalculatesMonthsDates(char birthday[80], char DateToCheck[80])
{
	return (birthday[3] * 10 + birthday[4]) - (DateToCheck[3] * 10 + DateToCheck[4]);
}

/*A function that calculates for club members, the relevant discount from the following options:
  Club members who purchased in the month of their birthday will receive a 50% discount - the function will return 1
  For every fifth purchase, the club member will receive a 25% discount on the purchase of the item - the function will return 2
  If the club member is not entitled to the discount, the function will return 0.
  IMPORTANT NOTE: There is no double discounts*/
int CalculateDiscount(int size_after, node_Customer* root, char DateToCheck[80])
{
	if (CalculatesMonthsDates(root->data.birthday, DateToCheck) == 0)
	{
		printf("HAPPY BIRTHDAY!!!\nAs a club member you are entitled to a 50 percent discount\n");
		return 1;
	}
	else if (size_after % 5 == 0)
	{
		printf("Congratulations!, as a club member you are entitled to a 25 percent discount on every fifth purchase\n");
		return 2;
	}
	else
		return 0;
}

/*Auxiliary function for writing transactions to a file.*/
void TransactionsDocumentations(node_Customer* root_customer, node_PurchasedItems* root_purchased)
{
	FILE* Transactions_File = NULL;
	Transactions_File = fopen("Transactions.txt", "r");
	if (!Transactions_File) // for the first time
	{
		Transactions_File = fopen("Transactions.txt", "a");
		assert(Transactions_File);
	}
	else
	{
		fclose(Transactions_File);
		Transactions_File = fopen("Transactions.txt", "r+");
		assert(Transactions_File);
	}
	CreateTransactionsTitle(Transactions_File);
	WriteTranscationsToFile(Transactions_File, root_customer, root_purchased);
	fclose(Transactions_File);
}

/* A function to create title for Transactions file.*/
void CreateTransactionsTitle(FILE* TransactionsFile)
{
	int pos1, pos2;

	pos1 = ftell(TransactionsFile);

	fprintf(TransactionsFile, "Customer ID");
	pos2 = ftell(TransactionsFile);
	CreateCustomersPadding(TransactionsFile, pos2, pos1 + 15);
	pos1 = ftell(TransactionsFile);

	fprintf(TransactionsFile, "Item ID");
	pos2 = ftell(TransactionsFile);
	CreateCustomersPadding(TransactionsFile, pos2, pos1 + 15);
	pos1 = ftell(TransactionsFile);

	fprintf(TransactionsFile, "Purchased date");
	pos2 = ftell(TransactionsFile);
	CreateCustomersPadding(TransactionsFile, pos2, pos1 + 48);
	pos1 = ftell(TransactionsFile);

	fprintf(TransactionsFile, "Is returned?");
	pos2 = ftell(TransactionsFile);
	CreateCustomersPadding(TransactionsFile, pos2, pos1 + 20);
	pos1 = ftell(TransactionsFile);
	

	fprintf(TransactionsFile, "\n");
	/*Total chars in line = 99 + /n = 100*/

}

/*Writing the transaction to file (ordered by date!)*/
void WriteTranscationsToFile(FILE* TransactionsFile, node_Customer* root_customer, node_PurchasedItems* root_purchased)
{
	int pos1, pos2;
	if (root_purchased) // if (root != NULL)
	{
		WriteTranscationsToFile(TransactionsFile, root_customer, root_purchased->left);

		fseek(TransactionsFile, 0, SEEK_END);
		pos1 = ftell(TransactionsFile);

		fprintf(TransactionsFile, "%d", root_customer->data.customer_id);
		pos2 = ftell(TransactionsFile);
		CreateCustomersPadding(TransactionsFile, pos2, pos1 + 15);

		pos1 = ftell(TransactionsFile);
		fprintf(TransactionsFile, "%d", root_purchased->data.id);
		pos2 = ftell(TransactionsFile);
		CreateCustomersPadding(TransactionsFile, pos2, pos1 + 15);

		pos1 = ftell(TransactionsFile);
		for (int i = 0; i < 19; i++)
			fputc(root_purchased->data.Purchased_Date[i], TransactionsFile);
		pos2 = ftell(TransactionsFile);
		CreateCustomersPadding(TransactionsFile, pos2, pos1 + 48);

		pos1 = ftell(TransactionsFile);
		fprintf(TransactionsFile, "%s", root_purchased->data.IsReturn ? "Yes" : "No");
		pos2 = ftell(TransactionsFile);
		CreateCustomersPadding(TransactionsFile, pos2, pos1 + 20);

		fprintf(TransactionsFile, "\n");

		WriteTranscationsToFile(TransactionsFile, root_customer, root_purchased->right);
	}
}

void Reload_Existing_Transactions_To_The_Tree(int customer_id, node_PurchasedItems** root_purchased)
{
	FILE* Transactions_File = fopen("Transactions.txt", "r");
	if (!Transactions_File)
		return;
	assert(Transactions_File);
	PurchasedItems PI;
	char IsReturn;
	int id;
	int line = 1;
	while (!feof(Transactions_File))
	{
		fseek(Transactions_File, line * transaction_line, SEEK_SET);
		fscanf(Transactions_File, "%d", &id);
		if (id == -858993460) //For the first time
			return;

		if (id == customer_id)
		{
			fseek(Transactions_File, line * transaction_line + 15, SEEK_SET);
			fscanf(Transactions_File, "%d", &PI.id);
			fseek(Transactions_File, line * transaction_line + 30, SEEK_SET);
			for (int i = 0; i < 19; i++)
				fscanf(Transactions_File, "%c", &PI.Purchased_Date[i]);
			fseek(Transactions_File, line * transaction_line + 78, SEEK_SET);
			if (getc(Transactions_File) == 'Y')
				PI.IsReturn = true;
			else 
				PI.IsReturn = false;
			InsertHead_PurchasesHistory(root_purchased, PI);
		}
		line++;
	}
}

void ReturnItems(int customer_id, node_PurchasedItems* root_purchased, node* root_items)
{
	int Item_ID;
	node_PurchasedItems* TEMP = NULL;
	node* RequestedItem = NULL;
	char* cur_time[80];
	printf("Please enter the product ID number that the customer wants to return to the store\n");
	scanf("%d", &Item_ID);
	TEMP = SearchPurchase(&root_purchased, Item_ID);
	while (!TEMP)
	{
		printf("Sorry,but this item is not in the customer's purchase history.. Please try again\n");
		scanf("%d", &Item_ID);
		TEMP = SearchPurchase(&root_purchased, Item_ID);
	}
	getTime(cur_time, sizeof(cur_time));
	if (DiffDaysBetweenDates(TEMP->data.Purchased_Date, cur_time) > 14)
	{
		printf("14 days have passed since the purchase of the item, you cannot return this item to the store\n");
		return;
	}
	if (TEMP->data.IsReturn == true)
	{
		printf("Sorry, but this item has already been returned by this customer to the store\n");
		return;
	}
	else
	{
		RequestedItem = IDSearch(&root_items, Item_ID);
		if (RequestedItem == NULL)
		{
			/* טיפול במקרה מיוחד(!) ובו לקוח רוצה להחזיר מוצר שכבר נמחק מהמלאי כתוצאה מפעולה קודמת שנעשתה */
			/* ההתייחסות במקרה זה היא למוצר שכבר לא מסופק ע"י החנות ולכן גם לא ניתן להחזירו*/
			/* נצא מנקודת הנחה כי (מספר התרחשויות של החזרת מוצר >> מספר התרחשויות של מחיקת מוצר מהמלאי) וזאת כדי למנוע התנגשות זאת*/
			printf("Sorry, this product has been removed from stock and cannot be returned\n");
			return;
		}
		RequestedItem->data.inventory += 1;
		TEMP->data.IsReturn = true;
		if (RequestedItem->data.inventory == 1)
			RequestedItem->data.inStock = true;
		printf("The item has been successfully returned to the store\n");
		DocReturningItem(customer_id, RequestedItem->data);
	}
}

node_PurchasedItems* SearchPurchase(node_PurchasedItems** root_purchased, int ItemID)
{

	if (!(*root_purchased) || (*root_purchased)->data.id == ItemID)
		return *root_purchased;
	node_PurchasedItems* L = SearchPurchase(&((*root_purchased)->left), ItemID);
	if (L)
		return L;
	node_PurchasedItems* R = SearchPurchase(&((*root_purchased)->right), ItemID);
	if (R) 
		return R;

}

int DiffDaysBetweenDates(char purchase[80], char DateToCheck[80])
{
	int Days_In_Month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int Days_Diff = 0, check = 0;

	//Years
	Days_Diff += ((DateToCheck[6] - '0') * 1000 + (DateToCheck[7] - '0') * 100 + (DateToCheck[8] - '0') * 10 + (DateToCheck[9] - '0')) - ((purchase[6] - '0') * 1000 + (purchase[7] - '0') * 100 + (purchase[8] - '0') * 10 + (purchase[9] - '0'));
	Days_Diff *= 365;
	int Month_P = (purchase[3] - '0') * 10 + (purchase[4] - '0');
	int Month_D = (DateToCheck[3] - '0') * 10 + (DateToCheck[4] - '0');
	//Months
	for (int i = Month_P; i < Month_D; i++)
	{
		check = 1;
		Days_Diff += Days_In_Month[i];
	}
	if (check == 0)
	{
		for (int i = Month_D; i < Month_P; i++)
		{
			Days_Diff -= Days_In_Month[i];
		}
	}

	//Days
	Days_Diff += ((DateToCheck[0] - '0') * 10 + (DateToCheck[1]- '0')) - ((purchase[0] - '0') * 10 + (purchase[1] - '0'));

	return Days_Diff;
		
}

void Insert_PurchasesHistory(node_PurchasedItems** tree, node_PurchasedItems * parent, PurchasedItems val)
{
		node_PurchasedItems* temp = NULL;
		//if tree node is empty, then create a new item and add it as head.
		if (!(*tree)) // if (tree == NULL)
		{
			temp = (node_PurchasedItems*)malloc(sizeof(node_PurchasedItems));
			//initialize left and right pointers to NULL, this node is currently a leaf
			temp->left = temp->right = NULL;
			//initialize father to the one who called me.
			temp->parent = parent;

			temp->data = val;
			*tree = temp;
			return;
		}

		/* Sorting by purchased date*/
		int result = CompareJoinDates(val.Purchased_Date, (*tree)->data.Purchased_Date);

		if (result == 0)
		{
			//insert into left pointer of tree, sending the pointer, father (himself) and value
			Insert_PurchasesHistory(&(*tree)->left, *tree, val);
		}
		else if (result == 1)
		{
			//insert into right pointer of tree, sending the pointer, father (himself) and value
			Insert_PurchasesHistory(&(*tree)->right, *tree, val);
		}

}

void InsertHead_PurchasesHistory(node_PurchasedItems** tree, PurchasedItems val)
{
		Insert_PurchasesHistory(tree, NULL, val);
}

void Print_Purchases_History(node_PurchasedItems * tree)
{
	if (tree) // if (tree != NULL)
	{
		Print_Purchases_History(tree->left);

		printf("Items Id: %d ", tree->data.id);
		printf("Purchase date: ");
		for (int i = 0; i < 19; i++)
			printf("%c", tree->data.Purchased_Date[i]);
		printf(" Is returned? %s", tree->data.IsReturn ? "Yes" : "No");
		printf("\n");
		Print_Purchases_History(tree->right);
	}
}

void PrintCustomer(node_Customer * tree)
{
		int SpaceCnt = 0, num;
		printf("%d", tree->data.customer_id);

		/* To beautify the print (to create uniform output)*/
		num = tree->data.customer_id;
		while (num != 0)
		{
			SpaceCnt++;
			num /= 10;
		}
		for (int i = 0; i < 15 - SpaceCnt; i++)
			printf(" ");

		for (int i = 0; i < name_lim; i++)
			printf("%c", tree->data.name[i]);

		printf("%0.2f", tree->data.purchases_amount);
		num = tree->data.purchases_amount;
		SpaceCnt = 0;
		if (num != 0)
			printf(" ");
		while (num != 0)
		{
			SpaceCnt++;
			num /= 10;
		}
		for (int i = 0; i < 10 - SpaceCnt; i++)
			printf(" ");


		for (int i = 0; i < 16; i++)
			printf(" ");
		printf("%s", tree->data.club_member ? "Club member        " : "Not a club member  ");
		for (int i = 0; i < birthday_lim; i++)
		{
			printf("%c", tree->data.birthday[i]);
		}

		for (int i = 0; i < 19; i++)
			printf("%c", tree->data.join_date[i]);
		printf("\n");
}


void insert_customer(node_Customer** tree, node_Customer* parent, Customer val)
{
	node_Customer* temp = NULL;
	//if tree node is empty, then create a new customer and add it as head.
	if (!(*tree)) // if (tree == NULL)
	{
		temp = (node_Customer*)malloc(sizeof(node_Customer));
		//initialize left and right pointers to NULL, this node is currently a leaf
		temp->left = temp->right = NULL;
		//initialize father to the one who called me.
		temp->parent = parent;

		temp->data = val;
		*tree = temp;
		return;
	}
	
	/* Sorting by join date*/

	int result = CompareJoinDates(val.join_date, (*tree)->data.join_date);

	if (result == 1)
	{
		//insert into left pointer of tree, sending the pointer, father (himself) and value
		insert_customer(&(*tree)->left, *tree, val);
	} 
	else if (result == 0)
	{
		//insert into right pointer of tree, sending the pointer, father (himself) and value
		insert_customer(&(*tree)->right, *tree, val);
	}
}

void insertHead_Customer(node_Customer** tree, Customer val)
{
	insert_customer(tree, NULL, val);
}

void DeleteTree(node_PurchasedItems* tree)
{
	// Post-oreder mechanism
	if (tree) // if (tree != NULL)
	{
		DeleteTree(tree->left);
		DeleteTree(tree->right);
		free(tree);
	}
}