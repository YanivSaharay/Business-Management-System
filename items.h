#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

typedef struct item{
    int id;
    char Manufacturing_Country[20]; 
    char category[20]; 
    float price;
    bool inStock;
    char date[11];
    int inventory;
    int age_limit;
} Item;

typedef struct bin_tree {
    Item data;
    struct bin_tree* right, * left, * parent;
}node;

void AddNewItem(node** root);
void PaddingItemsInfo(char* arr, int len);
void WriteToItemsFile(node* root);
void CreateItemsTitle(FILE* ItemsFile);
void CreateItemsPadding(FILE* ItemsFile, int current_pos, int Pad_Len);
void Inorder_Writing_Items(node* root, FILE* ItemsFile);
void Reload_Existing_Items_To_The_Tree(node** root);
node* RemoveItem(node* root, int val);
node* UpdateItem(node* root);
void SearchItem(node* root);
void PrintItem(node* tree);
int CmpDates(char date1[11], char date2[11]);
void SearchByID(node* tree, int value, char GS);
void SearchByPrice(node* tree, float value, char GS);
void SearchByInv(node* tree, int value, char GS);
void SearchByAge(node* tree, int value, char GS);
void SearchIfInStock(node* tree, int value);
void SearchBy1TextField(node* tree, char arr[20], char choice);
void SearchBy2TextFields(node* tree, char arr1[20], char arr2[20]);
void SearchByDate(node* tree, char arr1[11], int choice);
void SearchByDateWithRange(node* tree, char arr1[11], char arr2[11]);


void insert(node** tree, node* parent, Item val);
void insertHead(node** tree, Item val);
void print_inorder(node* tree);
void deltree(node* tree);
node* IDSearch(node** tree, int val);