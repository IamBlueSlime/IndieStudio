#include <irrlicht/irrlicht.h>
#include <iostream>
#include <string>
#include "indiestudio/utils/UniqueIrrPtr.hpp"
#include "ecs/Ecs.hpp"
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/BasicWorldGenerator.hpp"
#include "indiestudio/Game.hpp"
#include "indiestudio/SceneManager.hpp"

using namespace IndieStudio;
using namespace Ecs::Component;

// Components registers in the manager
using EcsManager = Ecs::Manager<
Component1,
Component2,
Component3
>;

class MyClass : public irr::IEventReceiver {
    bool OnEvent(const irr::SEvent& event) override {
		(void)event;
		std::cout << "Event" << std::endl;
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

// int map_maker(EcsManager manager, irr::scene::ISceneManager* scenemg, irr::IrrlichtDevice *device) {
//     auto &block = manager.addEntity();

//     AnimatedMesh node(scenemg->getMesh("asset/maps/gwendal_cube.obj"));

//     manager.setComponent(block, node);
// }

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

	// create window (EDT_OPENGL= force irrlicth to use openGL, dimension2d<T> window x y)

	// IndieStudio::uniqueIrr_ptr<irr::IrrlichtDevice> device(
	irr::IrrlichtDevice *device = IndieStudio::Game::getDevice();

	SceneManager manager;

	std::optional<SceneManager::Scene> scene = manager.createScene("test");
	if (scene == std::nullopt) {
		std::cout << "manager" << std::endl;
		return 1;
	}

	// if create window failed
	if (device == 0)
		return 1;

	// get video driver (manage scene background and draw loop)
	irr::video::IVideoDriver* driver = device->getVideoDriver();

	// get scene Manager (manage camera, object like model 3d and animation)
	// irr::scene::ISceneManager* scenemg = device->getSceneManager();
	irr::scene::ISceneManager* scenemg = scene.value().scene;

	menu(device, driver, scenemg);

	BasicWorldGenerator gen;
	MapPattern pattern(19, 13);
	gen.generate(pattern);


	irr::scene::IAnimatedMeshSceneNode* cube =
		scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("asset/maps/gwendal_cube.obj"));

	cube->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	scenemg->getMeshManipulator()->makePlanarTextureMapping(cube->getMesh(), 0.004F);

	irr::scene::ITriangleSelector* selector = 0;

