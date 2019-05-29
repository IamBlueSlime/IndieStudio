#include <irrlicht/irrlicht.h>
#include <iostream>
#include <string>
#include "indiestudio/utils/UniqueIrrPtr.hpp"
#include "ecs/Ecs.hpp"
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/BasicWorldGenerator.hpp"
#include "indiestudio/Game.hpp"
#include "indiestudio/SceneManager.hpp"
#include <cmath>

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

//     AnimatedMesh node(scenemg->getMesh("asset_deprecated/maps/gwendal_cube.obj"));

//     manager.setComponent(block, node);
// }

static void prep_travelling(irr::IrrlichtDevice *device, irr::scene::ISceneManager *scenemg)
{
	irr::core::array<irr::core::vector3df> points;
    irr::scene::ICameraSceneNode *cam = scenemg->getActiveCamera();
    irr::core::vector3df pos(cam->getAbsolutePosition());

	for (int y = 250; y >= 0; y -= 15) {
		points.push_back(irr::core::vector3df(pos.X, -y, pos.Z - sqrt(abs(pow(y, 2) - pow(250, 2)))));
	}
	cam->setPosition(irr::core::vector3df(pos.X, -300, pos.Z - 250));

	points.push_back(pos);
	irr::scene::ISceneNodeAnimator* sa = scenemg->createFollowSplineAnimator(device->getTimer()->getTime(), points, 3, 0.5F, false);
	cam->addAnimator(sa);
	sa->drop();

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

	int f = 1;
	int h = 13 * f;
	int w = 19 * f;

	BasicWorldGenerator gen;
	MapPattern pattern(w, h);
	gen.generate(pattern);


	irr::scene::IAnimatedMeshSceneNode* node =
		scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("assets/models/cube.obj"));



	/* WAVES */

    irr::scene::IAnimatedMesh* mesh = node->getMesh();
	mesh = scenemg->addHillPlaneMesh( "myHill",
		irr::core::dimension2d<irr::f32>(20,20),
		irr::core::dimension2d<irr::u32>(40,40), 0, 0,
		irr::core::dimension2d<irr::f32>(0,0),
		irr::core::dimension2d<irr::f32>(10,10));

	irr::scene::ISceneNode* node2 = 0;
	node2 = scenemg->addWaterSurfaceSceneNode(mesh->getMesh(0), 1.0f, 500.0f, 10.0f);
	node2->setPosition(irr::core::vector3df(150,7,20));

	node2->setMaterialTexture(0, driver->getTexture("assets/textures/water_stones.jpg"));
	node2->setMaterialTexture(1, driver->getTexture("assets/textures/water.jpg"));

	node2->setMaterialType(irr::video::EMT_REFLECTION_2_LAYER);



    scenemg->addSkyDomeSceneNode(driver->getTexture("asset_deprecated/sky-skydome.jpg"));



	node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
//	scenemg->getMeshManipulator()->makePlanarTextureMapping(node->getMesh(), 0.004F);

	irr::scene::ITriangleSelector* selector = 0;

	// irr::scene::ILightSceneNode*  pLight = device->getSceneManager()->addLightSceneNode();
	// irr::video::SLight & l = pLight->getLightData();
	// l.Type = irr::video::ELT_POINT;

	// float pd = 12.;
	// l.DiffuseColor = irr::video::SColorf(pd, pd, pd ,0.0);

	// irr::scene::ISceneNode* pNode = device->getSceneManager()->addEmptySceneNode();
	// pLight->setPosition(irr::core::vector3df(20 * 20, 20 * 20, 20 * -10)); //default is (1,1,0) for directional lights
	// pLight->setParent(pNode);

	scenemg->setShadowColor(irr::video::SColor(150, 0, 0, 0));

	float s = 20;

	// place map with node
	if (node) {
		node->setMaterialTexture(0, driver->getTexture("tmp/textures/block_ground_1.png"));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		node->setScale(irr::core::vector3df(s, s, s));
		node->setPosition(irr::core::vector3df(0.5, 50, 0.5));
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
				node->getPosition().Y + node->getScale().Y * (y == 1 ? 1 : 0),
				node->getPosition().Z + node->getScale().Z * z
			));

			if (c == MapPattern::GROUND_FIRST_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_1.png"));
			} else if (c == MapPattern::GROUND_SECOND_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_2.png"));
			} else if (c == MapPattern::BORDER_WALL_TILE || c == MapPattern::INNER_WALL_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_wall.png"));
			} else if (c == MapPattern::BREAKABLE_BLOCK_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_brick.png"));
			}
		});
	}



	irr::scene::ITriangleSelector* ninja = 0;

	// load 3d model animated
	irr::scene::IAnimatedMeshSceneNode* anms =
		scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("assets/models/player.md3"));
	if (anms) {
		// anms->addShadowVolumeSceneNode();
		// anms->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
		anms->setMaterialTexture(0, driver->getTexture("assets/textures/player_black.png"));
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

	irr::scene::ICameraSceneNode *camera = scenemg->addCameraSceneNode(0,
		irr::core::vector3df(s * (w / 2), 50 + s * (w / 1.5), s * 3),
		irr::core::vector3df(s * (w / 2), 50, s * (h / 2)));
		camera->setFarValue(10000);

	// prep_travelling(device, scenemg);

// SHADOW START

	irr::scene::ISceneNode* snode = 0;

	snode = scenemg->addLightSceneNode(0, irr::core::vector3df(355,100, 50),
		irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 5000.0f);
	irr::scene::ISceneNodeAnimator* anim = 0;
	// anim = scenemg->createFlyCircleAnimator (irr::core::vector3df(0,50,0),250.0f);
	// snode->addAnimator(anim);
	// anim->drop();

	snode = scenemg->addBillboardSceneNode(snode, irr::core::dimension2d<irr::f32>(50, 50));
	snode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	snode->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	snode->setMaterialTexture(0, driver->getTexture("asset_deprecated/particlewhite.bmp"));

	snode = scenemg->addLightSceneNode(0, irr::core::vector3df(0,-50, 0),
		irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 5000.0f);

// SHADOW END

	prep_travelling(device, scenemg);

	// game loop
	while (device->run()) {
		if (device->isWindowActive()) {

			// draw scene background
			driver->beginScene(true, true, irr::video::SColor(255,255,255,255), irr::video::SExposedVideoData());

			// draw all object

			// std::cout << "X: " << camera->getAbsolutePosition().X << std::endl;
			// std::cout << "Y: " << camera->getAbsolutePosition().Y << std::endl;
			// std::cout << "Z: " << camera->getAbsolutePosition().Z << std::endl;

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
	device->drop();

	return 0;
}