

#ifndef UNIQUE_INCLUDED
#define UNIQUE_INCLUDED

#include "filesort.h"



class Unique :public Sql_alloc
{
  DYNAMIC_ARRAY file_ptrs;
  ulong max_elements;
  size_t max_in_memory_size;
  IO_CACHE file;
  TREE tree;
  ulong filtered_out_elems;
  uint size;
  uint full_size;
  uint min_dupl_count;   
  bool with_counters;

  bool merge(TABLE *table, uchar *buff, bool without_last_merge);
  bool flush();

public:
  ulong elements;
  SORT_INFO sort;
  Unique(qsort_cmp2 comp_func, void *comp_func_fixed_arg,
	 uint size_arg, size_t max_in_memory_size_arg,
         uint min_dupl_count_arg= 0);
  ~Unique();
  ulong elements_in_tree() { return tree.elements_in_tree; }
  inline bool unique_add(void *ptr)
  {
    DBUG_ENTER("unique_add");
    DBUG_PRINT("info", ("tree %u - %lu", tree.elements_in_tree, max_elements));
    if (!(tree.flag & TREE_ONLY_DUPS) && 
        tree.elements_in_tree >= max_elements && flush())
      DBUG_RETURN(1);
    DBUG_RETURN(!tree_insert(&tree, ptr, 0, tree.custom_arg));
  }

  bool is_in_memory() { return (my_b_tell(&file) == 0); }
  void close_for_expansion() { tree.flag= TREE_ONLY_DUPS; }

  bool get(TABLE *table);
  
  
  inline static double get_search_cost(ulonglong tree_elems, uint compare_factor)
  {
    return log((double) tree_elems) / (compare_factor * M_LN2);
  }  

  static double get_use_cost(uint *buffer, size_t nkeys, uint key_size,
                             size_t max_in_memory_size, uint compare_factor,
                             bool intersect_fl, bool *in_memory);
  inline static int get_cost_calc_buff_size(size_t nkeys, uint key_size,
                                            size_t max_in_memory_size)
  {
    size_t max_elems_in_tree=
      max_in_memory_size / ALIGN_SIZE(sizeof(TREE_ELEMENT)+key_size);
    return (int) (sizeof(uint)*(1 + nkeys/max_elems_in_tree));
  }

  void reset();
  bool walk(TABLE *table, tree_walk_action action, void *walk_action_arg);

  uint get_size() const { return size; }
  size_t get_max_in_memory_size() const { return max_in_memory_size; }

  friend int unique_write_to_file(uchar* key, element_count count, Unique *unique);
  friend int unique_write_to_ptrs(uchar* key, element_count count, Unique *unique);

  friend int unique_write_to_file_with_count(uchar* key, element_count count,
                                             Unique *unique);
  friend int unique_intersect_write_to_ptrs(uchar* key, element_count count, 
				            Unique *unique);
};

#endif 
