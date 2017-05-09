ROOT_PATH=$(shell pwd)
INCLUDE_PATH=-I$(ROOT_PATH)/mysql_lib/include
LIB_PATH=-L$(ROOT_PATH)/mysql_lib/lib
LIB=-lmysqlclient

CC=g++

.PHONY:all
all:sql_insert sql_select sql_delete sql_update
	cp sql_insert $(ROOT_PATH)/../wwwroot/cgi_bin
	cp sql_select $(ROOT_PATH)/../wwwroot/cgi_bin
	cp sql_delete $(ROOT_PATH)/../wwwroot/cgi_bin
	cp sql_update $(ROOT_PATH)/../wwwroot/cgi_bin


sql_insert:sql_api.cpp sql_insert.cpp
	${CC} -o $@ $^ ${INCLUDE_PATH} ${LIB_PATH} ${LIB}
sql_select:sql_api.cpp sql_select.cpp
	${CC} -o $@ $^ ${INCLUDE_PATH} ${LIB_PATH} ${LIB}
sql_delete:sql_api.cpp sql_delete.cpp
	${CC} -o $@ $^ ${INCLUDE_PATH} ${LIB_PATH} ${LIB}
sql_update:sql_api.cpp sql_update.cpp
	${CC} -o $@ $^ ${INCLUDE_PATH} ${LIB_PATH} ${LIB}


.PHONY:clean
clean:
	rm -rf sql_insert sql_select sql_delete sql_update

.PHONY:debug
debug:
	@echo $(ROOT_PATH)
