#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include "customers.h"
#include "items.h"
#include "employees.h"


void IsFirstOp();
void DocAddingItem(Item item);
void DocNewCustomer(Customer customer);
void DocRemovingItem(Item item);
void DocRemovingCustomer(Customer customer);
void DocUpdateItem(Item item);
void DocSellToCustomer(Customer customer, Item item);
void DocReturningItem(int customer_id, Item item);
void DocChangeToClubMember(Customer customer);
void DocNewEmployee(Employee employee);
