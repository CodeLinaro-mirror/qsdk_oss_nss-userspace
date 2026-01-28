MKDIR = @mkdir -p $(@D)
SRCPATH = ./
OBJPATH = obj

BINARY = $(OBJPATH)/libnl-ppe.so
SOURCES = $(wildcard $(SRCPATH)/*.c)
OBJECTS = $(SOURCES:$(SRCPATH)/%.c=$(OBJPATH)/%.o)
HEADERS = $(wildcard $(SRCPATH)/*.h)

INCLUDE += -I./include
EXTRA_CFLAGS = -Wall -Werror -fPIC -Wl,-z,relro -Wl,-z,now

ifneq ($(filter $(SoC),ipq96xx ipq52xx),)
EXTRA_CFLAGS += -DNSS_PPE_DSCP_FEATURE
else
SOURCES := $(filter-out $(SRCPATH)/nss_ppenl_dscp_api.c, $(SOURCES))
endif
OBJECTS = $(SOURCES:$(SRCPATH)/%.c=$(OBJPATH)/%.o)

EXTRA_LDFLAGS = -pie -Wl,-z,relro -Wl,-z,now

all: release

release: $(BINARY)

$(OBJPATH)/%.o: $(SRCPATH)/%.c $(HEADERS)
	$(MKDIR)
	@echo [CC] $@
	@$(CC) -c $(CFLAGS) $(INCLUDE) $(EXTRA_CFLAGS) -o $@ $<

$(BINARY): $(OBJECTS)
	@echo $(BINARY)
	@echo [LD] $@
	@$(CC) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	@echo [Clean]
	@rm -f $(OBJECTS)
	@rm -f $(BINARY)
	@rm -rf $(OBJPATH)

.PHONY: clean
