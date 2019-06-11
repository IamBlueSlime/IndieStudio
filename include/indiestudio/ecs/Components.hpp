/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs Components.hpp
*/

#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <functional>
#include <ctime>

#include "indiestudio/ecs/Events.hpp"

namespace IndieStudio::ECS::Component {

    using namespace IndieStudio::ECS::Event;

    struct Position {
        Position(float _x = 0.0, float _y = 0.0, float _z = 0.0)
        :
        x(_x),y(_y),z(_z) {}

        float x;
        float y;
        float z;
    };

    struct Speed {
        Speed(float _x = 0.0, float _y = 0.0, float _z = 0.0)
        :
        x(_x), y(_y), z(_z) {}

        float x;
        float y;
        float z;
    };

    struct Alive {
    };

    struct Drawable {
    };

    struct Movable {
    };

    struct Indestructible {
    };

    struct LifeTime {
        LifeTime(int _lifeTime = std::time(nullptr))
        :
        lifeTime(_lifeTime) {}

//TODO: std::chrono
        int lifeTime;
    };

    struct ExplosionLifeTime {
        ExplosionLifeTime(int _explosionLifeTime = std::time(nullptr))
        :
        explosionLifeTime(_explosionLifeTime) {}

        int explosionLifeTime;
    };

    struct IsExploding {
    };

    struct ExplosionRange {
        ExplosionRange(float _explosionRange = 3.0f)
        :
        explosionRangeUp(_explosionRange),
        explosionRangeLeft(_explosionRange),
        explosionRangeDown(_explosionRange),
        explosionRangeRight(_explosionRange) {}

        float explosionRangeUp;
        float explosionRangeLeft;
        float explosionRangeDown;
        float explosionRangeRight;
    };

    struct BombType {
        enum {
            NORMAL = 1,
            CIRCULAR = 2,
            MEGA = 4
        };

        BombType(int _bombType = BombType::NORMAL)
        :
        bombType(_bombType) {}

        int bombType;
    };

    struct RealPlayer {
    };

    struct IsPlayer {
    };

    struct IsBomb {
    };

    struct IsPowerUp {
        enum {
            SPEED_POWERUP = 0,
            STOCK_POWERUP = 1,
            RANGE_POWERUP = 2
        };

        IsPowerUp(int powerUpType = 0)
        :
        type(powerUpType) {}

        int type;
    };

    struct IsBlock {
    };

    struct Given {
    };

    struct SoundID {
        SoundID(int _soundID = 0)
        :
        soundID(_soundID) {}

        int soundID;
    };

    struct Direction {
        enum {
            STAY = 1,
            UP = 2,
            LEFT = 4,
            DOWN = 8,
            RIGHT = 16
        };

        Direction(int _direction = Direction::STAY)
        :
        direction(_direction) {}

        int direction;
    };

    struct Stat {
        Stat()
        :
        playerIdx(0), bomb(0), range(1), kill(0), alive(true) {}

        int playerIdx;
        std::size_t bomb;
        std::size_t range;
        std::size_t kill;
        bool alive;
        bool winner;
        bool draw;
    };

    struct Scale {
        Scale(float _x = 1.0f, float _y = 1.0f, float _z = 1.0f)
        :
        x(_x), y(_y), z(_z) {}

        float x;
        float y;
        float z;
    };

    struct MaterialTexture {
        MaterialTexture(int _layer = 0, std::string _path = "")
        :
        layer(_layer), path(_path) {}

        int layer;
        std::string path;
    };

    struct TextureArray {
        TextureArray(irr::core::array<irr::video::ITexture *> _textureArray = 0)
        :
        textureArray(_textureArray){}

        irr::core::array<irr::video::ITexture *> textureArray;
    };

    struct MaterialFlag {
        MaterialFlag(irr::video::E_MATERIAL_FLAG _flag = irr::video::EMF_LIGHTING, bool _present = false)
        :
        flag(_flag), present(_present) {}

        irr::video::E_MATERIAL_FLAG flag;
        bool present;
    };

    struct TriangleSelector {
        TriangleSelector(irr::scene::ITriangleSelector *_selector = nullptr)
        :
        selector(_selector) {}

        irr::scene::ITriangleSelector *selector;
    };

    struct MeshPath {
        MeshPath(std::string _meshPath = "assets/models/cube.obj")
        :
        meshPath(_meshPath) {}

        std::string meshPath;
    };

    struct NodeCreate {
    };

    struct Node {
        Node(irr::scene::ISceneNode *_node = nullptr)
        :
        node(_node) {}

        irr::scene::ISceneNode *node;
    };

    struct Particle {
        Particle(irr::scene::IParticleSystemSceneNode *_particle = nullptr)
        :
        particle(_particle) {}

        irr::scene::IParticleSystemSceneNode *particle;
    };

    struct Setup {
    };

    // struct AnimatedMesh : irr::scene::IAnimatedMeshSceneNode {
    // };

    struct Movement {
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
    };

    struct NotSolid {
    };

    struct PosedBy {
        PosedBy(std::size_t _id = 0)
        :
        id(_id) {}

        std::size_t id;
    };

    template<typename ManagerType>
    struct EventCallbacks {

        using CallbackType = std::function<void(const EventData&, std::size_t, ManagerType &)>;

    public:
        const auto &getCallbacks() { return callbacks; }

        void addCallback(const EventData &event, CallbackType &&callback) {
            this->callbacks[event].push_back(std::forward<CallbackType>(callback));
        }

    private:
        std::unordered_map<EventData, std::vector<CallbackType>> callbacks;
    };

    struct IA {
        enum class Action {
            NOTHING,
            ATK,
            WALL,
            PICKUP
        };

        Action current_action = Action::NOTHING;
    };

}
