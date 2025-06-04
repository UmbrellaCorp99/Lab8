//Alexander Young
//Lab 8

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

int main()
{
    //setting up variables and pointers for use in the program
    const float FPS = 60;
    int width = 900, height = 800;
    const int mikasaSize = 150;
    bool done = false, redraw = false;
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_BITMAP* mikasa = NULL;
    ALLEGRO_BITMAP* image = NULL;
    float mikasa_x = width / 2.0 - mikasaSize / 2.0;
    float mikasa_y = height / 2.0 - mikasaSize / 2.0;
    float mikasa_dx = 4.0, mikasa_dy = 4.0;
    float angle = 0;
    int flag = 1;


    //initializing allegro and addons with error handling
    if (!al_init()) {
        al_show_native_message_box(NULL, "Error", "Allegro failed to initialize", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        al_show_native_message_box(NULL, "Error", "Timer failed to initialize", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    display = al_create_display(width, height);
    if (!display) {
        al_show_native_message_box(NULL, "Error", "Display failed to initialize", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_timer(timer);
        return -1;
    }
    al_init_image_addon();
    if (!al_init_image_addon()) {
        al_show_native_message_box(NULL, "Error", "Display failed to initialize", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    //loading images as bitmaps for background and moving image
    image = al_load_bitmap("forest.jpg");
    mikasa = al_load_bitmap("Mikasa150.png");

    //setting up event queue
    event_queue = al_create_event_queue();
    if (!event_queue) {
        al_destroy_bitmap(mikasa);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    //tying event queue to display, timer, and keyboard
    al_install_keyboard();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    //inital drawing of background and moving image
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(image, 0, 0, 1280, 720, 0, 0, width, height, 0);
    al_draw_rotated_bitmap(mikasa, 16, 16, mikasa_x, mikasa_y, angle, flag);
    al_flip_display();
    al_start_timer(timer);

    while (!done) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
            //if the image hits a display border, change the direction and rotate 180 degrees
            if (mikasa_x < 0){
                mikasa_dx = -mikasa_dx;
                angle += 3.14;
            }
            else if (mikasa_x > width - mikasaSize) {
                mikasa_dx = -mikasa_dx;
                angle -= 3.14;
            }
            else if (mikasa_y < 0) {
                mikasa_dy = -mikasa_dy;
                angle += 3.14;
            }
            else if ((mikasa_y + mikasaSize) > height) {
                mikasa_dy = -mikasa_dy;
                angle -= 3.14;
            }
            mikasa_x += mikasa_dx;
            mikasa_y += mikasa_dy;
            
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            //if user hits spacebar, stop the images. If arrow is pressed, change the moving image orientation to match
            if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                flag = 0;
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                flag = ALLEGRO_FLIP_HORIZONTAL;
                angle = 0;
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                angle = (1.57 + 3.14);
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                flag = ALLEGRO_FLIP_VERTICAL;
                angle = 1.57;
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                system("pause");
            }
        }
        //redraw the images
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_scaled_bitmap(image, 0, 0, 1280, 720, 0, 0, width, height, 0);
            al_draw_rotated_bitmap(mikasa, al_get_bitmap_width(mikasa)/2, al_get_bitmap_height(mikasa)/2, mikasa_x + al_get_bitmap_width(mikasa) / 2, mikasa_y + al_get_bitmap_height(mikasa) / 2, angle, flag);
            al_flip_display();
        }
    }
    //destroy pointers and clean up memory
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_bitmap(mikasa);
    al_destroy_bitmap(image);
}

