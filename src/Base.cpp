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

/*
	bomberman anim speed = 30
	frame loop = 0, 27 walk
	frame loop = 27, 76 idle
	set loop node = true
*/


class MyClass : public irr::IEventReceiver {
public:
	MyClass(irr::scene::IAnimatedMeshSceneNode *player) { this->player = player; }
    bool OnEvent(const irr::SEvent& event) override
	{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.Key >= irr::KEY_LEFT && event.KeyInput.Key <= irr::KEY_DOWN) {
			move(event.KeyInput.Key);
			return true;
		}
		return false;
	}
		(void)event;
        return false;
    }
private:
	void move(irr::EKEY_CODE key)
	{
		irr::core::vector3df pos(player->getAbsolutePosition());
		float dist = 4;
		if (key == irr::KEY_LEFT) {
			pos.X -= dist;
			player->setRotation(irr::core::vector3df(0, 90, 0));
		}
		if (key == irr::KEY_RIGHT) {
			pos.X += dist;
			player->setRotation(irr::core::vector3df(0, 270, 0));
		}
		if (key == irr::KEY_UP) {
			pos.Z += dist;
			player->setRotation(irr::core::vector3df(0, 180, 0));
		}
		if (key == irr::KEY_DOWN) {
			pos.Z -= dist;
			player->setRotation(irr::core::vector3df(0, 0, 0));
		}
		player->setPosition(pos);
	}
	irr::scene::IAnimatedMeshSceneNode *player;
};

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

void water_maker(irr::scene::ISceneManager* scenemg, irr::video::IVideoDriver* driver, irr::scene::IAnimatedMeshSceneNode* node)
{
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
	scenemg->addSkyDomeSceneNode(driver->getTexture("assets/textures/skydome.jpg"));
	node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	scenemg->setShadowColor(irr::video::SColor(150, 0, 0, 0));
}

void map_generator(irr::scene::ISceneManager* scenemg, irr::video::IVideoDriver* driver, irr::scene::IAnimatedMeshSceneNode* node, int f, int h, int w, float s)
{
	(void)f;

	IndieStudio::BasicWorldGenerator gen;
	IndieStudio::MapPattern pattern(w, h);

	water_maker(scenemg, driver, node);
	gen.generate(pattern);
	irr::scene::ITriangleSelector* selector = 0;

	if (node) {
		node->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_1.png"));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		node->setScale(irr::core::vector3df(s, s, s));
		node->setPosition(irr::core::vector3df(0.5, 50, 0.5));
		// node->addShadowVolumeSceneNode();
		selector = scenemg->createOctreeTriangleSelector(node->getMesh(), node, 128);
		node->setTriangleSelector(selector);
		selector->drop();
		pattern.forEach([&](int x, int y, int z, char c) {
			if (c == IndieStudio::MapPattern::EMPTY_TILE)
				return;
			irr::scene::ISceneNode* tmp = node->clone();
			tmp->setPosition(irr::core::vector3df(
				node->getPosition().X + node->getScale().X * x,
				node->getPosition().Y + node->getScale().Y * (y == 1 ? 1 : 0),
				node->getPosition().Z + node->getScale().Z * z
			));
			if (c == IndieStudio::MapPattern::GROUND_FIRST_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_1.png"));
			} else if (c == IndieStudio::MapPattern::GROUND_SECOND_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_ground_2.png"));
			} else if (c == IndieStudio::MapPattern::BORDER_WALL_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_wall.png"));
			} else if (c == IndieStudio::MapPattern::INNER_WALL_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_wall.png"));
				tmp->setScale(tmp->getScale() / 1.1);
			} else if (c == IndieStudio::MapPattern::BREAKABLE_BLOCK_TILE) {
				tmp->setMaterialTexture(0, driver->getTexture("assets/textures/block_brick.png"));
				tmp->setScale(tmp->getScale() / 1.1);
			}
		});
	}
}

