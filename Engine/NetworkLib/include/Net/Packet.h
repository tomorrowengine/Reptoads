#pragma once

#include <string>
#include <vector>
#include <cstring>

#include "memory/allocator_impl.h"
#include "memory/string.h"

using str = ptl::string;
using wstr = ptl::wstring;

namespace net
{
	class Server;
	class Client;
}

class Packet
{
public:
	Packet();
    Packet(const Packet& a_Other) = default;
    Packet(Packet&& a_Other) noexcept = default;
    Packet& operator=(const Packet& a_Other) = default;
    
    virtual ~Packet();

	typedef signed   char Int8;
	typedef unsigned char Uint8;

	
	typedef signed   short Int16;
	typedef unsigned short Uint16;

	
	typedef signed   int Int32;
	typedef unsigned int Uint32;

	
#if defined(_MSC_VER)
	typedef signed   __int64 Int64;
	typedef unsigned __int64 Uint64;
#else
	typedef signed	 long Int64;
	typedef unsigned long Uint64;
#endif

    using is_packet_class = std::true_type;

	friend class net::Server;
	friend class net::Client;

	
	
	
	
	
	
	
	
	
	void Append(const void* data, std::size_t sizeInBytes);

	
	
	
	
	
	
	
	
	void Clear();

	
	
	
	
	
	
	
	
	
	
	
	
	
	const void* GetData() const;

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	const void* GetData(std::size_t offset) const;

    template<typename T>
    void Out(T& data);
    template<typename T>
    void Out(T& data, std::size_t offset);

	
	
	
	
	
	
	
	
	
	
	
	std::size_t GetDataSize() const;

	
	
	
	
	
	
	
	
	
	
	
	
	bool EndOfPacket() const;
    void Reset();
public:

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	operator bool() const;

	
	
	
	
	Packet& operator >>(bool&         data);

	
	
	
	Packet& operator >>(Int8&         data);

	
	
	
	Packet& operator >>(Uint8&        data);

	
	
	
	Packet& operator >>(Int16&        data);

	
	
	
	Packet& operator >>(Uint16&       data);

	
	
	
	Packet& operator >>(Int32&        data);

	
	
	
	Packet& operator >>(Uint32&       data);

	
	
	
	Packet& operator >>(Int64&        data);

	
	
	
	Packet& operator >>(Uint64&       data);

	
	
	
	Packet& operator >>(float&        data);

	
	
	
	Packet& operator >>(double&       data);

	
	
	
	Packet& operator >>(char*         data);

	
	
	
	Packet& operator >>(str&  data);

	
	
	
	Packet& operator >>(wchar_t*      data);

	
	
	
	Packet& operator >>(wstr& data);

	
	
	
	
	Packet& operator <<(bool                data);

	
	
	
	Packet& operator <<(Int8                data);

	
	
	
	Packet& operator <<(Uint8               data);

	
	
	
	Packet& operator <<(Int16               data);

	
	
	
	Packet& operator <<(Uint16              data);

	
	
	
	Packet& operator <<(Int32               data);

	
	
	
	Packet& operator <<(Uint32              data);

	
	
	
	Packet& operator <<(Int64               data);

	
	
	
	Packet& operator <<(Uint64              data);

	
	
	
	Packet& operator <<(float               data);

	
	
	
	Packet& operator <<(double              data);

	
	
	
	Packet& operator <<(const char*         data);

	
	
	
	Packet& operator <<(const str&  data);

	
	
	
	Packet& operator <<(const wchar_t*      data);

	
	
	
	Packet& operator <<(const wstr& data);



protected:

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	virtual const void* OnSend(std::size_t& size);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	virtual void OnReceive(const void* data, std::size_t size);

private:

	
	
	
	
	bool operator ==(const Packet& right) const;
	bool operator !=(const Packet& right) const;

	
	
	
	
	
	
	
	
	
	
	bool CheckSize(std::size_t size) ;

	
	
	
	std::vector<char> m_data;    
	std::size_t       m_readPos; 
	std::size_t       m_sendPos; 
	bool              m_isValid; 

};

template <typename T>
void Packet::Out(T& data)
{
    std::memcpy(&data, &m_data[0], sizeof(T));
}

template <typename T>
void Packet::Out(T& data, std::size_t offset)
{
    std::memcpy(&data, &m_data[offset], sizeof(T));
}
