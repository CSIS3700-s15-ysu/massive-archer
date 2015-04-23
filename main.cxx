#include <cmath>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_primitives.h"

// for fonts
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"


#include "world.h"

using namespace std;
using namespace csis3700;



//const size_t WIDTH=800; //moved to world.h
//const size_t HEIGHT=600; //moved to world.h
const float FPS = 60;


int main(int argc, char **argv){
 
   //loading allegro
   if(!al_init()) {
     cerr << "Failed to initialize allegro!" << endl;
     exit(1);
   }
 
   //loading image addon
   if(!al_init_image_addon()) {
     cerr << "Failed to initialize al_init_image_addon!" << endl;
     exit(1);
   }
   
   //loading mouse
   if(!al_install_mouse()) {
     cerr << "Failed to install mouse." << endl;
     exit(1);
   }

   //loading keyboard
   if(!al_install_keyboard()) {
     cerr << "Failed to install keyboard." << endl;
     exit(1);
   }

	// create display
   ALLEGRO_DISPLAY *display = al_create_display(world::WIDTH, world::HEIGHT);
 
   if(!display) {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return 0;
   }

	//loading font addons
   al_init_font_addon();
   al_init_ttf_addon();
   
   //setting up font
    ALLEGRO_FONT *font = al_load_ttf_font("PRIMETIME.ttf",36,NULL);

   
	//setting up timer
   ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
   if(!timer) {
     al_show_native_message_box(display, "Error", "Error",
		"Failed to create timer!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
     al_destroy_display(display);
     return -1;
   }

	//setting up event queue
   ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
   
   if(!event_queue) {
     al_show_native_message_box(display, "Error", "Error", "Failed to create event queue!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
  
   al_register_event_source(event_queue, al_get_display_event_source(display)); 
   al_register_event_source(event_queue, al_get_timer_event_source(timer)); //timer
   al_register_event_source(event_queue, al_get_mouse_event_source()); //mouse
   al_register_event_source(event_queue, al_get_keyboard_event_source()); //keyboard
   al_start_timer(timer);

   //setting everything up for the first time
   csis3700::world theWorld(display, world::WIDTH, world::HEIGHT);

   double time = 0;
   bool redraw = true; // when true, the world is painting for the first time

   
   ALLEGRO_EVENT ev; //event type
   
   do
   {
      al_wait_for_event(event_queue, &ev);
 
      if(ev.type == ALLEGRO_EVENT_TIMER) {
         redraw = true;
      }
	  
	   
	  if (ev.type == ALLEGRO_EVENT_KEY_UP) {
		//a key has been released
		//now we need the world to handle the event
		theWorld.handle_event(ev);	  
	  }	  
	  
       
	//world is redrawing & advancing time
	//this happens whether or not you want it to
	//YOU CAN'T STOP TIME
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         time += 1/FPS;

         theWorld.advance_by_time(1/FPS); //tell world to advance

         theWorld.draw(display); //draw the world (in the background)

         al_flip_display(); //bring background to foreground
      }
   } while(ev.type != ALLEGRO_EVENT_DISPLAY_CLOSE);
 

 
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   al_destroy_font(font);
 
   return 0;
}
