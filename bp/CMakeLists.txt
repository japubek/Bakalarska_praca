cmake_minimum_required(VERSION 3.10)

project(bp)

set(CMAKE_CXX_STANDARD 17)

#modify value "NAME", when your library is stored somewhere else
set(NAME /usr/local)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")
set(PQXX ${NAME}/include/pqxx)

# ----- setting params to sqlite local database -----
#set(LIB_DIR_LOCATION /usr/local/lib/)
#find_library(sqlite3 PATHS ${LIB_DIR_LOCATION})
#set(INCLUDE_DIRS ${NAME}/include/sqlite)
#----------------------------------------------------

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

set(SERVER_DATABASE Database/db_handler.cpp Database/db_handler.h data/pathToConfig.h)
set(LOCAL_DATABASE Database/database_local/sqlite_local_path.h Database/database_local/db_localDatabase.cpp Database/database_local/db_localDatabase.h)
set(DATA data/Generated/generated_data.cpp data/Generated/generated_data.h data/configuration.cpp data/configuration.h)
set(CSV CSV/csv_handler.cpp CSV/csv_handler.h)
set(APP APP/app_handler.cpp APP/app_handler.h data/vector_type.h)
add_executable(bp main.cpp  Database/nlohmann/json.hpp ${SERVER_DATABASE} ${DATA} ${CSV} ${APP} )
#add_executable(bp main.cpp  Database/nlohmann/json.hpp ${SERVER_DATABASE} ${LOCAL_DATABASE} ${GENERATED_DATA} ${CSV} ${APP}) <-- include sqlite3 libraries

# TARGET_LINK_LIBRARIES(bp  ${PQXX_LIB} ${PQ_LIB} sqlite3)  <-- if you want sqlite3 library included in your project
# TARGET_INCLUDE_DIRECTORIES(bp PRIVATE ${INCLUDE_DIRS})   <--  -||-
TARGET_LINK_LIBRARIES(bp  ${PQXX_LIB} ${PQ_LIB})

#Jakub Trstensky 20.5.2020