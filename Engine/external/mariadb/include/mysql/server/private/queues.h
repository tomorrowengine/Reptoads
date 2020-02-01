



#ifndef _queues_h
#define _queues_h

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct st_queue {
  uchar **root;
  void *first_cmp_arg;
  uint elements;
  uint max_elements;
  uint offset_to_key;          
  uint offset_to_queue_pos;    
  uint auto_extent;
  int max_at_top;	
  int  (*compare)(void *, uchar *,uchar *);
} QUEUE;

#define queue_first_element(queue) 1
#define queue_last_element(queue) (queue)->elements
#define queue_empty(queue) ((queue)->elements == 0)
#define queue_top(queue) ((queue)->root[1])
#define queue_element(queue,index) ((queue)->root[index])
#define queue_end(queue) ((queue)->root[(queue)->elements])
#define queue_replace_top(queue) _downheap(queue, 1, (queue)->root[1])
#define queue_set_cmp_arg(queue, set_arg) (queue)->first_cmp_arg= set_arg
#define queue_set_max_at_top(queue, set_arg) \
  (queue)->max_at_top= set_arg ? -1 : 1
#define queue_remove_top(queue_arg) queue_remove((queue_arg), queue_first_element(queue_arg))
typedef int (*queue_compare)(void *,uchar *, uchar *);

int init_queue(QUEUE *queue,uint max_elements,uint offset_to_key,
	       pbool max_at_top, queue_compare compare,
	       void *first_cmp_arg, uint offset_to_queue_pos,
               uint auto_extent);
int reinit_queue(QUEUE *queue,uint max_elements,uint offset_to_key,
                 pbool max_at_top, queue_compare compare,
                 void *first_cmp_arg, uint offset_to_queue_pos,
                 uint auto_extent);
int resize_queue(QUEUE *queue, uint max_elements);
void delete_queue(QUEUE *queue);
void queue_insert(QUEUE *queue,uchar *element);
int queue_insert_safe(QUEUE *queue, uchar *element);
uchar *queue_remove(QUEUE *queue,uint idx);
void queue_replace(QUEUE *queue,uint idx);

#define queue_remove_all(queue) { (queue)->elements= 0; }
#define queue_is_full(queue) (queue->elements == queue->max_elements)
void _downheap(QUEUE *queue, uint idx, uchar *element);
void queue_fix(QUEUE *queue);
#define is_queue_inited(queue) ((queue)->root != 0)

#ifdef	__cplusplus
}
#endif
#endif
