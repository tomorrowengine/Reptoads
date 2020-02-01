#ifndef SESSION_TRACKER_INCLUDED
#define SESSION_TRACKER_INCLUDED



#include "m_string.h"
#include "thr_lock.h"

#ifndef EMBEDDED_LIBRARY

class THD;
class set_var;
class String;


enum enum_session_tracker
{
  SESSION_SYSVARS_TRACKER,                       
  CURRENT_SCHEMA_TRACKER,                        
  SESSION_STATE_CHANGE_TRACKER,
  SESSION_GTIDS_TRACKER,                         
  TRANSACTION_INFO_TRACKER,                      
  SESSION_TRACKER_END                            
};



class State_tracker
{
protected:
  
  bool m_enabled;

  
  bool m_changed;

public:
  
  State_tracker() : m_enabled(false), m_changed(false)
  {}

  
  virtual ~State_tracker()
  {}

  
  bool is_enabled() const
  { return m_enabled; }

  bool is_changed() const
  { return m_changed; }

  
  virtual bool enable(THD *thd)= 0;

  
  virtual bool update(THD *thd, set_var *var)= 0;

  
  virtual bool store(THD *thd, String *buf)= 0;

  
  virtual void mark_as_changed(THD *thd, LEX_CSTRING *name);
};

bool sysvartrack_validate_value(THD *thd, const char *str, size_t len);
bool sysvartrack_reprint_value(THD *thd, char *str, size_t len);
bool sysvartrack_update(THD *thd, set_var *var);
size_t sysvartrack_value_len(THD *thd);
bool sysvartrack_value_construct(THD *thd, char *val, size_t len);




class Session_tracker
{
private:
  State_tracker *m_trackers[SESSION_TRACKER_END];

  
  Session_tracker(Session_tracker const &other)
  {
    DBUG_ASSERT(FALSE);
  }
  Session_tracker& operator= (Session_tracker const &rhs)
  {
    DBUG_ASSERT(FALSE);
    return *this;
  }

public:

  Session_tracker();
  ~Session_tracker()
  {
    deinit();
  }

  
  void deinit()
  {
    for (int i= 0; i < SESSION_TRACKER_END; i++)
    {
      if (m_trackers[i])
        delete m_trackers[i];
      m_trackers[i]= NULL;
    }
  }

  void enable(THD *thd);
  static bool server_boot_verify(CHARSET_INFO *char_set);

  
  inline State_tracker *get_tracker(enum_session_tracker tracker) const
  {
    return m_trackers[tracker];
  }

  inline void mark_as_changed(THD *thd, enum enum_session_tracker tracker,
                              LEX_CSTRING *data)
  {
    if (m_trackers[tracker]->is_enabled())
      m_trackers[tracker]->mark_as_changed(thd, data);
  }


  void store(THD *thd, String *main_buf);
};





enum enum_tx_state {
  TX_EMPTY        =   0,  
  TX_EXPLICIT     =   1,  
  TX_IMPLICIT     =   2,  
  TX_READ_TRX     =   4,  
  TX_READ_UNSAFE  =   8,  
  TX_WRITE_TRX    =  16,  
  TX_WRITE_UNSAFE =  32,  
  TX_STMT_UNSAFE  =  64,  
  TX_RESULT_SET   = 128,  
  TX_WITH_SNAPSHOT= 256,  
  TX_LOCKED_TABLES= 512   
};



enum enum_tx_read_flags {
  TX_READ_INHERIT =   0,  
  TX_READ_ONLY    =   1,  
  TX_READ_WRITE   =   2,  
};



enum enum_tx_isol_level {
  TX_ISOL_INHERIT     = 0, 
  TX_ISOL_UNCOMMITTED = 1,
  TX_ISOL_COMMITTED   = 2,
  TX_ISOL_REPEATABLE  = 3,
  TX_ISOL_SERIALIZABLE= 4
};



enum enum_session_track_transaction_info {
  TX_TRACK_NONE      = 0,  
  TX_TRACK_STATE     = 1,  
  TX_TRACK_CHISTICS  = 2   
};




class Transaction_state_tracker : public State_tracker
{
private:
  
  enum_tx_state calc_trx_state(THD *thd, thr_lock_type l, bool has_trx);
public:
  
  Transaction_state_tracker();
  bool enable(THD *thd)
  { return update(thd, NULL); }
  bool update(THD *thd, set_var *var);
  bool store(THD *thd, String *buf);

  
  void set_read_flags(THD *thd, enum enum_tx_read_flags flags);
  void set_isol_level(THD *thd, enum enum_tx_isol_level level);

  
  void clear_trx_state(THD *thd, uint clear);
  void add_trx_state(THD *thd, uint add);
  void inline add_trx_state(THD *thd, thr_lock_type l, bool has_trx)
  {
    add_trx_state(thd, calc_trx_state(thd, l, has_trx));
  }
  void add_trx_state_from_thd(THD *thd);
  void end_trx(THD *thd);


private:
  enum enum_tx_changed {
    TX_CHG_NONE     = 0,  
    TX_CHG_STATE    = 1,  
    TX_CHG_CHISTICS = 2   
  };

  
  uint                     tx_changed;

  
  uint                     tx_curr_state,  tx_reported_state;

  
  enum enum_tx_read_flags  tx_read_flags;

  
  enum enum_tx_isol_level  tx_isol_level;

  void reset();

  inline void update_change_flags(THD *thd)
  {
    tx_changed &= uint(~TX_CHG_STATE);
    tx_changed |= (tx_curr_state != tx_reported_state) ? TX_CHG_STATE : 0;
    if (tx_changed != TX_CHG_NONE)
      mark_as_changed(thd, NULL);
  }
};

#define TRANSACT_TRACKER(X) \
 do { if (thd->variables.session_track_transaction_info > TX_TRACK_NONE) \
   {((Transaction_state_tracker *) \
         thd->session_tracker.get_tracker(TRANSACTION_INFO_TRACKER)) \
          ->X; } } while(0)
#define SESSION_TRACKER_CHANGED(A,B,C) \
  thd->session_tracker.mark_as_changed(A,B,C)
#else

#define TRANSACT_TRACKER(X) do{}while(0)
#define SESSION_TRACKER_CHANGED(A,B,C) do{}while(0)

#endif 

#endif 
