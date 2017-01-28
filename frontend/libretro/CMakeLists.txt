set(LIBGB1LR_SRC libretro.cc LRConnector.cc)
add_library(gb1-lr SHARED ${LIBGB1LR_SRC})
target_link_libraries(gb1-lr gb1)

if (MSVC)
    target_link_libraries(gb1-lr gb1 -LTCG)
endif(MSVC)
