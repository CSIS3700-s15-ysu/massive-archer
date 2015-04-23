ALLEGRO_DIR=~cdshafferallegro
export PKG_CONFIG_PATH=$(ALLEGRO_DIR)libpkgconfig
CFLAGS=$(shell export PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) && pkg-config --cflags allegro-5 allegro_main-5 allegro_primitives-5 allegro_image-5 allegro_dialog-5 allegro_primitives-5 allegro_ttf-5)
LDFLAGS=$(shell export PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) && pkg-config --libs allegro-5 allegro_main-5 allegro_primitives-5 allegro_image-5 allegro_dialog-5 allegro_primitives-5 allegro_ttf-5)

main main.o sprite.o world.o
	g++ -o main main.o sprite.o world.o ${LDFLAGS}

main.o main.cxx sprite.h world.h
	g++ -c main.cxx ${CFLAGS}

sprite.o sprite.h sprite.cxx
	g++ -c sprite.cxx ${CFLAGS}

world.o world.h sprite.h world.cxx
	g++ -c world.cxx ${CFLAGS}

clean
	rm -f .o main