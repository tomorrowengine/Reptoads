#include "ClientApplication.h"
#include "memory/default_memresource.h"

int main(int argc, char *argv[])
{
	auto  o = ptl::DefaultMemoryResource::get_default_allocator();

	printf("Initializing application...\n");
    ClientApplication app;
    app.Initialize(argc, argv);
    app.Run();

    return 0;
}
