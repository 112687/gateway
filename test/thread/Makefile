CC := gcc
CFLAGS := -g -O0 -Wall

.PHONY: create sum_sqr sum_sqr_thread cancel race_condition mutex_lock mqueue

create: 01_create.c
	$(CC) $(CFLAGS) $< -o $@ -pthread
	./$@
	rm $@

sum_sqr: 02_sum_sqr.c
	$(CC) $(CFLAGS) $< -o $@ -pthread
	./$@
	rm $@

sum_sqr_thread: 02_sum_sqr_thread.c
	$(CC) $(CFLAGS) $< -o $@ -pthread
	./$@
	rm $@

cancel: 03_cancel.c
	$(CC) $(CFLAGS) $< -o $@ -pthread
	./$@
	rm $@

race_condition: 04_race_condition.c
	$(CC) $(CFLAGS) $< -o $@ -pthread
	./$@
	rm $@

mutex_lock: 04_mutex_lock.c
	$(CC) $(CFLAGS) $< -o $@ -pthread
	./$@
	rm $@

mqueue: 05_mqueue.c
	$(CC) $(CFLAGS) $< -o $@ -pthread -lrt
	./$@
	rm $@
