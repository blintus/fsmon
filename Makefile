TARGET = fsmond
SRCDIR = src
OBJDIR = obj

SOURCES  = $(wildcard $(SRCDIR)/*.c)
INCLUDES = $(wildcard $(SRCDIR)/*.h)
OBJECTS  = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC=gcc -Wall -std=c99

fsmond: $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(SRCDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)