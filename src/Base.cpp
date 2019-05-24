#include <irrlicht/irrlicht.h>
#include <iostream>
#include <string>
#include "indiestudio/utils/UniqueIrrPtr.hpp"

class MyClass : public irr::IEventReceiver {
    bool OnEvent(const irr::SEvent& event) override {
        // push dans notre event queue
        return true;
    }
};

// TODO: rm this monster

void menu(irr::IrrlichtDevice *device, irr::video::IVideoDriver* driver, irr::scene::ISceneManager* scenemg)
{
	(void) device;
	(void) driver;
	(void) scenemg;
}

int test()
{

	// std::cout
	// << "Selec camera type:" << std::endl
	// << "1 (static camera)" << std::endl
	// << "2 (FPS camera)" << std::endl
	// << "3 (Maya camera)" << std::endl
	// << std::endl;

	std::string cam = "2";
	// std::cin >> cam;

	// std::cout
	// << "Selec sky type:" << std::endl
	// << "1 (skybox)" << std::endl
	// << "2 (skydome)" << std::endl <<
	// std::endl;

	std::string sky = "1";
	// std::cin >> sky;

	int width = 1280;
	int height = 960;
	// create window (EDT_OPENGL= force irrlicth to use openGL, dimension2d<T> window x y)

	// IndieStudio::uniqueIrr_ptr<irr::IrrlichtDevice> device(
		irr::IrrlichtDevice *device =  irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(width, height));

	// if create window failed
	if (device == 0)
		return 1;

	// Set window name
    device->setWindowCaption(L"Indie Studio!");

	// get video driver (manage scene background and draw loop)
	irr::video::IVideoDriver* driver = device->getVideoDriver();

	// get scene Manager (manage camera, object like model 3d and animation)
	irr::scene::ISceneManager* scenemg = device->getSceneManager();

	menu(device, driver, scenemg);

	irr::scene::IAnimatedMeshSceneNode* node =
		scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("asset/maps/gwendal_cube.obj"));

	irr::scene::ITriangleSelector* selector = 0;

	// place map with node
	if (node) {
		node->setMaterialTexture(0, driver->getTexture("asset/wall.bmp"));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		node->setScale(irr::core::vector3df(20.f,20.f,20.f));
		node->setPosition(irr::core::vector3df(100, -80, 90));
		selector = scenemg->createOctreeTriangleSelector(node->getMesh(), node, 128);
		node->setTriangleSelector(selector);
		selector->drop();
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 20; ++j)
				node->clone()->setPosition(irr::core::vector3df(node->getPosition().X + node->getScale().X * j, -80, node->getPosition().Z + node->getScale().Z * i));
	}

	irr::scene::ITriangleSelector* ninja = 0;

	// load 3d model animated
	irr::scene::IAnimatedMeshSceneNode* anms =
		scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("asset/models/Character/Bomberman.MD3"));
	if (anms) {
		anms->setMaterialTexture(0, driver->getTexture("asset/models/Character/BlackBombermanTextures.png"));
		anms->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		anms->setScale(irr::core::vector3df(20.f,20.f,20.f));
		anms->setAnimationSpeed(10);
		anms->setPosition(irr::core::vector3df(100, node->getPosition().Y + node->getScale().X / 2, 90));
		ninja = scenemg->createTriangleSelector(anms);
		anms->setTriangleSelector(ninja);
		ninja->drop();
	}

	// get texture
	irr::video::ITexture *text = driver->getTexture((sky == "1") ? "asset/skybox.jpg" : "asset/skydome.jpg");

	if (sky == "1") {
		// thats a skybox so (param= top, bottom, left, right, front, back)
		scenemg->addSkyBoxSceneNode(text, text, text, text, text, text);
	} else if (sky == "2") {
		// same but skydome
		scenemg->addSkyDomeSceneNode(text);
	} else {
		return 0;
	}

	irr::scene::ICameraSceneNode *camera;

	if (cam == "1") {
		/*
		static camera that does not react to user input (param = parent, position, camera direction)
		if parent move, the cam will also move
		*/
		camera = scenemg->addCameraSceneNode(0, irr::core::vector3df(400, 1500, 1500), irr::core::vector3df(0, 0, 10));
	} else if (cam == "2") {
		// camera style that move like a basic fps (T manage by keyboard & R manage by mouse)

		irr::SKeyMap keyMap[5];
		keyMap[0].Action = irr::EKA_MOVE_FORWARD;
		keyMap[0].KeyCode = irr::KEY_UP;
		keyMap[1].Action = irr::EKA_MOVE_BACKWARD;
		keyMap[1].KeyCode = irr::KEY_DOWN;
		keyMap[2].Action = irr::EKA_STRAFE_LEFT;
		keyMap[2].KeyCode = irr::KEY_LEFT;
		keyMap[3].Action = irr::EKA_STRAFE_RIGHT;
		keyMap[3].KeyCode = irr::KEY_RIGHT;
		keyMap[4].Action = irr::EKA_JUMP_UP;
		keyMap[4].KeyCode = irr::KEY_SPACE;


		camera = scenemg->addCameraSceneNodeFPS(0, 100.0f, .3f, -1, keyMap, 5, true, 5.f);
		camera->setPosition(irr::core::vector3df(50,50,-60));

		// remove cursor from screen
		device->getCursorControl()->setVisible(false);
	} else if (cam == "3") {
		// camera style that move like a map editor (T & R manage by mouse)
		camera = scenemg->addCameraSceneNodeMaya();
	} else {
		return 0;
	}

	if (selector) {
		irr::scene::ISceneNodeAnimator* anim = scenemg->createCollisionResponseAnimator(
			selector, camera, irr::core::vector3df(30,50,30),
			irr::core::vector3df(0,-10,0), irr::core::vector3df(0,30,0));
		selector->drop(); // As soon as we're done with the selector, drop it.
		// camera->addAnimator(anim);
		anim->drop();
	}

	// create a minimap that see map from above
	irr::scene::ICameraSceneNode *miniMap = scenemg->addCameraSceneNode(0, irr::core::vector3df(0, 100, 0), irr::core::vector3df(100, -80, 90));
	scenemg->setActiveCamera(camera);

	int scale = 4;
	// game loop
	while (device->run()) {
		if (device->isWindowActive()) {

			// draw scene background
			driver->beginScene(true, true, irr::video::SColor(255,200,200,200), irr::video::SExposedVideoData());

			// draw all object

			// draw camera first
			driver->setViewPort(irr::core::rect<irr::s32>(0, 0, width, height));
			scenemg->drawAll();

			// draw minimap on top of camera
			driver->setViewPort(irr::core::rect<irr::s32>(width - (width / scale), height - (height / scale), width, height));
			scenemg->setActiveCamera(miniMap);
			// driver->beginScene(true, true, irr::video::SColor(255,200,0,0), irr::video::SExposedVideoData());

			scenemg->drawAll();

			// display method
			driver->endScene();

			// reset camera param
			scenemg->setActiveCamera(camera);
		} else {
			// best feature! just render nothing so that bring down processor usage by irrlicht
			device->yield();
		}
	}

	return 0;
}