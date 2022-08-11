int login(char *email, char *password);
int create_new_user(char *email, char *password, char *username, char* range);

int search_user(char *email);
int get_data_user(int row, char *ID, char *email, char *password, char *username, char* range);

int change_password(char *ID, char *newPassword);
int change_username(char *ID, char *newUsername);
int delete_user(char* ID);

int set_admin(char* ID);
int set_user(char* ID);