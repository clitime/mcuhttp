GCC = gcc
CDBGFLAGS = -g -O0
CFLAGS = $(CDBGFLAGS) -Wall
CFLAGS += -I ./\
		  -I ./src/\
		  -I ./src/linux\
		  -I ./version

TARGET = mcuhttp
ROOT_PATH = ./scripts

vpath %.c ./ ./src ./src/linux ./version
vpath %.h ./ ./src ./src/linux ./version

.PHONY: all fsdata_make test

.ONESHELL:
all: test fsdata_make $(TARGET)

$(TARGET): main.o http_server.o http_read_parser.o http_parser.o http_method_parser.o\
	fsdata.o http_cgi_request.o fs.o\
	version.o http_header_parser.o\
	http_search_buffer.o http_body_parser.o read_request.o\
	base64.o http_cgi_function_list.o
	$(GCC) $(CFLAGS) $^ -o $@

main.o: main.c http_server.h
	$(GCC) $(CFLAGS) -c $<

http_server.o: http_server.c http_server.h fs.h http_read_parser.h
	$(GCC) $(CFLAGS) -c $<

http_read_parser.o: http_read_parser.c http_read_parser.h fs.h http_parser.h http_method_parser.h
	$(GCC) $(CFLAGS) -c $<

http_parser.o: http_parser.c http_parser.h
	$(GCC) $(CFLAGS) -c $<

http_method_parser.o: http_method_parser.c http_method_parser.h fs.h http_cgi_request.h
	$(GCC) $(CFLAGS) -c $<

fsdata.o: fsdata.c fsdata.h fs.h
	$(GCC) $(CFLAGS) -c $<

fs.o: fs.c fs.h fsdata.h
	$(GCC) $(CFLAGS) -c $<

http_cgi_function_list.o: http_cgi_function_list.c http_cgi_function_list.h
	$(GCC) $(CFLAGS) -c $<

http_cgi_request.o: http_cgi_request.c http_cgi_request.h http_cgi_function_list.h http_method_parser.h
	$(GCC) $(CFLAGS) -c $<

version.o: version.c version.h
	$(GCC) $(CFLAGS) -c $<

http_header_parser.o: http_header_parser.c http_header_parser.h
	$(GCC) $(CFLAGS) -c $<

http_search_buffer.o: http_search_buffer.c http_search_buffer.h
	$(GCC) $(CFLAGS) -c $<

http_body_parser.o: http_body_parser.c http_body_parser.h
	$(GCC) $(CFLAGS) -c $<

read_request.o: read_request.c read_request.h
	$(GCC) $(CFLAGS) -c $<

base64.o: base64.c base64.h
	$(GCC) $(CFLAGS) -c $<


test:
	ceedling test:all

fsdata_make:
	cd $(ROOT_PATH)
	python3 makefsdata.py httpd
	cd ../


.PHONY: clean


print: *.[hc]
	@printf "#\n# $?\n#\n"

clean:
	-rm -rfv *.o $(TARGET)
	ceedling clean
