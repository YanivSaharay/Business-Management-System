#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdbool.h>
#include<stdlib.h>
#include<assert.h>
#include <time.h>
#include "items.h"

typedef struct purchaseditems {
    int id;
    char Purchased_Date[80];
    bool IsReturn;
}PurchasedItems;

typedef struct bin_tree2 {
    PurchasedItems data;
    struct bin_tree* right, * left, * parent;
}node_PurchasedItems;

typedef struct customer {
    char name[20];
    int  customer_id;
    float purchases_amount;
    char join_date [80];
    char birthday[11];
    bool club_member;
    node_PurchasedItems* purchased_items;
}Customer;


typedef struct bin_tree3 {
    Customer data;
    struct bin_tree* right, * left, * parent;
}node_Customer;



void AddNewCustomer(node_Customer** root);
void PaddingCustomerInfo(char* arr, int len);
void WriteToCustomerssFile(node_Customer* root);
void CreateCustomersTitle(FILE* CustomersFile);
void CreateCustomersPadding(FILE* CustomersFile, int current_pos, int Pad_Len);
void Inorder_Writing_Customers(node_Customer* root, FILE* CustomersFile);
void Reload_Existing_Customers_To_The_Tree(node_Customer** root);
void PrintCustomer(node_Customer* tree);
int CompareJoinDates(char JoinDate1[80], char DateToCheck[80]);
node_Customer* SearchCustomerID(node_Customer** tree, int id);
node_Customer* RemoveCustomer(node_Customer* root, char val[80]);
void ChangeToClubMember(node_Customer* root);
void getTime(char* cur_time_adr, unsigned int Size);
void SellItemToCustomer(node_Customer* root_customer, node* root_items);
void PurchasesHistory(node_PurchasedItems* tree);
void InsertHead_PurchasesHistory(node_PurchasedItems** tree, PurchasedItems val);
void Insert_PurchasesHistory(node_PurchasedItems** tree, node_PurchasedItems* parent, PurchasedItems val);
void Print_Purchases_History(node_PurchasedItems* tree);
int Number_Of_Purchases(node_PurchasedItems* tree);
int CalculateDiscount(int size_after, node_Customer* root, char DateToCheck[80]);
int CalculatesMonthsDates(char birthday[80], char DateToCheck[80]);
void TransactionsDocumentations(node_Customer* root_customer, node_PurchasedItems* root_purchased);
void CreateTransactionsTitle(FILE* TransactionsFile);
void WriteTranscationsToFile(FILE* TransactionsFile, node_Customer* root_customer, node_PurchasedItems* root_purchased);
void Reload_Existing_Transactions_To_The_Tree(int customer_id, node_PurchasedItems** root_purchased);
void ReturnItems(int customer_id, node_PurchasedItems* root_purchased, node* root_items);
node_PurchasedItems* SearchPurchase(node_PurchasedItems** root_purchased, int ItemID);
int DiffDaysBetweenDates(char purchase[80], char DateToCheck[80]);


void insert_customer(node_Customer** tree, node_Customer* parent, Customer val);
void insertHead_Customer(node_Customer** tree, Customer val);
void print_customer_inorder(node_Customer* tree);
void DeleteTree(node_PurchasedItems* tree);