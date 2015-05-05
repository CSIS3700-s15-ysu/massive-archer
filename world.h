#ifndef WORLD_H
#define WORLD_H

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_primitives.h"

// for fonts
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

#include "sprite.h"

namespace csis3700 {
	class world {
		public:
			//static constants
			static const std::size_t WIDTH=800;
			static const std::size_t HEIGHT=600;

			/**
			 * Construct the world. The display is passed in simply to make it
			 * possible to modify options or access the backbuffer. DO NOT
			 * store the display in an instance variable. DO NOT start drawing
			 * on the screen. Just load bitmaps etc.
			 */
			world(ALLEGRO_DISPLAY *display, std::size_t window_width, std::size_t window_height);

			/**
			 * Free any resources being used by the world.
			 */
			~world();

			/**
			 * Block the copy constructor.  Worlds should not be copied to
			 * just assert(false)
			 */
			world(const world& other);

			/**
			 * Block operator =.  Worlds should not be assigned.
			 */
			world& operator =(const world& other);

			/**
			 * Update the state of the world based on the event ev.
			 */
			void handle_event(ALLEGRO_EVENT ev);

			bool bounding_box_collision(float b1_x, float b1_y);

			/**
			 * Advance the state of the world forward by the specified time.
			 */
			void advance_by_time(double dt);

			/**
			 * Draw the world. Note that the display variable is passed only
			 * because it might be needed to switch the target bitmap back to
			 * the backbuffer.
			 */
			void draw(ALLEGRO_DISPLAY *display);

		private:
			ALLEGRO_BITMAP *background_image;
			ALLEGRO_FONT *velocity_font;

			//stores our crap
			std::vector<sprite*> ghost_sprites;
			std::vector<sprite*> wand_sprites;
			std::vector<sprite*> shell_sprites;
			std::vector<sprite*> explosion_sprites;

			bool shot_hit_something;
			bool shot_is_correct;
			bool animating_trajectory;

			bool key_up;
			bool key_down;
			bool key_left;
			bool key_right;
			bool key_enter;
			bool key_space;

			bool player_turn;
			
			float velocity;

			int shells;

			int winner;
	};
}

#endif /* WORLD_H */
