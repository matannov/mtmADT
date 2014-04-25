all: chef_test
	chmod 755 chef_test

chef_test: chef.o libmtm.a
	gcc -c -o chef_test chefTest/chefTest.c
	
chef.o: chef.c dish.o
		gcc -c chef.c
	
dish_test: dish.o
	gcc -c -o dish_test dishTest/dishTest.c
	
dish.o: dish.c
	gcc -c dish.c