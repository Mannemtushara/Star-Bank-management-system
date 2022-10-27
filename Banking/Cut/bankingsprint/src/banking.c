#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include<clear.h>
#define MAX 21

typedef struct User
{
    int balance, type;
    char aadhar[13], name[MAX], password[MAX];
    struct User *next;
} User;

int user_logged_in = 0;
int admin_logged_in = 0;
char current_user[MAX];
char admin_id[MAX], admin_pass[MAX];
User *head = NULL, *tail = NULL;

int validatename(char *name)
{
	for(int i=0;i<strlen(name);i++)
	{
		if(isalpha(name[i]))
			continue;
		else
			return 0;
	}
	return 1;

}
int validatepass(char *password)
{
	for(int i=0;i<strlen(password);i++)
	{
		if(isalpha(password[i]))
			continue;
		else
			return 0;
	}
	return 1;
}
/*void clear( )
{
	system("clear");
}*/

void load_all()
{
    char line[256];
    FILE *file = NULL;
    file = fopen("../data/admin.txt", "r");
    fgets(line, sizeof(line), file);
    sscanf(line, "%s %s", admin_id, admin_pass);
    fclose(file);
    file = fopen("../data/users.txt", "r");
    head = tail = NULL;
    while (fgets(line, sizeof(line), file))
    {
        User *user = (User *)calloc(1, sizeof(User));
        user->next = NULL;
        sscanf(line, "%s %d %s %s %d", user->aadhar, &user->balance, user->name, user->password, &user->type);
        if (!tail)
            head = tail = user;
        else
        {
            tail->next = user;
            tail = tail->next;
        }
    }
    fclose(file);
}

void update_file()
{
    User *user = head;
    FILE *file = fopen("../data/users.txt", "w");
    while (user)
    {
        fprintf(file, "%s %d %s %s %d\n", user->aadhar, user->balance, user->name, user->password, user->type);
        user = user->next;
    }
}
int is_admin(char *id, char *pass)
{
	return admin_logged_in=!strcmp(admin_id,id) && !strcmp(admin_pass,pass);
    
}
User *find_user(char *id)
{
    User *node = head;
    while (node)
    {
        if (!strcmp(node->aadhar, id))
            return node;
        node = node->next;
    }
    return NULL;
}
int transfer(char *id1, char *id2, int amount)
{
    User *user1 = find_user(id1);
    User *user2 = find_user(id2);
    if (!user1 || !user2)
	   
        return 0;
    int min_bal = user1->type ? 5000 : 10000;
    if (user1->balance - amount >= min_bal)
    {
        user1->balance -= amount;
        user2->balance += amount;
	

        update_file();
	
        return 1;
    }
    return 0;
}

int debit(char *id, int amount)
{
    User *user = find_user(id);
    int min_bal = user->type ? 5000 : 10000;
    if (!user || user->balance - amount < min_bal)
	    printf("\nMinimum balance reaches money cannot withdraw\n");
	    
   
        return 0;
    user->balance -= amount;
    update_file();
    printf("\n******Withdraw successfull******\n");
    return 1;
}

int credit(char *id, int amount)
{
    User *user = find_user(id);
    int min_bal=user->type ? 5000:10000;
    if (!user)
        return 0;
    user->balance += amount;
    printf("\n******Deposited succesfully******\n");
    update_file();
    return 1;
}

			

User *create_user()
{
    User *user = (User *)calloc(1, sizeof(User));
    user->next = NULL;
    printf("Enter your details\n");
    while (1)
    {
        printf("Aadhar: ");
	scanf("%s",&user->aadhar);
        //gets(user->aadhar);
        if (strlen(user->aadhar) == 12)
            break;
        else
            printf("Aadhar number is invalid\n");
    }
    while (1)
    {
        printf("Name: ");
	scanf("%s",&user->name);
        //gets(user->name);
        if (validatename(user->name))
            break;
        else
            printf("Invalid name ");
    }
    while (1)
    {
        printf("Password: ");
	scanf("%s",&user->password);
        //gets(user->password);
        if (validatepass(user->password))
            break;
        else
            printf("Enter valid password");
    }
    while (1)
    {
        printf("Type: ");
        scanf("%d", &user->type);
        if (-1 < user->type && user->type < 2)
            break;
        else
            printf("Enter type value 1 or 0\n");
    }
    if (user->type == 0)
        user->balance = 10000;
    else
        user->balance = 5000;
    return user;
}


User *user_register()
{
    FILE *file = NULL;
    User *user = create_user();
    if (find_user(user->aadhar))
    {
        printf("You are already a user\n");
        free(user);
        return NULL;
    }
    if (!head)
        head = tail = user;
    else
    {
        tail->next = user;
        tail = tail->next;
    }
    file = fopen("../data/users.txt", "a");
    fprintf(file, "%s %d %s %s %d\n", user->aadhar, user->balance, user->name, user->password, user->type);
   
    fclose(file);
    return user;
}

