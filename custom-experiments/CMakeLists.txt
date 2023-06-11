add_executable(mytest test.cpp)

if(DEFINED ENV{INTERPOSE})
    target_include_directories(mytest PRIVATE interpose-include)
endif()

# target_link_libraries(mytest PRIVATE bal)
# target_link_libraries(mytest PRIVATE bbl)
target_link_libraries(mytest PRIVATE bdl)
target_link_libraries(mytest PRIVATE bsl)
# target_link_libraries(mytest PRIVATE inteldfp)
# target_link_libraries(mytest PRIVATE pcre2)
# target_link_libraries(mytest PRIVATE ryu)
# target_link_libraries(mytest PRIVATE s_baltst)

