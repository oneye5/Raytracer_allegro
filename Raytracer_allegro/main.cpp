#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "renderer.h"

const int SCREEN_W = 400;
const int SCREEN_H = 400;
ALLEGRO_DISPLAY* app_display;
ALLEGRO_EVENT_QUEUE* event_queue;

Renderer renderer;

#define fail(x) {std::cout<<x; exit(-1);}
void init();
void tick();

int main()
{
    init();

    while (1)
    {
        ALLEGRO_EVENT Event;
        if (!al_is_event_queue_empty(event_queue))
        {
            std::cout << "notempty\n";
            al_wait_for_event(event_queue, &Event);
            if (Event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                break;
        }

        tick();
    }

    al_destroy_display(app_display);
    al_destroy_event_queue(event_queue); // Clean up event queue
    return 1;
}

void init()
{
    if (!al_init())
        fail("allegro failed to initialize");

    app_display = al_create_display(SCREEN_W, SCREEN_H);

    if (!app_display)
        fail("allegro display failed to initialize");

    event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_display_event_source(app_display)); // Register display event source

    renderer = Renderer{ SCREEN_W,SCREEN_H };
}

void tick()
{
    renderer.render_frame();
    al_flip_display();
    renderer.camera.PITCH_YAW.y -= 0.02f;
    al_clear_to_color(al_map_rgb(0, 0, 0));
}
