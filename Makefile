##
## EPITECH PROJECT, 2019
## OOP_indie_studio_2018
## File description:
## Makefile
##

.PHONY:		all tests_run coverage clean fclean re

all:
			if [ ! -d  build ]; then mkdir build; fi
			cd build && cmake .. && cmake --build . -- --no-print-directory

tests_run:
			if [ ! -d  build ]; then mkdir build; fi
			cd build && cmake .. -DBUILD_TESTS=ON && cmake --build . -- --no-print-directory && make test ARGS="-V"

coverage:
			gcovr -r . --exclude-directories tests | tee coverage-litteral.txt
			gcovr -r . --exclude-directories tests --xml-pretty > coverage.xml

clean:
			rm -Rf build

fclean:		clean
			rm -f bomberman

re:			fclean all
