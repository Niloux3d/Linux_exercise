SRCS:=main.c list.c tcpInit.c tcpHandle.c timeout.c
OBJS:=$(SRCS:%.c=%.o)
server:$(OBJS)
	gcc $^ -o $@ -lpthread
%.o:%.c
	gcc -c $^ -o $@ -g
clean:
	$(RM) server $(OBJS)
