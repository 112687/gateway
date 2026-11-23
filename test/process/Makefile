.PHONY : testfork
testfork: fork.c
	gcc $(CFLAGS) $< -o $@
	./$@
	rm $@

other: other.c
	gcc $(CFLAGS) $< -o $@

.PHONY: execve
execve: execve.c other
	gcc $(CFLAGS) $< -o $@
	./$@
	rm $@ other


signal: signal.c
	gcc $(CFLAGS) $< -o $@
	./$@
	rm $@