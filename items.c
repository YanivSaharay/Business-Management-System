#include "items.h"
#include "log.h"


const Id_Lim = 20;
const Manufacturing_Country_lim = 20, Category_Lim = 20;
const Date_lim = 11;
const Items_Line = 120;

/* A function to add new item to data structure - "Save function" */
/* IMPORTANT NOTE: If there are two identical IDs, the first item  who received  
				   this identifier is the one that will appear in the inventory!*/
void AddNewItem(node** root)
{
	int inventory;
	Item It;
	getchar(); // To handling with enter key
	printf("Adding a new product to inventory\n");
	printf("Please enter id for item and press enter\n");
	scanf("%d", &It.id);

	getchar(); // To handling with enter key
	printf("Please enter the country of manufacture of the item and press enter\n");
	PaddingInfo(&It.Manufacturing_Country, Manufacturing_Country_lim);

	printf("Please enter which category the item belongs to and press enter\n");
	PaddingInfo(&It.category, Category_Lim);

	printf("Please entet the item cost\n");
	scanf("%f", &It.price);

	printf("Please enter the item inventory\n");
	scanf("%d", &inventory);
	while (inventory < 0)
	{
		printf("Invalid value.. Please try again\n");
		scanf("%d", &inventory);
	}
	It.inventory = inventory;

	if (It.inventory == 0)
		It.inStock = false;
	else
		It.inStock = true;

	printf("Please enter the age limit to use the item\n+");
	scanf("%d", &It.age_limit);
	
	getchar();
	printf("Please enter the item manufacturing date in the format of DD-MM-YYYY\n");
	for (int i = 0; i < Date_lim -1; i++)
	{
		scanf("%c", &It.date[i]);
	}
	It.date[10] = ' ';
	/* ASSUMPTION: The entered date is correct we will only check whether it matches the request format*/
	while (It.date[2] != '-' || It.date[5] != '-')
	{
		getchar();
		printf("Invalid date.. Please try again\n");
		for (int i = 0; i < Date_lim - 1; i++)
			scanf("%c", &It.date[i]);
		It.date[10] = ' ';
	}
	insertHead(root, It);
	DocAddingItem(It);

}

/*A function to receive the item data and pass them by reference.*/
void PaddingItemsInfo(char* arr, int len)
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

/*Auxiliary function for writing items to a file.*/
void WriteToItemsFile(node* root)
{  /*לדעתי אפשר לממש את הפונקציה הזאת בצורה יעילה יותר, אך לבינתיים זה עובד. נתעסק בזה אם הזמן יאפשר*/
	FILE* Items_File = NULL;
	Items_File = fopen("Items.txt", "r");
	if (!Items_File) // for the first time
	{
		Items_File = fopen("Items.txt", "a");
		assert(Items_File);
	}
	else
	{
		fclose(Items_File);
		Items_File = fopen("Items.txt", "w");
		assert(Items_File);
	}
	CreateItemsTitle(Items_File);
	Inorder_Writing_Items(root, Items_File);
	fclose(Items_File);
}

