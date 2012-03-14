
all:
	make -C src/

clean:
	@find . -name Ext2fsReader -exec rm {} \;
	@find . -name "*.o" -exec rm {} \;

