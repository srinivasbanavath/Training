#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define FILE_FINAL "framework.dat"
#define FILE_MENU "menu.cfg"
#define FILE_FIELDS "fields.cfg"

void createRecord();
void readRecords();
void updateRecord();
void deleteRecord();
FILE *fpFields;

void main()
{
	int option;
	fpFields = fopen(FILE_FIELDS, "r");
	char fieldMenu[100];
	FILE *fpMenu;
	if(fpFields == NULL)
	{
		printf("File creation failed\n");
		return;
	}

	do
	{
		fpMenu = fopen(FILE_MENU, "r");
		while(fgets(fieldMenu, sizeof(fieldMenu), fpMenu) != NULL)
		{
			fieldMenu[strlen(fieldMenu) - 1] = '\0';
			puts(fieldMenu);
		}

		printf("Choose your option and press enter: ");
		scanf("%d", &option);

		switch(option)
		{
			case 1: createRecord();
			break;
			case 2: readRecords();
			break;
			case 3: updateRecord();
			break;
			case 4: deleteRecord();
			break;
			case 5: ;
			break;
			default: printf("Choose only available options:\n");
			break;
			getch();
		}
	}while(option != 5);
	fclose(fpFields);
	fclose(fpMenu);  
}
void createRecord()
{
	char fieldName[100];
	char fieldValue[20];
	char status[20];
	
	FILE *fpData;
	// FILE *fpFields;
	fpData = fopen(FILE_FINAL, "a");
	// fpFields = fopen(FILE_FIELDS, "r");

	if(fpData == NULL)
	{
		printf("File creation failed\n");
		return;
	}
	strcpy(status, "Activated");
	fwrite(&status, sizeof(status), 1, fpData);

	rewind(fpFields);
	while(fgets(fieldName, sizeof(fieldName), fpFields) != NULL)
	{
		fieldName [strlen(fieldName) - 1] = '\0';
		printf("Enter %s ", fieldName);
		scanf("%s", fieldValue);
		fwrite(fieldValue, sizeof(fieldValue), 1, fpData);
	}

	printf("Record added successfully.\n");
	printf("Press enter to continue.\n");
	
	// fclose(fpFields);
	fclose(fpData);
	getch();
	system("cls");

}
void readRecords()
{
	char fieldValue[20], fieldName[100], status[20], status1[20];
	FILE *fpData;
	// FILE *fpFields;
	strcpy(status, "Activated");
	strcpy(status1, "Deactivated");
	
	// fpFields = fopen(FILE_FIELDS, "r");
	fpData = fopen(FILE_FINAL, "r");
	if(fpData == NULL)
	{
		printf("File creation failed\n");
		return;
	}
	rewind(fpFields);
	while(fread(fieldValue, sizeof(fieldValue), 1, fpData))
	{
		
		if(strcmp(fieldValue, status) == 0 || strcmp(fieldValue, status1) == 0 )
		{
			printf("************\n");
			printf("Status: ");
			printf("%s", fieldValue);
			printf("\n------------\n");
			fseek(fpFields, 0, SEEK_SET);	
		}
		else
		{
			fgets(fieldName, sizeof(fieldValue), fpFields);
			fieldName [strlen(fieldName) - 1] = '\0';
			printf("%s ", fieldName);
			printf("%s\n", fieldValue);
			if(feof(fpFields))
			{
				fseek(fpFields, 0, SEEK_SET);
			}		
		}
	}
	printf("----------\n");
	printf("No more records found.");
	printf("\n----------\n");
	printf("Press enter to continue.\n");
	fclose(fpData);
	// fclose(fpFields);
	getch();
	system("cls");
}
void updateRecord()
{
	char fieldValue[20], fieldName[100];
	FILE *fpData;
	// FILE *fpFields;
	// fpFields = fopen(FILE_FIELDS, "r+");
	fpData = fopen(FILE_FINAL, "r+");
	rewind(fpFields);
	if(fpData == NULL)
	{
		printf("File creation failed\n");
		return;
	}

	char tempFieldValue[sizeof(fieldValue)];
	
	fgets(fieldName, sizeof(fieldName), fpFields);
	fieldName[strlen(fieldName) - 1] = '\0';
	printf("To change your details enter your %s ", fieldName);
	scanf("%s", tempFieldValue);
	
	while(fread(fieldValue, sizeof(fieldValue), 1, fpData))
	{
		if(strcmp(tempFieldValue, fieldValue) == 0)
		{
			// printf("%s\n", fieldValue);
			fseek(fpData, sizeof(fieldValue), SEEK_CUR);
			fread(fieldValue, sizeof(fieldValue), 1, fpData);
			printf("%s should be changed to: ", fieldValue);
			scanf("%s", fieldValue);
			fseek(fpData, sizeof(fieldValue) * -1, SEEK_CUR);
			fwrite(fieldValue, sizeof(fieldValue), 1, fpData);
			printf("Updated successfully to %s.\n", fieldValue);
			printf("Press enter to continue.\n");
			break;
		}
	}
	fclose(fpData);
	// fclose(fpFields);
	getch();
	system("cls");
}



void deleteRecord()
{
	char fieldValue[20], fieldName[100], status [20];
	char decision;
	
	FILE *fpData;
	
	fpData = fopen(FILE_FINAL, "r+");
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

