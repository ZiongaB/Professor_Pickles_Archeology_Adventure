/**
 * This file contains most of the code needed to run the gameplay loop
 * Contributions:
 * Zach: Main.cpp, player class functions
 * Nick: Folder/file structure, headers, splitting classes into cpp files, makefile, enemy class functions
 * Alex: Asset files
*/
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
#include <string> // for std::string
#include <iostream>
#include <math.h>

#include "Player.h"
#include "Enemy.h"
#include "Mummy.h"
#include "Lion.h"
#include "Item.h"
#include "Door.h"

class Bullet{
    public:
        sf::CircleShape shape;
        sf::Vector2f currVelocity;
        float maxSpeed;

        Bullet(float radius = 5.f)
            :currVelocity(0.f, 0.f), maxSpeed(100.f)
        {
            this->shape.setRadius(radius);
            this->shape.setFillColor(sf::Color::Black);
        }

};


//Runs most of the processes that control the gameplay loop
int main() {

    //Window
    sf::Vector2f viewSize(1000, 800);
    sf::VideoMode vm(viewSize.x, viewSize.y);
    sf::RenderWindow window(vm, "Professor Pickle's Archeology Adventure", sf::Style::Default);
    window.setFramerateLimit(60);

    //Textures
    sf::Texture background;
    sf::Texture enemy1;
    sf::Texture enemy2;
    sf::Texture player;

    //Music
    sf::Music music;

    //Set up score variables
    float score = 0;
    int singleScore = 0;

    //Set up time variables
    float seconds = 0;
    int timestamp =0;

    //Set up life counter
    int life = 5;

    //Set up lion enemy movement direction
    int direction1 = 0;

    //Aiming Vectors
    sf::Vector2f playerPoint;
    sf::Vector2f playerUp;
    sf::Vector2f playerDown;
    sf::Vector2f playerLeft;
    sf::Vector2f playerRight;
    sf::Vector2f aimDir;
    sf::Vector2f aimDirNorm;

    int currentFireDir;

    //Set up timer for time between hits
    float prevHitTime = 0;
    float shootTime =0;

    sf::Time dt;

    //integer to decide level
    int level = 1;

    //Load Player Sprite
    player.loadFromFile("./assets/sprites/player.png");
    sf::Sprite play(player);
    play.setScale(0.25f,0.25f);


    //Load Enemy Sprites
    enemy1.loadFromFile("./assets/sprites/lion.png");
    sf::Sprite enem1(enemy1);
    enem1.setScale(0.5f,0.5f);

    enemy2.loadFromFile("./assets/sprites/mummy.png");
    sf::Sprite enem2(enemy2);
    enem2.setScale(0.25f,0.25f);
   

    //Load Background
    background.loadFromFile("./assets/backgrounds/sand.jpg");
    sf::Sprite back(background);
    back.setScale(1.f,1.f);

    //Load Font
    sf::Font scoreFont;
    if (!scoreFont.loadFromFile("./assets/sprites/Hylia.otf"))
    {
        std::cout<<"ERROR"<<std::endl;
    }

    //Create player
    Player hero(100,700,150,3,play);

    //Bullets
    Bullet b1;
    std::vector<Bullet> player_bullets;
    player_bullets.push_back(Bullet(b1));

    //Create lion
    Lion lion(500, 300, 400, 5, enem1 , 0);


    //Set up visible score text
    sf::Text scoreText;
    scoreText.setFont(scoreFont);
    scoreText.setCharacterSize(20);
    scoreText.setPosition(0, 0);
    scoreText.setFillColor(sf::Color::Red);

    //Description Text
    /*sf::Text interaction;
    scoreText.setFont(scoreFont);
    scoreText.setCharacterSize(20);
    scoreText.setPosition(700, 0);
    scoreText.setFillColor(sf::Color::Yellow);*/

    /*//List of items on 
    sf::Text list;
    scoreText.setFont(scoreFont);
    scoreText.setCharacterSize(20);
    scoreText.setPosition(400, 0);
    scoreText.setFillColor(sf::Color::Yellow);*/

    //Set up visible timer text
    sf::Text timer;
    timer.setFont(scoreFont); 
    timer.setCharacterSize(20);
    timer.setPosition(0, 20);
    timer.setFillColor(sf::Color::Blue);

    //Set up visible life text
    sf::Text lives;
    lives.setFont(scoreFont); 
    lives.setCharacterSize(20);
    lives.setPosition(0, 40);
    lives.setFillColor(sf::Color::Green);

    //Set up random function
    srand(time(NULL));

    //Load music file
    if( !music.openFromFile("./assets/sound/Electro.flac")){
        std::cout<<"ERROR"<<std::endl;
    }

    //Play music
    music.play();

    //Set up running clock for calculating time
    sf::Clock clock;

    bool paused = false;

    //While the game is running
	while (window.isOpen()) {

            //Update timer variables
            seconds += dt.asSeconds();
            timestamp = static_cast<int>(seconds);

            //Update score variables
            score = score + (2 * dt.asSeconds());
            singleScore = static_cast<int>(score);

            //Update time since player last hit
            prevHitTime = prevHitTime + dt.asSeconds();
            //.Update time since player shot
            shootTime = shootTime + dt.asSeconds();


            //set up event
            sf::Event event;
            while (window.pollEvent(event))
            {
                //If the event is close, then close the game
                if (event.type == sf::Event::Closed) 
                {
                    window.close();
                }
            }

            //Pause if P is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) 
            {
                paused = !paused;
            }

            //Set aim directions
            playerPoint = sf::Vector2f(hero.x, hero.y);
            playerUp = sf::Vector2f(hero.x, hero.y -0.1);
            playerDown = sf::Vector2f(hero.x, hero.y +0.1);
            playerLeft = sf::Vector2f(hero.x -0.1, hero.y);
            playerRight = sf::Vector2f(hero.x +0.1, hero.y);
        
            //If A key is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
            {
                //Move left
                hero.x -= dt.asSeconds()*hero.speed;
                // keep hero within left boundary
                if (hero.x < 0) 
                {
                    hero.x = 0;
                }
            }
            //If D key is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                //Move right
                hero.x += dt.asSeconds()*hero.speed;
                // keep hero within right boundary
                if (hero.x > viewSize.x - hero.playerSprite.getGlobalBounds().width) 
                {
                    hero.x = viewSize.x - hero.playerSprite.getGlobalBounds().width;
                }
            }
            //If W key is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                //Move up
                hero.y -= dt.asSeconds()*hero.speed;
                // keep hero within top boundary
                if (hero.y < 0)
                {
                    hero.y = 0;
                }
            }
            //If S key is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
            {
                //Move down
                hero.y += dt.asSeconds()*hero.speed;
                // keep hero within bottom boundary
                if (hero.y > viewSize.y - hero.playerSprite.getGlobalBounds().height) 
                {
                    hero.y = viewSize.y - hero.playerSprite.getGlobalBounds().height;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) )
            {
                aimDir = playerUp -playerPoint;

                if(shootTime >= 0.3)
                {
                    b1.shape.setPosition(playerPoint.x+25,playerPoint.y);
                    b1.currVelocity = aimDir *b1.maxSpeed;
                    player_bullets.push_back(Bullet(b1));
                    shootTime = 0;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) )
            {
                aimDir = playerDown - playerPoint;

                if(shootTime >= 0.3)
                {
                    b1.shape.setPosition(playerPoint.x+25,playerPoint.y+50);
                    b1.currVelocity = aimDir *b1.maxSpeed;
                    player_bullets.push_back(Bullet(b1));
                    shootTime = 0;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) )
            {
                aimDir = playerLeft - playerPoint;

                if(shootTime >= 0.3)
                {
                    b1.shape.setPosition(playerPoint.x,playerPoint.y+25);
                    b1.currVelocity = aimDir *b1.maxSpeed;
                    player_bullets.push_back(Bullet(b1));
                    shootTime = 0;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) )
            {
                aimDir = playerRight - playerPoint;

                if(shootTime >= 0.3)
                {
                    b1.shape.setPosition(playerPoint.x+50,playerPoint.y+25);
                    b1.currVelocity = aimDir *b1.maxSpeed;
                    player_bullets.push_back(Bullet(b1));
                    shootTime = 0;
                }
            }

            //For all items in bullet list
            for (size_t i = 0; i < player_bullets.size(); i++)
            {
                //Move at specific speed
                player_bullets[i].shape.move(player_bullets[i].currVelocity);
                //If they are out of bounds
                if(player_bullets[i].shape.getPosition().x <0 || player_bullets[i].shape.getPosition().x > window.getSize().x ||
                player_bullets[i].shape.getPosition().x <0 || player_bullets[i].shape.getPosition().y > window.getSize().y)
                {
                    //Erase them
                    player_bullets.erase(player_bullets.begin()+ i);
                }
                else
                {
                    if (player_bullets[i].shape.getGlobalBounds().intersects(lion.enemSprite.getGlobalBounds()))
                    {
                        lion.lowerSpeed();
                    }
                }
            }

            //Move based on direction
            switch (direction1)
            {
                case (0):
                    //Move Left
                    lion.x -= dt.asSeconds()*lion.speed;

                    //Keep npc inside boundary
                    if (lion.x < 0) 
                    {
                        lion.x = 0;
                        direction1 = 1;
                    }
                    break;
                case (1):
                    //Move Right 
                    lion.x += dt.asSeconds()*lion.speed;
                    //Keep npc inside boundary
                    if (lion.x > viewSize.x - lion.enemSprite.getGlobalBounds().width) 
                    {
                        lion.x = viewSize.x - lion.enemSprite.getGlobalBounds().width;
                        direction1 = 0;
                    }
                    break;
                default:
                    break;
            }

            //Update sprite positions
            hero.setSprite();
            lion.setSprite();

            //Check if player and npc1 are touching
            if (hero.playerSprite.getGlobalBounds().intersects(lion.enemSprite.getGlobalBounds())) 
            {
                //If it has been 2 seconds since last hit
                if(prevHitTime > 2)
                {
                    //Hit the player to lose life and score
                    hero.loseHealth(lion.damage);
                    std::cout << "lion ate a life!!!" << std::endl;
                    score = score-10;
                    //reset last hit timer
                    prevHitTime=0;
                }
           
                //If player is out of health
                if (hero.health <= 0) 
                {
                    //End game and print stats
                    std::cout << "Game over! You lost." << std::endl;
                    std::cout << "Score: " << singleScore << std::endl;
                    std::cout << "Seconds Lasted: " << timestamp << std::endl;

                    window.close();
                }
            }

            //Draw all the sprites
            window.clear();
		    window.draw(back);
            window.draw(hero.playerSprite);
            for (size_t i = 0; i < player_bullets.size(); i++)
            {
                window.draw(player_bullets[i].shape);
            }
            window.draw(lion.enemSprite);

            //Update all the text and display
            scoreText.setString("Score: " + std::to_string(singleScore));
		    window.draw(scoreText);
            timer.setString("Time: " + std::to_string(timestamp));
		    window.draw(timer);
            lives.setString("Health: " + std::to_string(hero.health));
            window.draw(lives);

            //update clock time
            dt = clock.restart();
        
            //Display window
		    window.display();

	    }

	return 0;
}