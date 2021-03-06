

#ifndef HEADER_ASN1T_H
# define HEADER_ASN1T_H

# include <stddef.h>
# include <openssl/e_os2.h>
# include <openssl/asn1.h>

# ifdef OPENSSL_BUILD_SHLIBCRYPTO
#  undef OPENSSL_EXTERN
#  define OPENSSL_EXTERN OPENSSL_EXPORT
# endif



#ifdef  __cplusplus
extern "C" {
#endif

# ifndef OPENSSL_EXPORT_VAR_AS_FUNCTION


#  define ASN1_ADB_ptr(iptr) ((const ASN1_ADB *)(iptr))



#  define ASN1_ITEM_start(itname) \
        const ASN1_ITEM itname##_it = {

#  define static_ASN1_ITEM_start(itname) \
        static const ASN1_ITEM itname##_it = {

#  define ASN1_ITEM_end(itname)                 \
                };

# else


#  define ASN1_ADB_ptr(iptr) ((const ASN1_ADB *)((iptr)()))



#  define ASN1_ITEM_start(itname) \
        const ASN1_ITEM * itname##_it(void) \
        { \
                static const ASN1_ITEM local_it = {

#  define static_ASN1_ITEM_start(itname) \
        static ASN1_ITEM_start(itname)

#  define ASN1_ITEM_end(itname) \
                }; \
        return &local_it; \
        }

# endif



# define ASN1_ITEM_TEMPLATE(tname) \
        static const ASN1_TEMPLATE tname##_item_tt

# define ASN1_ITEM_TEMPLATE_END(tname) \
        ;\
        ASN1_ITEM_start(tname) \
                ASN1_ITYPE_PRIMITIVE,\
                -1,\
                &tname##_item_tt,\
                0,\
                NULL,\
                0,\
                #tname \
        ASN1_ITEM_end(tname)
# define static_ASN1_ITEM_TEMPLATE_END(tname) \
        ;\
        static_ASN1_ITEM_start(tname) \
                ASN1_ITYPE_PRIMITIVE,\
                -1,\
                &tname##_item_tt,\
                0,\
                NULL,\
                0,\
                #tname \
        ASN1_ITEM_end(tname)





# define ASN1_SEQUENCE(tname) \
        static const ASN1_TEMPLATE tname##_seq_tt[]

# define ASN1_SEQUENCE_END(stname) ASN1_SEQUENCE_END_name(stname, stname)

# define static_ASN1_SEQUENCE_END(stname) static_ASN1_SEQUENCE_END_name(stname, stname)

