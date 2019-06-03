/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** SceneManager
*/

#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
#include <string>
#include <irrlicht/irrlicht.h>
#include "indiestudio/common/TextureManager.hpp"
#include "indiestudio/common/MeshManager.hpp"

namespace IndieStudio {

	class SceneManager {
	public:
		static const std::string MAIN_MENU_ID;
		static const std::string NEW_GAME_ID;
		static const std::string LOAD_GAME_ID;
		static const std::string SETTINGS_ID;
		static const std::string PLAY_ID;

		struct Scene
		{
			explicit Scene(SceneManager *manager = 0, irr::scene::ISceneManager *scene = 0,
				irr::gui::IGUITab *gui = 0);
			Scene(const Scene &other);
			Scene &operator=(const Scene &other);
			SceneManager *manager;
			irr::scene::ISceneManager *scene;
			irr::gui::IGUITab *gui;
			std::function<bool(const irr::SEvent &)> onEvent =
				[](const irr::SEvent &) { return false; };
		};

		SceneManager();
		~SceneManager() = default;

		void init();

		void subscribeEvent(std::function<bool(const irr::SEvent &)> onEvent);
		Scene &createScene(const std::string &key);
		void setActiveScene(const std::string &key);
		Scene &getScene(const std::string &key);
		bool draw();

		const std::string &getActive() { return this->active; }
		const std::unordered_map<std::string, Scene> &getContainer()
			{ return this->container; }

		TextureManager textureManager;
		MeshManager meshManager;
	protected:
	private:
		class EventReceiver : public irr::IEventReceiver {
		public:
			EventReceiver(SceneManager &manager);

		private:
			SceneManager &manager;
			bool OnEvent(const irr::SEvent &event) override;
		};

		std::string active = "none";
		irr::scene::ISceneManager *sceneRoot;
		irr::gui::IGUIEnvironment *guiRoot;
		std::unordered_map<std::string, Scene> container;
		std::unique_ptr<EventReceiver> eventReceiver;
	};

}