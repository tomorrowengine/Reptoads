

#ifdef __cplusplus
extern "C" {
#endif

ulong net_field_length(uchar **packet);
my_ulonglong net_field_length_ll(uchar **packet);
my_ulonglong safe_net_field_length_ll(uchar **packet, size_t packet_len);
uchar *net_store_length(uchar *pkg, ulonglong length);
uchar *safe_net_store_length(uchar *pkg, size_t pkg_len, ulonglong length);
unsigned int net_length_size(ulonglong num);

#ifdef __cplusplus
}
#endif
