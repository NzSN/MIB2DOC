cmake_minimum_required(VERSION 3.7)
project(GL5610_MIB_DOC_GEN)


set(SRC_DIR E:/CODE/GL5610_MIB_DOC_GEN/src)
set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES src/main.c include/type.h src/tokenOp.c include/type.h
        src/lex.yy.c src/yy_syn.tab.c include/tokenOp.h include/queue.h src/queue.c include/tree.h src/tree.c include/stack.h src/stack.c src/unitTest/unitTest.c)

add_custom_target(lexBison ALL DEPENDS)
add_custom_command(TARGET lexBison
        PRE_BUILD
        COMMAND win_flex -o ${SRC_DIR}/lex.yy.c ${SRC_DIR}/yy_lex.l)
add_custom_command(TARGET lexBison
        PRE_BUILD
        COMMAND win_bison -o ${SRC_DIR}/yy_syn.tab.c ${SRC_DIR}/yy_syn.y)

add_executable(GL5610_MIB_DOC_GEN ${SOURCE_FILES})

project(UNIT_TEST)
set(UNIT_TEST_SRC src/unitTest/unitTest.c)
link_libraries(cmocka)
add_executable(UNIT_TEST ${UNIT_TEST_SRC})