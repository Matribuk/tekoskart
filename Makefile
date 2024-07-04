##
## FIRST PERSONAL PROJECT, 2024
## TekosKart
## File description:
## Makefile
##

SUBDIRS := lib/server

all: $(SUBDIRS)

FLAGS	=	--no-print-directory

$(SUBDIRS):
	$(MAKE) $(FLAGS) -C $@

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) $(FLAGS) -C $$dir clean; \
	done

fclean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) $(FLAGS) -C $$dir fclean; \
	done

re: fclean all

.PHONY: all $(SUBDIRS) clean fclean re
