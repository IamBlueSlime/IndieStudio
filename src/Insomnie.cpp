/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Insomnie
*/

#include <irrlicht/irrlicht.h>
#include <iostream>
#include <random>

enum {
    BUTTON_ID_PLAY,
    BUTTON_ID_SETTING,
    BUTTON_ID_HOW_TO_PLAY,
    BUTTON_ID_QUIT
};

class EventReceiver : public irr::IEventReceiver {

public:
    EventReceiver(irr::IrrlichtDevice *device, bool *stop) { this->device = device; this->stop = stop; };

    bool OnEvent(const irr::SEvent& event) override {
        // std::cout << "Event" << std::endl;
        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            if (event.KeyInput.Key == irr::KEY_KEY_P && event.KeyInput.PressedDown) {
                std::cout << "P is pressed" << std::endl;
                std::cout << device->getSceneManager()->getSceneNodeFromId(42) << std::endl;
                int text, x, y, z;
                for (int i = 0; i < 100; i++) {
                x = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 985 * 15;
                y = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 985 * 15;
                z = rand() % 10 * (!(rand() % 2) ? 1 : -1) * rand() % 985 * 15;
                irr::scene::ISceneNodeAnimator *anim = device->getSceneManager()->createFlyStraightAnimator(
                    device->getSceneManager()->getSceneNodeFromId(42)->getAbsolutePosition(),
                    irr::core::vector3df(x, y, z),
                    1000000
                );
                irr::scene::ISceneNode *clone = device->getSceneManager()->getSceneNodeFromId(42)->clone();
                clone->addAnimator(anim);
                anim = device->getSceneManager()->createRotationAnimator(irr::core::vector3df(rand() % 3, rand() % 3, 1));
                clone->addAnimator(anim);
                text = rand() % 4;
                if (text == 0)
                    clone->setMaterialTexture(0, device->getVideoDriver()->getTexture("asset/models/Character/BlackBombermanTextures.png"));
                else if (text == 1)
                    clone->setMaterialTexture(0, device->getVideoDriver()->getTexture("asset/models/Character/PinkBombermanTextures.png"));
                else if (text == 2)
                    clone->setMaterialTexture(0, device->getVideoDriver()->getTexture("asset/models/Character/RedBombermanTextures.png"));
                else
                    clone->setMaterialTexture(0, device->getVideoDriver()->getTexture("asset/models/Character/WhiteBombermanTextures.png"));
                }
                return true;
            }
            if (event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
                device->closeDevice();
                return true;
            }
        }
        if (event.EventType == irr::EET_GUI_EVENT) {
            if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED && event.GUIEvent.Caller->getID() == BUTTON_ID_PLAY) {
                *stop = true;
                return true;
            }
            if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED && event.GUIEvent.Caller->getID() == BUTTON_ID_QUIT) {
                device->closeDevice();
                return true;
            }
            return false;
        }
        return false;
    }
private:
    irr::IrrlichtDevice *device;
    bool *stop;

};

void prep_button(irr::video::IVideoDriver* driver, irr::gui::IGUIEnvironment *env)
{
	env->addImage(driver->getTexture("title.png"), irr::core::position2d<int>(350, 75));

    irr::core::vector2di pos(515, 500);
    irr::gui::IGUIButton *button;
    env->getSkin()->setFont(env->getFont("fonts/VCR_OSD_MONO_1.001.bmp"));
    button = env->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}), 0, BUTTON_ID_PLAY, L"Play");
    button->setOverrideFont(env->getFont("fonts/VCR_OSD_MONO_1.001.ttf"));
    pos.Y += 60;
    env->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}), 0, BUTTON_ID_SETTING, L"Settings");
    pos.Y += 60;
    env->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}), 0, BUTTON_ID_HOW_TO_PLAY, L"How to Play");
    pos.Y += 60;
    env->addButton(irr::core::recti(pos, {pos.X + 225, pos.Y + 50}), 0, BUTTON_ID_QUIT, L"Quit");
}

