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

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Alpcraft", 7U, settings);
    window.setPosition({ 220, 70 });
    window.setFramerateLimit(300);
    window.setIcon(GameIcon.getSize().x, GameIcon.getSize().y, GameIcon.getPixelsPtr());

    glewInit();

    sf::Texture Crosshair;
    Crosshair.loadFromFile("textures/crosshair.png");
    sf::Sprite CrosshairSprite(Crosshair);
    CrosshairSprite.setScale({ 1, 1 });
    CrosshairSprite.setPosition((WINDOW_WIDTH / 2) - 24, (WINDOW_HEIGHT / 2) - 24);

    sf::Font font;
    font.loadFromFile("textures/MCREGULAR.otf");

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor({ 255, 255, 255, 255 });

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    UtilityManager UtilManager;

    World world;

    Player player({ 16.0f, 18.0f, 16.0f });

    sf::Clock deltaClock2;

    FPS fps;

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

            if (event.type == sf::Event::MouseWheelScrolled )
            {
                player.m_SelectedBlock = (Block)floor(event.mouseWheelScroll.delta + player.m_SelectedBlock);

                if (player.m_SelectedBlock < Grass) player.m_SelectedBlock = Grass;
                if (player.m_SelectedBlock > BLOCKAMOUNT - 1) player.m_SelectedBlock = BLOCKINFOS[BLOCKAMOUNT-1].BlockID;
            }
        }

        sf::Time time = deltaClock2.restart();
        float dt = time.asSeconds();

        UtilManager.BindBlockShader();



        player.Update(window, dt, false, UtilManager.GetShader());
        world.Update(player.GetPosition());


        // OPENGL 3D DRAWING //


        glClearColor(93.0f / 255.0f, 180.0f / 255.0f, 238.0f / 255.0f, 1.0f);
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

        window.pushGLStates();

        window.draw(CrosshairSprite);

        text.setString(
            "Player Position: " +
            std::to_string((int)player.GetPosition().x) + " " +
            std::to_string((int)player.GetPosition().y) + " " +
            std::to_string((int)player.GetPosition().z) + "\n"
            "Chunk Position: " +
            std::to_string((int)player.GetChunkPosition().x) + " " +
            std::to_string((int)player.GetChunkPosition().y) + " " +
            std::to_string((int)player.GetChunkPosition().z) + "\n\n"
            "Selected Block: " + GetBlockName(player.GetSelectedBlock()) + "\n\n"
            "Frames Per Second: " + std::to_string((int)fps.getFPS()) + "\n\n"
        );

        window.draw(text);

        window.popGLStates();
        window.display();

        fps.update();
    }

    return 0;
}