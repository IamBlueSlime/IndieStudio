/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** SceneManager
*/

#pragma none

#include <unordered_map>
#include <string>
#include <irrlicht/irrlicht.h>
#include "indiestudio/Game.hpp"

namespace IndieStudio {

	class SceneManager {
	public:
		struct Scene
		{
			explicit Scene(irr::scene::ISceneManager *scene = 0, irr::gui::IGUIEnvironment *gui = 0);
			Scene(const Scene &other);
			Scene &operator=(const Scene &other);
			irr::scene::ISceneManager *scene;
			irr::gui::IGUIEnvironment *gui;
		};

		SceneManager();
		~SceneManager() = default;

		Scene &createScene(const std::string &key);
		void setActiveScene(const std::string &key);
		Scene &getScene(const std::string &key);
		bool draw();

	protected:
	private:
		std::string active = "none";
		irr::scene::ISceneManager *sceneRoot;
		irr::gui::IGUIEnvironment *guiRoot;
		std::unordered_map<std::string, Scene> container;
	};

}