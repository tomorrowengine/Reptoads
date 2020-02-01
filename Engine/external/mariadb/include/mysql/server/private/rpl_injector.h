

#ifndef INJECTOR_H
#define INJECTOR_H


#include <my_bitmap.h>

#include "rpl_constants.h"
#include "table.h"                              


class handler;
class MYSQL_BIN_LOG;
struct TABLE;



class injector 
{
public:

  
  static injector *instance();

  
  static void free_instance();

    
    class transaction {
      friend class injector;
    public:
      
      typedef uchar* record_type;
      typedef uint32 server_id_type;

      
      class table 
      {
      public:
        table(TABLE *table, bool is_transactional_arg)
            : m_table(table), m_is_transactional(is_transactional_arg)
        { 
        }

        char const *db_name() const { return m_table->s->db.str; }
        char const *table_name() const { return m_table->s->table_name.str; }
        TABLE *get_table() const { return m_table; }
        bool is_transactional() const { return m_is_transactional; }

      private:
        TABLE *m_table;
        bool m_is_transactional;
      };

      
      class binlog_pos {
        friend class transaction;
      public:
        char const *file_name() const { return m_file_name; }
        my_off_t file_pos() const { return m_file_pos; }

      private:
        char const *m_file_name;
        my_off_t m_file_pos;
      };

      transaction() : m_thd(NULL) { }
      transaction(transaction const&);
      ~transaction();

      
      void clear() { m_thd= NULL; }

      
      bool good() const { return m_thd != NULL; }

      
      transaction& operator=(transaction t) {
        swap(t);
        return *this;
      }
      
      
      int use_table(server_id_type sid, table tbl);

      
      int commit();

      
      binlog_pos start_pos() const;

    private:
      
      transaction(MYSQL_BIN_LOG *, THD *);

      void swap(transaction& o) {
        
        {
          binlog_pos const tmp= m_start_pos;
          m_start_pos= o.m_start_pos;
          o.m_start_pos= tmp;
        }

        
        {
          THD* const tmp= m_thd;
          m_thd= o.m_thd;
          o.m_thd= tmp;
        }
        {
          enum_state const tmp= m_state;
          m_state= o.m_state;
          o.m_state= tmp;
        }
      }

      enum enum_state
      {
        START_STATE,                            
        TABLE_STATE,      
        ROW_STATE,          
        STATE_COUNT               
      } m_state;

      
      int check_state(enum_state const target_state)
      {
#ifndef DBUG_OFF
        static char const *state_name[] = {
          "START_STATE", "TABLE_STATE", "ROW_STATE", "STATE_COUNT"
        };

        DBUG_ASSERT(target_state <= STATE_COUNT);
        DBUG_PRINT("info", ("In state %s", state_name[m_state]));
#endif

        if (m_state <= target_state && target_state <= m_state + 1 &&
            m_state < STATE_COUNT)
          m_state= target_state;
        else
          m_state= STATE_COUNT;
        return m_state == STATE_COUNT ? 1 : 0;
      }


      binlog_pos m_start_pos;
      THD *m_thd;
    };

    
    transaction new_trans(THD *);
    void        new_trans(THD *, transaction *);

    int record_incident(THD*, Incident incident);
    int record_incident(THD*, Incident incident, const LEX_CSTRING *message);

private:
    explicit injector();
    ~injector() { }             
    injector(injector const&);   
};

#endif 