/* Writing the items to a file in sorted order(from the smallest ID to the largest) */
void Inorder_Writing_Items(node* root, FILE* ItemsFile)
{
	int pos1, pos2;
	if (root) // if (root != NULL)
	{
		Inorder_Writing_Items(root->left, ItemsFile);

		fseek(ItemsFile, 0 , SEEK_END);
		pos1 = ftell(ItemsFile);
		fprintf(ItemsFile, "%d", root->data.id);
		pos2 = ftell(ItemsFile);
		CreateItemsPadding(ItemsFile, pos2, pos1 + 10);

		pos1 = ftell(ItemsFile);
		for (int i = 0; i < Manufacturing_Country_lim - 1; i++)
			fputc(root->data.Manufacturing_Country[i], ItemsFile);
		pos2 = ftell(ItemsFile);
		CreateItemsPadding(ItemsFile, pos2, pos1 + Manufacturing_Country_lim + 10);

		pos1 = ftell(ItemsFile);
		for (int i = 0; i < Category_Lim - 1; i++)
			fputc(root->data.category[i], ItemsFile);
		pos2 = ftell(ItemsFile);
		CreateItemsPadding(ItemsFile, pos2, pos1 + Category_Lim);

		pos1 = ftell(ItemsFile);
		fprintf(ItemsFile, "%0.2f₪", root->data.price);
		pos2 = ftell(ItemsFile);
		CreateItemsPadding(ItemsFile, pos2, pos1 + 10);

		pos1 = ftell(ItemsFile);
		fprintf(ItemsFile, "%s", root->data.inStock ? "In Stock" : "Out of stock");
		pos2 = ftell(ItemsFile);
		CreateItemsPadding(ItemsFile, pos2, pos1 + 15);

		pos1 = ftell(ItemsFile);
		fprintf(ItemsFile, "+%d", root->data.age_limit);
		pos2 = ftell(ItemsFile);
		CreateItemsPadding(ItemsFile, pos2, pos1 + 10);

		pos1 = ftell(ItemsFile);
		fprintf(ItemsFile, "%d", root->data.inventory);
		pos2 = ftell(ItemsFile);
		CreateItemsPadding(ItemsFile, pos2, pos1 + 10);

		pos1 = ftell(ItemsFile);
		for (int i = 0; i < Date_lim - 1; i++)
			fputc(root->data.date[i], ItemsFile);
		pos2 = ftell(ItemsFile);
		CreateItemsPadding(ItemsFile, pos2, pos1 + Date_lim + 2);

		fprintf(ItemsFile, "\n");

		Inorder_Writing_Items(root->right, ItemsFile);
	}
}

/* A function to create title for Items file.*/
void CreateItemsTitle(FILE* ItemsFile)
{
	int pos1, pos2;
	
	pos1 = ftell(ItemsFile);

	fprintf(ItemsFile, "ID");
	pos2 = ftell(ItemsFile);
	CreateItemsPadding(ItemsFile, pos2, pos1 + 10);
	pos1 = ftell(ItemsFile);

	fprintf(ItemsFile, "Manufacturing country");
	pos2 = ftell(ItemsFile);
	CreateItemsPadding(ItemsFile, pos2, pos1 + Manufacturing_Country_lim + 10);
	pos1 = ftell(ItemsFile);

	fprintf(ItemsFile, "Category");
	pos2 = ftell(ItemsFile);
	CreateItemsPadding(ItemsFile, pos2, pos1 + Category_Lim);
	pos1 = ftell(ItemsFile);

	fprintf(ItemsFile, "Price");
	pos2 = ftell(ItemsFile);
	CreateItemsPadding(ItemsFile, pos2, pos1 + 10);
	pos1 = ftell(ItemsFile);

	fprintf(ItemsFile, "In stock?");
	pos2 = ftell(ItemsFile);
	CreateItemsPadding(ItemsFile, pos2, pos1 + 15);
	pos1 = ftell(ItemsFile);

	fprintf(ItemsFile, "Age limit");
	pos2 = ftell(ItemsFile);
	CreateItemsPadding(ItemsFile, pos2, pos1 + 10);
	pos1 = ftell(ItemsFile);

	fprintf(ItemsFile, "Inventory");
	pos2 = ftell(ItemsFile);
	CreateItemsPadding(ItemsFile, pos2, pos1 + 10);
	pos1 = ftell(ItemsFile);

	fprintf(ItemsFile, "Date");
	pos2 = ftell(ItemsFile);
	CreateItemsPadding(ItemsFile, pos2, pos1 + Date_lim + 2);
	pos1 = ftell(ItemsFile);

	fprintf(ItemsFile, "\n");
	/*Total chars in line = 119 + /n = 120*/
}

/* A function to create padding for items file.*/
void CreateItemsPadding(FILE* ItemsFile, int current_pos, int Pad_Len)
{
	for (int i = 0; i < Pad_Len - current_pos; i++)
		fputc(' ', ItemsFile);
}

