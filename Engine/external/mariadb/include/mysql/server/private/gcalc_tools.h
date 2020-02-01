


#ifndef GCALC_TOOLS_INCLUDED
#define GCALC_TOOLS_INCLUDED

#include "gcalc_slicescan.h"
#include "sql_string.h"




class Gcalc_function
{
private:
  String shapes_buffer;
  String function_buffer;
  int *i_states;
  int *b_states;
  uint32 cur_object_id;
  uint n_shapes;
  int count_internal(const char *cur_func, uint set_type,
                     const char **end);
public:
  enum value
  {
    v_empty=   0x0000000,
    v_find_t=  0x1000000,
    v_find_f=  0x2000000,
    v_t_found= 0x3000000,
    v_f_found= 0x4000000,
    v_mask=    0x7000000
  };
  enum op_type
  {
    op_not=           0x80000000,
    op_shape=         0x00000000,
    op_union=         0x10000000,
    op_intersection=  0x20000000,
    op_symdifference= 0x30000000,
    op_difference=    0x40000000,
    op_repeat=        0x50000000,
    op_border=        0x60000000,
    op_internals=     0x70000000,
    op_false=         0x08000000,
    op_any=           0x78000000 
  };
  enum shape_type
  {
    shape_point= 0,
    shape_line= 1,
    shape_polygon= 2,
    shape_hole= 3
  };
  enum count_result
  {
    result_false= 0,
    result_true= 1,
    result_unknown= 2
  };
  Gcalc_function() : n_shapes(0) {}
  gcalc_shape_info add_new_shape(uint32 shape_id, shape_type shape_kind);
  
  int single_shape_op(shape_type shape_kind, gcalc_shape_info *si);
  void add_operation(uint operation, uint32 n_operands);
  void add_not_operation(op_type operation, uint32 n_operands);
  uint32 get_next_expression_pos() { return function_buffer.length(); }
  void add_operands_to_op(uint32 operation_pos, uint32 n_operands);
  int repeat_expression(uint32 exp_pos);
  void set_cur_obj(uint32 cur_obj) { cur_object_id= cur_obj; }
  int reserve_shape_buffer(uint n_shapes);
  int reserve_op_buffer(uint n_ops);
  uint get_nshapes() const { return n_shapes; }
  shape_type get_shape_kind(gcalc_shape_info si) const
  {
    return (shape_type) uint4korr(shapes_buffer.ptr() + (si*4));
  }

  void set_states(int *shape_states) { i_states= shape_states; }
  int alloc_states();
  void invert_i_state(gcalc_shape_info shape) { i_states[shape]^= 1; }
  void set_i_state(gcalc_shape_info shape) { i_states[shape]= 1; }
  void clear_i_state(gcalc_shape_info shape) { i_states[shape]= 0; }
  void set_b_state(gcalc_shape_info shape) { b_states[shape]= 1; }
  void clear_b_state(gcalc_shape_info shape) { b_states[shape]= 0; }
  int get_state(gcalc_shape_info shape)
    { return i_states[shape] | b_states[shape]; }
  int get_i_state(gcalc_shape_info shape) { return i_states[shape]; }
  int get_b_state(gcalc_shape_info shape) { return b_states[shape]; }
  int count()
    { return count_internal(function_buffer.ptr(), 0, 0); }
  int count_last()
    { return count_internal(function_buffer.ptr(), 1, 0); }
  void clear_i_states();
  void clear_b_states();
  void reset();

  int check_function(Gcalc_scan_iterator &scan_it);
};




class Gcalc_operation_transporter : public Gcalc_shape_transporter
{
protected:
  Gcalc_function *m_fn;
  gcalc_shape_info m_si;
public:
  Gcalc_operation_transporter(Gcalc_function *fn, Gcalc_heap *heap) :
    Gcalc_shape_transporter(heap), m_fn(fn) {}

  int single_point(double x, double y);
  int start_line();
  int complete_line();
  int start_poly();
  int complete_poly();
  int start_ring();
  int complete_ring();
  int add_point(double x, double y);
  int start_collection(int n_objects);
  int empty_shape();
};




class Gcalc_result_receiver
{
  String buffer;
  uint32 n_points;
  Gcalc_function::shape_type common_shapetype;
  bool collection_result;
  uint32 n_shapes;
  uint32 n_holes;

  Gcalc_function::shape_type cur_shape;
  uint32 shape_pos;
  double first_x, first_y, prev_x, prev_y;
  double shape_area;
public:
  Gcalc_result_receiver() : collection_result(FALSE), n_shapes(0), n_holes(0)
    {}
  int start_shape(Gcalc_function::shape_type shape);
  int add_point(double x, double y);
  int complete_shape();
  int single_point(double x, double y);
  int done();
  void reset();

  const char *result() { return buffer.ptr(); }
  uint length() { return buffer.length(); }
  int get_nshapes() { return n_shapes; }
  int get_nholes() { return n_holes; }
  int get_result_typeid();
  uint32 position() { return buffer.length(); }
  int move_hole(uint32 dest_position, uint32 source_position,
                uint32 *position_shift);
};




