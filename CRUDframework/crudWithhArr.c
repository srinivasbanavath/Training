#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME "framework.dat"
#define FILE_MENU "menu.cfg"
#define FILE_FIELDS "fields.cfg"

void createRecord();
void displayRecords();
void updateRecord();
void deleteRecord();

FILE *fpFields;
char fieldName[50][100];
int fieldCount;

void main()
{  
	int option;
	do
	{
	fpFields = fopen(FILE_FIELDS, "r");
	char fieldMenu[100];
	int counter = 0;
	FILE *fpMenu;
	if(fpMenu == NULL)
	{
	printf("File creation failed\n");
	return;
	}
	
	while(fgets(fieldName[counter], 100, fpFields))
	{
		fieldName[counter][strlen(fieldName[counter]) - 1] = '\0';
		counter++;
	}
	fieldCount = counter;
	fpMenu = fopen(FILE_MENU, "r");
	while(fgets(fieldMenu, 100, fpMenu))
	{
	fieldMenu[strlen(fieldMenu) - 1] = '\0';
	 printf("%s\n", fieldMenu);

	}
	printf("\nPlease enter your option: ");
	scanf("%d", &option);
	switch(option)
	{
	  case 1: createRecord();
	  break;
	  case 2: displayRecords();
	  break;
	  case 3: updateRecord();
	  break;
	  case 4: deleteRecord();
	  break;
	  case 5: exit(0);
	  break;
	  default: printf("\nEnter a valid option!!");
	  break;
	  getch();
	}
    fclose(fpMenu);
  }while(option != 5);
fclose(fpFields);
}

void createRecord()
{
  printf("\nADD NEW RECORD.\n");
  FILE *fpData;
  char fieldValue[20], status[20];
  int counter = 0;
  fpData = fopen(FILE_NAME, "a");
  if(fpData == NULL)
  {
    printf("\nCan't open file or file doesn't exist. ");
    exit(0);
  }
  strcpy(status, "Activated");
  fwrite(&status, sizeof(status), 1, fpData);
  while(counter < fieldCount)
  {
    printf("Please enter %s: ", fieldName[counter]);
    scanf("%s", fieldValue);
    fwrite(fieldValue, sizeof(fieldValue), 1, fpData);
    counter++;
  }  
  printf("\nNew details successfully saved. \nPress Enter to continue. ");
  fclose(fpData);
  getch();
  system("cls");	
}

void displayRecords()
{
  printf("\nALL THE RECORDS\n");
  FILE *fpData;
  char fieldValue[20], status[20], status1[20];
  int counter = 0;
  strcpy(status, "Activated");
  strcpy(status1, "Deactivated");
  fpData = fopen(FILE_NAME, "r");
  if(fpData == NULL)
  {
    printf("\nCan't open file or file doesn't exist. ");
    exit(0);
  }
  while(fread(fieldValue, sizeof(fieldValue), 1, fpData))
  {
    if(strcmp(fieldValue, status) == 0 || strcmp(fieldValue, status1) == 0)
    {
      printf("status: ");
      printf("%s\n", fieldValue);
    }
    else
    {
      fgets(fieldName[counter], 100, fpFields);
      printf("%s: %s\n", fieldName[counter], fieldValue);
      counter++;
      if(counter == fieldCount)
      {
        printf("-----------\n");
        counter = 0;
      }
    }
  }
  printf("\nPress Enter to continue.");
  fclose(fpData);
  getch();
  system("cls");	
}

void updateRecord()
{
  FILE *fpData;
  char fieldValue[20], tempFieldValue[20], updatedFieldValue[20];
  int choice, counter = 0;
  fpData = fopen(FILE_NAME, "r+");
  if(fpData == NULL)
  {
    printf("File creation failed\n");
    return;
  }
  
  printf("\nTo change your details enter your ID: ");
  scanf("%s", tempFieldValue);
  for(counter = 1; counter < fieldCount; counter++)
  {
    printf("Enter %d to update %s: \n", counter, fieldName[counter]);
  }
  scanf("%d", &choice);
  counter = 0;
  while(fread(fieldValue, sizeof(fieldValue), 1, fpData))
  {
    counter++;
    if(strcmp(tempFieldValue, fieldValue) == 0)
    {
      // fseek(fpData, 20, SEEK_CUR);
      fread(fieldName[counter], sizeof(fieldName), 1, fpFields);
      printf("Enter new %s to be updated: ", fieldName[choice]);
      scanf("%s", fieldValue);
      fseek(fpData, sizeof(fieldValue)* + (choice - 1), SEEK_CUR);
      fwrite(fieldValue, sizeof(fieldValue), 1, fpData);
      printf("\nDetails successfully updated. \n");
      break;
    }
  }
  printf("\nEnter a key to continue.");
  fclose(fpData);
  getch();
  system("cls");	
}

void deleteRecord()
{
	char fieldValue[20], fieldName[100], status [20];
	char decision;
	
	FILE *fpData;
	
	fpData = fopen(FILE_NAME, "r+");
	if(fpData == NULL || fpFields == NULL)
	{
		printf("File creation failed\n");
		return;
	}

	char tempFieldValue[20];
	rewind(fpFields);
	strcpy(status, "Activated");
	fgets(fieldName, sizeof(fieldName), fpFields);
	fieldName[strlen(fieldName) - 1] = '\0';
	printf("To delete your details enter your %s ", fieldName);
	scanf("%s", tempFieldValue);


	while(fread(fieldValue, sizeof(fieldValue), 1, fpData))
	{
		if(strcmp(tempFieldValue, fieldValue) == 0)
		{
			fseek(fpData, -2 * sizeof(fieldValue), SEEK_CUR);
			fread(fieldValue, sizeof(fieldValue), 1, fpData);
			printf("Current status: %s.\n", fieldValue);

			if(strcmp(fieldValue, status) == 0)
			{	
				printf("To confirm press 'Y'.\n");
				printf("To cancel press 'N'.\n");
				printf("Enter your choice and press enter: ");
				scanf("%s", &decision);
				if(decision == 'Y')
				{
					strcpy(fieldValue, "Deactivated");
					fseek(fpData, sizeof(fieldValue) * -1, SEEK_CUR);
					fwrite(fieldValue, sizeof(fieldValue), 1, fpData);
					printf("Deleted successfully.\n", fieldValue);
					printf("Press enter to continue.\n");
				}
				else if(decision == 'N')
				{
					return;
				}
				else if(decision != 'N' || decision !='Y')
				{
					printf("Choose available options only.\n");
					return;
				}
			}
			else
			{
				printf("Record already deleted.");
			}

			break;
		}	
	}
	fclose(fpData);
	
	getch();
	system("cls");	
}