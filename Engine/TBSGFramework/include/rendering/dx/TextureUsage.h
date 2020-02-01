#pragma once






enum class TextureUsage
{
    Albedo,
    Diffuse = Albedo,       
    Heightmap,
    Depth = Heightmap,      
    Normalmap,
    RenderTarget,           
};
