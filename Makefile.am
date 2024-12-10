AUTOMAKE_OPTIONS = foreign

INCLUDES=-I$(prefix)/include -I/usr/local/include

bin_PROGRAMS = gdbm_store gdbm_delete gdbm_fetch listgdb gdbm_store_from_stdin
bin_SCRIPTS = $(scripts)

scripts = rrmap_to_pgm csmap_to_pgm murder.pl\
          2A-53_ghdf_to_browse level_1_to_browse_gif rrmap_to_browse_gif \
		  rrmap_to_ppm

images = 1C51_black.miff 1C51_color_bar.miff 2A53_black.miff\
         2A53_color_bar.miff

imagedir = $(prefix)/data
image_DATA = $(images)
rsl_color_dir = $(prefix)/lib/colors

rrmap_to_pgm: Makefile
	@for p in $(scripts); do \
	   echo -n "Checking file $$p ... ";\
	   cp $$p $$p.in;\
	   sed -e 's/\$PROG_VERSION[ ]*=.*/$PROG_VERSION = \"$(PACKAGE)-$(VERSION)\";/' \
		-e 's|#\!.*perl|#\! $(perlbin)|' \
		-e 's|(\@INC,.*|(\@INC, \".\", \"$(bindir)\"\)\;|' \
        -e 's|$i_dir[ ]*=.*|$i_dir = \"$(imagedir)\";|' \
        -e 's|$RSL_COLOR_DIR[ ]*=.*|$RSL_COLOR_DIR = \"$(rsl_color_dir)\";|' \
	   < $$p.in > $$p.new;\
	   if cmp -s $$p $$p.new; then \
          rm $$p.new; \
          echo "$$p remains untouched."; \
       else \
          mv $$p.new $$p; \
          echo "substitutions made in $$p."; \
       fi;\
	   rm -f $$p.in;\
    done

EXTRA_DIST = $(bin_SCRIPTS) $(images) CHANGES
