#include "employees.h"
#include "log.h"

const UserName_Lim = 20;
const FirstName_Lim = 10, Password_Lim = 10;
const Wage_lim = 4;
const line_len = 50;

/* A function that returns '1' if the file exists, otherwise retruns '0'*/
int checkIfEmployeeFileExists()
{
	FILE* check = NULL;
	check = fopen("Employees.txt", "r");
	if (!check)
		return 0; // The file doesn`t exists	
	fclose(check);
	return 1; // The file exists
}

/* A function that being called if the employee`s file doesn`t exist and create one*/
void createDefaultAdmin()
{
	Employee admin = {"admin", "manager","12345678", 1, 100};
	FILE* employees_file = NULL;
	employees_file = fopen("Employees.txt", "a");
	if (!employees_file)
	{
		printf("Could not open the file, please try again later\n If this error keeps happening, please contact our customer support");
		return;
	}
	printf("The file has opened successfully\n");
	CreateTitles(employees_file);
	WriteDefAdminToFile(employees_file, admin);
	fclose(employees_file);
	

}

/* A function that copy the data of DEFAULT ADMIN employee to the employee`s file*/
void WriteDefAdminToFile(FILE* employee_file, Employee emp)
{
	/* The employee`s file will be define with the following limitations:
	 User name : 20 characters (including padding)
	 First name : 10 characters (including padding) 
	 Password :10 characters (including padding)
	 Level : 1 character + (padding of 1 space)
	 Wage : 4 characters (including padding) + 1 new line
	 -> Total characters per line = 50
	 */

	int pos1, pos2; 

	fseek(employee_file, 0 , SEEK_END);

	pos1 = ftell(employee_file);
	fprintf(employee_file, "%s", emp.username);
	pos2 = ftell(employee_file);
	CreatePadding(employee_file, pos2, pos1 + UserName_Lim - 1);


	pos1 = ftell(employee_file);
	fprintf(employee_file, "%s", emp.firstname);
	pos2 = ftell(employee_file);
	CreatePadding(employee_file, pos2, pos1 + FirstName_Lim);

	pos1 = ftell(employee_file);
	fprintf(employee_file, "%s", emp.password);
	pos2 = ftell(employee_file);
	CreatePadding(employee_file, pos2, pos1 + Password_Lim + 1);

	pos1 = ftell(employee_file);
	fprintf(employee_file, "%d", emp.level);
	pos2 = ftell(employee_file);
	CreatePadding(employee_file, pos2, pos1 + 4);


	pos1 = ftell(employee_file);
	fprintf(employee_file, "%d", emp.Wage);
	pos2 = ftell(employee_file);
	CreatePadding(employee_file, pos2, pos1 + Wage_lim);

	fprintf(employee_file, "\n");
}

/* Creating the titles for each category */
void CreateTitles(FILE* employee_file)
{
	int pos1, pos2;
	
	pos1 = ftell(employee_file);

	fprintf(employee_file, "UserName");
	pos2 = ftell(employee_file);
	CreatePadding(employee_file, pos2, pos1 + UserName_Lim - 1);
	pos1 = ftell(employee_file);

	fprintf(employee_file, "FirstName");
	pos2 = ftell(employee_file);
	CreatePadding(employee_file, pos2, pos1 + FirstName_Lim);
	pos1 = ftell(employee_file);

	fprintf(employee_file, "Password");
	pos2 = ftell(employee_file);
	CreatePadding(employee_file, pos2, pos1 + Password_Lim - 1);

	fprintf(employee_file, "Level");
	fputc(' ', employee_file);

	fprintf(employee_file, "Wage");
	fprintf(employee_file, "\n");
}

/* Creating a padding based on the limitations*/
void CreatePadding(FILE* employee_file, int current_pos, int Pad_Len)
{
	for (int i = 0 ; i < Pad_Len - current_pos; i++)
		fputc(' ', employee_file);
}

/*Function to log on of registered user if 3 failed attempts detected -  Login fails*/
Employee login()
{
	int  cntfails = 0, result;
	char UserName[20];
	char Password[10];
	FILE* employee_file = NULL;
	Employee emp;
	employee_file = fopen("Employees.txt", "r+");
	if (employee_file)
	{
		printf("Login page\n\n");
		while (cntfails < 3)
		{
			LoginInfo(&UserName, UserName_Lim, &Password, Password_Lim);
			result = IsUserNameExist(&UserName, employee_file);
			if (!result)
			{
				cntfails++;
				printf("Please try again, user name does not exist in the system\n\n");
			}
			else
			{
				if (IsPasswordCorrect(&Password, employee_file, result))
					break;
				else
				{
					cntfails++;
					printf("Please try again, password is not correct\n\n");

				}
			}
		}
		if (cntfails == 3)
			emp.level = 0;
		else
		{
			for (int i = 0; i < UserName_Lim; i++)
				emp.username[i] = UserName[i];

			fseek(employee_file, result * line_len + UserName_Lim - 1, SEEK_SET);
			for (int i = 0; i < FirstName_Lim; i++)
			{
				emp.firstname[i] = fgetc(employee_file);
			}
			for (int i = 0; i < Password_Lim; i++)
				emp.password[i] = Password[i];
			fseek(employee_file, result * line_len + UserName_Lim + FirstName_Lim + Password_Lim, SEEK_SET);
			fscanf(employee_file, "%d", &emp.level);
			fseek(employee_file, result * line_len + UserName_Lim + FirstName_Lim + Password_Lim + 3, SEEK_SET);
			int pos = ftell(employee_file);
			fscanf(employee_file, "%d", &emp.Wage);
			
		}
		fclose(employee_file);
		return emp;
	}
}

