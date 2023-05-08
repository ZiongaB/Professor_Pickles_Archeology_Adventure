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
#include "Mummy.h"
#include "Lion.h"
#include "Item.h"
#include "Door.h"


//PLAYER BULLET CLASS
class Bullet{
    public:
        sf::CircleShape shape;
        sf::Vector2f currVelocity;
        float maxSpeed;

        Bullet(float radius = 5.f)
            :currVelocity(0.f, 0.f), maxSpeed(100.f)
        {
            this->shape.setRadius(radius);
            this->shape.setFillColor(sf::Color::White);
        }
};

//FIREBALL CLASS
class Fireball{
    public:
        sf::CircleShape shape;
        sf::Vector2f currVelocity;
        float maxSpeed;

        Fireball(float radius = 10.f)
            :currVelocity(0.f, 0.f), maxSpeed(100.f)
        {
            this->shape.setRadius(radius);
            this->shape.setFillColor(sf::Color::Red);
        }
};


//RUNS THE GAME'S CODE
int main() {

    //WINDOW
    sf::Vector2f viewSize(1000, 800);
    sf::VideoMode vm(viewSize.x, viewSize.y);
    sf::RenderWindow window(vm, "Professor Pickle's Archeology Adventure", sf::Style::Default);
    window.setFramerateLimit(60);

    //BACKGROUND
    sf::Texture background;
    sf::Texture background2;

    //ENEMY & PLAYER TEXTURES
    sf::Texture player;
    sf::Texture enemy1;
    sf::Texture enemy2;

    //ITEM & DOOR TEXTURES
    sf::Texture key;
    sf::Texture door;
    sf::Texture treasure1;
    sf::Texture treasure2;
    sf::Texture treasure3;

    //UI TEXTURES
    sf::Texture title;
    sf::Texture gemEnd;
    sf::Texture bookEnd;
    sf::Texture chestEnd;
    sf::Texture deadEnd;
    sf::Texture pause;

    //LOAD BACKGROUND SPRITES
    background.loadFromFile("./assets/backgrounds/sand.jpg");
    sf::Sprite back(background);
    back.setScale(1.f,1.f);
    background2.loadFromFile("./assets/backgrounds/floor.jpg");
    sf::Sprite back2(background2);
    back2.setScale(1.f,1.f);

    //LOAD PLAYER SPRITE
    player.loadFromFile("./assets/sprites/player.png");
    sf::Sprite play(player);
    play.setScale(0.25f,0.25f);

    //LOAD ENEMY SPRITES
    enemy1.loadFromFile("./assets/sprites/lion.png");
    sf::Sprite enem1(enemy1);
    enem1.setScale(0.75f,0.75f);
    enemy2.loadFromFile("./assets/sprites/mummy.png");
    sf::Sprite enem2(enemy2);
    enem2.setScale(0.25f,0.25f);

    //LOAD DOOR AND ITEM SPRITES
    door.loadFromFile("./assets/sprites/door.png");
    sf::Sprite doorSprite(door);
    doorSprite.setScale(0.2f,0.2f);
    key.loadFromFile("./assets/sprites/key.png");
    sf::Sprite keySprite(key);
    keySprite.setScale(0.05f,0.05f);


    //LOAD TREASURE SPRITES
    treasure1.loadFromFile("./assets/sprites/emerald.png");
    sf::Sprite trea1(treasure1);
    trea1.setScale (0.2f,0.2f);
    treasure2.loadFromFile("./assets/sprites/book.png");
    sf::Sprite trea2(treasure2);
    trea2.setScale (0.2f,0.2f);
    treasure3.loadFromFile("./assets/sprites/chest.png");
    sf::Sprite trea3(treasure3);
    trea3.setScale (0.35f,0.35f);

    //LOAD ENDING AND UI TEXTURES
    title.loadFromFile("./assets/backgrounds/PStart.png");
    sf::Sprite titleStart(title);
    titleStart.setScale(0.33f,0.5f);
    gemEnd.loadFromFile("./assets/backgrounds/Gem.png");
    sf::Sprite gemScreen(gemEnd);
    gemScreen.setScale(0.33f,0.5f);
    chestEnd.loadFromFile("./assets/backgrounds/Chest.png");
    sf::Sprite chestScreen(chestEnd);
    chestScreen.setScale(0.33f,0.5f);
    bookEnd.loadFromFile("./assets/backgrounds/Tome.png");
    sf::Sprite bookScreen(bookEnd);
    bookScreen.setScale(0.33f,0.5f);
    deadEnd.loadFromFile("./assets/backgrounds/Dead.png");
    sf::Sprite dead(deadEnd);
    dead.setScale(0.33f,0.5f);
    
    //MUSIC
    sf::Music music;

    //TIME
    sf::Time dt;

    //SET UP TIMER VALUES
    float seconds = 0;
    int timestamp =0;

    //TIME VALUES BETWEEN EVENTS
    float prevHitTime = 0;
    float shootTime =0;
    float fireTime =0;

    //SCORE VARIABLES
    float score = 0;
    int singleScore = 0;

    //ENEMY DIRECTION 
    int direction1 = 0;
    int direction2 = 0;

    //BOOLS FOR LOGIC
    bool mummyAlive = false;
    bool lionPresent = true;
    bool mummyScore = true;
    bool paused = false;
    bool unlocked = false;
    int opposite;
    
    //INT THAT DECIDES CURRENT LEVEL
    int level = 0;
    
    //PLAYER AIMING VECTORS
    sf::Vector2f playerPoint;
    sf::Vector2f playerUp;
    sf::Vector2f playerDown;
    sf::Vector2f playerLeft;
    sf::Vector2f playerRight;
    sf::Vector2f aimDir;

    //ENEMY AIMING VECTORS
    sf::Vector2f mummyPoint;
    sf::Vector2f mummyDown;
    sf::Vector2f mummyAim;

    //STRINGS TO DISPLAY ONSCREEN
    std::string display = "Get into the temple door!";
    std::string display2 = "Move with WASD and shoot with arrow keys!";
    std::string inventory;

    //LOAD FONT
    sf::Font scoreFont;
    if (!scoreFont.loadFromFile("./assets/sprites/Hylia.otf"))
    {
        std::cout<<"ERROR"<<std::endl;
    }

    //CREATE CHARACTERS
    Player hero(100,700,150,3,play);
    Lion lion(10, 100, 500, 5, enem1 , 0);
    Mummy mummy(990, 100, 200, 1, 4,enem2, 1);

    //CREATE ITEMS
    Item templeKey(400, 50, keySprite, "Temple Key" );
    Item emerald(100, 400, trea1, "treasure1" );
    Item book(400, 650, trea2, "treasure2" );
    Item chest(800, 395, trea3, "treasure3" );

    //CREATE DOORS
    Door templeDoor(800,650,doorSprite,2 );
    Door treasureDoor(500,50,doorSprite,3 );

    //PLAYER BULLETS
    Bullet b1;
    std::vector<Bullet> player_bullets;
    player_bullets.push_back(Bullet(b1));

    //ENEMY FIREBALLS
    Fireball f1;
    std::vector<Fireball> fireballs;
    fireballs.push_back(Fireball(f1));


    //VISIBLE UI TEXT
    sf::Text uiText;
    uiText.setFont(scoreFont);
    uiText.setCharacterSize(20);
    uiText.setPosition(0, 0);
    uiText.setFillColor(sf::Color::White);
    sf::Text held;
    held.setFont(scoreFont);
    held.setCharacterSize(20);
    held.setPosition(790,0 );
    held.setFillColor(sf::Color::White);
    sf::Text prompt;
    prompt.setFont(scoreFont);
    prompt.setCharacterSize(30);
    prompt.setPosition(200, 600);
    prompt.setFillColor(sf::Color::White);


    //SET UP RANDOM
    srand(time(NULL));

    //LOAD MUSIC
    if( !music.openFromFile("./assets/sound/Electro.flac")){
        std::cout<<"ERROR"<<std::endl;
    }

    //PLAY MUSIC
    music.play();

    //SET UP RUNNING CLOCK
    sf::Clock clock;

    //WHILE THE GAME IS OPEN
	while (window.isOpen()) {

        //UPDATE TIMER IF NOT PAUSED
        if ((paused == false)&& (level<4)&&(level>0))
        {
            seconds += dt.asSeconds();
            timestamp = static_cast<int>(seconds);

            //UPDATE SCORE BASED ON TIME
            score = score + (2 * dt.asSeconds());
            singleScore = static_cast<int>(score);

            //UPDATE TIME SINCE PLAYER HIT
            prevHitTime = prevHitTime + dt.asSeconds();

            //UPDATE TIME SINCE PLAYER SHOT
            shootTime = shootTime + dt.asSeconds();

            //UPDATED TIME SINCE MUMMY SHOT
            fireTime =fireTime +dt.asSeconds();
        }


        //SET UP EVENT
        sf::Event event;
        while (window.pollEvent(event))
        {
            //IF WINDOW IS CLOSED CLOSE THE PROGRAM
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
            //IF P IS PRESSED, PAUSE OR UNPAUSE
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P){
                paused =!paused;
            }
         }

        //IF IT'S THE TITLE SCREEN
        if (level < 1)
        {
            //START LEVEL ONE BY PRESSING K
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) )
            {
                level = 1;
            }
        }

        //IF IT'S AN END SCREEN
        if (level > 3)
        {
            //EXIT GAME BY PRESSING ESC
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )
            {
                window.close();
            }
        }

        //SET AIM DIRECTIONS CONSTANTLY BASED ON PLAYER AND ENEMY POSITION
        playerPoint = sf::Vector2f(hero.x, hero.y);
        playerUp = sf::Vector2f(hero.x, hero.y -0.1);
        playerDown = sf::Vector2f(hero.x, hero.y +0.1);
        playerLeft = sf::Vector2f(hero.x -0.1, hero.y);
        playerRight = sf::Vector2f(hero.x +0.1, hero.y);
        mummyPoint = sf::Vector2f(mummy.x, mummy.y);
        mummyDown = sf::Vector2f(mummy.x, mummy.y+0.1);

        //CONTROLS FOR ALL PLAYABLE LEVELS
        if ((level > 0)&&(level < 4))
        {
            //WHEN THE GAME IS NOT PAUSED
            if(paused ==false)
            {
                //IF A IS PRESSED
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
                {
                    //MOVE PLAYER LEFT WITHIN BOUNDARY
                    hero.x -= dt.asSeconds()*hero.speed;
                    if (hero.x < 0) 
                    {
                         hero.x = 0;
                    }
                }
                //IF D IS PRESSED
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                {
                    //MOVE PLAYER RIGHT WITHIN BOUNDARY
                    hero.x += dt.asSeconds()*hero.speed;
                    if (hero.x > viewSize.x - hero.playerSprite.getGlobalBounds().width) 
                    {
                        hero.x = viewSize.x - hero.playerSprite.getGlobalBounds().width;
                    }
                }
                //IF W IS PRESSED
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                {
                    //MOVE PLAYER UP WITHIN BOUNDARY
                    hero.y -= dt.asSeconds()*hero.speed;
                    if (hero.y < 0)
                    {
                        hero.y = 0;
                    }
                }
                //IF S IS PRESSED
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
                {
                    //MOVE PLAYER DOWN WITHIN BOUNDARY
                    hero.y += dt.asSeconds()*hero.speed;
                    if (hero.y > viewSize.y - hero.playerSprite.getGlobalBounds().height) 
                    {
                        hero.y = viewSize.y - hero.playerSprite.getGlobalBounds().height;
                    }
                }

                //IF UP KEY IS PRESSED
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) )
                {
                    aimDir = playerUp -playerPoint;

                    //AND THEY CAN SHOOT
                    if(shootTime >= 0.3)
                    {
                        //SHOOT UP AND RESET TIME BETWEEN SHOTS
                        b1.shape.setPosition(playerPoint.x+25,playerPoint.y);
                        b1.currVelocity = aimDir *b1.maxSpeed;
                        player_bullets.push_back(Bullet(b1));
                        shootTime = 0;
                    }
                }
                //IF DOWN KEY IS PRESSED
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) )
                {
                    aimDir = playerDown - playerPoint;

                    //AND THEY CAN SHOOT
                    if(shootTime >= 0.3)
                    {
                        //SHOOT DOWN AND RESET TIME BETWEEN SHOTS
                        b1.shape.setPosition(playerPoint.x+25,playerPoint.y+50);
                        b1.currVelocity = aimDir *b1.maxSpeed;
                        player_bullets.push_back(Bullet(b1));
                        shootTime = 0;
                    }
                }
                //IF LEFT KEY IS PRESSED
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) )
                {
                    aimDir = playerLeft - playerPoint;

                    //AND THEY CAN SHOOT
                    if(shootTime >= 0.3)
                    {
                        //SHOOT LEFT AND RESET TIME BETWEEN SHOTS
                        b1.shape.setPosition(playerPoint.x,playerPoint.y+25);
                        b1.currVelocity = aimDir *b1.maxSpeed;
                        player_bullets.push_back(Bullet(b1));
                        shootTime = 0;
                    }
                }
                //IF RIGHT KEY IS PRESSED
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) )
                {
                    aimDir = playerRight - playerPoint;

                    //AND THEY CAN SHOOT
                    if(shootTime >= 0.3)
                    {
                        //SHOOT RIGHT AND RESET TIME BETWEEN SHOTS
                        b1.shape.setPosition(playerPoint.x+50,playerPoint.y+25);
                        b1.currVelocity = aimDir *b1.maxSpeed;
                        player_bullets.push_back(Bullet(b1));
                        shootTime = 0;
                    }
                }
            }
        }

        //LEVEL 2 MUMMY FIRING 
        if (level == 2)
        {
            //IF MUMMY IS ALIVE AND THE GAME ISN'T PAUSED
            if ((mummyAlive)&& (paused == false))
            {
                mummyAim = mummyDown - mummyPoint;

                //AND TIME HAS PASSED SINCE THEY FIRED
                if(fireTime >= 1)
                {
                    //SHOOT A FIREBALL AND RESET TIMER
                    f1.shape.setPosition(mummyPoint.x+25,mummyPoint.y+50);
                    f1.currVelocity = mummyAim *f1.maxSpeed;
                    fireballs.push_back(Fireball(f1));
                    fireTime = 0;
                }
            }
        }

        //RENDER BULLETS AND FIREBALLS WHILE UNPAUSED
        if(paused == false)
        {
            //FOR ALL ITEMS IN BULLET LIST
            for (size_t i = 0; i < player_bullets.size(); i++)
            {
                //MOVE
                player_bullets[i].shape.move(player_bullets[i].currVelocity);
                //IF OUT OF BOUNDS
                if(player_bullets[i].shape.getPosition().x <0 || player_bullets[i].shape.getPosition().x > window.getSize().x ||
                player_bullets[i].shape.getPosition().x <0 || player_bullets[i].shape.getPosition().y > window.getSize().y)
                {
                    //ERASE
                    player_bullets.erase(player_bullets.begin()+ i);
                }
                //IF LION IS HIT BY BULLET
                else if(player_bullets[i].shape.getGlobalBounds().intersects(lion.enemSprite.getGlobalBounds()))
                {
                    //LOWER LION SPEED AND ERASE BULLET
                    lion.lowerSpeed();
                    score = score+ 50;
                    player_bullets.erase(player_bullets.begin()+ i);
                }
                //IF MUMMY IS HIT BY BULLET
                else if(player_bullets[i].shape.getGlobalBounds().intersects(mummy.enemSprite.getGlobalBounds()))
                {
                    //LOWER MUMMY HEALTH AND ERASE BULLET
                    mummy.loseHealth();
                    if(mummyAlive)
                    {
                        score = score+50;
                        player_bullets.erase(player_bullets.begin()+ i);
                    }
                }
            }
            

            //FOR ALL ITEMS IN FIREBALL LIST
            for (size_t i = 0; i < fireballs.size(); i++)
            {
                //MOVE
                fireballs[i].shape.move(fireballs[i].currVelocity);
                //IF OUT OF BOUNDS
                if(fireballs[i].shape.getPosition().x <0 || fireballs[i].shape.getPosition().x > window.getSize().x ||
                fireballs[i].shape.getPosition().x <0 || fireballs[i].shape.getPosition().y > window.getSize().y)
                {
                    //ERASE
                    fireballs.erase(fireballs.begin()+ i);
                }
                //IF PLAYER IS HIT BY FIREBALL
                else if(fireballs[i].shape.getGlobalBounds().intersects(hero.playerSprite.getGlobalBounds()))
                {
                    //IF PLAYER CAN BE HIT
                    if (prevHitTime >2)
                    {
                        //LOSE HEALTH AND RESET TIMER
                        hero.loseHealth(mummy.damage);
                        score= score-50;
                        prevHitTime =0;
                    }
                    //ERASE FIREBALL
                    fireballs.erase(fireballs.begin()+ i);
                }
            }

            //IF LION IS IN LEVEL
            if(lionPresent)
            {
                //MOVE BASED ON INPUT DIRECTION
                switch (direction1)
                {
                    //MOVE LION LEFT WITHIN BOUNDARY
                    case (0):
                        lion.x -= dt.asSeconds()*lion.speed;
                        if (lion.x < 0) 
                        {
                            lion.x = 0;
                            direction1 = 1;
                        }
                        break;
                    //MOVE LION RIGHT WITHIN BOUNDARY
                    case (1):
                        lion.x += dt.asSeconds()*lion.speed;
                        if (lion.x > viewSize.x - lion.enemSprite.getGlobalBounds().width) 
                        {
                            lion.x = viewSize.x - lion.enemSprite.getGlobalBounds().width;
                            direction1 = 0;
                        }
                        break;
                    default:
                        break;
                }
            }

            //IF MUMMY IS PRESENT IN LEVEL
            if(mummyAlive){

                //MOVE BASED ON INPUT DIRECTION
                switch (direction2)
                {
                    //MOVE MUMMY LEFT WITHIN BOUNDARY
                    case (0):
                        mummy.x -= dt.asSeconds()*mummy.speed;
                        if (mummy.x < 0) 
                        {
                            mummy.x = 0;
                            direction2 = 1;
                        }
                        break;
                    //MOVE MUMMY RIGHT WITHIN BOUNDARY
                    case (1):
                        mummy.x += dt.asSeconds()*mummy.speed;
                        if (mummy.x > viewSize.x - mummy.enemSprite.getGlobalBounds().width) 
                        {
                            mummy.x = viewSize.x - mummy.enemSprite.getGlobalBounds().width;
                            direction2 = 0;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
            

        //UPDATE SPRITE LOCATIONS
        hero.setSprite();
        lion.setSprite();
        mummy.setSprite();
        templeKey.setSprite();
        templeDoor.setSprite();

        treasureDoor.setSprite();
        emerald.setSprite();
        book.setSprite();
        chest.setSprite();


        //IF PLAYER IS TOUCHING LION
        if (hero.playerSprite.getGlobalBounds().intersects(lion.enemSprite.getGlobalBounds())) 
        {
            //AND ENOUGH TIME SINCE LAST HIT
            if(prevHitTime > 2)
            {
                //PLAYER LOSES HEALTH AND RESETS HIT TIMER
                hero.loseHealth(lion.damage);
                score = score-10;
                prevHitTime=0;
            }
        }

        //COLLECTING KEY COLLISION
        if (hero.playerSprite.getGlobalBounds().intersects(templeKey.itemSprite.getGlobalBounds())) 
        {
            inventory = templeKey.AddItem();
            unlocked = true;
        }

        //ENTERING TEMPLE DOOR COLLISION
        if (hero.playerSprite.getGlobalBounds().intersects(templeDoor.doorSprite.getGlobalBounds())) 
        {
            //IF YOU HAVE KEY
            if(inventory == templeKey.itemValue)
            {
                //CHANGE LEVEL AND SPAWN MUMMY
                level = 2;
                mummyAlive = true;
                int opposite = lion.x;
                lion.lionReset((1000 - opposite));
            }
        }

        //ITEM COLLISION FOR ENTERING TEMPLE
        if (hero.playerSprite.getGlobalBounds().intersects(treasureDoor.doorSprite.getGlobalBounds())) 
        {
            if( level ==2)
            {
                level = 3;
                lion.x = 2000;
                lion.y = 2000;
            }
        }

        //FINAL TREASURE CHOICE EVENTS
        if( level ==3)
        {
            //IF THE PLAYER TOUCHES THE CHEST
            if (hero.playerSprite.getGlobalBounds().intersects(chest.itemSprite.getGlobalBounds()))
            {
                //GO TO END SCREEN WITH SCORE
                score = score+400;
                prompt.setPosition(475,570);
                //prompt.setFillColor(sf::Color::White);
                prompt.setString(std::to_string(singleScore));
                level = 4;
            }
            //IF THE PLAYER TOUCHES THE EMERALD
            if (hero.playerSprite.getGlobalBounds().intersects(emerald.itemSprite.getGlobalBounds()))
            {
                //GO TO END SCREEN WITH SCORE
                score = score+100;
                prompt.setPosition(475,570);
                //prompt.setFillColor(sf::Color::White);
                prompt.setString(std::to_string(singleScore));
                level = 5;
            }
            //IF THE PLAYER TOUCHES THE BOOK
            if (hero.playerSprite.getGlobalBounds().intersects(book.itemSprite.getGlobalBounds()))
            {
                //GO TO END SCREEN WITH SCORE
                score =score+700;
                prompt.setPosition(475,570);
                //prompt.setFillColor(sf::Color::White);
                prompt.setString(std::to_string(singleScore));
                level = 6;
            }
        }
            
            

        //IF PLAYER IS TOUCHING THE MUMMY WHILE IT'S ALOVE
        if ((hero.playerSprite.getGlobalBounds().intersects(mummy.enemSprite.getGlobalBounds())) && (mummyAlive)) 
        {
            //IF PLAYER CAN BE HIT
            if(prevHitTime > 2)
            {
                //PLAYER LOSES LIFE AND HIT RESETS TIMER
                hero.loseHealth(mummy.damage);
                score = score-10;
                prevHitTime=0;
            }
        }

        //IF PLAYER IS OUT OF HEALTH
        if (hero.health <= 0) 
        {
            //TAKE TO DEATH SCREEN AND DISPLAY SCORE
            prompt.setPosition(475,570);
            //prompt.setFillColor(sf::Color::White);
            prompt.setString(std::to_string(singleScore));
            level =7;
        }

        //IF MUMMY IS OUT OF HEALTH
        if (mummy.health <= 0) 
        {
            //DISABLE MUMMY
            mummyAlive = false;
        }

        //CLEAR WINDOW
        window.clear();

        //ONLY DRAW TITLE SCREEN TO START
        if (level == 0)
        {
            window.draw(titleStart);
        }

        //FOR LEVEL ONE DRAW:
        if(level == 1)
        {
            //ALL ASSOCIATED SPRITES
            window.draw(back);
            window.draw(hero.playerSprite);
            window.draw(templeDoor.doorSprite);
            window.draw(lion.enemSprite);

            //DELETE TEMPLEKEY SPRITE WHEN GRABBED
            if(unlocked == false)
            {
                window.draw(templeKey.itemSprite);
            }
        }

        //FOR LEVEL 2 DRAW:
        if (level == 2)
        {
            //ALL ASSOCIATED SPRITES
            window.draw(back2);
            window.draw(hero.playerSprite);
            window.draw(treasureDoor.doorSprite);
            window.draw(lion.enemSprite);

            //DRAW MUMMY IF CURRENTLY
            if (mummyAlive)
            {
                window.draw(mummy.enemSprite);
            }
        }

        //FOR LEVEL 3 DRAW
        if (level == 3)
        {
            //ALL ASSOCIATED SPRITES
            window.draw(hero.playerSprite);
            window.draw(chest.itemSprite);
            window.draw(emerald.itemSprite);
            window.draw(book.itemSprite);
        } 

        //FOR ENDING SCREENS DRAW ENDSCREEN AND SCORE
        if (level == 4)
        {
            window.draw(chestScreen);
            window.draw(prompt);
        } 
        if (level == 5)
        {
            window.draw(gemScreen);
            window.draw(prompt);
        } 
        if (level == 6)
        {
            window.draw(bookScreen);
            window.draw(prompt);
        } 
        if (level == 7)
        {
            window.draw(dead);
            window.draw(prompt);
        } 

        //IF PAUSED DRAW PAUSE UI
        if (paused == true)
        {
            //window.draw();
        }

        //DRAW UI DURING MAIN PLAYABLE LEVELS
        if((level>0)&&(level<4))
        {
                
            //UPDATE/DISPLAY CURRENT SCORE, TIME, AND HEALTH
            uiText.setString("Score: " + std::to_string(singleScore)
            +"\nTime: " + std::to_string(timestamp) + "\nHealth: " + std::to_string(hero.health));
		    window.draw(uiText);

            //DISPLAY CURRENT PROMPT
            prompt.setString(display +"\n"+display2);
            window.draw(prompt);
            held.setString("Current held item: \n"+inventory);
            window.draw(held);
        }

        //DRAW BULLETS AND FIREBALLS 
        for (size_t i = 0; i < player_bullets.size(); i++)
        {
            window.draw(player_bullets[i].shape);
        }
        for (size_t i = 0; i < fireballs.size(); i++)
        {
            window.draw(fireballs[i].shape);
        }

        //UPDATE CLOCK TIME
        dt = clock.restart();
        
        //DISPLAY WINDOW
		window.display();
	}
	return 0;
}