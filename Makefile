# -g 开启gdb的debug调试（生成的二进制文件中包含调试信息）
# -O0 禁用优化（确保调试一致性）
# -Wall 显示所有警告
CFLAGS := -g -O0 -Wall



#定义变量
log:= thirdparty/log/log.c thirdparty/log/log.h

json:= thirdparty/json/cJSON.c thirdparty/json/cJSON.h

app_common:=app/app_common.c app/app_common.h 
 # 定义伪目标
.PHONY : log_test
.PHONY : cjson_test
.PHONY : app_common_test
log_test: test/log_test.c  $(log)

	gcc $(CFLAGS)  $^ -o $@ -I thirdparty

	./$@

	rm $@


cjson_test: test/json_test.c $(json) $(log)
	gcc $(CFLAGS) $^  -o $@ -I thirdparty

	./$@

	rm $@

app_common_test: test/app_common_test.c $(app_common) $(log)
	gcc $(CFLAGS) $^  -o $@ -I thirdparty -I app

	./$@

	rm $@

app_message := app/app_message.h app/app_message.c
app_message_test: test/app_message_test.c $(app_message) $(app_common) $(log) $(json)
	-gcc $^ -o $@ -I thirdparty -I app 
	-./$@
	-rm $@

mqtt_test: test/mqtt_test.c
	-gcc $^ -o $@ -l paho-mqtt3c
	-./$@
	-rm $@

app_mqtt := app/app_mqtt.c app/app_mqtt.h
.PHONY : app_mqtt_test
app_mqtt_test: test/app_mqtt_test.c $(app_mqtt) $(log)
	gcc $(CFLAGS) $(filter %.c,$^) -o $@ -I thirdparty -I app -lpaho-mqtt3c
	./$@
	rm $@