void prep_travelling(irr::IrrlichtDevice *device, irr::scene::ISceneManager *scenemg)
{
    irr::core::array<irr::core::vector3df> points;
    irr::scene::ICameraSceneNode *cam = scenemg->getActiveCamera();
    irr::core::vector3df pos(cam->getAbsolutePosition());

    int step = 2;

    while (pos.X >= 0) {
        points.push_back(pos);
        pos.X -= step;
        pos.Z += step;
    }
    while (pos.X >= -50) {
        points.push_back(pos);
        pos.X -= step;
        pos.Z -= step;
    }
    while (pos.X <= 0) {
        points.push_back(pos);
        pos.X += step;
        pos.Z -= step;
    }
    while (pos.X <= 50) {
        points.push_back(pos);
        pos.X += step;
        pos.Z += step;
    }
	irr::scene::ISceneNodeAnimator* sa = scenemg->createFollowSplineAnimator(device->getTimer()->getTime(), points, 2);
	cam->addAnimator(sa);
	sa->drop();
}

void da_vinci_code(irr::IrrlichtDevice *device, irr::scene::ISceneManager* smgr)
{
	irr::video::IVideoDriver* driver = device->getVideoDriver();

	irr::scene::IAnimatedMesh* mesh = smgr->getMesh("asset/room.3ds");

	smgr->getMeshManipulator()->makePlanarTextureMapping(mesh->getMesh(0), 0.004f);

	irr::scene::ISceneNode* node = 0;

	node = smgr->addAnimatedMeshSceneNode(mesh);
	node->setMaterialTexture(0, driver->getTexture("asset/wall.jpg"));
	node->getMaterial(0).SpecularColor.set(0,0,0,0);

	/*
	Now, for the first special effect: Animated water. It works like this:
	The WaterSurfaceSceneNode takes a mesh as input and makes it wave like
	a water surface. And if we let this scene node use a nice material like
	the EMT_REFLECTION_2_LAYER, it looks really cool. We are doing this
	with the next few lines of code. As input mesh, we create a hill plane
	mesh, without hills. But any other mesh could be used for this, you
	could even use the room.3ds (which would look really strange) if you
	want to.
	*/

	mesh = smgr->addHillPlaneMesh( "myHill",
		irr::core::dimension2d<irr::f32>(20,20),
		irr::core::dimension2d<irr::u32>(40,40), 0, 0,
		irr::core::dimension2d<irr::f32>(0,0),
		irr::core::dimension2d<irr::f32>(10,10));

	node = smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 3.0f, 300.0f, 30.0f);
	node->setPosition(irr::core::vector3df(0,7,0));

	node->setMaterialTexture(0, driver->getTexture("asset/stones.jpg"));
	node->setMaterialTexture(1, driver->getTexture("asset/water.jpg"));

	node->setMaterialType(irr::video::EMT_REFLECTION_2_LAYER);

	/*
	The second special effect is very basic, I bet you saw it already in
	some Irrlicht Engine demos: A transparent billboard combined with a
	dynamic light. We simply create a light scene node, let it fly around,
	and to make it look more cool, we attach a billboard scene node to it.
	*/

	// create light

	node = smgr->addLightSceneNode(0, irr::core::vector3df(0,0,0),
		irr::video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
	irr::scene::ISceneNodeAnimator* anim = 0;
	anim = smgr->createFlyCircleAnimator (irr::core::vector3df(0,150,0),250.0f);
	node->addAnimator(anim);
	anim->drop();

	// attach billboard to light

	node = smgr->addBillboardSceneNode(node, irr::core::dimension2d<irr::f32>(50, 50));
	node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	node->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	node->setMaterialTexture(0, driver->getTexture("asset/particlewhite.bmp"));

	// create a particle system

	irr::scene::IParticleSystemSceneNode* ps =
		smgr->addParticleSystemSceneNode(false);

	irr::scene::IParticleEmitter* em = ps->createBoxEmitter(
		irr::core::aabbox3d<irr::f32>(-7,0,-7,7,1,7), // emitter size
		irr::core::vector3df(0.0f,0.06f,0.0f),   // initial direction
		80,100,                             // emit rate
		irr::video::SColor(0,255,255,255),       // darkest color
		irr::video::SColor(0,255,255,255),       // brightest color
		800,2000,0,                         // min and max age, angle
		irr::core::dimension2df(10.f,10.f),         // min size
		irr::core::dimension2df(20.f,20.f));        // max size

	ps->setEmitter(em); // this grabs the emitter
	em->drop(); // so we can drop it here without deleting it

	irr::scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

	ps->addAffector(paf); // same goes for the affector
	paf->drop();

	ps->setPosition(irr::core::vector3df(-70,60,40));
	ps->setScale(irr::core::vector3df(2,2,2));
	ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	ps->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("asset/fire.bmp"));
	ps->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);

	/*
	Next we add a volumetric light node, which adds a glowing fake area light to
	the scene. Like with the billboards and particle systems we also assign a
	texture for the desired effect, though this time we'll use a texture animator
	to create the illusion of a magical glowing area effect.
	*/
	irr::scene::IVolumeLightSceneNode * n = smgr->addVolumeLightSceneNode(0, -1,
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
		irr::scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(textures, 150);
		n->addAnimator(glow);

		// drop the animator because it was created with a create() function
		glow->drop();
	}

	mesh = smgr->getMesh("asset/dwarf.x");
	irr::scene::IAnimatedMeshSceneNode* anode = 0;

	anode = smgr->addAnimatedMeshSceneNode(mesh);
	anode->setPosition(irr::core::vector3df(-50,20,-60));
	anode->setAnimationSpeed(15);

	// add shadow
	anode->addShadowVolumeSceneNode();
	smgr->setShadowColor(irr::video::SColor(150,0,0,0));

	anode->setScale(irr::core::vector3df(2,2,2));
	anode->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

	irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNode();
	camera->setPosition(irr::core::vector3df(25.6F, 360.5F, -372.7F));
    camera->setTarget(anode->getAbsolutePosition());
	camera->setFarValue(10000.0f); // this increase a shadow visible range.

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	while(device->run())
    	if (device->isWindowActive())
    	{
    		driver->beginScene(true, true, 0);

    		smgr->drawAll();

    		driver->endScene();

    	}

	device->drop();

	return;
}

