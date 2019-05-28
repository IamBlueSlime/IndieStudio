#include <irrlicht/irrlicht.h>
#include <iostream>
#include <string>
#include "indiestudio/utils/UniqueIrrPtr.hpp"
#include "ecs/Ecs.hpp"
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/BasicWorldGenerator.hpp"

using namespace IndieStudio;
using namespace Ecs::Component;

// Components registers in the manager
using EcsManager = Ecs::Manager<
Component1,
Component2,
Component3
//AnimatedMesh
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

	int width = 1280;
	int height = 720;
	// create window (EDT_OPENGL= force irrlicth to use openGL, dimension2d<T> window x y)

	// IndieStudio::uniqueIrr_ptr<irr::IrrlichtDevice> device(
	irr::IrrlichtDevice *device =  irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(width, height), 16, false, true);

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

	int f = 1;
	int h = 13 * f;
	int w = 19 * f;

	BasicWorldGenerator gen;
	MapPattern pattern(w, h);
	gen.generate(pattern);

	irr::scene::IAnimatedMeshSceneNode* node =
		scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("asset/maps/gwendal_cube.obj"));

	irr::scene::ITriangleSelector* selector = 0;

	irr::scene::ILightSceneNode*  pLight = device->getSceneManager()->addLightSceneNode();
	irr::video::SLight & l = pLight->getLightData();
	l.Type = irr::video::ELT_POINT;

	float pd = 12.;
	l.DiffuseColor = irr::video::SColorf(pd, pd, pd ,0.0);

	irr::scene::ISceneNode* pNode = device->getSceneManager()->addEmptySceneNode();
	pLight->setPosition(irr::core::vector3df(20 * 20, 20 * 20, 20 * -10)); //default is (1,1,0) for directional lights
	pLight->setParent(pNode);

	scenemg->setShadowColor(irr::video::SColor(150, 0, 0, 0));

	float s = 20;

	// place map with node
	if (node) {
		node->setMaterialTexture(0, driver->getTexture("tmp/textures/block_ground_1.png"));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		node->setScale(irr::core::vector3df(s, s, s));
		node->setPosition(irr::core::vector3df(0.5, 0, 0.5));
		// node->addShadowVolumeSceneNode();
		// selector = scenemg->createOctreeTriangleSelector(node->getMesh(), node, 128);
		// node->setTriangleSelector(selector);
		// selector->drop();

		pattern.forEach([&](int x, int y, int z, char c) {
			if (c == MapPattern::EMPTY_TILE)
				return;

			irr::scene::ISceneNode* tmp = node->clone();

			tmp->setPosition(irr::core::vector3df(
				node->getPosition().X + node->getScale().X * x,
				node->getScale().Y * (y == 1 ? 1 : 0),
				node->getPosition().Z + node->getScale().Z * z
			));

			if (c == MapPattern::GROUND_FIRST_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("tmp/textures/block_ground_1.png"));
			} else if (c == MapPattern::GROUND_SECOND_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("tmp/textures/block_ground_2.png"));
			} else if (c == MapPattern::BORDER_WALL_TILE || c == MapPattern::INNER_WALL_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("tmp/textures/block_wall.png"));
			} else if (c == MapPattern::BREAKABLE_BLOCK_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("tmp/textures/block_brick.png"));
			}
		});
	}

	irr::scene::ITriangleSelector* ninja = 0;

	// load 3d model animated
	irr::scene::IAnimatedMeshSceneNode* anms =
		scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("asset/models/Character/Bomberman.MD3"));
	if (anms) {
		anms->setMaterialTexture(0, driver->getTexture("asset/models/Character/BlackBombermanTextures.png"));
		anms->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		anms->setScale(irr::core::vector3df(s, s, s));
		anms->setAnimationSpeed(10);
		anms->setPosition(irr::core::vector3df(anms->getScale().X, node->getPosition().Y, anms->getScale().Z));
		ninja = scenemg->createTriangleSelector(anms);
		anms->setTriangleSelector(ninja);
		ninja->drop();
	}

	anms->clone()->setPosition(irr::core::vector3df(anms->getScale().X * (w - 2), node->getPosition().Y, anms->getScale().Z));
	anms->clone()->setPosition(irr::core::vector3df(anms->getScale().X, node->getPosition().Y, anms->getScale().Z * (h - 2)));
	anms->clone()->setPosition(irr::core::vector3df(anms->getScale().X * (w - 2), node->getPosition().Y, anms->getScale().Z * (h - 2)));

	// get texture
	// irr::video::ITexture *text = driver->getTexture((sky == "1") ? "asset/skybox.jpg" : "asset/skydome.jpg");
	// scenemg->addSkyBoxSceneNode(text, text, text, text, text, text);

	irr::scene::ICameraSceneNode *camera = scenemg->addCameraSceneNode(0,
		irr::core::vector3df(s * (w / 2), s * (w / 1.5), s * 3),
		irr::core::vector3df(s * (w / 2), 1, s * (h / 2)));

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
			driver->beginScene(true, true, irr::video::SColor(255,255,255,255), irr::video::SExposedVideoData());

			// draw all object

			// draw camera first
			scenemg->drawAll();
			int ll = 1000;
			driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
			driver->draw3DLine(irr::core::vector3df(0, 0, 0), irr::core::vector3df(ll, 0, 0), irr::video::SColor(255, 255, 0, 0));
			driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
			driver->draw3DLine(irr::core::vector3df(0, 0, 0), irr::core::vector3df(0, ll, 0), irr::video::SColor(255, 0, 255, 0));
			driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
			driver->draw3DLine(irr::core::vector3df(0, 0, 0), irr::core::vector3df(0, 0, ll), irr::video::SColor(255, 0, 0, 255));

			// draw minimap on top of camera
			// display method
			driver->endScene();

			// reset camera param
		} else {
			// best feature! just render nothing so that bring down processor usage by irrlicht
			device->yield();
		}
	}

	return 0;
}