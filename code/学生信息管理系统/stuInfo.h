#define USR_NAME_LEN 20
#define USR_PWD_LEN 20
#define USR_ROLE int //用户类别，1为特权用户，2为普通用户
#define PRIVILEGE_USR 1
#define NORMAL_USR 2
#define FILE_PATH_LEN 30
#define	USR_ACCOUNT_NUM int //账户数目
#define SYSTEM_MENU_TYPE void

#define SEARCH_STU_INFO 1
#define ADD_STU_INFO 2
#define UPDATE_STU_INFO 3
#define DEL_STU_INFO 4
#define ADD_USR_ACCOUNT 5
#define UPDATE_USR_ACCOUNT 6
#define DEL_USR_ACCOUNT 7
#define SEARCH_USR_ACCOUNT 8
#define EXIT 9


#define SEARCH_ALL 0
#define SEARCH_BY_NAME 1
#define SEARCH_BY_ID 2
#define SEARCH_RETURN 3

#define INFO_UPDATE_NAME 1
#define INFO_UPDATE_GENDER 2
#define INFO_UPDATE_COURSE_ID 3
#define INFO_UPDATE_SCORE 4

#define ACCOUNT_UPDATE_PWD 1
#define ACCOUNT_UPDATE_ROLE 2
#define ACCOUNT_UPDATE_RETURN 3
typedef struct usr_account_tag
{	//系统用户信息，分为普通用户和特权用户
	char usr_name[USR_NAME_LEN];
	char usr_pwd[USR_PWD_LEN];
	USR_ROLE usr_role;//权限信息
	struct usr_account_tag *pNext_Usr_Account;
}USR_ACCOUNT, *pUSR_ACCOUNT;

typedef struct usr_infor_tag
{
	int usr_id;
	char usr_name[USR_NAME_LEN];
	char gender;
	int usr_course_id;
	double usr_course_score;
	struct usr_infor_tag *pNext_Usr_Infor;
}USR_INFOR, *pUSR_INFOR;

void system_init(const char* config_file_name, pUSR_ACCOUNT *pUsr_Acount, pUSR_INFOR *pUsr_Infor);
USR_ROLE role_confirm(pUSR_ACCOUNT pUsr_Account);
SYSTEM_MENU_TYPE show_system_menu(USR_ROLE role);
SYSTEM_MENU_TYPE show_search_menu(USR_ROLE role);
void search_usr_infor_all(pUSR_INFOR pUsr_Infor_List);
void search_usr_infor_by_id(pUSR_INFOR pUsr_Infor_List, int search_id);
void search_usr_infor_by_name(pUSR_INFOR pUsr_Infor_List, char* search_name);
void add_usr_info(pUSR_INFOR *ppUsr_Infor_List);
void delete_usr_infor(pUSR_INFOR *ppUsr_Infor_List);
void update_usr_infor(pUSR_INFOR *ppUsr_Infor_List);
void add_usr_account(pUSR_ACCOUNT* ppUsr_Account_List);
void update_usr_account(pUSR_ACCOUNT pUsr_Account_List);
void delete_usr_account(pUSR_ACCOUNT *ppUsr_Account_List);
void search_usr_account(pUSR_ACCOUNT pUsr_Account_List);
void save_data_to_file(char * config_file_name, pUSR_ACCOUNT pUsr_Account_List, pUSR_INFOR pUsr_Infor_List);
//pUSR_INFOR add_usr_infor(pUSR_INFOR *ppUsr_Infor_List, USR_INFOR_NUM * usr_infor_num);
//pUSR_INFOR update_usr_infor(pUSR_INFOR pUsr_Infor_List, SEARCH_ID search_id);
//pUSR_INFOR delete_usr_infor(pUSR_INFOR *ppUsr_Infor_List, USR_INFOR_NUM	* usr_infor_num, SEARCH_ID search_id);
//void search_usr_account(pUSR_ACCOUNT pUsr_Account_List, char* search_name);
//pUSR_ACCOUNT add_usr_account(pUSR_ACCOUNT* ppUsr_Account_List, USR_ACCOUNT_NUM* usr_account_num);
//pUSR_ACCOUNT update_usr_account(pUSR_ACCOUNT pUsr_Account_List, char* search_name);
//pUSR_ACCOUNT delete_usr_account(pUSR_ACCOUNT *ppUsr_Account_List, USR_ACCOUNT_NUM* usr_account_num, char *search_name);
//void save_data_to_file(char * config_file_name, pUSR_ACCOUNT pUsr_Account_List, pUSR_INFOR pUsr_Infor_List);
