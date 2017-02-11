add_executable(blargg-tester BlarggTester.cc)
target_link_libraries(blargg-tester gb1)

set(BLARGG_TEST_PATH ${PROJECT_SOURCE_DIR}/test/blargg)

add_test(NAME "cpu_instrs-01-special"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/01-special.gb" 
                 200 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_01.txt")
add_test(NAME "cpu_instrs-02-interrupts"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/02-interrupts.gb" 
                 100 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_02.txt")
add_test(NAME "cpu_instrs-03-op-sp,hl"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/03-op sp,hl.gb" 
                 200 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_03.txt")
add_test(NAME "cpu_instrs-04-op-r,imm"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/04-op r,imm.gb" 
                 200 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_04.txt")
add_test(NAME "cpu_instrs-05-op-rp"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/05-op rp.gb" 
                 300 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_05.txt")
add_test(NAME "cpu_instrs-06-ld-r,r"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/06-ld r,r.gb" 
                 100 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_06.txt")
add_test(NAME "cpu_instrs-07-jr,jp,call,ret,rst"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb" 
                 100 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_07.txt")
add_test(NAME "cpu_instrs-08-misc-instrs"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/08-misc instrs.gb" 
                 100 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_08.txt")
add_test(NAME "cpu_instrs-09-op-r,r"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/09-op r,r.gb" 
                 600 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_09.txt")
add_test(NAME "cpu_instrs-10-bit-ops"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/10-bit ops.gb" 
                 900 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_10.txt")
add_test(NAME "cpu_instrs-11-op-a,hl"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/individual/11-op a,(hl).gb" 
                 1100 
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs_11.txt")
add_test(NAME "cpu_instrs-all"
         COMMAND blargg-tester
                 "${BLARGG_TEST_PATH}/cpu_instrs/cpu_instrs.gb" 
                 3300
                 "${BLARGG_TEST_PATH}/cpu_instrs/expected/cpu_instrs.txt")
