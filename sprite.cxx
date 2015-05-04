/*
We're using the smart world, dumb sprite model

This is still set up from Lab 7 -- lots of stuff will have to change

*/

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_primitives.h"

// for fonts
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

/*
sprite needs to:
- know its bitmap, position, velocity and acceleration
- update its variables when time moves forward
- use al_draw_bitmap when told to draw itself (its bitmap)
- have behaviours that depend on the application
*/

#include "sprite.h"

namespace csis3700 {
	sprite::sprite(ALLEGRO_BITMAP* initial_image,
				std::size_t world_width,
				std::size_t world_height,
				float initial_x,
				float initial_y,
				float initial_angle,
				float initial_vx,
				float initial_vy,
				float initial_center_x,
				float initial_center_y,
				int intitial_conditions) {
			//load images
			sprite_image = initial_image;
			w_width = world_width;
			w_height = world_height;
			x = initial_x;
			y = initial_y;
			angle = initial_angle;
			vx = initial_vx;
			vy = initial_vy;
			center_x = initial_center_x;
			center_y = initial_center_y;
			conditions = intitial_conditions;
			//tell sprites they aren't allowed to move by default
			move = false;	
		}

	sprite::~sprite() {
		al_destroy_bitmap(sprite_image);
	}

	/** these two should cause errors, no copying! */
	sprite::sprite(const sprite& other) {assert(false);}
	sprite& sprite::operator =(const sprite& other) {assert(false);}


	/** Draw this sprite. See world.draw for discussion of the display variable. */
	void sprite::draw(ALLEGRO_DISPLAY *display) {
		//sprites aren't allowed to move by default

		//draw a single sprite at x,y
		//al_draw_bitmap(sprite_image,x,y,0);

		al_draw_rotated_bitmap(sprite_image, center_x, center_y, x, y, angle, 0);
	}

	/** Move time forward by the specified amount */
	void sprite::advance_by_time(double dt) {
		//get sprite height and width
		int sprite_height=al_get_bitmap_height(get_bitmap());
		int sprite_width=al_get_bitmap_width(get_bitmap());
	
		//tell sprites what to do when time advances
		
		//if sprites haven't been told to move, do nothing
		
		//if sprites have been told to move, do something
		if (move == true) {
			//do something depending on sprite type
		}    
	}

	void sprite::advance_by_time(double dt, bool &key_up, bool &key_down) {
		//get sprite height and width
		int sprite_height=al_get_bitmap_height(get_bitmap());
		int sprite_width=al_get_bitmap_width(get_bitmap());
	
		//tell sprites what to do when time advances
		
		//if sprites haven't been told to move, do nothing
		
		//if sprites have been told to move, do something
		if (key_up == true) {
			angle = angle - .5;
			key_up = false;
		}
		if (key_down == true) {
			angle = angle + .5;
			key_down = false;
		}   
	}
	
	void sprite::velocity_advance_by_time(double dt, bool &key_right, bool &key_left) {
		//get sprite height and width
		int sprite_height=al_get_bitmap_height(get_bitmap());
		int sprite_width=al_get_bitmap_width(get_bitmap());
	
		//tell sprites what to do when time advances
		
		//if sprites haven't been told to move, do nothing
		
		//if sprites have been told to move, do something
		if (key_left == true) {
			velocity -= 100; //100 might be too big
			key_left = false;
			//do drawing here?
		}
		if (key_right == true) {
			velocity += 100; //100 might be too big
			key_right = false;
			//do drawing here?
		}   
	}
	
	
	
	
	void sprite::go() {
		//tell sprites it's okay to move
		move = true;
	}

	/** Return my bitmap */
	ALLEGRO_BITMAP *sprite::get_bitmap() const {

	}
}
