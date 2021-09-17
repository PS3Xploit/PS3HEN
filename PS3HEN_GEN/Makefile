OUTPUT=HEN_GEN
OBJS=main.o 
CFLAGS=-Wall -I. --std=gnu99
LDFLAGS=-L. -lstdc++

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT) *.o

$(OUTPUT): $(OBJS)
	$(LINK.c) $(LDFLAGS) -o $@ $^ $(LIBS)
