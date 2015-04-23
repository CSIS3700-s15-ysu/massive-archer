/*
we're using the smart world, dumb sprite model

this is still set up from Lab 7 -- lots of stuff will have to change

world is keeping track of game state
*/

#include "world.h"

// allegro stuff
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_primitives.h"
// for fonts
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

/*
tell each sprite when time moves forward in the world
when world is told to draw itself, it should draw background image/color and then tell each sprite to draw itself

use al_clear_to_color or al_draw_bitmap to redraw background image/color

when event occurs, send messages to sprites
*/


/** WHEN IN DOUBT USE A DAMN ITERATOR **/




namespace csis3700 {

    /**
     * Construct the world. The display is passed in simply to make it
     * possible to modify options or access the backbuffer. DO NOT
     * store the display in an instance variable. DO NOT start drawing
     * on the screen. Just load bitmaps etc.
     */
    world::world(ALLEGRO_DISPLAY *display, std::size_t window_width, std::size_t window_height) {
    	
    	//set default game state
		currentState = AIMING;
		turns = 0;
		player_turn = 1;
		hit_tank = false;
		
		
		
		//load bitmaps
    	ALLEGRO_BITMAP *sprite_image;
    	
    	image = al_load_bitmap("graveyard.png");
 
   		if(!image) {
      		al_show_native_message_box(display, "Error", "Error", "Failed to load background image!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
      		al_destroy_display(display);
      	     	      	
   		}
   		
   		float x, y;
   				
		//loads sprite image and tells it where to draw
		//NOT ACTUALLY DRAWING
		sprite_image = al_load_bitmap("ghost.png");
		for (int i=1; i<3; i++){
    		
    		if (i==1) {
				//i is equal to 1
				//draw first tank
				x = 100;
				y = world::HEIGHT * 0.75;
			}
			
			else {
				//i is equal to 2
				//draw second tank
				//tank will need flipped -- change bitmap flag to flip
				
				x = world::WIDTH - 100;
				y = world::HEIGHT * 0.75;	
			}
			
			the_sprites.push_back(new sprite(sprite_image, world::WIDTH,
				world::HEIGHT, x, y));
    	}
		
    
    }

    /**
     * Free any resources being used by the world.
     */
     //this includes bitmaps assigned to sprites
    world::~world() {
    	al_destroy_bitmap(image);
    
    }

    /**
     * Block the copy constructor.  Worlds should not be copied to
     * just assert(false)
     */
    world::world(const world& other) { assert(false);}

    /**
     * Block operator =.  Worlds should not be assigned.
     */
    world& world::operator =(const world& other) { assert(false);}
 
 
 	/**
    * Changes the game state.
    */
    world::game_state_t world::change_game_state (game_state_t incoming_state) {
    	if (incoming_state == AIMING) {
			return IMPACT;
		}
		
		
		else if (incoming_state == IMPACT) {
			if (hit_tank == true) {
				//the shell hit the tank
				return GAME_OVER;
			}
		
			else {
				//the shell didn't hit the tank
				return AIMING;
			}
		}
	}
 
 	/**
     * Changes player turn.
     */
 
 	int world::which_player (int turns) {
		if (turns % 2 == 0) {
			//even numbered turn
			//it's player 2's turn
			return 2;
		}
	
		else {
			//odd numbered turn
			//it's player 1's turn
			return 1;
	
		}
	}
 
    /**
     * Update the state of the world based on the event ev.
     */
    void world::handle_event(ALLEGRO_EVENT ev) {

		/*
		//lab 7: mouse button was released, tell each sprite to go
		for (std::vector<sprite*>::iterator it = the_sprites.begin(); it != the_sprites.end(); ++it) {
			(*it)->sprite::go();
		}
		*/
		
		//for project 1, we need to know what event is happening AND the state of the world
		
		if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					//up arrow was released
			
					if (currentState == AIMING) {
        				//increase angle
        				//tell sprite to move
        				//update text on screen
        			}
				break;

				case ALLEGRO_KEY_DOWN:
					//down arrow was released
					if (currentState == AIMING) {
        				//decrease angle
        				//tell sprite to move
        				//update text on screen
        			}
				break;

				case ALLEGRO_KEY_RIGHT:
					//right arrow was released
			
					if (currentState == AIMING) {
        				//increase velocity
        				//update text on screen
        				
        			}

				break;


				case ALLEGRO_KEY_LEFT:
					//left arrow was released
			
					if (currentState == AIMING) {
        				//decrease velocity
        				//update text on screen        				
        			}

				break;

				case ALLEGRO_KEY_ENTER:
					//enter key was released
					if (currentState == AIMING) {
        				//done entering stuff, fire
        				//change game state to IMPACT
        				currentState = IMPACT;
        				//add text to screen instructing user to press enter again
        					//cause lazy
        			}
        			
        			else if (currentState == IMPACT) {
        				//do math to see where impact occurs
						//did it hit the other tank?
							//yes: show tank hit sprite
								//change game state to GAME_OVER
								//display winner after number of turns
							//no: show miss sprite
								//change game state to AIMING
								//increase turn count
								++turns;
								//change current player
								player_turn = which_player(turns);
        			}
        									
				break;

				case ALLEGRO_KEY_SPACE:
					//space key was released
					//skip to next game state
					currentState = change_game_state(currentState);
					break;

				/*
				//I don't think we need this bit
				case ALLEGRO_KEY_ESCAPE:
					//exit
					
					break;*/
			}
		}
	}

	
		



    /**
     * Advance the state of the world forward by the specified time.
     */
    void world::advance_by_time(double dt) {    
    	// the world itself doesn't do anything when time is just advancing
    	
    	
    	//tell sprites to advance by time
    	for (std::vector<sprite*>::iterator it = the_sprites.begin(); it != the_sprites.end(); ++it) {
			//tell sprites to update themselves
			(*it)->sprite::advance_by_time(dt);
		}
    
    }

    /**
     * Draw the world. Note that the display variable is passed only
     * because it might be needed to switch the target bitmap back to
     * the backbuffer.
     */
    void world::draw(ALLEGRO_DISPLAY *display) {
    	//draw background
    	al_draw_bitmap(image,0,0,0);
    	
    	//for loop with iterator to get sprites to load
    	for (std::vector<sprite*>::iterator it = the_sprites.begin(); it != the_sprites.end(); ++it) {
			//draw sprites
			(*it)->draw(display);
		}    
    }
    
}
