# Makefile - 嵌入式通用代码库构建

# 工具链
CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy

# 目录
SRCDIR = src
INCDIR = include
BUILDDIR = build
LIBDIR = lib

# 源文件
SRC = $(wildcard $(SRCDIR)/*.c) \
      $(wildcard $(SRCDIR)/**/*.c)

# 头文件路径
INC = -I$(INCDIR) \
      -I$(INCDIR)/hal \
      -I$(INCDIR)/protocol \
      -I$(INCDIR)/data_structure \
      -I$(INCDIR)/driver \
      -I$(INCDIR)/system \
      -I$(INCDIR)/security \
      -I$(INCDIR)/utility

# 编译选项
CFLAGS = -Wall -Wextra -Werror -O2 -g
CFLAGS += -std=c99
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -DDEBUG

# 链接选项
LDFLAGS = -Wl,--gc-sections

# 目标库
LIB = $(LIBDIR)/libembedded.a

# 对象文件
OBJ = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))

# 默认目标
all: $(LIB) size

# 创建目录
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(LIBDIR):
	mkdir -p $(LIBDIR)

# 编译源文件
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# 生成静态库
$(LIB): $(OBJ) | $(LIBDIR)
	$(AR) rcs $@ $^
	@echo "Library created: $@"

# 显示大小
size: $(LIB)
	$(SIZE) -t $<

# 清理
clean:
	rm -rf $(BUILDDIR) $(LIBDIR)

# 安装
install: $(LIB)
	cp $(LIB) /usr/local/lib/
	cp -r $(INCDIR)/* /usr/local/include/

# 示例
examples:
	$(CC) $(CFLAGS) $(INC) examples/basic/main.c -L$(LIBDIR) -lembedded -o examples/basic/main.elf

# 文档
docs:
	doxygen Doxyfile

# 测试
test:
	$(CC) $(CFLAGS) $(INC) test/*.c -L$(LIBDIR) -lembedded -o test/test.elf
	./test/test.elf

.PHONY: all clean install examples docs test size
