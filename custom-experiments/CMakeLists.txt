add_executable(mytest test.cpp)

# target_link_libraries(mytest PRIVATE bal)
# target_link_libraries(mytest PRIVATE bbl)
target_link_libraries(mytest PRIVATE bdl)
target_link_libraries(mytest PRIVATE bsl)
# target_link_libraries(mytest PRIVATE inteldfp)
# target_link_libraries(mytest PRIVATE pcre2)
# target_link_libraries(mytest PRIVATE ryu)
# target_link_libraries(mytest PRIVATE s_baltst)

