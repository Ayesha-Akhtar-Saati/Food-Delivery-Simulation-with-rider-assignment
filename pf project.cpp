#include<stdio.h>
#include<conio.h>
#include<string.h>
int main()
{
	//admin credentials
	char admin_password[]="12345678";
	char admin_email[]="admin@gmail.com";
	
	//user variables
	char entered_password[50],entered_email[50],user_email[50],user_password[50];
	int user_created=0;//flag to check if user has signed up
	printf("--------F O O D  D E L I V E R Y  S Y S T E M--------");
	int role;
	
	//ask user to enter role
	printf("\nAre you a : \n");
	printf("1.Admin.\n");
	printf("2.User.\n");
	printf("Enter your choice : ");
	scanf("%d",&role);
	
	if(role == 1)
	{
		//admin login
		printf("\n-----Admin login-----\n");
		//take login input
	    printf("Enter your Email :  ");
	    scanf("%s",&entered_email);
	    printf("Enter your Password :  ");
	    scanf("%s",&entered_password);
	
	   if(strcmp(admin_password,entered_password) == 0 && strcmp(admin_email,entered_email) == 0)
	   {
	   	    printf("\nLogin Successful.\n");
	   	    printf("Welcome, Admin!.\n");
	   }
	   else
	   {
	   	    printf("Invalid Admin Credentials.\n");
	   }
    }
    else if(role == 2)
    {
    	int option;
    	
    	printf("\n-----User Panel-----\n");
    	printf("1.Sign In.\n");
    	printf("2.Sign Up.\n");
    	printf("Enter your option : ");
    	scanf("%d",&option);
    	if(option == 2)
    	{
    		//sign-up
    		printf("\n-----Sign Up-----\n");
    		printf("Create your Email : ");
        	scanf("%s",user_email);
    	    printf("Create your Password : ");
        	scanf("%s",user_password);
        	user_created=1;
        	printf("Account Created Successfully.\n");
		}
		if(option == 1 || user_created == 1)
		{
			//sign in
			printf("\n-----Sign In-----\n");
			printf("Enter your Email : ");
			scanf("%s",entered_email);
			printf("Enter your Password : ");
			scanf("%s",&entered_password);
			
			if(strcmp(entered_email,user_email) == 0 && strcmp(entered_password,user_password) == 0)
			{
				printf("Login Successful!.\nWelcome, user!\n");
			}
			else 
			    printf("Invalid login credentials.\n");
		}
	}
	
	
}
		
		
		
		
		
		
		
		
		
	