add_executable(gbppunit-tester GBPPUnitTester.cc)
target_link_libraries(gbppunit-tester gb1)

set(GBPPUNIT_TEST_PATH ${PROJECT_SOURCE_DIR}/test/gbppunit)

add_test(NAME "gbppunit-001-vblank"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/001-vblank.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/001-vblank.txt")
add_test(NAME "gbppunit-002-basic"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/002-basic.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/002-basic.txt")
add_test(NAME "gbppunit-003-bgmap"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/003-bgmap.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/003-bgmap.txt")
add_test(NAME "gbppunit-004-scyx"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/004-scyx.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/004-scyx.txt")
add_test(NAME "gbppunit-005-scyx-precise"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/005-scyx-precise.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/005-scyx-precise.txt")
add_test(NAME "gbppunit-006-scyx-wrap"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/006-scyx-wrap.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/006-scyx-wrap.txt")
add_test(NAME "gbppunit-007-altbgmap"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/007-altbgmap.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/007-altbgmap.txt")
add_test(NAME "gbppunit-008-altbgtile"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/008-altbgtile.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/008-altbgtile.txt")
add_test(NAME "gbppunit-009-wnd-basic"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/009-wnd-basic.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/009-wnd-basic.txt")
add_test(NAME "gbppunit-010-wyx"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/010-wyx.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/010-wyx.txt")
add_test(NAME "gbppunit-011-wyx-precise"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/011-wyx-precise.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/011-wyx-precise.txt")
add_test(NAME "gbppunit-012-wnd-disable"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/012-wnd-disable.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/012-wnd-disable.txt")
add_test(NAME "gbppunit-013-obj-basic"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/013-obj-basic.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/013-obj-basic.txt")
add_test(NAME "gbppunit-014-obj-xflip"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/014-obj-xflip.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/014-obj-xflip.txt")
add_test(NAME "gbppunit-015-obj-yflip"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/015-obj-yflip.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/015-obj-yflip.txt")
add_test(NAME "gbppunit-016-obj-xyflip"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/016-obj-xyflip.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/016-obj-xyflip.txt")
add_test(NAME "gbppunit-017-obj-leftpri"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/017-obj-leftpri.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/017-obj-leftpri.txt")
add_test(NAME "gbppunit-018-obj-bgpri"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/018-obj-bgpri.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/018-obj-bgpri.txt")
add_test(NAME "gbppunit-019-obj-wndpri"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/019-obj-wndpri.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/019-obj-wndpri.txt")
add_test(NAME "gbppunit-020-obj-mixpri"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/020-obj-mixpri.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/020-obj-mixpri.txt")
add_test(NAME "gbppunit-021-obj-mixpri2"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/021-obj-mixpri2.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/021-obj-mixpri2.txt")
add_test(NAME "gbppunit-022-obj-palpri"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/022-obj-palpri.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/022-obj-palpri.txt")
add_test(NAME "gbppunit-023-default"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/023-default.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/023-default.txt")
add_test(NAME "gbppunit-024-obj-8x16"
         COMMAND gbppunit-tester
                 "${GBPPUNIT_TEST_PATH}/roms/024-obj-8x16.gb"
                 "${GBPPUNIT_TEST_PATH}/expected/024-obj-8x16.txt")
