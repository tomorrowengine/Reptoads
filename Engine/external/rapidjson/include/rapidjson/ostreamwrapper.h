













#ifndef RAPIDJSON_OSTREAMWRAPPER_H_
#define RAPIDJSON_OSTREAMWRAPPER_H_

#include "stream.h"
#include <iosfwd>

#ifdef __clang__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(padded)
#endif

RAPIDJSON_NAMESPACE_BEGIN



   
template <typename StreamType>
class BasicOStreamWrapper {
public:
    typedef typename StreamType::char_type Ch;
    BasicOStreamWrapper(StreamType& stream) : stream_(stream) {}

    void Put(Ch c) {
        stream_.put(c);
    }

    void Flush() {
        stream_.flush();
    }

    
    char Peek() const { RAPIDJSON_ASSERT(false); return 0; }
    char Take() { RAPIDJSON_ASSERT(false); return 0; }
    size_t Tell() const { RAPIDJSON_ASSERT(false); return 0; }
    char* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
    size_t PutEnd(char*) { RAPIDJSON_ASSERT(false); return 0; }

private:
    BasicOStreamWrapper(const BasicOStreamWrapper&);
    BasicOStreamWrapper& operator=(const BasicOStreamWrapper&);

    StreamType& stream_;
};

typedef BasicOStreamWrapper<std::ostream> OStreamWrapper;
typedef BasicOStreamWrapper<std::wostream> WOStreamWrapper;

#ifdef __clang__
RAPIDJSON_DIAG_POP
#endif

RAPIDJSON_NAMESPACE_END

#endif 
