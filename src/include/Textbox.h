#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

class Textbox {
    Font font;
    String input;
    Text text;
public:
    Textbox(int x, int y) {
        font.loadFromFile("fonts/pixelmix.ttf");

        text.setFont(font);
        text.setPosition(x, y);
    }

    Text InputText(Event &event) {
        if (event.text.unicode < 128) {
            input += event.text.unicode;
            text.setString(input);
        }
        
        return text;
    }
};