# define ASN1_SEQUENCE_END_name(stname, tname) \
        ;\
        ASN1_ITEM_start(tname) \
                ASN1_ITYPE_SEQUENCE,\
                V_ASN1_SEQUENCE,\
                tname##_seq_tt,\
                sizeof(tname##_seq_tt) / sizeof(ASN1_TEMPLATE),\
                NULL,\
                sizeof(stname),\
                #tname \
        ASN1_ITEM_end(tname)

# define static_ASN1_SEQUENCE_END_name(stname, tname) \
        ;\
        static_ASN1_ITEM_start(tname) \
                ASN1_ITYPE_SEQUENCE,\
                V_ASN1_SEQUENCE,\
                tname##_seq_tt,\
                sizeof(tname##_seq_tt) / sizeof(ASN1_TEMPLATE),\
                NULL,\
                sizeof(stname),\
                #stname \
        ASN1_ITEM_end(tname)

# define ASN1_NDEF_SEQUENCE(tname) \
        ASN1_SEQUENCE(tname)

# define ASN1_NDEF_SEQUENCE_cb(tname, cb) \
        ASN1_SEQUENCE_cb(tname, cb)

# define ASN1_SEQUENCE_cb(tname, cb) \
        static const ASN1_AUX tname##_aux = {NULL, 0, 0, 0, cb, 0}; \
        ASN1_SEQUENCE(tname)

# define ASN1_BROKEN_SEQUENCE(tname) \
        static const ASN1_AUX tname##_aux = {NULL, ASN1_AFLG_BROKEN, 0, 0, 0, 0}; \
        ASN1_SEQUENCE(tname)

# define ASN1_SEQUENCE_ref(tname, cb) \
        static const ASN1_AUX tname##_aux = {NULL, ASN1_AFLG_REFCOUNT, offsetof(tname, references), offsetof(tname, lock), cb, 0}; \
        ASN1_SEQUENCE(tname)

# define ASN1_SEQUENCE_enc(tname, enc, cb) \
        static const ASN1_AUX tname##_aux = {NULL, ASN1_AFLG_ENCODING, 0, 0, cb, offsetof(tname, enc)}; \
        ASN1_SEQUENCE(tname)

# define ASN1_NDEF_SEQUENCE_END(tname) \
        ;\
        ASN1_ITEM_start(tname) \
                ASN1_ITYPE_NDEF_SEQUENCE,\
                V_ASN1_SEQUENCE,\
                tname##_seq_tt,\
                sizeof(tname##_seq_tt) / sizeof(ASN1_TEMPLATE),\
                NULL,\
                sizeof(tname),\
                #tname \
        ASN1_ITEM_end(tname)
# define static_ASN1_NDEF_SEQUENCE_END(tname) \
        ;\
        static_ASN1_ITEM_start(tname) \
                ASN1_ITYPE_NDEF_SEQUENCE,\
                V_ASN1_SEQUENCE,\
                tname##_seq_tt,\
                sizeof(tname##_seq_tt) / sizeof(ASN1_TEMPLATE),\
                NULL,\
                sizeof(tname),\
                #tname \
        ASN1_ITEM_end(tname)

# define ASN1_BROKEN_SEQUENCE_END(stname) ASN1_SEQUENCE_END_ref(stname, stname)
# define static_ASN1_BROKEN_SEQUENCE_END(stname) \
        static_ASN1_SEQUENCE_END_ref(stname, stname)

# define ASN1_SEQUENCE_END_enc(stname, tname) ASN1_SEQUENCE_END_ref(stname, tname)

# define ASN1_SEQUENCE_END_cb(stname, tname) ASN1_SEQUENCE_END_ref(stname, tname)
# define static_ASN1_SEQUENCE_END_cb(stname, tname) static_ASN1_SEQUENCE_END_ref(stname, tname)

# define ASN1_SEQUENCE_END_ref(stname, tname) \
        ;\
        ASN1_ITEM_start(tname) \
                ASN1_ITYPE_SEQUENCE,\
                V_ASN1_SEQUENCE,\
                tname##_seq_tt,\
                sizeof(tname##_seq_tt) / sizeof(ASN1_TEMPLATE),\
                &tname##_aux,\
                sizeof(stname),\
                #tname \
        ASN1_ITEM_end(tname)
# define static_ASN1_SEQUENCE_END_ref(stname, tname) \
        ;\
        static_ASN1_ITEM_start(tname) \
                ASN1_ITYPE_SEQUENCE,\
                V_ASN1_SEQUENCE,\
                tname##_seq_tt,\
                sizeof(tname##_seq_tt) / sizeof(ASN1_TEMPLATE),\
                &tname##_aux,\
                sizeof(stname),\
                #stname \
        ASN1_ITEM_end(tname)

# define ASN1_NDEF_SEQUENCE_END_cb(stname, tname) \
        ;\
        ASN1_ITEM_start(tname) \
                ASN1_ITYPE_NDEF_SEQUENCE,\
                V_ASN1_SEQUENCE,\
                tname##_seq_tt,\
                sizeof(tname##_seq_tt) / sizeof(ASN1_TEMPLATE),\
                &tname##_aux,\
                sizeof(stname),\
                #stname \
        ASN1_ITEM_end(tname)



# define ASN1_CHOICE(tname) \
        static const ASN1_TEMPLATE tname##_ch_tt[]

# define ASN1_CHOICE_cb(tname, cb) \
        static const ASN1_AUX tname##_aux = {NULL, 0, 0, 0, cb, 0}; \
        ASN1_CHOICE(tname)

# define ASN1_CHOICE_END(stname) ASN1_CHOICE_END_name(stname, stname)

# define static_ASN1_CHOICE_END(stname) static_ASN1_CHOICE_END_name(stname, stname)

# define ASN1_CHOICE_END_name(stname, tname) ASN1_CHOICE_END_selector(stname, tname, type)

# define static_ASN1_CHOICE_END_name(stname, tname) static_ASN1_CHOICE_END_selector(stname, tname, type)

# define ASN1_CHOICE_END_selector(stname, tname, selname) \
        ;\
        ASN1_ITEM_start(tname) \
                ASN1_ITYPE_CHOICE,\
                offsetof(stname,selname) ,\
                tname##_ch_tt,\
                sizeof(tname##_ch_tt) / sizeof(ASN1_TEMPLATE),\
                NULL,\
                sizeof(stname),\
                #stname \
        ASN1_ITEM_end(tname)

# define static_ASN1_CHOICE_END_selector(stname, tname, selname) \
        ;\
        static_ASN1_ITEM_start(tname) \
                ASN1_ITYPE_CHOICE,\
                offsetof(stname,selname) ,\
                tname##_ch_tt,\
                sizeof(tname##_ch_tt) / sizeof(ASN1_TEMPLATE),\
                NULL,\
                sizeof(stname),\
                #stname \
        ASN1_ITEM_end(tname)

# define ASN1_CHOICE_END_cb(stname, tname, selname) \
        ;\
        ASN1_ITEM_start(tname) \
                ASN1_ITYPE_CHOICE,\
                offsetof(stname,selname) ,\
                tname##_ch_tt,\
                sizeof(tname##_ch_tt) / sizeof(ASN1_TEMPLATE),\
                &tname##_aux,\
                sizeof(stname),\
                #stname \
        ASN1_ITEM_end(tname)



# define ASN1_EX_TEMPLATE_TYPE(flags, tag, name, type) { \
        (flags), (tag), 0,\
        #name, ASN1_ITEM_ref(type) }





# define ASN1_EX_TYPE(flags, tag, stname, field, type) { \
        (flags), (tag), offsetof(stname, field),\
        #field, ASN1_ITEM_ref(type) }



# define ASN1_IMP_EX(stname, field, type, tag, ex) \
         ASN1_EX_TYPE(ASN1_TFLG_IMPLICIT | (ex), tag, stname, field, type)

# define ASN1_EXP_EX(stname, field, type, tag, ex) \
         ASN1_EX_TYPE(ASN1_TFLG_EXPLICIT | (ex), tag, stname, field, type)



# ifndef OPENSSL_EXPORT_VAR_AS_FUNCTION
#  define ASN1_ADB_OBJECT(tblname) { ASN1_TFLG_ADB_OID, -1, 0, #tblname, (const ASN1_ITEM *)&(tblname##_adb) }
#  define ASN1_ADB_INTEGER(tblname) { ASN1_TFLG_ADB_INT, -1, 0, #tblname, (const ASN1_ITEM *)&(tblname##_adb) }
# else
#  define ASN1_ADB_OBJECT(tblname) { ASN1_TFLG_ADB_OID, -1, 0, #tblname, tblname##_adb }
#  define ASN1_ADB_INTEGER(tblname) { ASN1_TFLG_ADB_INT, -1, 0, #tblname, tblname##_adb }
# endif

# define ASN1_SIMPLE(stname, field, type) ASN1_EX_TYPE(0,0, stname, field, type)

# define ASN1_EMBED(stname, field, type) ASN1_EX_TYPE(ASN1_TFLG_EMBED,0, stname, field, type)


# define ASN1_OPT(stname, field, type) ASN1_EX_TYPE(ASN1_TFLG_OPTIONAL, 0, stname, field, type)
# define ASN1_OPT_EMBED(stname, field, type) ASN1_EX_TYPE(ASN1_TFLG_OPTIONAL|ASN1_TFLG_EMBED, 0, stname, field, type)


# define ASN1_IMP(stname, field, type, tag) ASN1_IMP_EX(stname, field, type, tag, 0)
# define ASN1_IMP_EMBED(stname, field, type, tag) ASN1_IMP_EX(stname, field, type, tag, ASN1_TFLG_EMBED)


# define ASN1_IMP_OPT(stname, field, type, tag) ASN1_IMP_EX(stname, field, type, tag, ASN1_TFLG_OPTIONAL)
# define ASN1_IMP_OPT_EMBED(stname, field, type, tag) ASN1_IMP_EX(stname, field, type, tag, ASN1_TFLG_OPTIONAL|ASN1_TFLG_EMBED)



# define ASN1_EXP(stname, field, type, tag) ASN1_EXP_EX(stname, field, type, tag, 0)
# define ASN1_EXP_EMBED(stname, field, type, tag) ASN1_EXP_EX(stname, field, type, tag, ASN1_TFLG_EMBED)
# define ASN1_EXP_OPT(stname, field, type, tag) ASN1_EXP_EX(stname, field, type, tag, ASN1_TFLG_OPTIONAL)
# define ASN1_EXP_OPT_EMBED(stname, field, type, tag) ASN1_EXP_EX(stname, field, type, tag, ASN1_TFLG_OPTIONAL|ASN1_TFLG_EMBED)


# define ASN1_SEQUENCE_OF(stname, field, type) \
                ASN1_EX_TYPE(ASN1_TFLG_SEQUENCE_OF, 0, stname, field, type)


# define ASN1_SEQUENCE_OF_OPT(stname, field, type) \
                ASN1_EX_TYPE(ASN1_TFLG_SEQUENCE_OF|ASN1_TFLG_OPTIONAL, 0, stname, field, type)



# define ASN1_SET_OF(stname, field, type) \
                ASN1_EX_TYPE(ASN1_TFLG_SET_OF, 0, stname, field, type)

# define ASN1_SET_OF_OPT(stname, field, type) \
                ASN1_EX_TYPE(ASN1_TFLG_SET_OF|ASN1_TFLG_OPTIONAL, 0, stname, field, type)



# define ASN1_IMP_SET_OF(stname, field, type, tag) \
                        ASN1_IMP_EX(stname, field, type, tag, ASN1_TFLG_SET_OF)

# define ASN1_EXP_SET_OF(stname, field, type, tag) \
                        ASN1_EXP_EX(stname, field, type, tag, ASN1_TFLG_SET_OF)

# define ASN1_IMP_SET_OF_OPT(stname, field, type, tag) \
                        ASN1_IMP_EX(stname, field, type, tag, ASN1_TFLG_SET_OF|ASN1_TFLG_OPTIONAL)

# define ASN1_EXP_SET_OF_OPT(stname, field, type, tag) \
                        ASN1_EXP_EX(stname, field, type, tag, ASN1_TFLG_SET_OF|ASN1_TFLG_OPTIONAL)

# define ASN1_IMP_SEQUENCE_OF(stname, field, type, tag) \
                        ASN1_IMP_EX(stname, field, type, tag, ASN1_TFLG_SEQUENCE_OF)

# define ASN1_IMP_SEQUENCE_OF_OPT(stname, field, type, tag) \
                        ASN1_IMP_EX(stname, field, type, tag, ASN1_TFLG_SEQUENCE_OF|ASN1_TFLG_OPTIONAL)

# define ASN1_EXP_SEQUENCE_OF(stname, field, type, tag) \
                        ASN1_EXP_EX(stname, field, type, tag, ASN1_TFLG_SEQUENCE_OF)

# define ASN1_EXP_SEQUENCE_OF_OPT(stname, field, type, tag) \
                        ASN1_EXP_EX(stname, field, type, tag, ASN1_TFLG_SEQUENCE_OF|ASN1_TFLG_OPTIONAL)


# define ASN1_NDEF_EXP(stname, field, type, tag) \
                        ASN1_EXP_EX(stname, field, type, tag, ASN1_TFLG_NDEF)


# define ASN1_NDEF_EXP_OPT(stname, field, type, tag) \
                        ASN1_EXP_EX(stname, field, type, tag, ASN1_TFLG_OPTIONAL|ASN1_TFLG_NDEF)



# define ASN1_ADB(name) \
        static const ASN1_ADB_TABLE name##_adbtbl[]

# ifndef OPENSSL_EXPORT_VAR_AS_FUNCTION

#  define ASN1_ADB_END(name, flags, field, adb_cb, def, none) \
        ;\
        static const ASN1_ADB name##_adb = {\
                flags,\
                offsetof(name, field),\
                adb_cb,\
                name##_adbtbl,\
                sizeof(name##_adbtbl) / sizeof(ASN1_ADB_TABLE),\
                def,\
                none\
        }

# else

#  define ASN1_ADB_END(name, flags, field, adb_cb, def, none) \
        ;\
        static const ASN1_ITEM *name##_adb(void) \
        { \
        static const ASN1_ADB internal_adb = \
                {\
                flags,\
                offsetof(name, field),\
                adb_cb,\
                name##_adbtbl,\
                sizeof(name##_adbtbl) / sizeof(ASN1_ADB_TABLE),\
                def,\
                none\
                }; \
                return (const ASN1_ITEM *) &internal_adb; \
        } \
        void dummy_function(void)

# endif

# define ADB_ENTRY(val, template) {val, template}

# define ASN1_ADB_TEMPLATE(name) \
        static const ASN1_TEMPLATE name##_tt



struct ASN1_TEMPLATE_st {
    unsigned long flags;        
    long tag;                   
    unsigned long offset;       
    const char *field_name;     
    ASN1_ITEM_EXP *item;        
};



# define ASN1_TEMPLATE_item(t) (t->item_ptr)
# define ASN1_TEMPLATE_adb(t) (t->item_ptr)

typedef struct ASN1_ADB_TABLE_st ASN1_ADB_TABLE;
typedef struct ASN1_ADB_st ASN1_ADB;

struct ASN1_ADB_st {
    unsigned long flags;        
    unsigned long offset;       
    int (*adb_cb)(long *psel);  
    const ASN1_ADB_TABLE *tbl;  
    long tblcount;              
    const ASN1_TEMPLATE *default_tt; 
    const ASN1_TEMPLATE *null_tt; 
};

struct ASN1_ADB_TABLE_st {
    long value;                 
    const ASN1_TEMPLATE tt;     
};




# define ASN1_TFLG_OPTIONAL      (0x1)


# define ASN1_TFLG_SET_OF        (0x1 << 1)


# define ASN1_TFLG_SEQUENCE_OF   (0x2 << 1)


# define ASN1_TFLG_SET_ORDER     (0x3 << 1)


# define ASN1_TFLG_SK_MASK       (0x3 << 1)




# define ASN1_TFLG_IMPTAG        (0x1 << 3)


# define ASN1_TFLG_EXPTAG        (0x2 << 3)

# define ASN1_TFLG_TAG_MASK      (0x3 << 3)


# define ASN1_TFLG_IMPLICIT      (ASN1_TFLG_IMPTAG|ASN1_TFLG_CONTEXT)


# define ASN1_TFLG_EXPLICIT      (ASN1_TFLG_EXPTAG|ASN1_TFLG_CONTEXT)




# define ASN1_TFLG_UNIVERSAL     (0x0<<6)

# define ASN1_TFLG_APPLICATION   (0x1<<6)

# define ASN1_TFLG_CONTEXT       (0x2<<6)

# define ASN1_TFLG_PRIVATE       (0x3<<6)

# define ASN1_TFLG_TAG_CLASS     (0x3<<6)



# define ASN1_TFLG_ADB_MASK      (0x3<<8)

# define ASN1_TFLG_ADB_OID       (0x1<<8)

# define ASN1_TFLG_ADB_INT       (0x1<<9)



# define ASN1_TFLG_NDEF          (0x1<<11)


# define ASN1_TFLG_EMBED         (0x1 << 12)



struct ASN1_ITEM_st {
    char itype;                 
    long utype;                 
    const ASN1_TEMPLATE *templates; 
    long tcount;                
    const void *funcs;          
    long size;                  
    const char *sname;          
};



# define ASN1_ITYPE_PRIMITIVE            0x0

# define ASN1_ITYPE_SEQUENCE             0x1

# define ASN1_ITYPE_CHOICE               0x2

# define ASN1_ITYPE_EXTERN               0x4

# define ASN1_ITYPE_MSTRING              0x5

# define ASN1_ITYPE_NDEF_SEQUENCE        0x6



struct ASN1_TLC_st {
    char valid;                 
    int ret;                    
    long plen;                  
    int ptag;                   
    int pclass;                 
    int hdrlen;                 
};


typedef int ASN1_ex_d2i(ASN1_VALUE **pval, const unsigned char **in, long len,
                        const ASN1_ITEM *it, int tag, int aclass, char opt,
                        ASN1_TLC *ctx);

typedef int ASN1_ex_i2d(ASN1_VALUE **pval, unsigned char **out,
                        const ASN1_ITEM *it, int tag, int aclass);
typedef int ASN1_ex_new_func(ASN1_VALUE **pval, const ASN1_ITEM *it);
typedef void ASN1_ex_free_func(ASN1_VALUE **pval, const ASN1_ITEM *it);

typedef int ASN1_ex_print_func(BIO *out, ASN1_VALUE **pval,
                               int indent, const char *fname,
                               const ASN1_PCTX *pctx);

typedef int ASN1_primitive_i2c(ASN1_VALUE **pval, unsigned char *cont,
                               int *putype, const ASN1_ITEM *it);
typedef int ASN1_primitive_c2i(ASN1_VALUE **pval, const unsigned char *cont,
                               int len, int utype, char *free_cont,
                               const ASN1_ITEM *it);
typedef int ASN1_primitive_print(BIO *out, ASN1_VALUE **pval,
                                 const ASN1_ITEM *it, int indent,
                                 const ASN1_PCTX *pctx);

typedef struct ASN1_EXTERN_FUNCS_st {
    void *app_data;
    ASN1_ex_new_func *asn1_ex_new;
    ASN1_ex_free_func *asn1_ex_free;
    ASN1_ex_free_func *asn1_ex_clear;
    ASN1_ex_d2i *asn1_ex_d2i;
    ASN1_ex_i2d *asn1_ex_i2d;
    ASN1_ex_print_func *asn1_ex_print;
} ASN1_EXTERN_FUNCS;

typedef struct ASN1_PRIMITIVE_FUNCS_st {
    void *app_data;
    unsigned long flags;
    ASN1_ex_new_func *prim_new;
    ASN1_ex_free_func *prim_free;
    ASN1_ex_free_func *prim_clear;
    ASN1_primitive_c2i *prim_c2i;
    ASN1_primitive_i2c *prim_i2c;
    ASN1_primitive_print *prim_print;
} ASN1_PRIMITIVE_FUNCS;



typedef int ASN1_aux_cb(int operation, ASN1_VALUE **in, const ASN1_ITEM *it,
                        void *exarg);

typedef struct ASN1_AUX_st {
    void *app_data;
    int flags;
    int ref_offset;             
    int ref_lock;               
    ASN1_aux_cb *asn1_cb;
    int enc_offset;             
} ASN1_AUX;


typedef struct ASN1_PRINT_ARG_st {
    BIO *out;
    int indent;
    const ASN1_PCTX *pctx;
} ASN1_PRINT_ARG;


typedef struct ASN1_STREAM_ARG_st {
    
    BIO *out;
    
    BIO *ndef_bio;
    
    unsigned char **boundary;
} ASN1_STREAM_ARG;




# define ASN1_AFLG_REFCOUNT      1

# define ASN1_AFLG_ENCODING      2

# define ASN1_AFLG_BROKEN        4



# define ASN1_OP_NEW_PRE         0
# define ASN1_OP_NEW_POST        1
# define ASN1_OP_FREE_PRE        2
# define ASN1_OP_FREE_POST       3
# define ASN1_OP_D2I_PRE         4
# define ASN1_OP_D2I_POST        5
# define ASN1_OP_I2D_PRE         6
# define ASN1_OP_I2D_POST        7
# define ASN1_OP_PRINT_PRE       8
# define ASN1_OP_PRINT_POST      9
# define ASN1_OP_STREAM_PRE      10
# define ASN1_OP_STREAM_POST     11
# define ASN1_OP_DETACHED_PRE    12
# define ASN1_OP_DETACHED_POST   13


# define IMPLEMENT_ASN1_TYPE(stname) IMPLEMENT_ASN1_TYPE_ex(stname, stname, 0)
# define IMPLEMENT_ASN1_TYPE_ex(itname, vname, ex) \
                                ASN1_ITEM_start(itname) \
                                        ASN1_ITYPE_PRIMITIVE, V_##vname, NULL, 0, NULL, ex, #itname \
                                ASN1_ITEM_end(itname)


# define IMPLEMENT_ASN1_MSTRING(itname, mask) \
                                ASN1_ITEM_start(itname) \
                                        ASN1_ITYPE_MSTRING, mask, NULL, 0, NULL, sizeof(ASN1_STRING), #itname \
                                ASN1_ITEM_end(itname)

# define IMPLEMENT_EXTERN_ASN1(sname, tag, fptrs) \
        ASN1_ITEM_start(sname) \
                ASN1_ITYPE_EXTERN, \
                tag, \
                NULL, \
                0, \
                &fptrs, \
                0, \
                #sname \
        ASN1_ITEM_end(sname)



# define IMPLEMENT_ASN1_FUNCTIONS(stname) IMPLEMENT_ASN1_FUNCTIONS_fname(stname, stname, stname)

# define IMPLEMENT_ASN1_FUNCTIONS_name(stname, itname) IMPLEMENT_ASN1_FUNCTIONS_fname(stname, itname, itname)

# define IMPLEMENT_ASN1_FUNCTIONS_ENCODE_name(stname, itname) \
                        IMPLEMENT_ASN1_FUNCTIONS_ENCODE_fname(stname, itname, itname)

# define IMPLEMENT_STATIC_ASN1_ALLOC_FUNCTIONS(stname) \
                IMPLEMENT_ASN1_ALLOC_FUNCTIONS_pfname(static, stname, stname, stname)

# define IMPLEMENT_ASN1_ALLOC_FUNCTIONS(stname) \
                IMPLEMENT_ASN1_ALLOC_FUNCTIONS_fname(stname, stname, stname)

# define IMPLEMENT_ASN1_ALLOC_FUNCTIONS_pfname(pre, stname, itname, fname) \
        pre stname *fname##_new(void) \
        { \
                return (stname *)ASN1_item_new(ASN1_ITEM_rptr(itname)); \
        } \
        pre void fname##_free(stname *a) \
        { \
                ASN1_item_free((ASN1_VALUE *)a, ASN1_ITEM_rptr(itname)); \
        }

# define IMPLEMENT_ASN1_ALLOC_FUNCTIONS_fname(stname, itname, fname) \
        stname *fname##_new(void) \
        { \
                return (stname *)ASN1_item_new(ASN1_ITEM_rptr(itname)); \
        } \
        void fname##_free(stname *a) \
        { \
                ASN1_item_free((ASN1_VALUE *)a, ASN1_ITEM_rptr(itname)); \
        }

# define IMPLEMENT_ASN1_FUNCTIONS_fname(stname, itname, fname) \
        IMPLEMENT_ASN1_ENCODE_FUNCTIONS_fname(stname, itname, fname) \
        IMPLEMENT_ASN1_ALLOC_FUNCTIONS_fname(stname, itname, fname)

# define IMPLEMENT_ASN1_ENCODE_FUNCTIONS_fname(stname, itname, fname) \
        stname *d2i_##fname(stname **a, const unsigned char **in, long len) \
        { \
                return (stname *)ASN1_item_d2i((ASN1_VALUE **)a, in, len, ASN1_ITEM_rptr(itname));\
        } \
        int i2d_##fname(stname *a, unsigned char **out) \
        { \
                return ASN1_item_i2d((ASN1_VALUE *)a, out, ASN1_ITEM_rptr(itname));\
        }

# define IMPLEMENT_ASN1_NDEF_FUNCTION(stname) \
        int i2d_##stname##_NDEF(stname *a, unsigned char **out) \
        { \
                return ASN1_item_ndef_i2d((ASN1_VALUE *)a, out, ASN1_ITEM_rptr(stname));\
        }

# define IMPLEMENT_STATIC_ASN1_ENCODE_FUNCTIONS(stname) \
        static stname *d2i_##stname(stname **a, \
                                   const unsigned char **in, long len) \
        { \
                return (stname *)ASN1_item_d2i((ASN1_VALUE **)a, in, len, \
                                               ASN1_ITEM_rptr(stname)); \
        } \
        static int i2d_##stname(stname *a, unsigned char **out) \
        { \
                return ASN1_item_i2d((ASN1_VALUE *)a, out, \
                                     ASN1_ITEM_rptr(stname)); \
        }


# define IMPLEMENT_ASN1_ENCODE_FUNCTIONS_const_fname(stname, itname, fname) \
        stname *d2i_##fname(stname **a, const unsigned char **in, long len) \
        { \
                return (stname *)ASN1_item_d2i((ASN1_VALUE **)a, in, len, ASN1_ITEM_rptr(itname));\
        } \
        int i2d_##fname(const stname *a, unsigned char **out) \
        { \
                return ASN1_item_i2d((ASN1_VALUE *)a, out, ASN1_ITEM_rptr(itname));\
        }

# define IMPLEMENT_ASN1_DUP_FUNCTION(stname) \
        stname * stname##_dup(stname *x) \
        { \
        return ASN1_item_dup(ASN1_ITEM_rptr(stname), x); \
        }

# define IMPLEMENT_ASN1_PRINT_FUNCTION(stname) \
        IMPLEMENT_ASN1_PRINT_FUNCTION_fname(stname, stname, stname)

# define IMPLEMENT_ASN1_PRINT_FUNCTION_fname(stname, itname, fname) \
        int fname##_print_ctx(BIO *out, stname *x, int indent, \
                                                const ASN1_PCTX *pctx) \
        { \
                return ASN1_item_print(out, (ASN1_VALUE *)x, indent, \
                        ASN1_ITEM_rptr(itname), pctx); \
        }

# define IMPLEMENT_ASN1_FUNCTIONS_const(name) \
                IMPLEMENT_ASN1_FUNCTIONS_const_fname(name, name, name)

# define IMPLEMENT_ASN1_FUNCTIONS_const_fname(stname, itname, fname) \
        IMPLEMENT_ASN1_ENCODE_FUNCTIONS_const_fname(stname, itname, fname) \
        IMPLEMENT_ASN1_ALLOC_FUNCTIONS_fname(stname, itname, fname)



DECLARE_ASN1_ITEM(ASN1_BOOLEAN)
DECLARE_ASN1_ITEM(ASN1_TBOOLEAN)
DECLARE_ASN1_ITEM(ASN1_FBOOLEAN)
DECLARE_ASN1_ITEM(ASN1_SEQUENCE)
DECLARE_ASN1_ITEM(CBIGNUM)
DECLARE_ASN1_ITEM(BIGNUM)
DECLARE_ASN1_ITEM(INT32)
DECLARE_ASN1_ITEM(ZINT32)
DECLARE_ASN1_ITEM(UINT32)
DECLARE_ASN1_ITEM(ZUINT32)
DECLARE_ASN1_ITEM(INT64)
DECLARE_ASN1_ITEM(ZINT64)
DECLARE_ASN1_ITEM(UINT64)
DECLARE_ASN1_ITEM(ZUINT64)

# if OPENSSL_API_COMPAT < 0x10200000L

DECLARE_ASN1_ITEM(LONG)
DECLARE_ASN1_ITEM(ZLONG)
# endif

DEFINE_STACK_OF(ASN1_VALUE)



int ASN1_item_ex_new(ASN1_VALUE **pval, const ASN1_ITEM *it);
void ASN1_item_ex_free(ASN1_VALUE **pval, const ASN1_ITEM *it);

int ASN1_item_ex_d2i(ASN1_VALUE **pval, const unsigned char **in, long len,
                     const ASN1_ITEM *it, int tag, int aclass, char opt,
                     ASN1_TLC *ctx);

int ASN1_item_ex_i2d(ASN1_VALUE **pval, unsigned char **out,
                     const ASN1_ITEM *it, int tag, int aclass);

#ifdef  __cplusplus
}
#endif
#endif
