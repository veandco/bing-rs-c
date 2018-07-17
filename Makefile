NAME=example

all:
	gcc ${NAME}.c -L . -lbing_rs -o ${NAME}

clean:
	-@rm ${NAME} 2> /dev/null || true