	// place map with cube
	if (cube) {
		cube->setMaterialTexture(0, driver->getTexture("asset/maps/clean_grass.png"));
		cube->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		cube->setScale(irr::core::vector3df(20.f,20.f,20.f));
		cube->setPosition(irr::core::vector3df(0, -20 + cube->getScale().X, 0));
		selector = scenemg->createOctreeTriangleSelector(cube->getMesh(), cube, 128);
		cube->setTriangleSelector(selector);
		selector->drop();

		pattern.forEach([&](int y, int x, char c) {
			irr::scene::ISceneNode* tmp = cube->clone();

			tmp->setPosition(irr::core::vector3df(
				cube->getPosition().X + cube->getScale().X * x,
				cube->getScale().Y * (c != MapPattern::EMPTY_TILE ? 1 : 0),
				cube->getPosition().Z + cube->getScale().Z * y
			));

			if (c == MapPattern::EMPTY_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("asset/maps/grass.png"));
				tmp->getMaterial(0).SpecularColor.set(0, 0, 0, 0);
			} else if (c == MapPattern::BORDER_WALL_TILE || c == MapPattern::INNER_WALL_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("asset/maps/my_wall.jpg"));
				tmp->getMaterial(0).SpecularColor.set(0, 0, 0, 0);
			} else if (c == MapPattern::BREAKABLE_BLOCK_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("asset/maps/cubre.jpg"));
				tmp->getMaterial(0).SpecularColor.set(0, 0, 0, 0);
			}
		});

		cube->addShadowVolumeSceneNode();
		scenemg->setShadowColor();
		// for (int i = 0; i < 13; ++i)
		// 	for (int j = 0; j < 19; ++j) {
		// 		if (!i && !j)
		// 			continue;
		// 		irr::scene::ISceneNode* tmp = cube->clone();
		// 		tmp->setPosition(irr::core::vector3df(node->getPosition().X + node->getScale().X * j, node->getScale().Y, node->getPosition().Z + node->getScale().Z * i));
		// 		if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))
		// 			tmp->setMaterialTexture(0, driver->getTexture("asset/maps/grass.png"));
		// 		if (i == 0 || i == 12 || j == 0 || j == 18) {
		// 			tmp->setPosition(irr::core::vector3df(node->getPosition().X + node->getScale().X * j, 2 * node->getScale().X, node->getPosition().Z + node->getScale().Z * i));
		// 			tmp->setMaterialTexture(0, driver->getTexture("asset/maps/my_wall.jpg"));
		// 		}
		// 	}
		// node->setPosition(irr::core::vector3df(node->getPosition().X, 2 * node->getScale().X, node->getPosition().Z));
		// node->setMaterialTexture(0, driver->getTexture("asset/maps/my_wall.jpg"));
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
		anms->setPosition(irr::core::vector3df(anms->getScale().X, cube->getPosition().Y, anms->getScale().Z));
		ninja = scenemg->createTriangleSelector(anms);
		anms->setTriangleSelector(ninja);
		ninja->drop();
	}

	irr::scene::ISceneNode *node = scenemg->addLightSceneNode(0, irr::core::vector3df(0,0,0),
		irr::video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
	irr::scene::ISceneNodeAnimator* anim = 0;
	anim = scenemg->createFlyCircleAnimator (irr::core::vector3df(0,150,0),250.0f);
	node->addAnimator(anim);
	anim->drop();

	// attach billboard to light

	node = scenemg->addBillboardSceneNode(node, irr::core::dimension2d<irr::f32>(50, 50));
	node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	node->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	node->setMaterialTexture(0, driver->getTexture("asset/particlewhite.bmp"));

	// create a particle system

	// irr::scene::IParticleSystemSceneNode* ps =
	// 	scenemg->addParticleSystemSceneNode(false);

	// irr::scene::IParticleEmitter* em = ps->createBoxEmitter(
	// 	irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // emitter size
	// 	irr::core::vector3df(0.0f,0.06f,0.0f),   // initial direction
	// 	80,100,                             // emit rate
	// 	irr::video::SColor(0,255,255,255),       // darkest color
	// 	irr::video::SColor(0,255,255,255),       // brightest color
	// 	800,2000,0,                         // min and max age, angle
	// 	irr::core::dimension2df(10.f,10.f),         // min size
	// 	irr::core::dimension2df(20.f,20.f));        // max size

	// ps->setEmitter(em); // this grabs the emitter
	// em->drop(); // so we can drop it here without deleting it

	// irr::scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

	// ps->addAffector(paf); // same goes for the affector
	// paf->drop();

	// ps->setPosition(irr::core::vector3df(-70,60,40));
	// ps->setScale(irr::core::vector3df(2,2,2));
	// ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	// ps->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	// ps->setMaterialTexture(0, driver->getTexture("asset/fire.bmp"));
	// ps->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

	/*
	Next we add a volumetric light node, which adds a glowing fake area light to
	the scene. Like with the billboards and particle systems we also assign a
	texture for the desired effect, though this time we'll use a texture animator
	to create the illusion of a magical glowing area effect.
	*/
	irr::scene::IVolumeLightSceneNode * n = scenemg->addVolumeLightSceneNode(0, -1,
				32,                              // Subdivisions on U axis
				32,                              // Subdivisions on V axis
				irr::video::SColor(0, 255, 255, 255), // foot color
				irr::video::SColor(0, 0, 0, 0));      // tail color

	if (n)
	{
		n->setScale(irr::core::vector3df(56.0f, 56.0f, 56.0f));
		n->setPosition(irr::core::vector3df(-120,50,40));

		// load textures for animation
		irr::core::array<irr::video::ITexture*> textures;
		for (irr::s32 g=7; g > 0; --g)
		{
			irr::core::stringc tmp;
			tmp = "asset/portal";
			tmp += g;
			tmp += ".bmp";
			irr::video::ITexture* t = driver->getTexture( tmp.c_str() );
			textures.push_back(t);
		}

		// create texture animator
		irr::scene::ISceneNodeAnimator* glow = scenemg->createTextureAnimator(textures, 150);
		n->addAnimator(glow);

		// drop the animator because it was created with a create() function
		glow->drop();
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

	if (cam == "2") {
		/*
		static camera that does not react to user input (param = parent, position, camera direction)
		if parent move, the cam will also move
		*/
		camera = scenemg->addCameraSceneNode(0, irr::core::vector3df(184, 330, -42), irr::core::vector3df(185, 50, 110));
		camera->setFarValue(10000);
		// camera->setRotation(irr::core::vector3df(0, 0, 0));
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


		camera = scenemg->addCameraSceneNodeFPS(0, 100.0f, .3f, -1, keyMap, 5, false, 5.f);
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

	// game loop
	while (device->run()) {
		if (device->isWindowActive()) {

			// draw scene background
			driver->beginScene(true, true, irr::video::SColor(255,200,200,200), irr::video::SExposedVideoData());

			// draw all object

			// draw camera first
			// scenemg->drawAll();
			manager.draw();

			// draw minimap on top of camera
			// display method
			driver->endScene();

			// reset camera param
		} else {
			// best feature! just render nothing so that bring down processor usage by irrlicht
			device->yield();
		}
	}
	device->drop();

	return 0;
}