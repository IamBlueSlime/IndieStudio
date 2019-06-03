/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** utils Packed.hpp
*/

#pragma once

#ifdef __GNUC__
    #define PACKED( class_to_pack ) class_to_pack __attribute__((__packed__))
#else
    #define PACKED( class_to_pack ) __pragma( pack(push, 1) ) class_to_pack __pragma( pack(pop) )
#endif