int get_balance(char *id)
{
    User *node = head;
    while (node)
    {
        if (!strcmp(node->aadhar, id))
            return node->balance;
        node = node->next;
    }
    return -1;
}
int change_password(char *id, char *pass)
{
    User *user = find_user(id);
    if (!user)
        return 0;
    strcpy(user->password, pass);
    printf("\n******Password chaged successfully******\n");
    update_file();
    return 1;
}
int is_user(char *id, char *pass)
{
    User *user = find_user(id);
    if (!user || strcmp(pass, user->password))
        return 0;
    strcpy(current_user, id);
    return 1;
}

void print_user(char *id)
{
    User *user = find_user(id);
    printf("\n");
    printf("+------------+---------------------+-------------+-----+\n");
    printf("|          ID|                 name|      balance| type|\n");
    printf("+------------+---------------------+-------------+-----+\n");
    printf("|%s| %20s| %12d| %4d|\n", user->aadhar, user->name, user->balance, user->type);
    printf("+------------+---------------------+-------------+-----+\n");
    printf("\n\n");
}

void print_all()
{
	
    User *user = head;
    if (!user)
        load_all();
    
    printf("\n");
    printf("+------------+---------------------+-------------+-----+\n");
    printf("|          ID|                 name|      balance| type|\n");
    printf("+------------+---------------------+-------------+-----+\n");
    while (user)
    {
        printf("|%s| %20s| %12d| %4d|\n", user->aadhar, user->name, user->balance, user->type);
        printf("+------------+---------------------+-------------+-----+\n");
        user = user->next;
    }
    printf("\n\n");
}
void options( )
{
	
	char id[MAX],pass[MAX],to_id[MAX];
	int choice,money;
	while(1)
	{
		printf("\n1.Admin\n2.User\n3.Exit\n");
		printf("Enter your choice\n");
		scanf("%d",&choice);
		switch(choice)
		{
		

			case 1:printf("\nEnter admin id: \n");
			       scanf("%s",id);
			       printf("Enter admin password: \n");
			       scanf("%s", pass);
				if (is_admin(id, pass))
					{
						printf("\n******Admin login successfully******\n");
						
						int c;
						while(1)
						{
							printf("\n1.Withdraw\n2.Deposit\n3.Transfer\n4.Main menu\n5.Exit\n");
							printf("Enter your choice\n");
							scanf("%d",&c);
							clear( );
							switch(c)
							{
							
								case 1:printf("Enter account number: \n");
								       scanf("%s",id);
								       printf("Enter amount: \n");
									scanf("%d",&money);
								       debit(id, money);
								       
									break;
								case 2:printf("Enter account number: \n");
								       scanf("%s",id);
								       printf("Enter amount: \n");
									scanf("%d",&money);
              								credit(id, money);
									
									break;
								case 3:printf("Enter your account number: \n");
								       scanf("%s",id);
								       printf("Enter recepients account number: \n");
								       scanf("%s",to_id);
								       printf("Enter amount: \n");

									scanf("%d",&money);
                							transfer(id, to_id, money);
									printf("\n******Transaction successfull******\n");
									
                							break;
								case 4:options( );
								       break;
								case 5:exit(0);
								default:printf("Invalid choice\n");
							}
						}
					}
				else
				{
					printf("\nInvalid");
				}
				break;

			case 2:clear( );
				int u;
			       while(1)
			       {
				printf("\n1.Create account\n2.Login\n3.Exit\n");
				printf("Enter your choice\n");
				scanf("%d",&u);
				clear( );
				switch(u)
				{
					case 1:user_register( );
					       printf("\n******User created successfully******\n");
					       break;
					case 2:printf("Enter account number: \n");
					       scanf("%s",id);
					       printf("Enter password: \n");
					       scanf("%s",pass);
					       if(is_user(id,pass))
					       {
						       printf("\n******User login successfully******\n");
						       
						       int l;
						       while(1)
						       {
							       printf("\n1.Check balance\n2.Change password\n3.Main menu\n4..Exit\n");
							       printf("Enter your choice \n");
							       scanf("%d",&l);
							       clear( );
							       switch(l)
							       {
								       case 1:printf("Balance: %d\n",get_balance(current_user));
									
									      break;
								       case 2:printf("Enter new password: ");
									      scanf("%s",&pass);
									      change_password(current_user,pass);
									      break;
								       case 3:options( );
									      break;
								       case 4:exit(0);
								       default:printf("Invalid\n");
							       }
						       }
					       }
					       else
					       {
						       printf("Invalid login credentials\n");
					       }
					       break;
					case 3:exit(0);
				}
			       }
			       break;
			case 3:exit(0);
		}
	}
}
int main()
{
	load_all( );
	printf("\t\t\t\t\tWelcome to star banking management system\n");
	options( );
	return EXIT_SUCCESS;
}

