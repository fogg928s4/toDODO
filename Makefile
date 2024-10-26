tododo: src/todo.c
	$(CC) src/todo.c -o tododo -lGL -lleif -lglfw -lclipboard -lxcb -lm -std=c99
