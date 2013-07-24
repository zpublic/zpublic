
PAGES=$(wildcard src/*.page)

all: src/default.css src/default.template src/virtual ${PAGES}
	@ webgen

deploy: all
	scp out/* khorniszon,tut-framework@web.sourceforge.net:/home/groups/t/tu/tut-framework/htdocs

