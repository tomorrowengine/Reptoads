#include "IApplication.h"
#include <Net/Client.h>
#include <core/Input.h>
#include "resource_loading/ConfigLoader.h"
#include "resource_loading/ArgumentParsing.h"

#if defined(_WIN32)
#include <rendering/dx/DX12Renderer.h>
#include <rendering/dx/DX12Application.h>
#endif


namespace tbsg {

	
	bool IApplication::m_ShouldStop = false;

	IApplication::~IApplication()
	{
		
		
		delete m_input;
	}

	void IApplication::Initialize(int argc,char *argv[])
	{
		
#if defined(PLATFORM_WINDOWS)
		LoadConfig("./config.json");
		tbsg::ParsedArguments arguments;
		ArgumentParsing(tbsg::Config::Get(), argc, argv, arguments);
		HINSTANCE hInstance = GetModuleHandle(NULL);
		m_renderer = new gfx::DX12Renderer(hInstance, true);
#endif
		
		m_renderer->Initialize();
		m_input = new Input(tbsg::Config::Get().width, tbsg::Config::Get().height);
	}

	void IApplication::Run()
	{
	}

	void IApplication::Shutdown()
	{
		m_renderer->Shutdown();
		delete m_renderer; 
	    m_renderer = nullptr;
	}
}
