#ifndef INCLUDED_UITEST_CONTROLLER
#define INCLUDED_UITEST_CONTROLLER

#include "../luxa/ComponentManager.h"

#include <command/CommandDirectory.h>
#include <hookah/Window.h>
#include <input/KeyboardDevice.h>
#include <input/MouseDevice.h>
#include <gui/InputEventAdapter.h>

class Controller
{
	public:
		Controller();
		~Controller();

		bool run();

	private:
		boost::shared_ptr<Luxa::ComponentManager> cm_;
		boost::shared_ptr<Hookah::Window> window_;
		boost::shared_ptr<v3D::KeyboardDevice> keyboard_;
		boost::shared_ptr<v3D::MouseDevice> mouse_;
		boost::shared_ptr<InputEventAdapter> listenerAdapter_;
		v3D::CommandDirectory directory_;
};


#endif // INCLUDED_UITEST_CONTROLLER
