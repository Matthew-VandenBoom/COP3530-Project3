#include <SFML/Graphics.hpp>

#include "TextureManager.h"
#include "Random.h"
// include graph header file

using namespace sf;

class WindowUI {
private:
    // images on ui
    Sprite background_left;
    Sprite background_right;
    Sprite title;
    Sprite intro;
    Sprite user1;
    Sprite user1instruct;
    Sprite user2instruct;
    Sprite user2;
    Sprite dice1;
    Sprite dice2;
    Sprite run_algorithm;
    Sprite reset;
    Sprite visualization_header;
    Sprite fibonacci_visual;
    Sprite binary_visual;
    Sprite fibonacci_header;
    Sprite binary_header;
    Sprite output;
    Sprite output_nopath;
    Sprite output_path;

    // text objects
    Font font;
    Text user1ID;
    Text user2ID;

    // user 1 and user 2 IDs
    int randomUser1;
    int randomUser2;

    // insert graph object so functions can be called

public:
    WindowUI() {
        // load font
        font.loadFromFile("fonts/pixelmix.ttf");
        
        // set font characteristics
        user1ID.setFont(font);
        user1ID.setCharacterSize(24);
        user1ID.setFillColor(Color::Black);
        user1ID.setStyle(Text::Regular);

        user2ID.setFont(font);
        user2ID.setCharacterSize(24);
        user2ID.setFillColor(Color::Black);
        user2ID.setStyle(Text::Regular);
        
        // set and load textures
        background_left.setTexture(TextureManager::GetTexture("background_left"));
        background_right.setTexture(TextureManager::GetTexture("background_right"));
        title.setTexture(TextureManager::GetTexture("title"));
        intro.setTexture(TextureManager::GetTexture("intro"));
        user1.setTexture(TextureManager::GetTexture("user_input"));
        user2.setTexture(TextureManager::GetTexture("user_input"));
        user1instruct.setTexture(TextureManager::GetTexture("user1instruct"));
        user2instruct.setTexture(TextureManager::GetTexture("user2instruct"));
        dice1.setTexture(TextureManager::GetTexture("dice"));
        dice2.setTexture(TextureManager::GetTexture("dice"));
        run_algorithm.setTexture(TextureManager::GetTexture("run_algorithm"));
        reset.setTexture(TextureManager::GetTexture("reset"));
        visualization_header.setTexture(TextureManager::GetTexture("visualization_header"));
        fibonacci_header.setTexture(TextureManager::GetTexture("fibonacci_header"));
        binary_header.setTexture(TextureManager::GetTexture("binary_header"));
        fibonacci_visual.setTexture(TextureManager::GetTexture("visualization"));
        binary_visual.setTexture(TextureManager::GetTexture("visualization"));
        output.setTexture(TextureManager::GetTexture("output"));
        output_nopath.setTexture(TextureManager::GetTexture("output_nopath"));
        output_path.setTexture(TextureManager::GetTexture("output_path"));

        // set position
        background_left.setPosition(0, 0);
        background_right.setPosition(437, 0);
        title.setPosition(629, 33);
        intro.setPosition(581, 93);
        user1.setPosition(476, 272);
        user2.setPosition(864, 272);
        user1instruct.setPosition(476, 232);
        user2instruct.setPosition(864, 232);
        dice1.setPosition(734, 292);
        dice2.setPosition(1122, 292);
        run_algorithm.setPosition(602, 386);
        reset.setPosition(836, 386);
        visualization_header.setPosition(0, 0);
        fibonacci_header.setPosition(0, 46);
        binary_header.setPosition(0, 323);
        fibonacci_visual.setPosition(0, 92);
        binary_visual.setPosition(0, 369);
        output.setPosition(602, 484);
        output_nopath.setPosition(602, 484);
        output_path.setPosition(602, 484);

        // set path/no path output to not show
        output_nopath.setColor(Color(255,255,255,0));
        output_path.setColor(Color(255,255,255,0));
    }

    int RandomUser(){
        // set max with graph function - 100 is temp value
        int random = Random::Int(0,100);

        return random;
    }
    
    void Update(RenderWindow &window, Cursor &cursor) {
        // left-click
        if(Mouse::isButtonPressed(Mouse::Left)) {
            Vector2i position = Mouse::getPosition(window);
            // if user 1 dice is left clicked...
            if (position.x >= 734 && position.x <= 774) {
                if (position.y >= 292 && position.y <= 332) {
                    randomUser1 = RandomUser();
                    String randomUser = to_string(randomUser1);

                    user1ID.setString(randomUser);
                    user1ID.setPosition(483, 278);
                }
            }
            // if user 2 dice is left clicked...
            if (position.x >= 1122 && position.x <= 1162) {
                if (position.y >= 292 && position.y <= 332) {
                    randomUser2 = RandomUser();
                    String randomUser = to_string(randomUser2);

                    user2ID.setString(randomUser);
                    user2ID.setPosition(871, 278);
                }
            }
            // if run algorithm is clicked
            if (position.x >= 602 && position.x <= 801) {
                if (position.y >= 386 && position.y < 450) {
                    // call function to create graph
                    // call functions to run algorithms
                        // return if successful to change output text
                    
                    // if successful, make output_path visible
                    // if not successful, make output_nopath visible
                }
            }
            // reset button
            if (position.x >= 836 && position.x <= 1035) {
                if (position.y >= 386 && position.y < 450) {
                    // resets output
                    output_nopath.setColor(Color(255,255,255,0));
                    output_path.setColor(Color(255,255,255,0));

                    // clear input field
                    user1ID.setString("");
                    user2ID.setString("");
                    
                    // will need to reset graph visualization - still not figured out

                    // call function to reset graph
                }
            }
        }
        Draw(window);
    }

    void Draw(RenderWindow &window) {
        // draw go
        window.draw(background_left);
        window.draw(background_right);
        window.draw(title);
        window.draw(intro);
        window.draw(user1);
        window.draw(user2);
        window.draw(user1instruct);
        window.draw(user2instruct);
        window.draw(dice1);
        window.draw(dice2);
        window.draw(run_algorithm);
        window.draw(reset);
        window.draw(visualization_header);
        window.draw(fibonacci_header);
        window.draw(binary_header);
        window.draw(fibonacci_visual);
        window.draw(binary_visual);
        window.draw(output);
        window.draw(output_nopath);
        window.draw(output_path);
        window.draw(user1ID);
        window.draw(user2ID);
    }
};