void intro()
{
    int width = 1280;
    int height = 960;

	irr::IrrlichtDevice *device = irr::createDevice(irr::video::EDT_OPENGL,
        irr::core::dimension2d<irr::u32>(width, height), 16, false, true);

    if (!device)
        return;

    device->setWindowCaption(L"Indie Studio!");

    device->getFileSystem()->addFileArchive("asset");

	irr::video::IVideoDriver* driver = device->getVideoDriver();

	irr::scene::ISceneManager* scenemg = device->getSceneManager();

    irr::gui::IGUIEnvironment *env = device->getGUIEnvironment();

    prep_button(driver, env);

    // irr::video::ITexture *text = driver->getTexture("/textures/skyboxes/skybox5.jpg");
    // scenemg->addSkyDomeSceneNode(driver->getTexture("/textures/skyboxes/skybox5.jpg"));
    // scenemg->addSkyBoxSceneNode(text, text, text, text, text, text);
    scenemg->addSkyDomeSceneNode(driver->getTexture("sky-skydome.jpg"));

    scenemg->addCameraSceneNode(0, irr::core::vector3df(50, 0, 0), irr::core::vector3df(0, 0, 0));

    irr::scene::IAnimatedMeshSceneNode* anms =
		scenemg->addAnimatedMeshSceneNode(scenemg->getMesh("asset/models/Character/Bomberman.MD3"), 0, 42);
	if (anms) {
		anms->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        anms->setMaterialTexture(0, driver->getTexture("asset/models/Character/BlackBombermanTextures.png"));
		anms->setScale(irr::core::vector3df(8.f,8.f,8.f));
		anms->setAnimationSpeed(18);
		anms->setPosition(irr::core::vector3df(0, 0, 0));
    }

    prep_travelling(device, scenemg);
    bool stop = 0;

    EventReceiver event(device, &stop);
    device->setEventReceiver(&event);

    while (device->run() && !stop) {
		if (device->isWindowActive()) {
			driver->beginScene(true, true, irr::video::SColor(255,200,200,200), irr::video::SExposedVideoData());

			scenemg->drawAll();
            env->drawAll();

            driver->endScene();
		} else {
			device->yield();
		}
	}

    if (device->run())
        da_vinci_code(device, scenemg->createNewSceneManager());

}