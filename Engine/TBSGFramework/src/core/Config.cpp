#include  "core/Config.h"

tbsg::Config& tbsg::Config::Get()
{
    static Config instance;
    return instance;
}
