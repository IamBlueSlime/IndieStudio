/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** UniqueIrrPtr
*/

#ifndef UNIQUEIRRPTR_HPP_
#define UNIQUEIRRPTR_HPP_

#include <memory>

namespace utils {

template <class T>
struct MyDestructor {
	void operator()(T *p) { p->drop(); };
};

template <class T>
using uniqueIrr_ptr = std::unique_ptr<T, MyDestructor<T>>;

}
#endif /* !UNIQUEIRRPTR_HPP_ */
