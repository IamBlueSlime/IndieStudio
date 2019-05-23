#include <irrlicht/irrlicht.h>
#include <iostream>
#include <string>

class MyClass : public irr::IEventReceiver {
    bool OnEvent(const irr::SEvent& event) override {
        // push dans notre event queue
        return true;
    }
};

// TODO: rm this monster

int test()
{

	std::cout
	<< "Selec camera type:" << std::endl
	<< "1 (static camera)" << std::endl
	<< "2 (FPS camera)" << std::endl
	<< "3 (Maya camera)" << std::endl
	<< std::endl;

	std::string cam;
	std::cin >> cam;

	std::cout
	<< "Selec sky type:" << std::endl
	<< "1 (skybox)" << std::endl
	<< "2 (skydome)" << std::endl <<
	std::endl;

	std::string sky;
	std::cin >> sky;

	int width = 1280;
	int height = 960;
	// create window (EDT_OPENGL= force irrlicth to use openGL, dimension2d<T> window x y)
	irr::IrrlichtDevice *device =
		irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(width, height));

	// if create window failed
	if (device == 0)
		return 1;

	// Set window name
    device->setWindowCaption(L"Indie Studio!");

	// get video driver (manage scene background and draw loop)
	irr::video::IVideoDriver* driver = device->getVideoDriver();

	// get scene Manager (manage camera, object like model 3d and animation)
	irr::scene::ISceneManager* scenemg = device->getSceneManager();

	// add archive in file system
	device->getFileSystem()->addFileArchive("asset/map-20kdm2.pk3");

	// get file from file system (20kdm2.bsp is in map-20kdm2.pk3)
	irr::scene::IAnimatedMesh* mesh = scenemg->getMesh("20kdm2.bsp");

	// (ISceneNode use for rendering complex geometry object and interact with it)
	irr::scene::ISceneNode* node = 0;

	// get node if getMesh successfully load "20kdm2.bdp" map
	if (mesh)
		node = scenemg->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);

	// place map with node
	if (node)
		node->setPosition(irr::core::vector3df(-1300,-144,-1249));

	// load 3d model animated
	irr::scene::IAnimatedMeshSceneNode* anms =
		scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("asset/ninja.b3d"));
	if (anms) {
		anms->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		anms->setScale(irr::core::vector3df(8.f,8.f,8.f));
		anms->setAnimationSpeed(10);
		anms->setPosition(irr::core::vector3df(100, -80, 90));
	}

		// get texture
	irr::video::ITexture *text = driver->getTexture((sky == "1") ? "asset/skybox.jpg" : "asset/skydome.jpg");

	if (sky == "1") {
		// that a skybpox so (param= top, bottom, left, right, front, back)
		scenemg->addSkyBoxSceneNode(text, text, text, text, text, text);
	} else if (sky == "2") {
		// same but skydome
		scenemg->addSkyDomeSceneNode(text);
	} else {
		device->drop();
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
		camera = scenemg->addCameraSceneNodeFPS();

		// remove cursor from screen
		device->getCursorControl()->setVisible(false);
	} else if (cam == "3") {
		// camera style that move like a map editor (T & R manage by mouse)
		camera = scenemg->addCameraSceneNodeMaya();
	} else {
		device->drop();
		return 0;
	}

	// create a minimap that see map from above
	irr::scene::ICameraSceneNode *miniMap = scenemg->addCameraSceneNode(0, irr::core::vector3df(0, 100, 0), irr::core::vector3df(100, -80, 90));


	int scale = 4;
	// game loop
	while (device->run()) {
		if (device->isWindowActive()) {

			// draw scene background
			driver->beginScene(true, true, irr::video::SColor(255,200,200,200));

			// draw all object

			// draw camera first
			driver->setViewPort(irr::core::rect<irr::s32>(0, 0, width, height));
			scenemg->setActiveCamera(camera);
			scenemg->drawAll();

			// draw minimap on top of camera
			driver->setViewPort(irr::core::rect<irr::s32>(width - (width / scale), height - (height / scale), width, height));
			scenemg->setActiveCamera(miniMap);
			scenemg->drawAll();

			// display method
			driver->endScene();

			// reset camera param
			driver->setViewPort(irr::core::rect<irr::s32>(0, 0, width, height));
			scenemg->setActiveCamera(camera);
		} else {
			// best feature! just render nothing so that bring down processor usage by irrlicht
			device->yield();
		}
	}

	// basically window close
	device->drop();
	return 0;
}