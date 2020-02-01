

#ifndef _my_rnd_h
#define _my_rnd_h

C_MODE_START

struct my_rnd_struct {
  unsigned long seed1,seed2,max_value;
  double max_value_dbl;
};

void my_rnd_init(struct my_rnd_struct *rand_st, ulong seed1, ulong seed2);
double my_rnd(struct my_rnd_struct *rand_st);
double my_rnd_ssl(struct my_rnd_struct *rand_st);

C_MODE_END

#endif 