/*A function for receiving login details and pass them by reference*/
void LoginInfo(char* UserName, int UserNameLen, char* Password, int PasswordLen)
{
	int index = 0;
	char c;
	printf("Please enter your user name (limited to 19 characters!) and than press enter:\n");
	while (index < UserNameLen - 1)
	{
		scanf("%c", &c);
		if (c == '\n')
			break;
		UserName[index] = c;
		index++;
	}
	while (index < UserNameLen)
		UserName[index++] = ' ';
	index = 0;
	/* If more than 9 characters are entered, the first 9 will be considered*/
	printf("Please enter your password (limited to 9 characters!) and than press enter:\n");
	while (index < PasswordLen - 1)
	{
		scanf("%c", &c);
		if (c == '\n')
			break;
		Password[index] = c;
		index++;
	}
	while (index < PasswordLen)
		Password[index++] = ' ';
}

/*A function that retruns the number of line of the user name if he exist in file, otherwise returns '0'*/
int IsUserNameExist(char* UserName, FILE* employee_file)
{
	int linecnt = 1;
	int cnt = 0;
	char UserNameForChecking[20];
	fseek(employee_file, 0, SEEK_END);
	int pos = ftell(employee_file) / line_len;
	while (linecnt < pos)
	{
		fseek(employee_file, line_len * linecnt , SEEK_SET);
		for(int i = 0; i< UserName_Lim - 1; i++)
			UserNameForChecking[i] = fgetc(employee_file);
		UserNameForChecking[UserName_Lim - 1] = ' ';
		for (int j = 0; j < UserName_Lim; j++)
		{
			if (UserName[j] != UserNameForChecking[j])
			{
				linecnt++;
				break;
			}
			else
				cnt++;
			if (cnt == 20)
				return linecnt;
		}		

	}
	return 0;
}

/* A that return '1' if the password is correct, otherwise returns '0'*/
int IsPasswordCorrect(char* Password, FILE* employee_file, int RelevantLine)
{
	int linecnt = 1;
	int cnt = 0;
	char PasswordForChecking[10];
	fseek(employee_file, RelevantLine * line_len +UserName_Lim + FirstName_Lim - 1, SEEK_SET);
	for (int i = 0; i < Password_Lim - 1; i++)
		PasswordForChecking[i] = fgetc(employee_file);
	PasswordForChecking[Password_Lim - 1] = ' ';
	for (int j = 0; j < Password_Lim; j++)
	{
		if (Password[j] != PasswordForChecking[j])
		{
			linecnt++;
			break;
		}
		else
			cnt++;
	}
	if (cnt == 10)
		return 1;
	return 0;
}

/* A function to upload all the new employees to the queue, before registering them to the file */
void AddNewEmployee(int size_new_employees)
{
	FILE* Employee_File = NULL;
	Employee* emp = NULL;
	Employee_File = fopen("Employees.txt", "r+");
	if (!Employee_File)
		return;
	emp = (Employee*)malloc(size_new_employees * sizeof(Employee));
	assert(emp);
	
	for (int i = 0; i < size_new_employees; i++)
	{

		getchar(); // To handling with enter key
		printf("updating an new employee #%d\n", i + 1);
		printf("Please enter user name for employee #%d and press enter\n", i + 1);
		PaddingInfo(&emp[i].username, UserName_Lim);
		while (IsUserNameExist(&emp[i].username, Employee_File))
		{
			printf("Sorry, but the user name is already exsit in system.\nPlease enter a new one\n");
			PaddingInfo(&emp[i].username, UserName_Lim);
		}

		printf("Great! please enter first name for employee #%d and press enter\n", i + 1);
		PaddingInfo(&emp[i].firstname, FirstName_Lim);
		printf("Please choose the initial password for employee #%d and press enter\n", i + 1);
		PaddingInfo(&emp[i].password, Password_Lim);

		printf("What is his permission level?\n");
		scanf("%d", &emp[i].level);
		while (emp[i].level < 1 || emp[i].level > 3)
		{
			printf("Sorry, but no such permission level, please select again (1-3)\n");
			scanf("%d", &emp[i].level);
		}
		printf("Great! please enter hourly wage for employee #%d and press enter\n", i + 1);
		scanf("%d", &emp[i].Wage);
	
	}
	for (int i = 0; i < size_new_employees; i++)
	{
		WriteToEmployeeFile(Employee_File, emp[i]);
		DocNewEmployee(emp[i]);
	}
	fclose(Employee_File);
	
}