class Gcalc_operation_reducer : public Gcalc_dyn_list
{
public:
  enum modes
  {
    
    default_mode= 0,
    prefer_big_with_holes= 1,
    polygon_selfintersections_allowed= 2,  
    line_selfintersections_allowed= 4      
  };

  Gcalc_operation_reducer(size_t blk_size=8192);
  Gcalc_operation_reducer(const Gcalc_operation_reducer &gor);
  void init(Gcalc_function *fn, modes mode= default_mode);
  Gcalc_operation_reducer(Gcalc_function *fn, modes mode= default_mode,
		       size_t blk_size=8192);
  GCALC_DECL_TERMINATED_STATE(killed)
  int count_slice(Gcalc_scan_iterator *si);
  int count_all(Gcalc_heap *hp);
  int get_result(Gcalc_result_receiver *storage);
  void reset();

#ifndef GCALC_DBUG_OFF
  int n_res_points;
#endif 
  class res_point : public Gcalc_dyn_list::Item
  {
  public:
    int intersection_point;
    union
    {
      const Gcalc_heap::Info *pi;
      res_point *first_poly_node;
    };
    union
    {
      res_point *outer_poly;
      uint32 poly_position;
    };
    res_point *up;
    res_point *down;
    res_point *glue;
    Gcalc_function::shape_type type;
    Gcalc_dyn_list::Item **prev_hook;
#ifndef GCALC_DBUG_OFF
    int point_n;
#endif 
    void set(const Gcalc_scan_iterator *si);
    res_point *get_next() { return (res_point *)next; }
  };

  class active_thread : public Gcalc_dyn_list::Item
  {
  public:
    res_point *rp;
    res_point *thread_start;

    const Gcalc_heap::Info *p1, *p2;
    res_point *enabled() { return rp; }
    active_thread *get_next() { return (active_thread *)next; }
  };

  class poly_instance : public Gcalc_dyn_list::Item
  {
  public:
    uint32 *after_poly_position;
    poly_instance *get_next() { return (poly_instance *)next; }
  };

  class line : public Gcalc_dyn_list::Item
  {
  public:
    active_thread *t;
    int incoming;
    const Gcalc_scan_iterator::point *p;
    line *get_next() { return (line *)next; }
  };

  class poly_border : public Gcalc_dyn_list::Item
  {
  public:
    active_thread *t;
    int incoming;
    int prev_state;
    const Gcalc_scan_iterator::point *p;
    poly_border *get_next() { return (poly_border *)next; }
  };

  line *m_lines;
  Gcalc_dyn_list::Item **m_lines_hook;
  poly_border *m_poly_borders;
  Gcalc_dyn_list::Item **m_poly_borders_hook;
  line *new_line() { return (line *) new_item(); }
  poly_border *new_poly_border() { return (poly_border *) new_item(); }
  int add_line(int incoming, active_thread *t,
               const Gcalc_scan_iterator::point *p);
  int add_poly_border(int incoming, active_thread *t, int prev_state,
                      const Gcalc_scan_iterator::point *p);

protected:
  Gcalc_function *m_fn;
  Gcalc_dyn_list::Item **m_res_hook;
  res_point *m_result;
  int m_mode;

  res_point *result_heap;
  active_thread *m_first_active_thread;

  res_point *add_res_point(Gcalc_function::shape_type type);
  active_thread *new_active_thread() { return (active_thread *)new_item(); }

  poly_instance *new_poly() { return (poly_instance *) new_item(); }

private:
  int start_line(active_thread *t, const Gcalc_scan_iterator::point *p,
                 const Gcalc_scan_iterator *si);
  int end_line(active_thread *t, const Gcalc_scan_iterator *si);
  int connect_threads(int incoming_a, int incoming_b,
                      active_thread *ta, active_thread *tb,
                      const Gcalc_scan_iterator::point *pa,
                      const Gcalc_scan_iterator::point *pb,
                      active_thread *prev_range,
                      const Gcalc_scan_iterator *si,
                      Gcalc_function::shape_type s_t);
  int add_single_point(const Gcalc_scan_iterator *si);
  poly_border *get_pair_border(poly_border *b1);
  int continue_range(active_thread *t, const Gcalc_heap::Info *p,
                     const Gcalc_heap::Info *p_next);
  int continue_i_range(active_thread *t,
                       const Gcalc_heap::Info *ii);
  int end_couple(active_thread *t0, active_thread *t1, const Gcalc_heap::Info *p);
  int get_single_result(res_point *res, Gcalc_result_receiver *storage);
  int get_result_thread(res_point *cur, Gcalc_result_receiver *storage,
			int move_upward, res_point *first_poly_node);
  int get_polygon_result(res_point *cur, Gcalc_result_receiver *storage,
                         res_point *first_poly_node);
  int get_line_result(res_point *cur, Gcalc_result_receiver *storage);

  void free_result(res_point *res);
};

#endif 

