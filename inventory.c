/*This program is for CRUD operations using singly linked list.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myFunctions.c"
#define DATAFILE "items.dat"
#define LENGTHOFITEMID 20
#define LENGTHOFNAME 20

typedef struct inventory
{
	char itemId[LENGTHOFITEMID];
	char itemName[LENGTHOFNAME];
	float price;
} Inventory;
typedef struct node
{
	Inventory item;
	struct node *next;
} Node;
void showMenu();
void create();
void show();
void update();
void delete();
void loadList();
void readItemDetails(Inventory*);
void appendItemToList(Inventory);
void saveListIntoFile();
void changeCurrentToEnd();
void updateItemName(Inventory*);
void updateItemPrice(Inventory*);
void printItemDetails(Inventory);
void printItemNotFoundMessage(char[]);
Node* findItemRecord(char[], Node**);

struct node *start = NULL;
struct node *current = NULL;
FILE *fpItems;
int main()
{
	loadList();
	showMenu();
	return 0;
}
void showMenu()
{
	int choice;
	do {
		int length = printf("\nInventory");
		printf("\n");
		underline(length);
		printf("\n1. Create\n2. Read\n3. Update\n4. Delete\n5. Exit\n-----------------------\nEnter your choice: ");
		scanf("%d", &choice);
		getchar();
		switch (choice)
		{
			case 1:create(); break;
			case 2:show(); break;
			case 3:update(); break;
			case 4:delete(); break;
			case 5:exit(0); 
			default:printf("Please enter a valid choice.\n"); break;
		} 
	} while (choice != 5);
}
void readItemDetails(Inventory *item)
{
	printf("Enter Item Id: ");
	readText(item->itemId, LENGTHOFITEMID);
	printf("Enter Item Name: ");
	readText(item->itemName, LENGTHOFNAME);
	printf("Enter Price: ");
	readFloat(&item->price);
}
void appendItemToList(Inventory itemPara)
{
	Node *newNode = malloc(sizeof(Node));
	newNode->item = itemPara;
	newNode->next = NULL;
	if (start == NULL)
    {
        start = newNode;
        current = newNode;
    }
    else
    {
        changeCurrentToEnd();
        current->next = newNode;
        current = newNode;
    }
}
void changeCurrentToEnd()
{
	if (start == NULL)
	{
		current = NULL;
	}
	else
	{
		current = start;
		while (current->next != NULL)
		{
			current = current->next;
		} 
	}
}
void saveListIntoFile()
{
	fpItems = fopen(DATAFILE, "wb");
	current = start;
	if (current == NULL)
	{
		printf("List is Empty.\n");
	}
	else
	{
		while (current != NULL)
		{
			fwrite(&current->item, sizeof(Inventory), 1, fpItems);	
			current = current->next;
		}
	}
	fclose(fpItems);
}
void loadList()
{
	fpItems = fopen(DATAFILE, "rb");
	Inventory item;
	start = current = NULL;
	while (fread(&item, sizeof(Inventory), 1, fpItems))
	{
		Node *newNode = malloc(sizeof(Node));
		newNode->item = item;
		newNode->next = NULL;
		if (start == NULL)
		{
			start = newNode;
			current = newNode;
		}
		else
		{
			current->next = newNode;
			current = newNode;
		}
	}
	fclose(fpItems);
}
void create()
{
	Inventory item;
	readItemDetails(&item);
	appendItemToList(item);
	saveListIntoFile();
	printf("List saved successfully.\n");
}
void show()
{
	current = start;
	if (current == NULL)
	{
		printf("List is Empty.\n");
	}
	else
	{
		while (current != NULL)
		{
			printItemDetails(current->item);
			current = current->next;
		}
	}
}
void update()
{
	char targetItemId[LENGTHOFITEMID];
	printf("Enter Target Item Id: ");
	readText(targetItemId, LENGTHOFITEMID);
	struct node *previous = NULL;
	struct node *currentAddress = findItemRecord(targetItemId, &previous);
	int choice = 0;
	if(currentAddress == NULL)
	{
		printItemNotFoundMessage(targetItemId);
		return;
	}
	printf("Item %s is found.\n");
	printf("Old Item Name: %s.\n", currentAddress->item.itemName);
	printf("Old Item Price: %.2f.\n", currentAddress->item.price);
	do
	{
		printf("\n1. Update Item Name\n2. Update Item Price\n3. Update Both Item Name and Price.\n------------------------\nEnter your choice: ");
		scanf("%d", &choice);
		getchar();
		switch (choice)
		{
			case 1: updateItemName(&currentAddress->item); break;
			case 2: updateItemPrice(&currentAddress->item); break;
			case 3: updateItemName(&currentAddress->item); updateItemPrice(&currentAddress->item); break;
			default: printf("Please enter valid choice.\n"); break;
		}
	} while (!(choice >= 1 && choice <= 3));
	saveListIntoFile();
	printf("Item %s is updated successfully.\n", targetItemId);
}
void delete()
{
	char targetItemId[LENGTHOFITEMID];
	printf("Enter Target Item Id: ");
	readText(targetItemId, LENGTHOFITEMID);
	struct node *previous = NULL;
	struct node *currentAddress = findItemRecord(targetItemId, &previous);
	if (currentAddress == NULL)
	{
		printItemNotFoundMessage(targetItemId);
		return;
	}
	if (previous == NULL)
	{
		start = currentAddress->next;
	}
	else
	{
		previous->next = currentAddress->next;
	}
	free(currentAddress);
	saveListIntoFile();
	printf("Item %s is deleted successfully.\n", targetItemId);
}

Node* findItemRecord(char targetItemId[LENGTHOFITEMID], Node **previous)
{
	*previous =  NULL;
	current = start;
	while (current != NULL)
	{
		if (strcmp(targetItemId, current->item.itemId) == 0)
		{
			printf("Item %s is found.\n");
			return current;
		}
		*previous = current; 
		current = current->next;
	}
	return NULL;
}

void printItemDetails(Inventory item)
{
	printf("\nItem Id: %s\nItem name: %s\nPrice: %.2f.\n", item.itemId, item.itemName, item.price);
}
void printItemNotFoundMessage(char targetItemId[LENGTHOFITEMID])
{
	printf("Item %s is not found.\n", targetItemId);
}
void updateItemName(Inventory *item)
{
	printf("Enter New Item Name: ");
	readText(item->itemName, LENGTHOFITEMID);
}
void updateItemPrice(Inventory *item)
{
	printf("Enter New Item Price: ");
	readFloat(&item->price);
}
