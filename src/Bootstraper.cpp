/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Bootstraper.cpp
*/

#include <iostream>
#include "indiestudio/Bootstraper.hpp"
#include "indiestudio/ecs/Signature.hpp"
#include "indiestudio/mod/ModManager.hpp"
#include "indiestudio/world/World.hpp"
#include "indiestudio/world/WorldManager.hpp"

namespace IndieStudio {

    struct MyComponent {
        int a;
        bool b;
        float c;
        std::string d;
    };

    #define O(c, x) (offsetof(c, x))

    auto MyComponentSignature =
        Signature
            ::of<MyComponent>
            ::with<int, bool, float, std::string>
            ::at({O(MyComponent, a), O(MyComponent, b), O(MyComponent, c), O(MyComponent, d)});

    int Bootstraper::start(int ac, char **av)
    {
        (void) ac;
        (void) av;

        std::cout << std::endl << "== TEST SIGNATURE ==" << std::endl;
        MyComponent cpnt = { 3, true, 3.5, "hello world" };
        ByteBuffer a;
        MyComponentSignature.pack(cpnt, a);
        std::cout << "buffer length: " << a.getSize() << std::endl;
        MyComponent cpnt2 = { 0, 0, 0, "" };
        MyComponentSignature.unpack(cpnt2, a);
        std::cout << cpnt2.a << std::endl;
        std::cout << cpnt2.b << std::endl;
        std::cout << cpnt2.c << std::endl;
        std::cout << cpnt2.d << std::endl;

        std::cout << std::endl << "== TEST WORLD ==" << std::endl;
        WorldManager worldManager;
        World w = World();
        w.getSettings().name = "my_world";
        worldManager.save("my_world.bmm", w);
        World w2 = worldManager.load("my_world.bmm");
        std::cout << w2.getSettings().name << std::endl;

        std::cout << std::endl << "== TEST MOD ==" << std::endl;
        ModManager modManager;
        modManager.reload("mods");

        test();

        return 0;
    }

}