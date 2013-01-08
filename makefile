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
CGIS=$(addprefix $(CGIS_DIR)/, login colaboradores itens retiradas relatorios)

# Tools
CHMOD=chmod
CP=cp
MKDIR=mkdir
MV=mv
RM=rm

all: init $(OBJS) $(CGIS) postbuild

init:
	@$(MKDIR) -p $(OBJS_DIR)
	@$(MKDIR) -p $(CGIS_DIR)
	@$(MKDIR) -p $(WWW_DIR)/cgi-bin

$(BUILD_DIR)/cgi.o: $(SRC_DIR)/cgi.c
	@$(CC) -c -o $@ $<

$(CGIS_DIR)/login: $(addprefix $(OBJS_DIR)/, cgi.o list.o colaboradores.o error.o login.o login_cgi.o)
	@$(CC) -o $@ $^

$(CGIS_DIR)/colaboradores: $(addprefix $(OBJS_DIR)/, cgi.o list.o error.o login.o colaboradores.o colaboradores_cgi.o)
	@$(CC) -o $@ $^

$(CGIS_DIR)/itens: $(addprefix $(OBJS_DIR)/, cgi.o list.o error.o login.o itens.o itens_cgi.o)
	@$(CC) -o $@ $^

$(CGIS_DIR)/retiradas: $(addprefix $(OBJS_DIR)/, cgi.o list.o error.o login.o colaboradores.o itens.o retiradas.o retiradas_cgi.o)
	@$(CC) -o $@ $^

$(CGIS_DIR)/relatorios: $(addprefix $(OBJS_DIR)/, cgi.o list.o error.o login.o colaboradores.o itens.o retiradas.o relatorios_cgi.o)
	@$(CC) -o $@ $^

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) -c -o $@ $< $(CFLAGS)

postbuild:
	@$(CP) -Rf $(ASSETS_DIR)/* $(WWW_DIR)
	@$(CHMOD) +x $(OUTPUT_DIR)/*
	@$(CP) -f $(CGIS_DIR)/* $(WWW_DIR)/cgi-bin
ifdef CROSS_COMPILE
	@$(MV) -f $(WWW_DIR)/templates $(WWW_DIR)/cgi-bin
	@$(MV) -f $(WWW_DIR)/*.txt $(WWW_DIR)/cgi-bin
endif
	@echo "Copie o conteúdo do diretório www para o diretório do servidor"

clean:
	$(RM) -rf $(OUTPUT_DIR)
	$(RM) -rf $(WWW_DIR)

.PHONY:
	all clean init postbuild
