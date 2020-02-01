#pragma once
#include "core/Config.h"
namespace gfx
{
    class IRenderer;
}
class Client;
namespace tbsg {
    class Input;

    class IApplication
    {
    public:
        virtual ~IApplication();
        virtual void Initialize(int argc,char *argv[]);
		virtual void Run();

    public:
		
		
		static bool m_ShouldStop;

    protected:
		virtual void Shutdown();

    protected:
        gfx::IRenderer* m_renderer{ nullptr };
        Input* m_input{ nullptr };
    };
}