/* A function to reload existing items to the tree - "Loading fucntion*/
void Reload_Existing_Items_To_The_Tree(node** root)
{
	FILE* Items_File = NULL;
	Item It;
	int line = 1;
	Items_File = fopen("Items.txt", "r+");
	if (!Items_File)
		return;
	while (!feof(Items_File))
	{
		fseek(Items_File, line * Items_Line, SEEK_SET);
		fscanf(Items_File, "%d", &It.id);
		if (It.id == -858993460) // For the first time
			return;

		fseek(Items_File, line * Items_Line + 10, SEEK_SET);
		for (int i = 0; i < Manufacturing_Country_lim; i++)
			fscanf(Items_File, "%c", &It.Manufacturing_Country[i]);

		fseek(Items_File, line * Items_Line + Manufacturing_Country_lim + 20, SEEK_SET);
		for (int i = 0; i < Category_Lim; i++)
			fscanf(Items_File, "%c", &It.category[i]);

		fseek(Items_File, line * Items_Line + Manufacturing_Country_lim + Category_Lim + 10, SEEK_SET);
		fscanf(Items_File, "%f", &It.price);

		fseek(Items_File, line * Items_Line + Manufacturing_Country_lim + Category_Lim + 40, SEEK_SET);
		fscanf(Items_File, "%d",&It.age_limit);

		fseek(Items_File, line * Items_Line + Manufacturing_Country_lim + Category_Lim + 55, SEEK_SET);
		fscanf(Items_File, "%d", &It.inventory);

		if (It.inventory == 0)
			It.inStock = false;
		else
			It.inStock = true;

		fseek(Items_File, line * Items_Line + Manufacturing_Country_lim + Category_Lim + 65, SEEK_SET);
		for (int i = 0; i < Date_lim - 1; i++)
			fscanf(Items_File, "%c", &It.date[i]);
		It.date[10] = ' ';
		insertHead(root, It);
		line++;
	}

}

/*Based on an algorithm learned in the Algorithm and Data Structure course*/
node* RemoveItem(node* root, int val)
{
	node* TempDel = root;
	node* Temp = NULL;
	node* NewTemp = NULL;

	while (TempDel != NULL && TempDel->data.id != val)
	{
		Temp = TempDel;
		if (val < TempDel->data.id)
			TempDel = TempDel->left;
		else
			TempDel = TempDel->right;
	}

	if (TempDel == NULL)
	{
		printf("The ID number dost not exist.. Please try again later\n\n");
		return root;
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

		DocRemovingItem(TempDel->data);
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

		DocRemovingItem(Temp->data);
		free(Temp);
	}
	return root;
}


/* A function for updating and existing item*/
node* UpdateItem(node* root)
{
	int idtmp;
	node* element = root;
	printf("Please enter an ID number for the product you would like to update\n");
	scanf("%d", &idtmp);
	element = IDSearch(&element, idtmp);
	if(element)
	{
		DocUpdateItem(element->data);
		root = RemoveItem(root, idtmp);
		printf("Please enter the updated details of the item you wish to update\n");
		AddNewItem(&root);
	}
	else
		printf("The ID number dost not exist.. Please try again later\n\n");
	return root;

}

