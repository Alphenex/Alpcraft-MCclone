#include <GL/glew.h>

#include <SFML/Graphics.hpp>

#include "Alpcraft/UtilityManager.h"
#include "Alpcraft/Player/player.h"
#include "Alpcraft/AWorld.h"

#include "gameinfo.h"

int main(void)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 0;
    settings.minorVersion = 0;

    sf::Image GameIcon;
    GameIcon.loadFromFile("textures/icon.png");

    sf::Texture Crosshair;
    Crosshair.loadFromFile("textures/crosshair.png");

    sf::Sprite CrosshairSprite(Crosshair);
    CrosshairSprite.setScale({ 1, 1 });
    CrosshairSprite.setPosition((WINDOW_WIDTH / 2) - 24, (WINDOW_HEIGHT / 2) - 24);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Alpcraft", 7U, settings);
    window.setPosition({ 220, 70 });
    window.setFramerateLimit(300);
    window.setIcon(GameIcon.getSize().x, GameIcon.getSize().y, GameIcon.getPixelsPtr());

    glewInit();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    UtilityManager UtilManager;

    World world;

    Player player({ 0.0f, 34.0f, 0.0f });

    sf::Clock deltaClock2;


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                player.HandleMouseInput(world, window);
            }
        }

        sf::Time time = deltaClock2.restart();
        float dt = time.asSeconds();

        UtilManager.BindBlockShader();

        player.Update(window, dt, false, UtilManager.GetShader());
        world.Update(player.GetPosition());


        // OPENGL 3D DRAWING //


        glClearColor(0.2f, 0.75f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CW);
        glEnable(GL_DEPTH_TEST);

        UtilManager.BindTextureAtlas();

        world.RenderChunks(UtilManager.GetShader().ID);

        UtilManager.UnBindTextureAtlas();

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        window.draw(CrosshairSprite);

        window.display();
    }

    return 0;
}