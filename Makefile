
CC:=arm-linux-gcc
CXX:=arm-linux-g++






INCLUDE:=-I./include/ -I. 
CXXFLAGS:=$(INCLUDE)  -g -Wall  -DDEBUG

LDFLAGS:=-L./lib -L../jpeg-6b/arnold/lib/ -L../libpng-1.2.46/arnold/lib/ -L../zlib-1.2.8/arnold/lib/
LIBS:=-lminigui_ths -lpthread -lts -ljpeg -lpng -lz -lsqlite3 



OBJS+=src/main.o
OBJS+=src/main_frame.o
OBJS+=src/main_role.o
OBJS+=src/hs_keyboard.o
OBJS+=src/main_db.o
OBJS+=src/main_record.o
OBJS+=src/main_adc.o
OBJS+=src/hs_udev.o
#OBJS+=src/main_trans.o
OBJS+=src/main_machine.o
OBJS+=src/main_key.o
OBJS+=src/main_cali.o


TARGET:=main_app



all:$(TARGET)
	cp $(TARGET)  ~/friendlyarm/rootfs/
	

$(TARGET):$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS)  $(LIBS) -o $(TARGET)



clean:
	-rm $(TARGET)
	-rm $(OBJS)