void SearchItem(node* root)
{
	int SearchChoice;
	printf("\n============> Item Search MENU <============= \n");
	printf("1 - Search by country of manufacture and/or category to which the item belongs\n");
	printf("2 - Search by ID\n");
	printf("3 - Search by price\n");
	printf("4 - Search by inventory quantity\n");
	printf("5 - Search by age limit\n");
	printf("6 - Search by whether in stock\n");
	printf("7 - Search by date\n");
	printf("Please select by which category you want to search for the item\n");

	printf("Your choice:");
	scanf("%d", &SearchChoice);
	do
	{
		node* temp = NULL;
		int flag;
		switch (SearchChoice)
		{
			getchar();
		case 1: //Search by text fields
			int Text_Fields;
			char c;
			int index = 0;
			char country[20], category[20];
			printf("Please enter the number of text fields you would like to search by (1 or 2)\n");
			scanf("%d", &Text_Fields);
			while (Text_Fields != 1 && Text_Fields != 2)
			{
				printf("Invalid choice.. Please try again\n");
				scanf("%d", &Text_Fields);
			}
			getchar();
			if (Text_Fields == 1)
			{
				char choice;
				printf("If you would like to search by country of manufacture please enter m\nIf you would like to search by category please enter c\n");
				scanf("%c", &choice);
				while (choice != 'm' && choice != 'c')
				{
					printf("Invalid choice.. Please try again\n");
					scanf("%c", &choice);
				}
				getchar();
				if (choice == 'm')
				{
					printf("Please enter manufacturing country\n");
					while (index < Manufacturing_Country_lim)
					{
						scanf("%c", &c);
						if (c == '\n')
							break;
						country[index] = c;
						index++;
					}
					while (index < Manufacturing_Country_lim)
						country[index++] = ' ';
					printf("\nId             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
					SearchBy1TextField(root, country, choice);
				}
				else if (choice == 'c')
				{
					printf("Please enter a category\n");
					while (index < Category_Lim)
					{
						scanf("%c", &c);
						if (c == '\n')
							break;
						category[index] = c;
						index++;
					}
					while (index < Category_Lim)
						category[index++] = ' ';
					printf("\nId             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
					SearchBy1TextField(root, category, choice);
				}
			}
			else if (Text_Fields == 2)
			{
				printf("Please enter manufacturing country\n");
				while (index < Manufacturing_Country_lim)
				{
					scanf("%c", &c);
					if (c == '\n')
						break;
					country[index] = c;
					index++;
				}
				while (index < Manufacturing_Country_lim)
					country[index++] = ' ';

				index = 0;
				printf("Please enter a category\n");
				while (index < Category_Lim)
				{
					scanf("%c", &c);
					if (c == '\n')
						break;
					category[index] = c;
					index++;
				}
				while (index < Category_Lim)
					category[index++] = ' ';
				printf("\nId             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
				SearchBy2TextFields(root, country, category);
			}
			break;
		case 2: //Search by ID
			char GS_ID;
			int ID;
			printf("Please enter an ID number\n");
			scanf("%d", &ID);
			getchar();
			printf("To search for an ID number greater than the number typed, press G, otherwise press S\n");
			scanf("%c", &GS_ID);
			while (GS_ID != 'G' && GS_ID != 'S')
			{
				printf("Invalid input.. Please try again\n");
				scanf("%c", &GS_ID);
			}
			printf("\nId             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
			SearchByID(root, ID, GS_ID);
			break;
		case 3: //Search by price
			char GS_Price;
			float price;
			printf("Please enter a price\n");
			scanf("%f", &price);
			getchar();
			printf("To search for a price greater than the number typed, press G, otherwise press S\n");
			scanf("%c", &GS_Price);
			while (GS_Price != 'G' && GS_Price != 'S')
			{
				printf("Invalid range.. Please try again\n");
				scanf("%c", &GS_Price);
			}
			printf("\nId             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
			SearchByPrice(root, price, GS_Price);
			break;
		case 4: //Search by inventory quantity
			char GS_Inventory;
			int Inventory;
			printf("Please enter an inventory quantity\n");
			scanf("%d", &Inventory);
			getchar();
			printf("To search for an inventory greater than the number typed, press G, otherwise press S\n");
			scanf("%c", &GS_Inventory);
			while (GS_Inventory != 'G' && GS_Inventory != 'S')
			{
				printf("Invalid range.. Please try again\n");
				scanf("%c", &GS_Inventory);
			}
			printf("\nId             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
			SearchByInv(root, Inventory, GS_Inventory);
			break;
		case 5:
			char GS_Age;
			int Age;
			printf("Please enter the age limit\n");
			scanf("%d", &Age);
			getchar();
			printf("To search for an age limit greater than the number typed, press G, otherwise press S\n");
			scanf("%c", &GS_Age);
			while (GS_Age != 'G' && GS_Age != 'S')
			{
				printf("Invalid range.. Please try again\n");
				scanf("%c", &GS_Age);
			}
			printf("\nId             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
			SearchByAge(root, Age, GS_Age);
			break;
		case 6: //Search by whether in stock
			int Is_Stock;
			printf("If you want to search for Items that are in stock please enter 1, otherwise 0\n");
			scanf("%d", &Is_Stock);
			while (Is_Stock != 0 && Is_Stock != 1)
			{
				printf("Invalid choice.. Please try again\n");
				scanf("%d", &Is_Stock);
			}
			printf("\nId             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
			SearchIfInStock(root, Is_Stock);

			break;
		case 7: //Search by date
			getchar();
			char Date[11];
			int GSR_Date;
			printf("Please enter the item manufacturing date in the format of DD-MM-YYYY\n");
			for (int i = 0; i < Date_lim - 1; i++)
			{
				scanf("%c", &Date[i]);
			}
			Date[10] = ' ';
			getchar();
			/* ASSUMPTION: The entered date is correct we will only check whether it matches the request format*/
			while (Date[2] != '-' || Date[5] != '-')
			{
				printf("Invalid date.. Please try again\n");
				for (int i = 0; i < Date_lim - 1; i++)
					scanf("%c", &Date[i]);
				Date[10] = ' ';
				getchar();
			}
			printf("Please select the search you want to perform:\n");
			printf("1 - Search for an item whose production date is before the selected date\n");
			printf("2 - Search for an item whose production date is after the selected date\n");
			printf("3 - Entering another date and searching between this date range\n");
			scanf("%d", &GSR_Date);
			while (GSR_Date != 1 && GSR_Date != 2 && GSR_Date != 3)
			{
				printf("Invalid choice.. Please try again\n");
				scanf("%d", &GSR_Date);
			}
			if (GSR_Date == 1 || GSR_Date == 2)
			{
				printf("\nId             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
				SearchByDate(root, Date, GSR_Date);
			}
			else if (GSR_Date == 3)
			{
				getchar();
				char Date2 [11];
				printf("Please enter another item manufacturing date in the format of DD-MM-YYYY\n");
				for (int i = 0; i < Date_lim - 1; i++)
				{
					scanf("%c", &Date2[i]);
				}
				Date2[10] = ' ';
			/* ASSUMPTION: The entered date is correct we will only check whether it matches the request format*/
				while (Date2[2] != '-' || Date2[5] != '-')
				{
					getchar();
					printf("Invalid date.. Please try again\n");
					for (int i = 0; i < Date_lim - 1; i++)
						scanf("%c", &Date2[i]);
					Date2[10] = ' ';
				}
				printf("\nId             Category            Manufacturing country Price   Age limit     Inventory  In Stock?    Date\n");
				SearchByDateWithRange(root, Date, Date2);
			}
			break;
		default:
			printf("\n\n==> Choice is not on the list.. Try Again..!\n");
			break;
		}
		printf("\n\n==> To close the search menu press -1\n");
		printf("Your choice:");
		scanf("%d", &SearchChoice);

	} while (SearchChoice != -1);
}

void SearchByID(node* tree, int value, char GS)
{
	if (GS == 'G')
	{
		if (!tree)
			return;

		if (tree->data.id <= value)
			SearchByID(tree->right, value, GS);
		else
		{
			PrintItem(tree);
			SearchByID(tree->left, value, GS);
			SearchByID(tree->right, value, GS);
		}
	}
	else if (GS == 'S')
	{
		if (!tree)
			return;

		if (tree->data.id >= value)
			SearchByID(tree->left, value, GS);
		else
		{
			PrintItem(tree);
			SearchByID(tree->right, value, GS);
			SearchByID(tree->left, value, GS);
		}
	}

}

void SearchByPrice(node* tree, float value, char GS)
{ 
	if (!tree)
		return;
	SearchByPrice(tree->left, value, GS);

	if (GS == 'G')
	{
		if (tree->data.price > value)
		{
			PrintItem(tree);
		}
	}
	else if (GS == 'S')
	{
		if (tree->data.price < value)
		{
			PrintItem(tree);
		}
	}

	SearchByPrice(tree->right, value, GS);
}

void SearchByInv(node* tree, int value, char GS)
{
	if (!tree)
		return;
	SearchByInv(tree->left, value, GS);

	if (GS == 'G')
	{
		if (tree->data.inventory > value)
		{
			PrintItem(tree);
		}
	}
	else if (GS == 'S')
	{
		if (tree->data.inventory < value)
		{
			PrintItem(tree);
		}
	}

	SearchByInv(tree->right, value, GS);
}

void SearchIfInStock(node* tree, int value)
{
	 if (!tree)
		 return;
	 SearchIfInStock(tree->left, value);

	 if (value == 1)
	 {
		 if (tree->data.inStock)
		 {
			 PrintItem(tree);
		 }
	 }
	 else if (value == 0)
	 {
		 if (!tree->data.inStock)
		 {
			 PrintItem(tree);
		 }
	 }

	 SearchIfInStock(tree->right, value);
}

void SearchByAge(node* tree, int value, char GS)
{
	/* Post Order*/
	if (!tree)
		return;
	SearchByAge(tree->left, value, GS);
	
	if (GS == 'G')
	{
		if (tree->data.age_limit > value)
		{
			PrintItem(tree);
		}
	}
	else if (GS == 'S')
	{
		if (tree->data.age_limit < value)
		{
			PrintItem(tree);
		}
	}

	SearchByAge(tree->right, value, GS);

}

void SearchBy1TextField(node* tree, char arr [20], char choice)
{
	node* element = NULL;
	int cnt = 0;
	if (!tree)
		return;
	SearchBy1TextField(tree->left, arr, choice);

	if (choice == 'm')
	{
		for (int i = 0; i < Manufacturing_Country_lim ; i++)
			if (tree->data.Manufacturing_Country[i] == arr[i])
				cnt++;
		if (cnt == 20)
		{
			PrintItem(tree);
			insertHead(&element, tree->data);
		}
	}
	else if (choice == 'c')
	{
		for (int i = 0; i < Category_Lim; i++)
			if (tree->data.category[i] == arr[i])
				cnt++;
		if (cnt == 20)
		{
			PrintItem(tree);
			insertHead(&element, tree->data);
		}
	}

	SearchBy1TextField(tree->right, arr, choice);
}

void SearchBy2TextFields(node* tree, char arr1[20], char arr2[20])
{
	int cnt = 0;
	if (!tree)
		return;
	SearchBy2TextFields(tree->left, arr1, arr2);

	for (int i = 0; i < Manufacturing_Country_lim; i++) // Manufacturing_Country_lim = Categoty_lim = 20
		if ((tree->data.Manufacturing_Country[i] == arr1[i]) && (tree->data.category[i] == arr2[i]))
			cnt++;
	if (cnt == 20)
		PrintItem(tree);	

	SearchBy2TextFields(tree->right, arr1, arr2);
}

void SearchByDate(node* tree, char arr1[11], int choice)
{
	if (!tree)
		return;
	SearchByDate(tree->left, arr1, choice);
	
	if (choice == 1) //before the selected date
		if (CmpDates(arr1, tree->data.date) > 0)
			PrintItem(tree);
	if (choice == 2)
		if (CmpDates(arr1, tree->data.date) < 0)
			PrintItem(tree);

	SearchByDate(tree->right, arr1, choice);

}

void SearchByDateWithRange(node* tree, char arr1[11], char arr2[11])
{
	if (!tree)
		return;
	SearchByDateWithRange(tree->left, arr1, arr2);

	if(CmpDates(arr1, tree->data.date) < 0 && CmpDates(arr2, tree->data.date) > 0)
		PrintItem(tree);

	SearchByDateWithRange(tree->right, arr1, arr2);
}

/*A function that compares two dates and returns the difference in days between them*/
int CmpDates(char date1[11], char date2[11])
{
	int days, months, years;
	int TotalDays;

	int year1, year2;
	year1 = (int)(date1[6] * 1000 + date1[7] * 100 + date1[8] * 10 + date1[9]);
	year2 = (int)(date2[6] * 1000 + date2[7] * 100 + date2[8] * 10 + date2[9]);
	years = year1 - year2;

	int month1, month2;
	month1 = (int)(date1[3] * 10 + date1[4]);
	month2 = (int)(date2[3] * 10 + date2[4]);
	months = month1 - month2;

	int day1, day2;
	day1 = (int)(date1[0] * 10 + date1[1]);
	day2 = (int)(date2[0] * 10 + date2[1]);
	days = day1 - day2;

	TotalDays = 365 * years + 30 * months + days;
	return TotalDays;
}

void PrintItem(node* tree)
{
	int SpaceCnt = 0, num;
	printf("%d", tree->data.id);

	/* To beautify the print (to create uniform output)*/
	num = tree->data.id;
	while (num != 0)
	{
		SpaceCnt++;
		num /= 10;
	}
	for (int i = 0; i < 15 - SpaceCnt; i++)
		printf(" ");

	for (int i = 0; i < Category_Lim; i++)
	{
		printf("%c", tree->data.category[i]);
	}

	for (int i = 0; i < Manufacturing_Country_lim; i++)
	{
		printf("%c", tree->data.Manufacturing_Country[i]);
	}
	printf("  ");

	printf("%0.2f", tree->data.price);
	num = tree->data.price;
	SpaceCnt = 0;
	while (num != 0)
	{
		SpaceCnt++;
		num /= 10;
	}
	for (int i = 0; i < 5 - SpaceCnt; i++)
		printf(" ");

	printf("%+d ", tree->data.age_limit);
	num = tree->data.age_limit;
	SpaceCnt = 0;
	while (num != 0)
	{
		SpaceCnt++;
		num /= 10;
	}
	for (int i = 0; i < 12 - SpaceCnt; i++)
		printf(" ");

	printf("%d", tree->data.inventory);
	num = tree->data.inventory;
	SpaceCnt = 0;
	while (num != 0)
	{
		SpaceCnt++;
		num /= 10;
	}
	for (int i = 0; i < 10 - SpaceCnt; i++)
		printf(" ");

	printf("%s", tree->data.inStock ? " In Stock     " : "Out of stock ");
	for (int i = 0; i < Date_lim; i++)
	{
		printf("%c", tree->data.date[i]);
	}
	printf("\n");
}


void insert(node** tree, node* parent, Item val)
{
	node* temp = NULL;
	//if tree node is empty, then create a new item and add it as head.
	if (!(*tree)) // if (tree == NULL)
	{
		temp = (node*)malloc(sizeof(node));
		//initialize left and right pointers to NULL, this node is currently a leaf
		temp->left = temp->right = NULL;
		//initialize father to the one who called me.
		temp->parent = parent;

		temp->data = val;
		*tree = temp;
		return;
	}
	/* Sorting by id number*/
	if (val.id < (*tree)->data.id)
	{
		//insert into left pointer of tree, sending the pointer, father (himself) and value
		insert(&(*tree)->left, *tree, val);
	} //
	else if (val.id > (*tree)->data.id)
	{
		//insert into right pointer of tree, sending the pointer, father (himself) and value
		insert(&(*tree)->right, *tree, val);
	}
}

void insertHead(node** tree, Item val)
{
	insert(tree, NULL, val);
}

void print_inorder(node* tree)
{
	if (tree) // if (tree != NULL)
	{
		print_inorder(tree->left);
		PrintItem(tree);
		print_inorder(tree->right);
	}
}

void deltree(node* tree)
{
	// Post-oreder mechanism
	if (tree) // if (tree != NULL)
	{
		deltree(tree->left);
		deltree(tree->right);
		free(tree);
	}
}

node* IDSearch(node** tree, int val)
{
	if (!(*tree)) // if (tree == NULL)
	{
		return NULL;
	}

	if (val < (*tree)->data.id)
	{
		IDSearch(&((*tree)->left), val);
	}
	else if (val > (*tree)->data.id)
	{
		IDSearch(&((*tree)->right), val);
	}
	else if (val == (*tree)->data.id)
	{
		return *tree;
	}
}
