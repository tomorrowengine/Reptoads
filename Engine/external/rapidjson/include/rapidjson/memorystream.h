













#ifndef RAPIDJSON_MEMORYSTREAM_H_
#define RAPIDJSON_MEMORYSTREAM_H_

#include "stream.h"

#ifdef __clang__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(unreachable-code)
RAPIDJSON_DIAG_OFF(missing-noreturn)
#endif

RAPIDJSON_NAMESPACE_BEGIN



struct MemoryStream {
    typedef char Ch; 

    MemoryStream(const Ch *src, size_t size) : src_(src), begin_(src), end_(src + size), size_(size) {}

    Ch Peek() const { return RAPIDJSON_UNLIKELY(src_ == end_) ? '\0' : *src_; }
    Ch Take() { return RAPIDJSON_UNLIKELY(src_ == end_) ? '\0' : *src_++; }
    size_t Tell() const { return static_cast<size_t>(src_ - begin_); }

    Ch* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
    void Put(Ch) { RAPIDJSON_ASSERT(false); }
    void Flush() { RAPIDJSON_ASSERT(false); }
    size_t PutEnd(Ch*) { RAPIDJSON_ASSERT(false); return 0; }

    
    const Ch* Peek4() const {
        return Tell() + 4 <= size_ ? src_ : 0;
    }

    const Ch* src_;     
    const Ch* begin_;   
    const Ch* end_;     
    size_t size_;       
};

RAPIDJSON_NAMESPACE_END

#ifdef __clang__
RAPIDJSON_DIAG_POP
#endif

#endif 
