

#ifndef WSREP_MYSQLD_C_H
#define WSREP_MYSQLD_C_H

enum enum_wsrep_certification_rules {
    WSREP_CERTIFICATION_RULES_STRICT,
    WSREP_CERTIFICATION_RULES_OPTIMIZED
};


extern ulong wsrep_certification_rules __attribute__((weak));

#endif 
