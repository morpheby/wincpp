# rc-build.mk -- initializes resource building

SUFFIXES += .rc .manifest

VERSION_4D  = @VERSION_4D@

.rc.$(OBJEXT):
	$(RESCOMP) --use-temp-file -i $< $(INCLUDES) $(DEFAULT_INCLUDES) -I./resources $(DEFS) -o $@

.premf.manifest:
	$(SED) 's/_VERSION_/$(VERSION_4D)/g' $< > $@


