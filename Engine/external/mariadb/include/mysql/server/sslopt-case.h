#ifndef SSLOPT_CASE_INCLUDED
#define SSLOPT_CASE_INCLUDED



#if defined(HAVE_OPENSSL) && !defined(EMBEDDED_LIBRARY)
    case OPT_SSL_KEY:
    case OPT_SSL_CERT:
    case OPT_SSL_CA:
    case OPT_SSL_CAPATH:
    case OPT_SSL_CIPHER:
    case OPT_SSL_CRL:
    case OPT_SSL_CRLPATH:
    
      opt_use_ssl= 1;
      
#ifdef HAVE_YASSL
      opt_ssl_crl= NULL;
      opt_ssl_crlpath= NULL;
#endif
      break;
#endif
#endif 
