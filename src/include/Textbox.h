#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
// using namespace sf;

class Textbox
{
    sf::Font font;
    sf::String input;
    sf::Text text;

public:
    Textbox(int x, int y)
    {
        font.loadFromFile("../GUI/fonts/pixelmix.ttf");

        text.setFont(font);
        text.setPosition(x, y);
    }

    sf::Text InputText(sf::Event &event)
    {
        if ( event.text.unicode < 128 )
        {
            input += event.text.unicode;
            text.setString(input);
        }

        return text;
    }
};