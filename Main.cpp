#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using namespace sf;


float offsetX = 0, offsetY = 32;

const int H = 25;
const int W = 80;



String TileMap[H] = {
    "B                                        BBB           B                       B",
    "B                                                      B                       B",
    "B                                                      B                       B",
    "Bs                                            B        B                       B",
    "BB                 f   f   f            f   f   f      B                       B",
    "B                 s                                    B                       B",
    "B      B          B                              B     x                       B",
    "B                                                      x                       B",
    "B             s                ss                      B                       B",
    "BB            B                BB                  B   B  B    f   f   f       B",
    "B                                                      B                       B",
    "B                                                     BB                       B",
    "B     B                                                B            sssss      B",
    "B                  f   f   f            f   f   f     kB           BBBBBBB     B",
    "B                                                    BBB                       B",
    "BB                                                     B                       B",
    "B 1                 l      l      l      l      l      B       f   f   f       B",
    "Be                      1                             BB                      wB",
    "BBBB          B                                        B           1     B    BB",
    "B                             1             1     B    BB                      B",
    "B                                                      B              1        B",
    "B        BBB          BBBB       B                    BB    B          B       B",
    "B       1                                              D               BBBBBBBBB",
    "B                    1                         1 B     d            BBBBBBBBBBBB",
    "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL",
};


class Player {

public:
    float dx, dy;
    FloatRect rect;
    bool onGround;
    Sprite sprite;
    float currentFrame;
    bool key = false;
    int gold = 0;
    float stunCd = 0;

    Player(Texture &image)
    {
        sprite.setTexture(image);
        rect = FloatRect(32 * 1, 32 * 16, 40, 60);
        dx = dy = 0;
        currentFrame = 0;
    }

    void update(float time)
    {
        rect.left += dx * time;

        Collusion(0);


        if (!onGround) dy = dy + 10 * time;
           
      
        rect.top += dy * time;
        onGround = false;
        Collusion(1);
        
        currentFrame += 0.75 * time;
        if (currentFrame > 8) currentFrame -= 8;

        if (dy != 0) 
        {
            if (dx > 0) sprite.setTextureRect(IntRect(40 * int(currentFrame), 240, 40, 60));
            if (dx < 0) sprite.setTextureRect(IntRect(40 * int(currentFrame) + 40, 240, -40, 60));
        }          
        else 
        {
            if (dx > 0) sprite.setTextureRect(IntRect(40 * int(currentFrame), 0, 40, 60));
            if (dx < 0) sprite.setTextureRect(IntRect(40 * int(currentFrame) + 40, 0, -40, 60));
            if (dx == 0) sprite.setTextureRect(IntRect(40 * int(currentFrame), 60, 40, 60));
        }
        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

        dx = 0;

        stunCd += time;
    }

    void Collusion(int dir)
    {
        for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
            for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++)
            {
                if (0 <= i && 0 <= j)
                {
                    if (TileMap[i][j] == 'B' || TileMap[i][j] == 'l' || TileMap[i][j] == 'L')
                    {
                        if ((dx > 0) && (dir == 0)) rect.left = j * 32 - rect.width;
                        if ((dx < 0) && (dir == 0)) rect.left = j * 32 + 32;
                        if ((dy > 0) && (dir == 1)) { rect.top = i * 32 - rect.height; dy = 0; onGround = true; }
                        if ((dy < 0) && (dir == 1)) { rect.top = i * 32 + 32; dy = 0; }
                    }
                    if (TileMap[i][j] == 'k')
                    {
                        TileMap[i][j] = ']';
                        key = true;
                    }
                    if (TileMap[i][j] == 'd' && key)
                    {
                        TileMap[i][j] = 'o';
                    }
                    if (TileMap[i][j] == 'D' && key)
                    {
                        TileMap[i][j] = 'O';
                    }
                    if (TileMap[i][j] == 'd' && key == false)
                    {
                        if ((dx > 0) && (dir == 0)) rect.left = j * 32 - rect.width;
                        if ((dx < 0) && (dir == 0)) rect.left = j * 32 + 32;
                    }
                    if (TileMap[i][j] == 's')
                    {
                        TileMap[i][j] = '-';
                        gold += 100;
                    }
                    if (TileMap[i][j] == 'x')
                    {
                        TileMap[i][j] = '[';
                    }

                }
            }
    }


    
};

