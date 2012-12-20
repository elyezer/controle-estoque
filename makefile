CC = $(CROSS_COMPILE)gcc

# Caminhos
ASSETS_DIR=assets
SRC_DIR=src
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OUTPUT_DIR=build
WWW_DIR=www

# Objects
OBJS_DIR=$(OUTPUT_DIR)/objs
OBJS=$(notdir $(SRC_FILES))
OBJS:=$(addprefix $(OBJS_DIR)/, $(OBJS))
OBJS:=$(OBJS:.c=.o)

# CGIs
CGIS_DIR=$(OUTPUT_DIR)/cgis
CGIS=$(addprefix $(CGIS_DIR)/, login colaboradores)

# Tools
CHMOD=chmod
CP=cp
MKDIR=mkdir
RM=rm

all: init $(OBJS) $(CGIS) postbuild

init:
	@$(MKDIR) -p $(OBJS_DIR)
	@$(MKDIR) -p $(CGIS_DIR)
	@$(MKDIR) -p $(WWW_DIR)/cgi-bin

$(BUILD_DIR)/cgi.o: $(SRC_DIR)/cgi.c
	@$(CC) -c -o $@ $<

$(CGIS_DIR)/login: $(OBJS_DIR)/cgi.o $(OBJS_DIR)/list.o $(OBJS_DIR)/login.o
	@$(CC) -o $@ $^

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) -c -o $@ $< $(CFLAGS)

postbuild:
	@$(CP) -R $(ASSETS_DIR)/* $(WWW_DIR)
	@$(CHMOD) +x $(OUTPUT_DIR)/*
	@$(CP) $(CGIS_DIR)/* $(WWW_DIR)/cgi-bin
	@echo "Copie o conteúdo do diretório www para o diretório do servidor"

clean:
	$(RM) -rf $(OUTPUT_DIR)
	$(RM) -rf $(WWW_DIR)/*

.PHONY:
	all clean init postbuild
