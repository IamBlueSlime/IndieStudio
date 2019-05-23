/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** tests ecs Signature.cpp
*/

#include "gtest/gtest.h"
#include "indiestudio/ecs/Signature.hpp"
#include "indiestudio/utils/ByteBufferUtils.hpp"

namespace IndieStudio {

    #ifndef O
    #define O(c, x) (offsetof(c, x))
    #endif

    struct BasicComponent {
        int a;
        bool b;
    };

    auto BasicComponentSignature =
        Signature
            ::of<BasicComponent>
            ::with<int, bool>
            ::at({O(BasicComponent, a), O(BasicComponent, b)});

    struct ComplexComponent {
        int a;
        std::string b;
        float c;
    };

    auto ComplexComponentSignature =
        Signature
            ::of<ComplexComponent>
            ::with<int, std::string, float>
            ::at({O(ComplexComponent, a), O(ComplexComponent, b), O(ComplexComponent, c)});

    TEST(SignaturePack, BasicComponent) {
        BasicComponent cpnt = { 42, true };

        ByteBuffer bufferReference;
        bufferReference << cpnt.a;
        bufferReference << cpnt.b;

        ByteBuffer buffer;
        BasicComponentSignature.pack(cpnt, buffer);

        ASSERT_EQ(buffer.getWriteCursor(), bufferReference.getWriteCursor());
        ASSERT_EQ(std::memcmp(*buffer, *bufferReference, bufferReference.getWriteCursor()), 0);
    }

    TEST(SignaturePack, ComplexComponent) {
        ComplexComponent cpnt = { 42, "hello world!", 3.2f };

        ByteBuffer bufferReference;
        bufferReference << cpnt.a;
        ByteBufferUtils::writeString(bufferReference, cpnt.b);
        bufferReference << cpnt.c;

        ByteBuffer buffer;
        ComplexComponentSignature.pack(cpnt, buffer);

        ASSERT_EQ(buffer.getWriteCursor(), bufferReference.getWriteCursor());
        ASSERT_EQ(std::memcmp(*buffer, *bufferReference, bufferReference.getWriteCursor()), 0);
    }

    TEST(SignatureUnpack, BasicComponent) {
        BasicComponent cpntReference = { 42, true };
        BasicComponent cpnt = { 0, false };
        ByteBuffer buffer;

        BasicComponentSignature.pack(cpntReference, buffer);
        BasicComponentSignature.unpack(cpnt, buffer);

        ASSERT_EQ(cpnt.a, 42);
        ASSERT_EQ(cpnt.b, true);
    }

    TEST(SignatureUnpack, ComplexComponent) {
        ComplexComponent cpntReference = { 42, "hello world!", 3.2f };
        ComplexComponent cpnt = { 0, "", 0.0f };
        ByteBuffer buffer;

        ComplexComponentSignature.pack(cpntReference, buffer);
        ComplexComponentSignature.unpack(cpnt, buffer);

        ASSERT_EQ(cpnt.a, 42);
        ASSERT_EQ(cpnt.b, "hello world!");
        ASSERT_EQ(cpnt.c, 3.2f);
    }

}