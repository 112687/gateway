#定义变量
log= thirdparty/log/log.c thirdparty/log/log.h
# 定义伪目标
.PHONY : log_test
log_test: test/log_test.c  $(log)

	gcc $^  -o $@ -I irdparty 

	./$@

	rm $@

# 	rm log_test