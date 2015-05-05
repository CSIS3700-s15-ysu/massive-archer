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
		animating_trajectory = false;
		shot_is_correct = false;
		shot_hit_something = false;
		player_turn = 0;
		float x, y;
		float angle;
		float center_x;
		float center_y;
		shells = 0;
		winner = 0;
		velocity = 500;

		//set the default key states
		key_up = false;
		key_down = false;
		key_left = false;
		key_right = false;
		key_enter = false;
		key_space = false;
		
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
				x = 190; //image is 180px wide, ghost is 80px wide
				y = (world::HEIGHT * 0.75) + 35; //image is 70px tall
				angle = 0;
				center_x = 90;
				center_y = 35;
			}
			else {
				//i is equal to 2
				//draw second tank
								
				x = world::WIDTH - 190;
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
	 * Update the state of the world based on the event ev.
	 */
	void world::handle_event(ALLEGRO_EVENT ev) {
		
		if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					//up arrow was released
					key_up = true;
			
				break;

				case ALLEGRO_KEY_DOWN:
					//down arrow was released
					key_down = true;

				break;

				case ALLEGRO_KEY_RIGHT:
					//right arrow was released
					key_right = true;

				break;

				case ALLEGRO_KEY_LEFT:
					//left arrow was released
					key_left = true;
			
				break;

				case ALLEGRO_KEY_ENTER:
					//enter key was released
					key_enter = true;
						
				break;
				
				case ALLEGRO_KEY_SPACE:
					//space bar was released
					key_space = true;
						
				break;
			}
		}
	}
	
	bool world::bounding_box_collision(float b1_x, float b1_y) {
		float b1_w, b1_h, b2_x, b2_y, b2_w, b2_h, b3_x, b3_y, b3_w, b3_h;
		
		b1_w = 50;
		b1_h = 50;
		b2_x = 100; //location of player 1 ghost
		b2_y = world::HEIGHT * 0.75;
		b2_w = 80;
		b2_h = 76;
		b3_x = world::WIDTH - 180;
		b3_y = b2_y;
		b3_w = b2_w;
		b3_h = b2_h;
		
		//b1 is shell, b2 is player 1 ghost, b3 is player 2 ghost
		
    	if ((b1_x > b2_x + b2_w - 1) || // is b1 on the right side of b2?
        	(b1_y > b2_y + b2_h - 1) || // is b1 under b2?
        	(b2_x > b1_x + b1_w - 1) || // is b2 on the right side of b1?
        	(b2_y > b1_y + b1_h - 1) || // is b2 under b1?
        	(b1_x > b3_x + b3_w - 1) ||	// is b1 on the right side of b3?
        	(b1_y > b3_y + b3_h - 1) || // is b1 under b3?
        	(b3_x > b1_x + b1_w - 1) || // is b3 on the right side of b1?
        	(b3_y > b1_y + b1_h - 1))	// is b3 under b1?
    	{
        	// no collision
        	return 0;
    	}
 
    	// collision
    	return 1;
	}

	/**
	 * Advance the state of the world forward by the specified time.
	 */
	void world::advance_by_time(double dt) {    
		// the world itself doesn't do anything when time is just advancing

		if (animating_trajectory) {
			//animate that shell_sprite
			shell_sprites[shells]->sprite::shell_advance_by_time(dt, animating_trajectory);
			float sprite_x, sprite_y, sprite_w, sprite_h, tank_x, tank_y, tank_w, tank_h;
			
			//I need the current x and y values here
			sprite_x = shell_sprites[shells]->sprite::return_x();
			sprite_y = shell_sprites[shells]->sprite::return_y();

			if (sprite_y > 600) {
				shot_hit_something = true;
			}

			if (player_turn == 0) {
				//trying to hit player 2
				if (sprite_x > world::WIDTH - 250 && 
					sprite_x < world::WIDTH - 100 &&
					sprite_y > world::HEIGHT * 0.70 &&
					sprite_y < world::HEIGHT * 0.80
					) {
					shot_is_correct = true;
					shot_hit_something = true;

					ALLEGRO_BITMAP *ghosthit = al_load_bitmap("ghosthit.png");
					ghost_sprites[1]->set_sprite_image(ghosthit);

					winner = 1;
				}
			}
			else if (player_turn == 1) {
				//trying to hit player 1
				if (sprite_x > 100 &&
					sprite_x < 180 &&
					sprite_y > world::HEIGHT * 0.70 &&
					sprite_y < world::HEIGHT * 0.80
					) {
					shot_is_correct = true;
					shot_hit_something = true;

					ALLEGRO_BITMAP *ghosthit = al_load_bitmap("ghosthit.png");
					ghost_sprites[0]->set_sprite_image(ghosthit);

					winner = 2;
				}
			}
			
			if (shot_hit_something) {
				//show explosion
				animating_trajectory = false;
				shot_hit_something = false;
				velocity = 500;
				shells++;

				ALLEGRO_BITMAP *ghosthit = al_load_bitmap("miss_sprite.png");
				explosion_sprites.push_back(
					new sprite(
						ghosthit,
						world::WIDTH,
						world::HEIGHT,
						sprite_x,
						world::HEIGHT - 50,
						0,
						0,
						0,
						0,
						0,
						0
					)
				);
				
				if (player_turn == 0) {
					player_turn = 1;
				}
				else if (player_turn == 1) {
					player_turn = 0;
				}
			}
		}
		else {
			if (key_space == true) {
				//shells
				//loads shell sprites image and tells it where to draw
				//NOT ACTUALLY DRAWING
				ALLEGRO_BITMAP *shell_image = al_load_bitmap("shell_sprite.png");
				float x, y;
				//these should draw somewhat close to the tip of the wands

				if (player_turn == 0) {
					//player 1 shell
					x = 110;
					y = (world::HEIGHT * 0.75);
				}
				else if (player_turn == 1) {
					//player 2 shell		
					x = world::WIDTH - 180;
					y = (world::HEIGHT * 0.75) + 20;
				}

				shell_sprites.push_back(
					new sprite(
						shell_image,
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
				
				//set initial velocity
				shell_sprites[shells]->sprite::initial_shell_velocity(velocity, wand_sprites[player_turn]->return_angle());
				
				//launch the rocket thing!
				animating_trajectory = true;

				key_space = false;
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
					if (velocity >= 200) {
						velocity -= 100;
					}
				}
				
				if (key_right == true) {			
					key_right = false;
					if (velocity <= 900) {
						velocity += 100;
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

		for (std::vector<sprite*>::iterator it = explosion_sprites.begin(); it != explosion_sprites.end(); ++it) {
			//draw sprites
			(*it)->draw(display);
		}
		
		//shell sprites need to be drawn here, otherwise I can't get them to show up on the screen AT ALL
		//sorry, not sorry
		if (animating_trajectory == true) {
			shell_sprites[shells]->draw(display);		
		}

		if (winner == 1) {
			al_draw_textf(velocity_font, al_map_rgb(255,255,255), world::WIDTH/2, 40, ALLEGRO_ALIGN_CENTER, "THE WINNER IS 1", velocity);
		}
		else if (winner == 2) {
			al_draw_textf(velocity_font, al_map_rgb(255,255,255), world::WIDTH/2, 40, ALLEGRO_ALIGN_CENTER, "THE WINNER IS 2", velocity);
		}
		else {
			//DRAW THE DAMN VELOCITY TEXT HERE
			al_draw_textf(velocity_font, al_map_rgb(255,255,255), world::WIDTH/2, 40, ALLEGRO_ALIGN_CENTER, "Current velocity: %g", velocity);
		}	
	}
}
