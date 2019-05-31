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