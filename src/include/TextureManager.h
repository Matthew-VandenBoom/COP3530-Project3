#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;
// using namespace sf;

class TextureManager
{
    // static = one and only one of these in memory, ever
    static unordered_map<string, sf::Texture> textures;

public:
    static void LoadTexture(string textureName);
    static sf::Texture &GetTexture(string textureName);
    static void Clear(); // call this once at the end of main()
};