class Enemy {

public:
    float dx, dy;
    FloatRect rect;
    bool onGround;
    Sprite sprite;
    float currentFrame;
    bool life;
    float timer=0;
    int seed = 0;
    int speed = 10;

    void set(Texture& image, int x, int y, int seed_i)
    {
        sprite.setTexture(image);
        rect = FloatRect(x, y, 40, 60);
        currentFrame = 0;
        life = true;
        seed = seed_i;
        dx = speed;
    }

    void update(float time)
    {
        rect.left += dx * time;

        Collusion();       
        currentFrame += 0.75 * time;
        if (currentFrame > 8) currentFrame -= 8;


        if (dx > 0) sprite.setTextureRect(IntRect(40 * int(currentFrame), 120, 40, 60));
        if (dx < 0) sprite.setTextureRect(IntRect(40 * int(currentFrame) + 40, 120, -40, 60));
        if (dx == 0) sprite.setTextureRect(IntRect(40 * int(currentFrame), 180, 40, 60));

        timer += 0.075 * time;   
            if ((int(timer) + seed) % 8 == 0) dx = speed;
            if ((int(timer) + seed) % 8 == 4) dx = -speed;
        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
    }

    void Collusion()
    {
        for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
            for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++)
            {
                if (0 <= i && 0 <= j)
                {
                    if (TileMap[i][j] == 'B' || TileMap[i][j] == 'd')
                    {
                        if (dx > 0)
                        {
                            rect.left = j * 32 - rect.width; dx *= -1;
                        }
                        else if (dx < 0) 
                        { 
                            rect.left = j * 32 + 32; dx *= -1;
                        }
                    }
                }
            }
    }



};




