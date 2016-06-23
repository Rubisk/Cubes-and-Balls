#ifndef CUBES_AND_BALLS_SRC_RENDERER_DRAWER_H_
#define CUBES_AND_BALLS_SRC_RENDERER_DRAWER_H_


// Abstract base class for anything that wants to draw onto the screen.
// Child objects should be added to the Renderer in order to have their
// Draw function called.
class Drawer {
public:
	virtual void Draw() = 0;

	virtual ~Drawer() {};
};

#endif