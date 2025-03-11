#include "Log.h"

/* For the first operation recorded in the log file.*/
void IsFirstOp()
{
	FILE* Log_File = NULL;
	Log_File = fopen("Log.txt", "r");
	if (!Log_File) // for the first time
	{
		Log_File = fopen("Log.txt", "w");
		assert(Log_File);
	}
	fclose(Log_File);
}


/*Documentation of an adding of an item in the log file.*/
void DocAddingItem(Item item)
{
	char Time[80];
	IsFirstOp();
	FILE* Log_File = NULL;
	Log_File = fopen("Log.txt", "a");
	assert(Log_File);
	fprintf(Log_File, "%s", "An item with an ID number ");
	fprintf(Log_File, "%d", item.id);
	fprintf(Log_File, "%s" ," has been added to stock. Execution Date: ");
	getTime(Time, sizeof(Time));
	for (int i = 0; i < 19; i++)
		fputc(Time[i], Log_File);
	fprintf(Log_File, "\n");
}

/*Documentation of an adding of a new  customer in the log file.*/
void DocNewCustomer(Customer customer)
{
	char Time[80];
	IsFirstOp();
	FILE* Log_File = NULL;
	Log_File = fopen("Log.txt", "a");
	assert(Log_File);
	fprintf(Log_File, "%s", "A new customer has joined our customer list and his ID number is: ");
	fprintf(Log_File, "%d. Execution Date: ", customer.customer_id);
	getTime(Time, sizeof(Time));
	for (int i = 0; i < 19; i++)
		fputc(Time[i], Log_File);
	fprintf(Log_File, "\n");
}

/*Documentation of a removing of an existing item in the log file.*/
void DocRemovingItem(Item item)
{
	char Time[80];
	IsFirstOp();
	FILE* Log_File = NULL;
	Log_File = fopen("Log.txt", "a");
	assert(Log_File);
	fprintf(Log_File, "%s", "An item with an ID number ");
	fprintf(Log_File, "%d", item.id);
	fprintf(Log_File, "%s", " has been removed from stock. Execution Date: ");
	getTime(Time, sizeof(Time));
	for (int i = 0; i < 19; i++)
		fputc(Time[i], Log_File);
	fprintf(Log_File, "\n");
}

/*Documentation of a removing of an existing customer in the log file.*/
void DocRemovingCustomer(Customer customer)
{
	char Time[80];
	IsFirstOp();
	FILE* Log_File = NULL;
	Log_File = fopen("Log.txt", "a");
	assert(Log_File);
	fprintf(Log_File, "%s", "A new customer has been removed from our customer list, his ID number is: ");
	fprintf(Log_File, "%d. Execution Date: ", customer.customer_id);
	getTime(Time, sizeof(Time));
	for (int i = 0; i < 19; i++)
		fputc(Time[i], Log_File);
	fprintf(Log_File, "\n");
}

/*Documentation of an update of an item in the log file.*/
void DocUpdateItem(Item item)
{
	char Time[80];
	IsFirstOp();
	FILE* Log_File = NULL;
	Log_File = fopen("Log.txt", "a");
	assert(Log_File);
	fprintf(Log_File, "%s", "Details have been updated for the item with the following ID number: ");
	fprintf(Log_File, "%d. Execution Date: ", item.id);
	getTime(Time, sizeof(Time));
	for (int i = 0; i < 19; i++)
		fputc(Time[i], Log_File);
	fprintf(Log_File, "\n");
}

/*Documentation of the sale of an item to a customer in the log file.*/
void DocSellToCustomer(Customer customer, Item item)
{
	char Time[80];
	IsFirstOp();
	FILE* Log_File = NULL;
	Log_File = fopen("Log.txt", "a");
	assert(Log_File);
	fprintf(Log_File, "%s", "An item with ID number ");
	fprintf(Log_File, "%d", item.id);
	fprintf(Log_File, " was sold to a customer with ID number ");
	fprintf(Log_File, "%d. Execution Date: ", customer.customer_id);
	getTime(Time, sizeof(Time));
	for (int i = 0; i < 19; i++)
		fputc(Time[i], Log_File);
	fprintf(Log_File, "\n");
}

/*Documentation of an item return by a customer in the log file.*/
void DocReturningItem(int customer_id, Item item)
{
	char Time[80];
	IsFirstOp();
	FILE* Log_File = NULL;
	Log_File = fopen("Log.txt", "a");
	assert(Log_File);
	fprintf(Log_File, "%s", "An item with ID number ");
	fprintf(Log_File, "%d", item.id);
	fprintf(Log_File, " was returned to the store by a customer with the ID number ");
	fprintf(Log_File, "%d. Execution Date: ", customer_id);
	getTime(Time, sizeof(Time));
	for (int i = 0; i < 19; i++)
		fputc(Time[i], Log_File);
	fprintf(Log_File, "\n");
}

/*Documentation of changing customer status to club member.*/
void DocChangeToClubMember(Customer customer)
{
	char Time[80];
	IsFirstOp();
	FILE* Log_File = NULL;
	Log_File = fopen("Log.txt", "a");
	assert(Log_File);
	fprintf(Log_File, "%s", "A customer with ID ");
	fprintf(Log_File, "%d ", customer.customer_id);
	fprintf(Log_File, "%s", "has joined to the store's club member list. Execution Date: ");
	getTime(Time, sizeof(Time));
	for (int i = 0; i < 19; i++)
		fputc(Time[i], Log_File);
	fprintf(Log_File, "\n");
}

/*Documentation of a new employee who joined our staff in the log file.*/
void DocNewEmployee(Employee employee)
{
	char Time[80];
	IsFirstOp();
	FILE* Log_File = NULL;
	Log_File = fopen("Log.txt", "a");
	assert(Log_File);
	fprintf(Log_File, "%s", "New employee with the following username:");
	for (int i = 0; i < 20; i++)
	{
		if(employee.username[i] != ' ')
			fprintf(Log_File, "%c", employee.username[i]);
	}
	fprintf(Log_File, "%s", " has joined our staff. Execution Date: ");
	getTime(Time, sizeof(Time));
	for (int i = 0; i < 19; i++)
		fputc(Time[i], Log_File);
	fprintf(Log_File, "\n");
}