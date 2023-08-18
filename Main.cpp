#include <SFML/Graphics.hpp>
#include "Board.h"
#include <Windows.h>
#include <string>
#include <sstream>

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    rand();
    int w = 50;
    
    sf::RenderWindow window(sf::VideoMode(450, 600), L"Странствия в бездне");
    sf::RectangleShape square(sf::Vector2f(50.f, 50.f));

    sf::Texture pl;
    pl.loadFromFile("\\MyGame\\игрок.png");
    sf::Sprite player(pl);

    sf::Texture mon;
    mon.loadFromFile("\\MyGame\\монстр.png");
    sf::Sprite monster(mon);

    sf::Texture port;
    port.loadFromFile("\\MyGame\\портал.png");
    sf::Sprite portal(port);

    sf::Texture sp;
    sp.loadFromFile("\\MyGame\\копьё.png");
    sf::Sprite spear(sp);
    sf::Texture sp2;
    sp2.loadFromFile("\\MyGame\\копьё2.png");//для диагонали
    sf::Sprite spear2(sp2);

    Board board(9, 12);

    while (window.isOpen())
    {
        sf::Event event;
        // Получаем координаты курсора мышки относительно окна нашего приложения
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x = pos.x / w;
        int y = pos.y / w;
        bool mot = false;
        while (window.pollEvent(event))
        {
                if (event.type == sf::Event::Closed)
                    window.close();
       
                if (event.type == sf::Event::KeyPressed)
                    // Эта кнопка – стрелка вверх?
                    if (event.key.code == sf::Keyboard::W)
                    {
                        mot = board.motion(board.getPlayer().row, board.getPlayer().col, Board::Direction::Up, true);
                    }
                    else if (event.key.code == sf::Keyboard::S)
                    {
                        mot = board.motion(board.getPlayer().row, board.getPlayer().col, Board::Direction::Down, true);
                    }
                    else if (event.key.code == sf::Keyboard::A)
                    {
                        mot = board.motion(board.getPlayer().row, board.getPlayer().col, Board::Direction::Left, true);
                    }
                    else if (event.key.code == sf::Keyboard::D)
                    {
                        mot = board.motion(board.getPlayer().row, board.getPlayer().col, Board::Direction::Right, true);
                    }
                    else if (event.key.code == sf::Keyboard::R)
                    {
                        board.SetupBoard();
                    }
                // Определяем, была ли нажата кнопка мыши?
                if (event.type == sf::Event::MouseButtonPressed)

                    // Если была нажата левая кнопка мыши, то открываем клетку
                    if (event.key.code == sf::Mouse::Left && board.getSpear() && board.getBoard(y,x) == 4 && abs(y - board.getPlayer().row)<=2 && abs(x - board.getPlayer().col) <= 2)
                    {
                        board.attackSpear(y, x);
                        mot = true;
                    }
           
        }
        if (board.getPlayer().row != board.getRows())//жив ли игрок
        {
            if (mot)
            {
                board.moveMonster();
            }
        }
        else
        {
            sf::Font font;
            font.loadFromFile("c:\\Windows\\Fonts\\arial.ttf");
            sf::Text text;

            text.setFont(font);
           
            std::string str;
            std::ostringstream ss;
            ss << board.getMonster()-5;
            str = ss.str();
            text.setString(str);

          
            text.setCharacterSize(40);

            
            text.setFillColor(sf::Color::Red);

           
            text.setStyle(sf::Text::Bold | sf::Text::Underlined);
            text.setPosition(200, 250);

            window.clear(sf::Color(0,0,0));
            window.draw(text);
            window.display();
            while (window.isOpen())
            {
                sf::Event event;
                while (window.pollEvent(event))
                    if (event.type == sf::Event::Closed)
                        window.close();
                Sleep(150);
            }
           // window.close();
        }
        window.clear(sf::Color::White);
        for(int row = 0; row < board.getRows();row++)
            for(int col = 0; col < board.getColumns();col++)
                if (board.getBoard(row, col) == 1)
                {
                    square.setFillColor(sf::Color::Red);
                    square.setPosition(col * w, row * w);
                    window.draw(square);
                }
                else if (board.getBoard(row, col) == 0)
                {
                    square.setFillColor(sf::Color::Black);
                    square.setPosition(col * w, row * w);
                    window.draw(square);
                }
                else if (board.getBoard(row, col) == 2)
                {
                   
                    player.setPosition(col * w, row * w);
                    window.draw(player);
                }
                else if (board.getBoard(row, col) == 3)
                {
                  
                    portal.setPosition(col * w, row * w);
                    window.draw(portal);
                }
                else if (board.getBoard(row, col) == 4)
                {
                    
                    monster.setPosition(col * w, row * w);
                    window.draw(monster);
                }
                else if (board.getBoard(row, col) == 5)
                {
                    if (board.getSlant() == Board::Slant::SUp)
                    {
                        spear.setRotation(0.f);
                        spear.setPosition(col* w, row* w);
                        window.draw(spear);
                    }
                    else if (board.getSlant() == Board::Slant::SDown)
                    {
                        spear.setRotation(180.f);
                        spear.setPosition(col* w+w, row* w + w);
                        window.draw(spear);
                    }
                    else if (board.getSlant() == Board::Slant::SLeft)
                    {
                        spear.setRotation(270.f);
                        spear.setPosition(col* w, row* w+w);
                        window.draw(spear);
                    }
                    else if (board.getSlant() == Board::Slant::SRight)
                    {
                        spear.setRotation(90.f);
                        spear.setPosition(col* w+w, row* w);
                        window.draw(spear);
                    }
                    else if (board.getSlant() == Board::Slant::LeftUp)
                    {
                        spear2.setRotation(0.f);
                        spear2.setPosition(col * w, row*w);
                        window.draw(spear2);
                    }
                    else if (board.getSlant() == Board::Slant::RightUp)
                    {
                        spear2.setRotation(90.f);
                        spear2.setPosition(col * w+w, row*w);
                        window.draw(spear2);
                    }
                    else if (board.getSlant() == Board::Slant::LeftDown)
                    {
                        spear2.setRotation(270.f);
                        spear2.setPosition(col * w, row*w+w);
                        window.draw(spear2);
                    }
                    else if (board.getSlant() == Board::Slant::RightDown)
                    {
                        spear2.setRotation(180.f);
                        spear2.setPosition(col * w+w, row*w+w);
                        window.draw(spear2);
                    }
                }

        window.display();
        Sleep(150);
    }

    return 0;
}