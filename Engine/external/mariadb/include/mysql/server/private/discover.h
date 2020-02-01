

#ifndef DISCOVER_INCLUDED
#define DISCOVER_INCLUDED

int extension_based_table_discovery(MY_DIR *dirp, const char *ext,
                                    handlerton::discovered_list *tl);

#ifdef MYSQL_SERVER
int readfrm(const char *name, const uchar **data, size_t *length);
int writefrm(const char *path, const char *db, const char *table,
             bool tmp_table, const uchar *frmdata, size_t len);


inline void deletefrm(const char *path)
{
  char frm_name[FN_REFLEN];
  strxmov(frm_name, path, reg_ext, NullS);
  mysql_file_delete(key_file_frm, frm_name, MYF(0));
}

int ext_table_discovery_simple(MY_DIR *dirp,
                               handlerton::discovered_list *result);
#endif

#endif 
