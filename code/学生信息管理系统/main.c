#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<stdbool.h>
#include"stuInfo.h"

//申明为全局变量，方便函数调用
pUSR_ACCOUNT pUsrAccount_List = NULL;
pUSR_INFOR pUsrInfo_List = NULL;

int main() {
	char * config_path = "Configure.txt";
	system_init(config_path, &pUsrAccount_List, &pUsrInfo_List);
	pUSR_ACCOUNT LogAccount = (pUSR_ACCOUNT)calloc(1, sizeof(USR_ACCOUNT));

	int role = 0;
	while (!role) {
		//键盘输入用户名及密码并确认
		LogAccount->usr_name[0] = 0;
		LogAccount->usr_pwd[0] = 0;
		printf("enter usr_name: ");
		scanf("%s", LogAccount->usr_name);
		printf("\nenter usr_pwd:");
		char ch;
		while ((ch = _getch()) != '\r') {
			if (ch != '\b') {
				LogAccount->usr_pwd[strlen(LogAccount->usr_pwd) + 1] = '\0';
				LogAccount->usr_pwd[strlen(LogAccount->usr_pwd)] = ch;
				printf("*");
			}
			else {
				LogAccount->usr_pwd[strlen(LogAccount->usr_pwd) - 1] = '\0';
				printf("\b \b");
			}
		}
		if (role = role_confirm(LogAccount)) {
			LogAccount->usr_role = role;
			break;
		}
		system("cls");
		printf("用户名不存在或密码错误，请重新输入\n");
	}
	show_system_menu(role);
	save_data_to_file(config_path, pUsrAccount_List, pUsrInfo_List);
	printf("\nData has been updated!");
	return 0;
	 
}
void system_init(const char* config_file_name, pUSR_ACCOUNT *pUsr_Account, pUSR_INFOR *pUsr_Infor) {
	FILE *config_fp = fopen(config_file_name, "r");
	char account_path[FILE_PATH_LEN];
	fscanf(config_fp, "%s", account_path);
	char info_path[FILE_PATH_LEN];
	fscanf(config_fp, "%s", info_path);
	fclose(config_fp);
	char name[USR_NAME_LEN];
	char pwd[USR_PWD_LEN];
	int role;
	FILE *account_fp = fopen(account_path, "r");
	while ((fscanf(account_fp, "%s%s%d", name, pwd,&role)) != EOF) {
		pUSR_ACCOUNT pNewA = (pUSR_ACCOUNT)calloc(1, sizeof(USR_ACCOUNT));
		pNewA->usr_role = role;
		strcpy(pNewA->usr_name, name);
		strcpy(pNewA->usr_pwd, pwd);
		pNewA->pNext_Usr_Account = *pUsr_Account;
		*pUsr_Account = pNewA;
	}
	fclose(account_fp);
	int UsrId;
	char gender;
	int CourseId;
	double score;
	FILE *info_fp = fopen(info_path, "r");
	while ((fscanf(info_fp, "%d%s	%c%d%lf", &UsrId,name,&gender,&CourseId,&score)) != EOF) {
		pUSR_INFOR pNewI = (pUSR_INFOR)calloc(1, sizeof(USR_INFOR));
		pNewI->usr_id = UsrId;
		pNewI->gender = gender;
		pNewI->usr_course_id = CourseId;
		pNewI->usr_course_score = score;
		strcpy(pNewI->usr_name, name);
		//查找插入的位置，进行有序插入，根据id进行升序排列

		if (!*pUsr_Infor || UsrId < (*pUsr_Infor)->usr_id ) {
			pNewI->pNext_Usr_Infor = *pUsr_Infor;
			*pUsr_Infor = pNewI;
		}
		else {
			pUSR_INFOR pTempPre = *pUsr_Infor;
			pUSR_INFOR pTemp = pTempPre->pNext_Usr_Infor;
			while (pTemp && pTemp->usr_id < UsrId ) {
				pTempPre = pTempPre->pNext_Usr_Infor;
				pTemp = pTemp->pNext_Usr_Infor;
			}
			pNewI->pNext_Usr_Infor = pTemp;
			pTempPre->pNext_Usr_Infor = pNewI;
		}
	}

}
USR_ROLE role_confirm(pUSR_ACCOUNT pUsr_Account) {
	int role = 0;
	pUSR_ACCOUNT pTemp = pUsrAccount_List;
	while (pTemp) {
		if (strcmp(pUsr_Account->usr_name , pTemp->usr_name) == 0 &&
			strcmp(pUsr_Account->usr_pwd , pTemp->usr_pwd) == 0){
			role = pTemp->usr_role;
			break;
		}
		pTemp = pTemp->pNext_Usr_Account;
	}
	return role;
}
SYSTEM_MENU_TYPE show_system_menu(USR_ROLE role) {
	int authority[10] = { 0 };//用此数组表示各个用户的权限
	if (role == PRIVILEGE_USR) {
		for (int i = 1;i < 10;++i) {
			authority[i] = 1;
		}
	}
	else
	{
		authority[1] = 1;
		authority[9] = 1;
	}
	bool flag = true;
	while (flag) {
		system("cls");
		printf("************************************************************");
		if (role == PRIVILEGE_USR) {
			printf("\n1.search student information");
			printf("\n2.add student information");
			printf("\n3.update student information");
			printf("\n4.delete student information");
			printf("\n5.add user account");
			printf("\n6.update user account");
			printf("\n7.delete user account");
			printf("\n8.search user account");
			printf("\n9.exit\n");

		}
		else {
			printf("\n1.search student information");
			printf("\n9.exit\n");
		}
		printf("************************************************************");
		printf("\nplease input the number to be excuted:");
		int operation;
		scanf("%d", &operation);
		if (authority[operation] == 0) {
			printf("\nYou do not have enough permissions to perform this operation.");
			system("pause");
			continue;
		}
		switch (operation) {
			case SEARCH_STU_INFO:
				show_search_menu(role);
				break;
			case ADD_STU_INFO:
				add_usr_info(&pUsrInfo_List);
				break;
			case UPDATE_STU_INFO:
				update_usr_infor(&pUsrInfo_List);
				break;
			case DEL_STU_INFO:
				delete_usr_infor(&pUsrInfo_List);
				break;
			case ADD_USR_ACCOUNT:
				add_usr_account(&pUsrAccount_List);
				break;
			case UPDATE_USR_ACCOUNT:
				update_usr_account(pUsrAccount_List);
				break;
			case DEL_USR_ACCOUNT:
				delete_usr_account(&pUsrAccount_List);
				break;
			case SEARCH_USR_ACCOUNT:
				search_usr_account(pUsrAccount_List);
				break;
			case EXIT:
				flag = false;
				break;
		}
		/*if (operation == SEARCH_STU_INFO) {
			show_search_menu(role);
		}*/
	}
	
}
SYSTEM_MENU_TYPE show_search_menu(USR_ROLE role) {
	int authority[4] = { 1,1,1,1 };//用此数组表示各个用户的权限
	if (role == NORMAL_USR) {
		authority[0] = 0;
	}
	bool flag = true;
	while (flag) {
		system("cls");
		printf("************************************************************");
		if (role == PRIVILEGE_USR) {
			printf("\n0.search all");
		}
		printf("\n1.search by name");
		printf("\n2.search by id");
		printf("\n3.return\n");
		printf("************************************************************");
		printf("\nplease input the number to be excuted:");
		int operation;
		scanf("%d", &operation);
		if (authority[operation] == 0) {
			printf("\nYou do not have enough permissions to perform this operation.");
			system("pause");
			continue;
		}
		int id;
		char Temp_name[USR_NAME_LEN];
		switch (operation) {
		case SEARCH_ALL:
			search_usr_infor_all(pUsrInfo_List);
			break;
		case SEARCH_BY_NAME:
			printf("\nplease input the name: ");
			scanf("%s", Temp_name);
			search_usr_infor_by_name(pUsrInfo_List, Temp_name);
			break;
		case SEARCH_BY_ID:
			printf("\nplease input the id: ");
			scanf("%d", &id);
			search_usr_infor_by_id(pUsrInfo_List, id);
			break;
		case SEARCH_RETURN:
			flag = false;
			break;
		}
	}
	system("pause");
	/*if (operation == SEARCH_RETURN) {
		show_system_menu(role);
	}*/
}
void search_usr_infor_all(pUSR_INFOR pUsr_Infor_List) {
	pUSR_INFOR pTemp = pUsr_Infor_List;
	while (pTemp) {
		printf("\n%d	%s	%c	%d	%lf\n", pTemp->usr_id, pTemp->usr_name,
		pTemp->gender, pTemp->usr_course_id, pTemp->usr_course_score);
		pTemp = pTemp->pNext_Usr_Infor;
	}
	system("pause");
}
void search_usr_infor_by_id(pUSR_INFOR pUsr_Infor_List, int search_id) {
	pUSR_INFOR pTemp = pUsr_Infor_List;
	int flag = 0;
	while (pTemp) {
		if (pTemp->usr_id == search_id) {
			printf("\n%d	%s	%c	%d	%lf\n", pTemp->usr_id, pTemp->usr_name,
				pTemp->gender, pTemp->usr_course_id, pTemp->usr_course_score);
			flag = 1;
			break;
		}
		pTemp = pTemp->pNext_Usr_Infor;
	}
	if (flag == 0) {
		printf("\nThe id is not exist");
	}
	system("pause");
}
void search_usr_infor_by_name(pUSR_INFOR pUsr_Infor_List, char* search_name) {
	pUSR_INFOR pTemp = pUsr_Infor_List;
	int flag = 0;
	while (pTemp) {
		if (strcmp(search_name,pTemp->usr_name)==0) {
			printf("\n%d	%s	%c	%d	%lf\n", pTemp->usr_id, pTemp->usr_name,
				pTemp->gender, pTemp->usr_course_id, pTemp->usr_course_score);
			flag = 1;
			break;
		}
		pTemp = pTemp->pNext_Usr_Infor;
	}
	if (flag == 0) {
		printf("\nThe name is not exist");
	}
	system("pause");
}
void add_usr_info(pUSR_INFOR *ppUsr_Infor_List) {
	pUSR_INFOR pTemp = *ppUsr_Infor_List;
	int id;
	char name[USR_NAME_LEN];
	char g;
	int  course_id;
	double score;
	printf("please input the information about the new student: \nstu_id: ");
	scanf("%d", &id);
	while (pTemp) {
		if (pTemp->usr_id == id) {
			pTemp = *ppUsr_Infor_List;
			printf("\nThe id has existed before,please input the stu_id again\nstu_id: ");
			scanf("%d", &id);
		}
		else {
			pTemp = pTemp->pNext_Usr_Infor;
		}
	}

	printf("\nstu_name: ");
	scanf("%s", name);
	printf("\ngender: ");
	scanf("\r%c", &g);
	printf("\ncourse_id: ");
	scanf("%d", &course_id);
	printf("\nscore: ");
	scanf("%lf", &score);
	pTemp = *ppUsr_Infor_List;
	pUSR_INFOR pNewStuInfo = (pUSR_INFOR)calloc(1, sizeof(USR_INFOR));
	pNewStuInfo->usr_id = id;
	pNewStuInfo->gender = g;
	pNewStuInfo->usr_course_id = course_id;
	pNewStuInfo->usr_course_score = score;
	strcpy(pNewStuInfo->usr_name, name);

	if (!*ppUsr_Infor_List || id < (*ppUsr_Infor_List)->usr_id) {
		pNewStuInfo->pNext_Usr_Infor = *ppUsr_Infor_List;
		*ppUsr_Infor_List = pNewStuInfo;
	}
	else {
		pUSR_INFOR pTempPre = *ppUsr_Infor_List;
		pUSR_INFOR pTemp = pTempPre->pNext_Usr_Infor;
		while (pTemp && pTemp->usr_id < id) {
			pTempPre = pTempPre->pNext_Usr_Infor;
			pTemp = pTemp->pNext_Usr_Infor;
		}
		pNewStuInfo->pNext_Usr_Infor = pTemp;
		pTempPre->pNext_Usr_Infor = pNewStuInfo;
	}

	printf("\nAdd successfully.");
	system("pause");
}
void delete_usr_infor(pUSR_INFOR *ppUsr_Infor_List)
{
	system("cls");
	printf("************************************************************");
	printf("\nplease input the id need to be deleted: ");
	pUSR_INFOR pTemp = *ppUsr_Infor_List;
	int id;
	scanf("%d", &id);
	if ((*ppUsr_Infor_List)->usr_id == id) {
		*ppUsr_Infor_List = (*ppUsr_Infor_List)->pNext_Usr_Infor;
		free(pTemp);
	}
	else {
		pUSR_INFOR pTempPre = *ppUsr_Infor_List;
		pTemp = pTempPre->pNext_Usr_Infor;
		while (pTemp) {
			if (pTemp->usr_id == id) {
				pTempPre->pNext_Usr_Infor = pTemp->pNext_Usr_Infor;
				free(pTemp);
				break;
			}
			pTemp = pTemp->pNext_Usr_Infor;
			pTempPre = pTempPre->pNext_Usr_Infor;
		}
	}
	printf("\nstudent information after delete: ");
	search_usr_infor_all(*ppUsr_Infor_List);
}
void update_usr_infor(pUSR_INFOR *ppUsr_Infor_List) {
	system("cls");
	printf("************************************************************");
	printf("\nplease input the id need to be updated: ");
	pUSR_INFOR pTemp = *ppUsr_Infor_List;
	int id;
	scanf("%d", &id);
	while (pTemp) {
		if (pTemp->usr_id == id) {
			printf("\nSearch successfully.");
			break;
		}
		pTemp = pTemp->pNext_Usr_Infor;
	}
	if (!pTemp) {
		printf("Search unsuccessfully.\nThe id is not exist.");
	}
	else {
		printf("************************************************************");
		printf("\n1.update the name");
		printf("\n2.update the gender");
		printf("\n3.update the course_id");
		printf("\n4.update the score");
		printf("\nplease input the number to be excuted:");
		int operation;
		scanf("%d", &operation);
		char name[USR_NAME_LEN];
		char g;
		int courseId;
		double score;
		printf("\nplease input the new information: ");
		switch (operation) {
			case INFO_UPDATE_NAME:
				scanf("%s", name);
				strcpy(pTemp->usr_name, name);
				break;
			case INFO_UPDATE_GENDER:
				scanf("%c", &g);
				pTemp->gender = g;
				break;
			case INFO_UPDATE_COURSE_ID:
				scanf("%d", &courseId);
				pTemp->usr_course_id = courseId;
				break;
			case INFO_UPDATE_SCORE:
				scanf("%lf", &score);
				pTemp->usr_course_score = score;
				break;
		}
		printf("\nstudent information after update: ");
		search_usr_infor_all(*ppUsr_Infor_List);
	}
}
void add_usr_account(pUSR_ACCOUNT* ppUsr_Account_List) {
	system("cls");
	printf("************************************************************");
	pUSR_ACCOUNT pTemp = *ppUsr_Account_List;
	printf("\nplease input the information about the new account: \nusr_name: ");
	char name[USR_NAME_LEN];
	scanf("%s", name);
	while (pTemp) {
		if (strcmp(pTemp->usr_name,name) == 0) {
			pTemp = *ppUsr_Account_List;
			printf("\nThe name has existed before,please input the name again\usr_name: ");
			scanf("%s", name);
		}
		else {
			pTemp = pTemp->pNext_Usr_Account;
		}
	}
	char pwd[USR_PWD_LEN];
	pwd[0] = '\0';
	int role;
	printf("\nstu_pwd: ");
	//_getch()输入密码，以保证密码的隐秘性
	char ch;
	while ((ch = _getch()) != '\r') {
		if (ch != '\b') {
			pwd[strlen(pwd) + 1] = '\0';
			pwd[strlen(pwd)] = ch;
			printf("*");
		}
		else {
			pwd[strlen(pwd) - 1] = '\0';
			printf("\b \b");
		}
	}
	printf("\nusr_role(1-->PRIVILEGE,2-->NORMAL): ");
	scanf("%d", &role);
	pUSR_ACCOUNT pNewAccount =(pUSR_ACCOUNT)calloc(1,sizeof(USR_ACCOUNT));
	pNewAccount->usr_role = role;
	strcpy(pNewAccount->usr_name, name);
	strcpy(pNewAccount->usr_pwd, pwd);
	pNewAccount->pNext_Usr_Account = *ppUsr_Account_List;
	*ppUsr_Account_List = pNewAccount;

}
void update_usr_account(pUSR_ACCOUNT pUsr_Account_List) {
	system("cls");
	printf("************************************************************");
	printf("\nplease input the name need to be updated: ");
	pUSR_ACCOUNT pTemp = pUsr_Account_List;
	char name[USR_NAME_LEN];
	scanf("%s", &name);
	while (pTemp) {
		if (strcmp(pTemp->usr_name,name) == 0) {
			printf("\nSearch successfully.");
			break;
		}
		pTemp = pTemp->pNext_Usr_Account;
	}
	if (!pTemp) {
		printf("Search unsuccessfully.\nThe name is not exist.");
	}
	else {
		printf("************************************************************");
		printf("\n1.update the pwd");
		printf("\n2.update the role");
		printf("\n3.return");
		printf("\nplease input the number to be excuted:");
		int operation;
		scanf("%d", &operation);
		char pwd[USR_PWD_LEN];
		int role;
		printf("\nplease input the new information: ");
		switch (operation) {
		case ACCOUNT_UPDATE_PWD:
			scanf("%s", pwd);
			strcpy(pTemp->usr_pwd, pwd);
			break;
		case ACCOUNT_UPDATE_ROLE:
			scanf("%d", &role);
			pTemp->usr_role = role;
			break;
		case ACCOUNT_UPDATE_RETURN:
			break;
		}
		printf("\nAccount information after update: ");
		search_usr_account(pUsr_Account_List);
	}
}
void delete_usr_account(pUSR_ACCOUNT *ppUsr_Account_List) {
	system("cls");
	printf("************************************************************");
	printf("\nplease input the name need to be deleted: ");
	pUSR_ACCOUNT pTemp = *ppUsr_Account_List;
	char name[USR_NAME_LEN];
	scanf("%s", &name);
	if (strcmp(pTemp->usr_name,name) == 0) {
		*ppUsr_Account_List = (*ppUsr_Account_List)->pNext_Usr_Account;
		free(pTemp);
	}
	else {
		pUSR_ACCOUNT pTempPre = *ppUsr_Account_List;
		pTemp = pTempPre->pNext_Usr_Account;
		while (pTemp) {
			if (strcmp(pTemp->usr_name, name) == 0) {
				pTempPre->pNext_Usr_Account = pTemp->pNext_Usr_Account;
				free(pTemp);
				break;
			}
			pTemp = pTemp->pNext_Usr_Account;
			pTempPre = pTempPre->pNext_Usr_Account;
		}
	}
	printf("\nall accounts after delete: ");
	search_usr_account(*ppUsr_Account_List);
}
void search_usr_account(pUSR_ACCOUNT pUsr_Account_List) {
	system("cls");
	printf("************************************************************");
	printf("\n there are all accounts:\n");
	pUSR_ACCOUNT pTemp = pUsr_Account_List;
	while (pTemp) {
		printf("%s	%s	%d\n", pTemp->usr_name, pTemp->usr_pwd, pTemp->usr_role);
		pTemp = pTemp->pNext_Usr_Account;
	}
	system("pause");
}
void save_data_to_file(char * config_file_name, pUSR_ACCOUNT pUsr_Account_List, pUSR_INFOR pUsr_Infor_List) {
	FILE *config_fp = fopen(config_file_name, "r");
	char account_path[FILE_PATH_LEN];
	fscanf(config_fp, "%s", account_path);
	char info_path[FILE_PATH_LEN];
	fscanf(config_fp, "%s", info_path);
	fclose(config_fp);
	FILE *account_fp = fopen(account_path, "w");
	pUSR_ACCOUNT pTempAcc = pUsr_Account_List;
	while (pTempAcc) {
		fprintf(account_fp, "%s	%s	%d\n", pTempAcc->usr_name, pTempAcc->usr_pwd, pTempAcc->usr_role);
		pTempAcc = pTempAcc->pNext_Usr_Account;
	}
	fclose(account_fp);
	FILE *info_fp = fopen(info_path, "w");
	pUSR_INFOR pTempInfo = pUsr_Infor_List;
	while (pTempInfo) {
		fprintf(info_fp, "%d	%s	%c	%d	%lf\n", pTempInfo->usr_id, pTempInfo->usr_name, pTempInfo->gender,
			pTempInfo->usr_course_id, pTempInfo->usr_course_score);
		pTempInfo = pTempInfo->pNext_Usr_Infor;
	}
	fclose(info_fp);
}