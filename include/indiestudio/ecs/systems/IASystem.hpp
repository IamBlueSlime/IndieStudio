/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IA
*/

#pragma once

#include "indiestudio/ecs/Events.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Event;

    template<typename ManagerType>
    class IASystem : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override {
            (void) world;

            manager.template forEntitiesWith<IA>(
                [&manager, this](auto &data, auto id) {
                    // Component, containing :
                    //  - the state (action currently executed or 'nothing')
                    //

                    //auto &ia = manager.template getComponent<IA>(data);

                    // get the tile map
//                    auto &tilemap = getfromworld,scene,oreventsingleton
                        // tile map types :
                        //  - player
                        //  - bomb
                        //  - explosion
                        //  - powerup (any kind)
                        //  - mur destructible
                        //  - mur indestructible

                    if (this->emergency_move()) {
                        // cancel current action
                    } else {
                        // si il n y a pas d action en cours :
                            // decider entre
                            //  - ramasser powerup (si impossible, reroll)
                            //  - attaquer (to define) (si impossible, reroll)
                            //  - briser mur (si impossible, reroll)
                            // et set la current_action correctement
                        // executer la current action
                    }
                }
            );
        }

        bool emergency_move() {
            // dodge les cases en explosions
            // dodge le rayon d action des bombes "endormies"
            // renvoie true si il y a quelque chose à eviter
            // renvoie false sinon

            // This removes a warning
            return false;
        }

        IA::Action select_action() {
            int rand = std::rand() % 3;

            switch (rand) {
                case 0: return IA::Action::ATK;
                case 1: return IA::Action::WALL;
                case 2: return IA::Action::PICKUP;
            }
        }

        void execute_action(IA::Action action) {
            switch (action) {
                case IA::Action::ATK: return atk_player();
                case IA::Action::WALL: return destroy_wall();
                case IA::Action::PICKUP: return pick_powerup();
            }
        }

        void atk_player() {}

        void destroy_wall() {}

        void pick_powerup() {}

    protected:
    private:
    };
}
