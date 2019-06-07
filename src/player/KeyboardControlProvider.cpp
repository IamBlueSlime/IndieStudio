/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** player KeyboardControlProvider.cpp
*/

#include "indiestudio/player/KeyboardControlProvider.hpp"

namespace IndieStudio {

    const std::string KeyboardControlProvider::ICON_PATH = "assets/textures/icons/keyboard.png";

    void KeyboardControlProvider::initConfigurationArea(irr::gui::IGUIEnvironment *guiEnv,
        irr::gui::IGUIElement *area, int playerIdx)
    {
        this->playerMappings[playerIdx] = {
            irr::EKEY_CODE::KEY_KEY_Z,
            irr::EKEY_CODE::KEY_KEY_Q,
            irr::EKEY_CODE::KEY_KEY_S,
            irr::EKEY_CODE::KEY_KEY_D,
            irr::EKEY_CODE::KEY_KEY_E
        };

        int dx = 20;

        guiEnv->addButton(irr::core::recti(
            {dx + 35, 0}, {dx + 35 + 30, 30}
        ), area, 4242 + (playerIdx * 10), L"?", L"Up key");

        guiEnv->addButton(irr::core::recti(
            {dx + 0, 35}, {dx + 30, 35 + 30}
        ), area, 4242 + (playerIdx * 10) + 1, L"?", L"Left key");

        guiEnv->addButton(irr::core::recti(
            {dx + 35, 35}, {dx + 35 + 30, 35 + 30}
        ), area, 4242 + (playerIdx * 10) + 2, L"?", L"Down key");

        guiEnv->addButton(irr::core::recti(
            {dx + 35 + 35, 35}, {dx + 35 + 35 + 30, 35 + 30}
        ), area, 4242 + (playerIdx * 10) + 3, L"?", L"Right key");

        guiEnv->addButton(irr::core::recti(
            {dx + 150, 17}, {dx + 150 + 30, 17 + 30}
        ), area, 4242 + (playerIdx * 10) + 4, L"?", L"Drop key");

        this->updateButtons(area, playerIdx);
    }

    bool KeyboardControlProvider::onConfigurationEvent(const irr::SEvent &event,
        irr::gui::IGUIElement *area, int playerIdx)
    {
        if (event.EventType == irr::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED) {
            irr::gui::IGUIButton *button = static_cast<irr::gui::IGUIButton *>(
                event.GUIEvent.Caller);
            
            if ((button->getID() - 4242) / 10 != playerIdx)
                return false;
            
            if (button->isPressed()) {
                button->setPressed(false);
                this->updateButtons(area, playerIdx);
            } else {
                button->setText(L"?");
                button->setPressed(true);
            }

            return true;
        } else if (event.EventType == irr::EET_KEY_INPUT_EVENT
        && this->isValidKeyboardKey(event.KeyInput.Key)
        && event.KeyInput.PressedDown) {
            for (int i = 0; i < 4; i += 1) {
                irr::EKEY_CODE *codes[5] = {
                    &this->playerMappings[i].keyUp,
                    &this->playerMappings[i].keyLeft,
                    &this->playerMappings[i].keyDown,
                    &this->playerMappings[i].keyRight,
                    &this->playerMappings[i].keyDrop
                };

                for (int j = 0; j < 5; j += 1) {
                    irr::gui::IGUIButton *button = static_cast<irr::gui::IGUIButton *>(
                        area->getElementFromId(4242 + (i * 10) + j));
                    
                    if (button == nullptr)
                        continue;

                    if (button->isPressed()) {
                        *codes[j] = event.KeyInput.Key;
                        button->setPressed(false);
                        this->updateButtons(area, i);
                        return true;
                    }
                }
            }
        }

        return false;
    }

    IControlProvider::Mappings KeyboardControlProvider::getPlayerMappings(int playerIdx) const
    {
        return this->playerMappings[playerIdx];
    }

    void KeyboardControlProvider::updateButtons(irr::gui::IGUIElement *area,
        int playerIdx) const
    {
        irr::EKEY_CODE codes[5] = {
            this->playerMappings[playerIdx].keyUp,
            this->playerMappings[playerIdx].keyLeft,
            this->playerMappings[playerIdx].keyDown,
            this->playerMappings[playerIdx].keyRight,
            this->playerMappings[playerIdx].keyDrop
        };

        for (int i = 0; i < 5; i += 1) {
            std::wstring tmp;
            tmp += codes[i];
            area->getElementFromId(4242 + (playerIdx * 10) + i)->setText(tmp.c_str());
        }
    }

    bool KeyboardControlProvider::isValidKeyboardKey(irr::EKEY_CODE code) const
    {
        for (int i = 0; i < 4; i += 1) {
            irr::EKEY_CODE codes[5] = {
                this->playerMappings[i].keyUp,
                this->playerMappings[i].keyLeft,
                this->playerMappings[i].keyDown,
                this->playerMappings[i].keyRight,
                this->playerMappings[i].keyDrop
            };

            for (int j = 0; j < 5; j += 1)
                if (codes[j] == code)
                    return false;
        }

        return true;
    }

}