int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Castle Thieve");

    Font font; 
    font.loadFromFile("Res/font2.otf"); 

    Text text("Game over\nR-Restart\nQ-Quit", font);
    text.setCharacterSize(100);
    text.setFillColor(Color::Black);
    text.setCharacterSize(100);
    text.setFillColor(Color::Black);
    text.setPosition(270, 250);

    FloatRect endrect(2515,500,32,32);

    bool running = true;
   
    Texture t;
    t.loadFromFile("Textures/Tileset.png");

    Sprite tile(t);
    float currentFrame = 0;

    bool win = false;
    Player p(t);
    
    Enemy enemy, enemy2, enemy3, enemy4, enemy5, enemy6;
    enemy.set(t, 32 * 10, 32* 22 + 4 , 1);
    enemy2.set(t, 32 * 55, 32 * 22 + 4, 2);
    enemy3.set(t, 32 * 75, 32 * 22 + 4, 3);
    enemy4.set(t, 32 * 30, 32 * 22 + 4, 4);
    enemy5.set(t, 32 * 45, 32 * 22 + 4, 5);
    enemy6.set(t, 32 * 75, 32 * 20 + 4, 6);

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        time /= 100000;
        clock.restart();      
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (running)
        {
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                p.dx = -20;
            }

            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                p.dx = 20;
            }

            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                if (p.onGround) { p.dy = -50; p.onGround = false; }
            }
        }
        else
        {
            if (Keyboard::isKeyPressed(Keyboard::R))
            {
                p.rect.left = 32 * 1;
                p.rect.top = 32 * 16;
                offsetX = -25;
                p.sprite.setPosition(p.rect.left - offsetX, p.rect.top - offsetY);
                running = true;
                win = false;
                p.gold = 0;
                p.key = false;
                for (int i = 0; i < H; i++)
                    for (int j = 0; j < W; j++)
                    {
                        if (TileMap[i][j] == '-') TileMap[i][j] = 's';
                        if (TileMap[i][j] == 'o') TileMap[i][j] = 'd';
                        if (TileMap[i][j] == 'O') TileMap[i][j] = 'D';
                        if (TileMap[i][j] == ']') TileMap[i][j] = 'k';
                        if (TileMap[i][j] == '[') TileMap[i][j] = 'x';
                       
                    }
            }
            if (Keyboard::isKeyPressed(Keyboard::Q))
            {
                window.close();
            }
        }
        p.update(time);
        enemy.update(time);
        enemy2.update(time);
        enemy3.update(time);
        enemy4.update(time);
        enemy5.update(time);
        enemy6.update(time);

        if (p.rect.intersects(endrect))
        {
            win = true;
            running = false;
        }

        if (p.rect.intersects(enemy.rect) || p.rect.intersects(enemy2.rect) || p.rect.intersects(enemy3.rect)
            || p.rect.intersects(enemy4.rect) || p.rect.intersects(enemy5.rect) || p.rect.intersects(enemy6.rect))
        {
            if (p.dy != 0 )
            {
                p.dy = -0.5* p.stunCd;
                p.stunCd -= p.stunCd/2 + 2;
            }
            else
            {
                running = false;
            }
        }

    

        if (p.rect.left > 1836) {}
        else if (p.rect.left > 300) offsetX = p.rect.left - 600/2;
        

        window.clear();

  
        for (int i = 0; i < H; i++)       
            for (int j = 0; j < W; j++)
            {
                if (TileMap[i][j] == ' ' || TileMap[i][j] == '-' || TileMap[i][j] == ']' || TileMap[i][j] == '[') tile.setTextureRect(IntRect(968, 968, 32, 32));

                if (TileMap[i][j] == 'B' || TileMap[i][j] == 'x') tile.setTextureRect(IntRect(936, 968, 32, 32));

                if (TileMap[i][j] == 'k') tile.setTextureRect(IntRect(904, 968, 32, 32));

                if (TileMap[i][j] == 'd') tile.setTextureRect(IntRect(872, 968, 32, 32));

                if (TileMap[i][j] == 'D') tile.setTextureRect(IntRect(872, 936, 32, 32));

                if (TileMap[i][j] == 'o') tile.setTextureRect(IntRect(840, 968, 32, 32));

                if (TileMap[i][j] == 'O') tile.setTextureRect(IntRect(840, 936, 32, 32));

                if (TileMap[i][j] == 'q') tile.setTextureRect(IntRect(808, 968, 32, 32));

                if (TileMap[i][j] == 't') tile.setTextureRect(IntRect(776, 968, 32, 32));

                if (TileMap[i][j] == 'l') tile.setTextureRect(IntRect(744, 968, 32, 32));

                if (TileMap[i][j] == 'w')
                {
                    tile.setTextureRect(IntRect(712, 968, 32, 32));
             
                }

                if (TileMap[i][j] == 'W') tile.setTextureRect(IntRect(712, 936, 32, 32));

                if (TileMap[i][j] == 'f') tile.setTextureRect(IntRect(712 - 32, 968, 32, 32));

                if (TileMap[i][j] == 'L') tile.setTextureRect(IntRect(712 - 32 * 2, 968, 32, 32));
                     
                if (TileMap[i][j] == '1') tile.setTextureRect(IntRect(712 - 32 * 3, 968, 32, 32));

                if (TileMap[i][j] == 'e') tile.setTextureRect(IntRect(712 - 32 * 4, 968, 32, 32));

                if (TileMap[i][j] == 's') tile.setTextureRect(IntRect(712 - 32 * 5, 968, 32, 32));

                tile.setPosition(j * 32 - offsetX, i * 32 - offsetY);
                window.draw(tile);
            }
        


      
            window.draw(p.sprite);
            window.draw(enemy.sprite);
            window.draw(enemy2.sprite);
            window.draw(enemy3.sprite);
            window.draw(enemy4.sprite);
            window.draw(enemy5.sprite);
            window.draw(enemy6.sprite);
        
       if (running == false)
        {
           if (win) 
           {
               String str = "Level complete\nGold stolen: " + std::to_string(p.gold) + "/1000\nR-Restart\nQ-Quit";
               Text textwin(str, font);
               textwin.setPosition(270, 250);
               textwin.setCharacterSize(100);
               textwin.setFillColor(Color::Black);
               window.draw(textwin);
           }
           else window.draw(text);
        }
        
        window.display();
    }

    return 0;
}