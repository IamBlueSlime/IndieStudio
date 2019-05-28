/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** mod ModManager.cpp
*/

#include <iostream>
#include <experimental/filesystem>
#include <sstream>
#include "indiestudio/Game.hpp"
#include "indiestudio/mod/ModManager.hpp"

namespace IndieStudio {

    namespace stdfs = std::experimental::filesystem;

    ModManager::ModManager(Game &game) : game(game), logger("modmanager")
    {}

    void ModManager::reload(const std::string &path)
    {
        this->flush();

        logger.info("Searching for mods in the '" + path + "' folder...");

        if (stdfs::is_directory(path)) {
            for (auto &fpath : stdfs::directory_iterator(path)) {
                if (stdfs::is_regular_file(fpath))
                    loadMod(fpath.path().string());
            }
        }

        std::stringstream ss;
        ss << "Found " << mods.size() << " mod" <<
            (mods.size() > 1 ? "s" : "") << ".";
        logger.info(ss.str());
    }

    void ModManager::flush()
    {
        for (auto it = this->mods.begin(); it != this->mods.end(); ++it)
            it->second->onDisable(this->game);
        
        this->mods.clear();

        for (LibraryWrapper &wrapper : this->modsLibraries)
            wrapper.close();

        this->modsLibraries.clear();
    }

    void ModManager::loadMod(const std::string &path)
    {
        LibraryWrapper wrapper;

        try {
            wrapper = this->openLibrary(path);
        } catch (const std::runtime_error &e) {
            this->logger.warning(e.what());
            return;
        }

        this->logger.debug("Looking for descriptor...");
        AMod::Descriptor *descriptor =
            wrapper.getSymbol<AMod::Descriptor *>("MOD_DESCRIPTOR");

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

        this->logger.debug("Instanciating mod...");
        AMod *mod = descriptor->entryPoint();
        std::unique_ptr<ILogger> modLogger = std::make_unique<Logger>(descriptor->id);
        mod->setLogger(modLogger);
        mods.insert(std::make_pair(descriptor, UAMod(mod)));

        logger.debug("Enabling mod '" + descriptor->name + "'.");
        mod->onEnable(this->game);
        logger.debug("Enabled mod '" + descriptor->name + "'.");

        logger.info("Loaded mod '" + descriptor->name + "' (" +
            descriptor->version + ") by " + descriptor->author + ".");
    }

    LibraryWrapper ModManager::openLibrary(const std::string &path)
    {
        logger.debug("Opening mod '" + path + "'...");
        LibraryWrapper wrapper;

        if (!wrapper.open(path)) {
            std::stringstream ss;
            ss << "Failed to load mod '" << path << "'";

#ifndef WIN32
            ss << " (" << dlerror() << ")";
#endif

            throw std::runtime_error(ss.str());
        }

        return wrapper;
    }

}