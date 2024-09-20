CC = g++ -Wall -Wextra -Werror -std=c++17
LIBS = -I/usr/local/include/ -L/usr/local/lib -lgtest -lpthread -lncurses

PROJECT_DIR = games
DIST_DIR = games_2.0

SNAKE_CLI = gui/cli/snake
SNAKE_DESK = gui/desktop/snake

TETRIS_CLI = gui/cli/tetris
TETRIS_DESK = gui/desktop/tetris

SNAKE_BACK = brick_game/snake
TETRIS_BACK = brick_game/tetris



all: clean install test gcov_report

snake_cli:
	cd $(PROJECT_DIR); ./snake_cli

snake_desk:
	cd $(PROJECT_DIR); ./snake_desk

tetris_cli:
	cd $(PROJECT_DIR); ./tetris_cli

tetris_desk:
	cd $(PROJECT_DIR); ./tetris_desk

test: test_snake

test_snake: clean
	$(CC) $(SNAKE_BACK)/*.cpp tests/snake_tests.cpp -o test.out $(LIBS)
	make test_gen

test_tetris: clean
	$(CC) $(TETRIS_BACK)/*.cpp tests/tetris_tests.cpp -o test.out $(LIBS)
	make test_gen

test_gen: assets
	./test.out
	make clean_assets

gcov_report: report_snake

report_snake: clean
	$(CC) --coverage $(SNAKE_BACK)/*.cpp tests/snake* -lstdc++ -o test.out $(LIBS)
	make report_gen

report_tetris: clean
	$(CC) --coverage $(TETRIS_BACK)/*.cpp tests/tetris* -lstdc++ -o test.out $(LIBS)
	make report_gen

report_gen: assets
	./test.out
	lcov -t "./test.out" --exclude='*tests*' --ignore-errors source -o report.info --no-external -c -d .
	genhtml --ignore-errors source -o report report.info
	open ./report/index.html

valgrind:
	$(CC) $(SNAKE_BACK)/*.cpp tests/snake* -o test.out $(LIBS)
	make assets
	make valgrind_gen
	$(CC) $(TETRIS_BACK)/*.cpp tests/tetris* -o test.out $(LIBS)
	make valgrind_gen
	make clean_assets

valgrind_gen:
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./test.out

assets:
	cp -R brick_game/assets .

install:
	mkdir $(PROJECT_DIR)
	cp -R brick_game/assets $(PROJECT_DIR)
	$(CC) $(SNAKE_BACK)/*.cpp $(SNAKE_CLI)/*.cpp $(LIBS) -o $(PROJECT_DIR)/snake_cli
	$(CC) $(TETRIS_BACK)/*.cpp $(TETRIS_CLI)/*.cpp $(LIBS) -o $(PROJECT_DIR)/tetris_cli
	cd $(SNAKE_DESK); make snake
	cd $(TETRIS_DESK); make tetris
	@echo "\nGames installed successfully.\n"

uninstall:
	rm -rf $(PROJECT_DIR)
	@echo "\nGames uninstalled.\n"

reinstall: uninstall install

dvi:
	texi2dvi documentation/games_man.tex
	@echo "\nManual successfully converted to dvi.\n"	

dist:
	mkdir $(DIST_DIR)
	cp -R  brick_game documentation gui tests Makefile $(DIST_DIR)
	tar -czvf $(DIST_DIR).tar.gz $(DIST_DIR)
	rm -rf $(DIST_DIR)
	@echo "\nDistribution directory 'games_2.0' ready.\n"

rebuild: clean all

clean: clean_assets
	rm -f *.o *.a *.gcno *.gcda *.info *.out *.gcov *.gch
	rm -rf *.aux *.dvi *.log *.toc *.tar *.gz *.txt 
	rm -rf ./report *.dSYM
	@echo "\nAll clean!\n"

clean_assets:
	rm -rf assets test.out
	