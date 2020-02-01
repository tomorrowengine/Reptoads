

#ifndef _tree_h
#define _tree_h
#ifdef	__cplusplus
extern "C" {
#endif

#include "my_base.h"		
#include "my_alloc.h"           


#define MAX_TREE_HEIGHT	64

#define ELEMENT_KEY(tree,element)\
(tree->offset_to_key ? (void*)((uchar*) element+tree->offset_to_key) :\
			*((void**) (element+1)))

#define tree_set_pointer(element,ptr) *((uchar **) (element+1))=((uchar*) (ptr))



#define TREE_ELEMENT_UNIQUE ((TREE_ELEMENT *) 1)
#define TREE_NO_DUPS 1
#define TREE_ONLY_DUPS 2

typedef enum { left_root_right, right_root_left } TREE_WALK;
typedef uint32 element_count;
typedef int (*tree_walk_action)(void *,element_count,void *);

typedef enum { free_init, free_free, free_end } TREE_FREE;
typedef int (*tree_element_free)(void*, TREE_FREE, void *);

typedef struct st_tree_element {
  struct st_tree_element *left,*right;
  uint32 count:31,
	 colour:1;			
} TREE_ELEMENT;

#define ELEMENT_CHILD(element, offs) (*(TREE_ELEMENT**)((char*)element + offs))

typedef struct st_tree {
  TREE_ELEMENT *root,null_element;
  TREE_ELEMENT **parents[MAX_TREE_HEIGHT];
  uint offset_to_key,elements_in_tree,size_of_element;
  size_t memory_limit, allocated;
  qsort_cmp2 compare;
  void *custom_arg;
  MEM_ROOT mem_root;
  my_bool with_delete;
  tree_element_free free;
  myf my_flags;
  uint flag;
} TREE;

	
void init_tree(TREE *tree, size_t default_alloc_size, size_t memory_limit,
               int size, qsort_cmp2 compare,
	       tree_element_free free_element, void *custom_arg,
               myf my_flags);
int delete_tree(TREE*, my_bool abort);
int reset_tree(TREE*);

  
#define is_tree_inited(tree) ((tree)->root != 0)

	
TREE_ELEMENT *tree_insert(TREE *tree,void *key, uint key_size, 
                          void *custom_arg);
void *tree_search(TREE *tree, void *key, void *custom_arg);
int tree_walk(TREE *tree,tree_walk_action action,
	      void *argument, TREE_WALK visit);
int tree_delete(TREE *tree, void *key, uint key_size, void *custom_arg);
void *tree_search_key(TREE *tree, const void *key, 
                      TREE_ELEMENT **parents, TREE_ELEMENT ***last_pos,
                      enum ha_rkey_function flag, void *custom_arg);
void *tree_search_edge(TREE *tree, TREE_ELEMENT **parents, 
                        TREE_ELEMENT ***last_pos, int child_offs);
void *tree_search_next(TREE *tree, TREE_ELEMENT ***last_pos, int l_offs, 
                       int r_offs);
ha_rows tree_record_pos(TREE *tree, const void *key, 
                     enum ha_rkey_function search_flag, void *custom_arg);
#define reset_free_element(tree) (tree)->free= 0

#define TREE_ELEMENT_EXTRA_SIZE (sizeof(TREE_ELEMENT) + sizeof(void*))

#ifdef	__cplusplus
}
#endif
#endif
