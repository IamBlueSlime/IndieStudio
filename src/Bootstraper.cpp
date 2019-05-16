/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Bootstraper.cpp
*/

#include <iostream>
#include "indiestudio/Bootstraper.hpp"
#include "indiestudio/ecs/Signature.hpp"

namespace IndieStudio {

    struct MyComponent {
        int a;
        bool b;
        float c;
        std::string d;
    } __attribute__((packed));

    using MyComponentSignature =
        Signature::of<MyComponent>::with<int, bool, float, std::string>;

    int Bootstraper::start(int ac, char **av)
    {
        (void) ac;
        (void) av;

        MyComponent cpnt = { 3, true, 3.5, "hello world" };
        ByteBuffer a;

        MyComponentSignature::pack(cpnt, a);
        MyComponent cpnt2 = { 0, 0, 0, "" };
        MyComponentSignature::unpack(cpnt2, a);

        std::cout << cpnt2.a << std::endl;
        std::cout << cpnt2.b << std::endl;
        std::cout << cpnt2.c << std::endl;
        std::cout << cpnt2.d << std::endl;

        test();

        return 0;
    }

}