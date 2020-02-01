

#ifndef TRANSACTION_H
#define TRANSACTION_H

#ifdef USE_PRAGMA_INTERFACE
#pragma interface                      
#endif

#include <m_string.h>

class THD;

bool trans_begin(THD *thd, uint flags= 0);
bool trans_commit(THD *thd);
bool trans_commit_implicit(THD *thd);
bool trans_rollback(THD *thd);
bool trans_rollback_implicit(THD *thd);

bool trans_commit_stmt(THD *thd);
bool trans_rollback_stmt(THD *thd);

bool trans_savepoint(THD *thd, LEX_CSTRING name);
bool trans_rollback_to_savepoint(THD *thd, LEX_CSTRING name);
bool trans_release_savepoint(THD *thd, LEX_CSTRING name);

bool trans_xa_start(THD *thd);
bool trans_xa_end(THD *thd);
bool trans_xa_prepare(THD *thd);
bool trans_xa_commit(THD *thd);
bool trans_xa_rollback(THD *thd);

void trans_reset_one_shot_chistics(THD *thd);

#endif 
