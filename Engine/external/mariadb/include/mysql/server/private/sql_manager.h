

#ifndef SQL_MANAGER_INCLUDED
#define SQL_MANAGER_INCLUDED

void start_handle_manager();
void stop_handle_manager();
bool mysql_manager_submit(void (*action)());

#endif 