void players_initialisation(irr::scene::ISceneManager* scenemg, irr::video::IVideoDriver* driver, irr::scene::IAnimatedMeshSceneNode* node, int h, int w, float s)
{
	irr::scene::ITriangleSelector* ninja = 0;

	irr::scene::IAnimatedMeshSceneNode* anms = scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("assets/models/player.md3"), 0, 42);
	if (anms) {
		anms->setMaterialTexture(0, driver->getTexture("assets/textures/player_black.png"));
		anms->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		anms->setScale(irr::core::vector3df(s, s, s));
		anms->setAnimationSpeed(20);
		anms->setPosition(irr::core::vector3df(anms->getScale().X, node->getPosition().Y, anms->getScale().Z));
		ninja = scenemg->createTriangleSelector(anms);
		anms->setTriangleSelector(ninja);
		ninja->drop();
	}
	std::cout << "animated: " << anms << std::endl;
	std::cout << "node: " << scenemg->getSceneNodeFromId(42) << std::endl;
	anms->clone()->setPosition(irr::core::vector3df(anms->getScale().X * (w - 2), node->getPosition().Y, anms->getScale().Z));
	anms->clone()->setPosition(irr::core::vector3df(anms->getScale().X, node->getPosition().Y, anms->getScale().Z * (h - 2)));
	anms->clone()->setPosition(irr::core::vector3df(anms->getScale().X * (w - 2), node->getPosition().Y, anms->getScale().Z * (h - 2)));
}

void shadows_init(irr::scene::ISceneManager* scenemg, irr::video::IVideoDriver* driver)
{
	irr::scene::ISceneNode* snode = 0;
	snode = scenemg->addLightSceneNode(0, irr::core::vector3df(355,250, 50),
		irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 5000.0f);
	snode = scenemg->addBillboardSceneNode(snode, irr::core::dimension2d<irr::f32>(50, 50));
	snode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	snode->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	snode->setMaterialTexture(0, driver->getTexture("assets/textures/particlewhite.bmp"));
	snode = scenemg->addLightSceneNode(0, irr::core::vector3df(0,-50, 0),
		irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 5000.0f);
}

void draw3d_Markline(irr::video::IVideoDriver *driver, int lineLong)
{
	driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
	driver->draw3DLine(irr::core::vector3df(0, 0, 0), irr::core::vector3df(lineLong, 0, 0), irr::video::SColor(255, 255, 0, 0));
	driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
	driver->draw3DLine(irr::core::vector3df(0, 0, 0), irr::core::vector3df(0, lineLong, 0), irr::video::SColor(255, 0, 255, 0));
	driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
	driver->draw3DLine(irr::core::vector3df(0, 0, 0), irr::core::vector3df(0, 0, lineLong), irr::video::SColor(255, 0, 0, 255));
}

int game()
{
	irr::IrrlichtDevice *device = IndieStudio::Game::getDevice();
	if (device == 0)
		return 1;
	SceneManager manager;
	std::optional<SceneManager::Scene> scene = manager.createScene("test");
	if (scene == std::nullopt) {
		std::cout << "manager Failed" << std::endl;
		return 1;
	}
	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::scene::ISceneManager* scenemg = scene.value().scene;
	irr::scene::IAnimatedMeshSceneNode* node = scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("assets/models/cube.obj"));

	int f = 1;
	int h = 13 * f;
	int w = 19 * f;
	float s = 20;
	irr::scene::ICameraSceneNode *camera = scenemg->addCameraSceneNode(0,
		irr::core::vector3df(s * (w / 2), 50 + s * (w / 1.5), s * 3),
		irr::core::vector3df(s * (w / 2), 50, s * (h / 2)));
		camera->setFarValue(10000);
	map_generator(scenemg, driver, node, f, h, w, s);
	players_initialisation(scenemg, driver, node, h, w, s);
	shadows_init(scenemg, driver);
	// prep_travelling(device, scenemg);
	MyClass event((irr::scene::IAnimatedMeshSceneNode *)scenemg->getSceneNodeFromId(42));
	device->setEventReceiver(&event);


	// game loop
	while (device->run()) {
		if (device->isWindowActive()) {
			driver->beginScene(true, true, irr::video::SColor(255,255,255,255), irr::video::SExposedVideoData());
			draw3d_Markline(driver, 1000);
			manager.draw();
			driver->endScene();
		} else {
			device->yield();
		}
	}
	device->drop();
	return 0;
}