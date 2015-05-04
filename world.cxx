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
		//currentState = AIMING;
		animating_trajectory = false;
		shot_is_correct = false;
		shot_hit_something = false;
		//turns = 0;
		player_turn = 0;
		//hit_tank = false;
		float x, y;
		float angle;
		float center_x;
		float center_y;
		
		velocity = 0;



		//set the default key states
		key_up = false;
		key_down = false;
		key_left = false;
		key_right = false;
		key_enter = false;
		
		//Background image
		//NOT ACTUALLY DRAWING
		background_image = al_load_bitmap("graveyard.png");
 
		if(!background_image) {
			al_show_native_message_box(display, "Error", "Error", "Failed to load background image!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			al_destroy_display(display);
		}

		//Wands
		//loads wand sprites image and tells it where to draw
		//NOT ACTUALLY DRAWING
		ALLEGRO_BITMAP *wand_image = al_load_bitmap("wand.png");
		
		for (int i=1; i<3; i++){
			if (i==1) {
				//i is equal to 1
				//draw first tank
				x = 190;
				y = (world::HEIGHT * 0.75) + 35;
				angle = 0;
				center_x = 90;
				center_y = 35;
			}
			else {
				//i is equal to 2
				//draw second tank
				//tank will need flipped -- change bitmap flag to flip
				
				x = world::WIDTH - 190; //image is 80px
				y = (world::HEIGHT * 0.75) + 35;
				angle = 3.14;
				center_x = 90;
				center_y = 35;
			}
			
			wand_sprites.push_back(
				new sprite(
					wand_image,
					world::WIDTH,
					world::HEIGHT,
					x,
					y,
					angle,
					0,
					0,
					center_x,
					center_y,
					0
				)
			);
		}
		
		//Ghosts
		//loads ghost sprite image and tells it where to draw
		//NOT ACTUALLY DRAWING
		ALLEGRO_BITMAP *ghost_image = al_load_bitmap("ghost.png");

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
				
				x = world::WIDTH - 180; //image is 80px
				y = world::HEIGHT * 0.75;	
			}
			
			ghost_sprites.push_back(
				new sprite(
					ghost_image,
					world::WIDTH,
					world::HEIGHT,
					x,
					y,
					0,
					0,
					0,
					0,
					0,
					0
				)
			);
		}
		
		//setting up font
		velocity_font = al_load_ttf_font("IronBrine.ttf",36,NULL);
		
	}

	/**
	 * Free any resources being used by the world.
	 */
	 //this includes bitmaps assigned to sprites
	world::~world() {
		al_destroy_bitmap(background_image);
		al_destroy_font(velocity_font);
	
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
			if (shot_is_correct && shot_hit_something) {
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

	/*
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
	*/

	/**
	 * Update the state of the world based on the event ev.
	 */
	void world::handle_event(ALLEGRO_EVENT ev) {

		/*
		//lab 7: mouse button was released, tell each sprite to go
		for (std::vector<sprite*>::iterator it = ghost_sprites.begin(); it != ghost_sprites.end(); ++it) {
			(*it)->sprite::go();
		}
		*/
		
		//for project 1, we need to know what event is happening AND the state of the world
		
		if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					key_up = true;
			
					if (currentState == AIMING) {
						//call aiming function (still pseudo code)
						//update text on screen
					}
				break;

				case ALLEGRO_KEY_DOWN:
					//down arrow was released
					key_down = true;

					if (currentState == AIMING) {
						//call aiming function (still pseudo code)
						//update text on screen
					}
				break;

				case ALLEGRO_KEY_RIGHT:
					//right arrow was released
					key_right = true;
			
					if (currentState == AIMING) {
						//call aiming function (still pseudo code)
						//update text on screen
						
					}

				break;


				case ALLEGRO_KEY_LEFT:
					//left arrow was released
					key_left = true;
			
					if (currentState == AIMING) {
						//call aiming function (still pseudo code)
						//update text on screen        				
					}

				break;

				case ALLEGRO_KEY_ENTER:
					//enter key was released
					key_enter = true;

					if (currentState == AIMING) {
						//done entering stuff, fire
												
						//change game state to IMPACT
						//should probably call the change state fnc instead
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
								//++turns;
								//change current player
								//player_turn = which_player(turns);
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

		if (animating_trajectory) {
			//animate that shell_sprite

			if (shot_is_correct && shot_hit_something) {
				//game over, show victory screen + ending explosion
			}
			else if (shot_hit_something) {
				//show explosion
				animating_trajectory = false;
			}
		}
		else {
			if (key_enter) {
				//determine if the rocket thing will hit the other player
				//if so, shot_is_correct = true;
				//launch the rocket thing!
				//animating_trajectory = true

				if (player_turn == 0) {
					player_turn = 1;
				}
				else if (player_turn == 1) {
					player_turn = 0;
				}
				key_enter = false;
			}
			else {
				//tell sprites to advance by time
				for (std::vector<sprite*>::iterator it = ghost_sprites.begin(); it != ghost_sprites.end(); ++it) {
					//tell sprites to update themselves
					(*it)->sprite::advance_by_time(dt);
				}

				//wands rotating
				wand_sprites[player_turn]->sprite::advance_by_time(dt, key_up, key_down);
				
				
				//velocity stuff here
				if (key_left == true) {
					key_left = false;			
					if (velocity > 10) {
						velocity -= 10;
					}
				}
				
				if (key_right == true) {			
					key_right = false;
					if (velocity < 90) {
						velocity += 10;
					}
				}   
				
				
			}
		}
	}

	/**
	 * Draw the world. Note that the display variable is passed only
	 * because it might be needed to switch the target bitmap back to
	 * the backbuffer.
	 */
	void world::draw(ALLEGRO_DISPLAY *display) {
		//draw background
		al_draw_bitmap(background_image,0,0,0);

		//for loop with iterator to get sprites to load
		for (std::vector<sprite*>::iterator it = ghost_sprites.begin(); it != ghost_sprites.end(); ++it) {
			//draw sprites
			(*it)->draw(display);
		}

		for (std::vector<sprite*>::iterator it = wand_sprites.begin(); it != wand_sprites.end(); ++it) {
			//draw sprites
			(*it)->draw(display);
		}
		
		
		//DRAW THE DAMN VELOCITY TEXT HERE
		
		al_draw_textf(velocity_font, al_map_rgb(255,255,255), world::WIDTH/2, 40, ALLEGRO_ALIGN_CENTER, "Current velocity: %i", velocity); 
		
		

	}
	
}
