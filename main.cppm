#include <SDL.h>
import Individual_Menu;
import <iostream>;
/// <summary>
/// to do:
/// zefaktoryzowac sin i cos
/// Poprawic wyswietlanie hitbox dla obracania!
/// rozwiazac problem z destruktoem w player
/// </summary>

class Server//Public_Menu
{
public:
    std::vector<player> players;
    std::vector<NPC> npc;
    std::vector<wall> w;

    std::vector<Individual_Menu> menus;
    SDL_Event e;
    bool running = true;

    void create_world()
    {
        w = { {100, 100, 200, 500} , {100, 100, 500, 90} , {500, 400, 500, 90} , {800, 290, 500, 290} };
        npc = { {-50,50} };
        menus.reserve(8);
        players.reserve(2);
        std::cout << menus.size();
    }

    void init_menus()
    {
        int players_num = 2;
        for (int i = 0; i < players_num; i++)
        {
            menus.push_back({ players, npc, w, 1300 * i, 500 });//jakis gracz wbil
            if (!menus[i].init())
                running = false;
        }
    }

    void main_loop()
    {
        while (running)
        {
            SDL_PollEvent(&e);

            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE || e.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                running = false;
            }
            for (int i = 0; i < menus.size(); i++)
                menus[i].main_loop(e);
        }

        for (int i = 0; i < menus.size(); i++)
            menus[i].clean();
    }
};

int main(int argc, char* argv[])
{
    Server server;
    server.create_world();
    server.init_menus();
    server.main_loop();


    return 0;
}