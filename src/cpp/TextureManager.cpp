#include "TextureManager.h"

// "redeclare" the variable, so this file knows it exists and can use it
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string textureName)
{
    string path = "../GUI/images/";
    path += textureName + ".png";

    textures[textureName].loadFromFile(path);
}

sf::Texture &TextureManager::GetTexture(string textureName)
{
    // if the texture doesn't exist...
    // load it first, then return it?
    if ( textures.find(textureName) == textures.end() )
    { // we didn't find it
        LoadTexture(textureName);
    }

    return textures[textureName];
}

void TextureManager::Clear()
{
    textures.clear(); // get rid of all stored objects
}