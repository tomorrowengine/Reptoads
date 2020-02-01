#pragma once
#include <enet/enet.h>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>


class Packet;

namespace Utils{
	    inline std::string EnetHostToIpString(enet_uint32 host)
	    {
		    ENetAddress addr;
		    addr.host = host;
		    std::string ip;

		    enet_address_get_host_ip(&addr, &ip[0], 256);

		    return ip;
	    }

	    inline std::string EnetAddressToString(ENetAddress address)
	    {
	        auto ip = new char[256];
		    enet_address_get_host_ip(&address,ip, 256);
            std::string ipStr {ip};
            delete[] ip;
            ipStr += ":";
            ipStr += std::to_string(address.port);
			return ipStr;
	    }

		inline std::string EnetAddressToIpString(ENetAddress address)
		{
			auto ip = new char[256];
			enet_address_get_host_ip(&address, ip, 256);
			std::string ipStr{ ip };
			delete[] ip;
			return ipStr;
		}





        class Timer {
        private:
            using timepoint = std::chrono::time_point<std::chrono::system_clock>;
        public:
            void Begin() noexcept;
            void End() noexcept;
            void Print();
            void Print(const char* msg);

            const timepoint& GetBegin() const noexcept { return start; };
            const timepoint& GetEnd() const noexcept { return end; };
            const std::chrono::duration<double>& GetElapsedSeconds() const noexcept { return elapsedSeconds; }
            const std::time_t& GetEndTime() const  noexcept { return endTime; }
        private:
            timepoint start;
            timepoint end;
            std::chrono::duration<double> elapsedSeconds = {};
            std::time_t endTime = 0;
        };

        template<typename T, typename ...Ts>
        void ExtractPacket(Packet& packet, T& content, Ts&... contents)
        {
            packet >> content;
            ExtractPacket(packet, contents...);
        }

        template<typename T>
        void ExtractPacket(Packet& packet, T& content)
        {
            packet >> content;
        }
        template<typename T, typename ...Ts>
        void FillPacket(Packet& packet, T& content, Ts&... contents)
        {
            packet << content;
            FillPacket(packet, contents...);
        }

        template<typename T>
        void FillPacket(Packet& packet, T& content)
        {
            packet << content;
        }
}
