/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** mod ModManager.cpp
*/

#include <filesystem>
#include <sstream>
#include "indiestudio/mod/ModManager.hpp"

namespace IndieStudio {

    ModManager::ModManager() : logger("modmanager")
    {}

    void ModManager::reload(const std::string &path)
    {
        logger.info("Searching for mods in the '" + path + "' folder...");

        if (std::filesystem::is_directory(path))
            for (auto &path : std::filesystem::directory_iterator(path))
                loadMod(path.path().string());

        std::stringstream ss;
        ss << "Found " << mods.size() << " mod" <<
            (mods.size() > 1 ? "s" : "") << ".";
        logger.info(ss.str());
    }

    void ModManager::loadMod(const std::string &path)
    {
        LibraryWrapper wrapper = this->openLibrary(path);
        IMod::Descriptor *descriptor =
            wrapper.getSymbol<IMod::Descriptor *>("MOD_DESCRIPTOR");

        if (descriptor == nullptr) {
            throw std::runtime_error(
                "Failed to find the descriptor in the mod '" + wrapper.getName() + "'!"
            );
        }

        for (auto it = mods.begin(); it != mods.end(); ++it) {
            if (it->first->name.compare(descriptor->name) == 0) {
                logger.debug("Mod '" + descriptor->name + "' already loaded, skipping.");
                return;
            }
        }

        modsLibraries.push_back(wrapper);
        IMod *mod = descriptor->entryPoint();
        mods.insert(std::make_pair(descriptor, UIMod(mod)));

        logger.debug("Enabling mod '" + descriptor->name + "'.");
        mod->onEnable();
        logger.debug("Enabled mod '" + descriptor->name + "'.");

        logger.info("Loaded mod '" + descriptor->name + "'.");
    }

    LibraryWrapper ModManager::openLibrary(const std::string &path)
    {
        logger.debug("Opening mod '" + path + "'...");
        LibraryWrapper wrapper;

        if (!wrapper.open(path)) {
            logger.error(dlerror());
            throw std::runtime_error("Failed to load mod '" + path + "'");
        }

        return wrapper;
    }

}