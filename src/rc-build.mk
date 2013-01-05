# rc-build.mk -- initializes resource building

SUFFIXES += .rc .manifest

VERSION_4D  = @VERSION_4D@
MANIFEST_ARCH = @MANIFEST_ARCH@

.rc.$(OBJEXT):
	$(RESCOMP) --use-temp-file --codepage=65001 -i $< $(INCLUDES) $(DEFAULT_INCLUDES) -I./resources $(DEFS) -o $@

.premf.manifest:
	$(SED) 's/_VERSION_/$(VERSION_4D)/g' $< | $(SED) 's/_ARCH_/$(MANIFEST_ARCH)/g' > $@