/* A function that register an new employee by the admin return the relevant info about him based on passing by ref.*/
void PaddingInfo(char* arr, int len)
{
	char c;
	int index = 0;
	while (index < len)
	{
		scanf("%c", &c);
		if (c == '\n')
			break;
		*(arr+index) = c;
		index++;
	}
	while (index < len)
		arr[index++] = ' ';
	return arr;
}

/* Write general employee to the file (the padding was adding to user name, first name and password)*/
void WriteToEmployeeFile(FILE* employee_file, Employee emp)
{
	int pos1, pos2;
	
		fseek(employee_file, 0, SEEK_END);

		for (int i = 0; i < UserName_Lim - 1; i++)
			fputc(emp.username[i], employee_file);
		for (int i = 0; i < FirstName_Lim; i++)
			fputc(emp.firstname[i], employee_file);
		for (int i = 0; i < Password_Lim; i++)
			fputc(emp.password[i], employee_file);
	

		pos1 = ftell(employee_file);
		fprintf(employee_file, " %d", emp.level);
		pos2 = ftell(employee_file);
		CreatePadding(employee_file, pos2, pos1 + 4);

		pos1 = ftell(employee_file);
		fprintf(employee_file, " %d", emp.Wage);
		pos2 = ftell(employee_file);
		CreatePadding(employee_file, pos2, pos1 + Wage_lim + 1);
		fprintf(employee_file, "\n");
}

/* A function for upgrading employee`s wage*/
void UpgradeWage(Employee admin_emp)
{
	Employee emp;
	int line, line_admin;
	char pass[10] , PassToCheck[10];
	FILE* Employee_File = NULL;
	Employee_File = fopen("Employees.txt", "r+");
	if (!Employee_File)
		return;
	getchar();
	printf("\nPlease type in the username of the employee whose salary you would like to update\n");
	PaddingInfo(&emp.username, UserName_Lim);
	line = IsUserNameExist(&emp.username, Employee_File);
	while (line == 0)
	{
		printf("Sorry, but the user name is not exsit in system.\nPlease enter a new one\n");
		PaddingInfo(&emp.username, UserName_Lim);
		line = IsUserNameExist(&emp.username, Employee_File);
	}
	printf("Enter the updated wage\n");
	scanf("%d", &emp.Wage);
	line_admin = IsUserNameExist(&admin_emp.username, Employee_File);
	fseek(Employee_File, line_len * line_admin + UserName_Lim + FirstName_Lim - 1, SEEK_SET);
	for (int i = 0; i < Password_Lim; i++)
		pass[i] = fgetc(Employee_File);
	getchar();
	printf("Please verify your password to confirm this operation\n");
	PaddingInfo(&PassToCheck, Password_Lim);
	if (!ComparePass(&PassToCheck, &pass))
		printf("The password entered is incorrect, please try again later\n");
	else
	{
		fseek(Employee_File, line_len * line + UserName_Lim + FirstName_Lim + Password_Lim + 4, SEEK_SET);
		if (emp.Wage < 100)
			fprintf(Employee_File, "%d ", emp.Wage);
		else
			fprintf(Employee_File, "%d", emp.Wage);
		printf("The operation completed successfully\n");
	}
	fclose(Employee_File);
}

/* A function for changing the password*/
void ChangePass(Employee emp)
{

	char pass[10], repeatpass[10];
	int line;
	FILE* Employee_File = NULL;
	Employee_File = fopen("Employees.txt", "r+");
	if (!Employee_File)
		return;
	getchar();
	line = IsUserNameExist(emp.username, Employee_File);
	printf("Please enter an new password\n");
	PaddingInfo(&pass, Password_Lim);
	printf("Please enter a password again to confirm\n");
	PaddingInfo(&repeatpass, Password_Lim);
	while (!ComparePass(&pass, &repeatpass))
	{
		printf("The passwords do not match.. Please try again\n");
		printf("Please enter an new password\n");
		PaddingInfo(&pass, Password_Lim);
		printf("Please enter a password again to confirm\n");
		PaddingInfo(&repeatpass, Password_Lim);
	}
	fseek(Employee_File, line_len * line + UserName_Lim + FirstName_Lim - 1, SEEK_SET);
	for (int i = 0; i < Password_Lim; i++)
		fputc(pass[i], Employee_File);

	fclose(Employee_File);
}

/*A function that returns '1' if two chars arrays are equal, otherwise retruns '0' */
int ComparePass(char* pass1, char* pass2)
{
	for (int i = 0; i < Password_Lim; i++)
		if (pass1[i] != pass2[i])
			return 0;
	return 1;
}