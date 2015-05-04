/*
We're using the smart world, dumb sprite model

This is based on the set up from Lab 7

Added:
- Subclass structures
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <cstdlib>
#include <vector>

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_primitives.h"

// for fonts
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

namespace csis3700 {
	//stuff that applies to ALL sprites
	class sprite {
		public:
			/** Construct a sprite giving it a bitmap to display when it is
			* drawn and the position of its upper-left hand corner in the
			* world.  Also specify the initial velocity of the sprite. */

			//THIS CONSTRUCTOR IS TO BE USED FOR ALL SPRITES
			
			sprite(ALLEGRO_BITMAP* initial_image,
				std::size_t world_width,
				std::size_t world_height,
				float initial_x,
				float initial_y,
				int initial_angle,
				float initial_vx,
				float initial_vy,
				int intitial_conditions);

			/** Destructor */
			~sprite();

			/** these two should cause errors, no copying! */
			sprite(const sprite& other);
			sprite& operator =(const sprite& other);

			/** Draw this sprite. See world.draw for discussion of the display variable. */
			void draw(ALLEGRO_DISPLAY *display);

			/** Move time forward by the specified amount */
			void advance_by_time(double dt);

			/** Return my bitmap */
			ALLEGRO_BITMAP *get_bitmap() const;

			// Add other methods here
			/** this tells sprites that it's okay to move **/
			void go();

		protected:
			// Put your i-vars here
			ALLEGRO_BITMAP *sprite_image;
			std::size_t w_width;
			std::size_t w_height;
			float x;
			float y;
			int angle;
			float vx;
			float vy;
			bool move;
			int conditions;
	};

	class tank_sprite : public sprite {
		/** things specific to tank sprite:
		image shown
		*/
		public:
			
		protected:
			
	};

	class muzzle_sprite : public sprite {
		/** things specific to muzzle sprite:
		angle, image shown

		*/
		public:
			
		
		protected:
			
	};

	class shell_sprite : public sprite {
		/** things specific to shell sprite:

		*/
		public:

		protected:

	};

	class hit_sprite : public sprite {
		/** things specific to hit sprite:
		image shown (tank blown up/broken)
		*/
		public:
			
		protected:

	};

	class miss_sprite : public sprite {
		/** things specific to miss sprite:
		image shown (generic impact image?)
		*/
		public:
			
		protected:

	};
}

#endif /* SPRITE_H */