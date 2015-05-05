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

	//for wand movement
	void sprite::advance_by_time(double dt, bool &key_up, bool &key_down) {
		
		//if sprites have been told to update, do something
		if (key_up == true) {
			angle = angle - .5;
			key_up = false;
		}
		if (key_down == true) {
			angle = angle + .5;
			key_down = false;
		}   
	}
	
	//for shell sprites
	void sprite::shell_advance_by_time(double dt, bool ok_to_fire) {
		//tell sprites what to do when time advances
		
		if (ok_to_fire == true) {				
			//update the x and y positions
			x = shell_x_location(x, dt);
			y = shell_y_location(y, dt);
		}
	}

	void sprite::set_sprite_image(ALLEGRO_BITMAP* initial_image) {
		sprite_image = initial_image;
	}
		
	//originals are in pseudocodesnippets.txt -- mostly I changed the functions from floats to voids
	//I think this is the best place to put it since we're changing the sprites x, y, vx, vy
	void sprite::initial_shell_velocity (float velocity_from_user, float angle) {

		vy = velocity_from_user * sin(angle);
		vx = velocity_from_user * cos(angle);
	}
	
	//there's no function for shell_x_velocity because vx won't change (accel_x was defined as 0)
	
	float sprite::shell_y_velocity (float incoming_vy, float dt) {
		//initial shell velocity (vy) has already been computed
		//should also be able to call this whenever time updates

		float accel_y = 300;
	
		vy = incoming_vy + (accel_y * dt);
		
		return vy;
	}

	float sprite::shell_x_location (float incoming_x_position, float dt) {
	
		x = incoming_x_position + (vx * dt);
		return x;
		
	}	

	float sprite::shell_y_location (float incoming_y_position, float dt) {
	
		y = incoming_y_position + (shell_y_velocity(vy, dt) * dt);
		return y;
	}
	
	/** Return my bitmap */
	ALLEGRO_BITMAP *sprite::get_bitmap() const {

	}
}
