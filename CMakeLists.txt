cmake_minimum_required(VERSION 3.7)
project(GL5610_MIB_DOC_GEN)


set(SRC_DIR /home/ayden/CLionProjects/MIB2DOC/src)
set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES src/main.c include/type.h src/mibTreeGen.c
        src/lex.yy.c src/yy_syn.tab.c src/queue.c src/mibTreeObjTree.c include/stack.h src/stack.c src/docGen.c src/list.c src/lexDeal.c include/lexDeal.h)

add_custom_target(lexBison ALL DEPENDS)
add_custom_command(TARGET lexBison
        PRE_BUILD
        COMMAND flex -o ${SRC_DIR}/lex.yy.c ${SRC_DIR}/yy_lex.l)
add_custom_command(TARGET lexBison
        PRE_BUILD
        COMMAND bison -o ${SRC_DIR}/yy_syn.tab.c ${SRC_DIR}/yy_syn.y)

add_executable(GL5610_MIB_DOC_GEN ${SOURCE_FILES})

project(UNIT_TEST)
set(UNIT_TEST_SRC src/unitTest/unitTest.c include/type.h src/mibTreeGen.c
        src/lex.yy.c src/yy_syn.tab.c src/queue.c src/mibTreeObjTree.c include/stack.h src/stack.c src/docGen.c src/list.c)
link_libraries(cmocka)
add_executable(UNIT_TEST ${UNIT_TEST_SRC})