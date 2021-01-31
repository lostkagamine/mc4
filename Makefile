CFLAGS ?= -O2 -g
BUILD_DIR ?= bld

$(BUILD_DIR)/mc4: CPUEmulator/emu.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean
