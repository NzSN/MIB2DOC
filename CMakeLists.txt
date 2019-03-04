cmake_minimum_required(VERSION 3.5)
project(Mib2Doc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(HOME_DIR .)
set(SRC_DIR ${HOME_DIR}/src)
set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES 
    ${SRC_DIR}/main.c 
    ${SRC_DIR}/mibTreeGen.c
    ${SRC_DIR}/lexer.c ${SRC_DIR}/yy_syn.tab.c
    ${SRC_DIR}/queue.c ${SRC_DIR}/mibTreeObjTree.c 
    ${SRC_DIR}/stack.c ${SRC_DIR}/list.c ${SRC_DIR}/lexDeal.c
    ${SRC_DIR}/options.c ${SRC_DIR}/util.c ${SRC_DIR}/dispatcher.c
    ${SRC_DIR}/docGenerate.c ${SRC_DIR}/misc.c ${SRC_DIR}/hash.c
    ${SRC_DIR}/yy_syn.def.c)
add_executable(Mib2Doc ${SOURCE_FILES})

project(UNIT_TEST)
set(UTEST_DIR ${SRC_DIR}/unitTest)
set(UNIT_TEST_SRC ${UTEST_DIR}/basicDataStruct_Test.c
    ${SRC_DIR}/mibTreeGen.c ${SRC_DIR}/lexer.c
    ${SRC_DIR}/yy_syn.tab.c ${SRC_DIR}/queue.c 
    ${SRC_DIR}/mibTreeObjTree.c ${SRC_DIR}/stack.c 
    ${SRC_DIR}/list.c ${SRC_DIR}/dispatcher.c
    ${SRC_DIR}/lexDeal.c ${SRC_DIR}/misc.c
    ${SRC_DIR}/util.c ${SRC_DIR}/options.c
    ${SRC_DIR}/hash.c ${SRC_DIR}/yy_syn.def.c)
link_libraries(cmocka)
add_executable(UNIT_TEST ${UNIT_TEST_SRC})
target_compile_definitions(UNIT_TEST PUBLIC MIB2DOC_UNIT_TESTING)

set(HEADER_DIR ${HOME_DIR}/include)
include_directories(${HEADER_DIR})

add_custom_target(lexBison ALL DEPENDS)
add_custom_target(lexBison_H ALL DEPENDS)
add_custom_command(TARGET lexBison PRE_BUILD
    COMMAND flex -o ${SRC_DIR}/lex.yy.c ${SRC_DIR}/yy_lex.l)
add_custom_command(TARGET lexBison PRE_BUILD
    COMMAND bison -o ${SRC_DIR}/yy_syn.tab.c ${SRC_DIR}/yy_syn.y)
add_custom_command(TARGET lexBison_H PRE_BUILD
    COMMAND bison -d -o ${SRC_DIR}/yy_syn.tab.c ${SRC_DIR}/yy_syn.y)
add_custom_command(TARGET lexBison_H PRE_BUILD 
    COMMAND mv ${SRC_DIR}/yy_syn.tab.h ${HEADER_DIR})


