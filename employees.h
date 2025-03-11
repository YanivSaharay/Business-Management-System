#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>


typedef struct employee {
    char username[20];
    char firstname[10];
    char password[10];
    unsigned int level; // 1 - admin, 2 - employee, 3 - practicioner
    int Wage;
}Employee;


int checkIfEmployeeFileExists();
void createDefaultAdmin();
void WriteDefAdminToFile(FILE* employee_file, Employee emp);
void CreateTitles(FILE* employee_file);
void CreatePadding(FILE* employee_file,int current_pos, int Pad_Len);
Employee login();
void LoginInfo(char* UserName, int UserNameLen, char* Password, int PasswordLen);
int IsUserNameExist(char* UserName, FILE* employee_file);
int IsPasswordCorrect(char* Password, FILE* employee_file, int RelevantLine);
void AddNewEmployee(int size_new_employees);
void PaddingInfo(char* arr, int len);
void WriteToEmployeeFile(FILE* employee_file, Employee emp);
void UpgradeWage(Employee admin_emp);
void ChangePass(Employee emp);
int ComparePass(char* pass1, char* pass2);