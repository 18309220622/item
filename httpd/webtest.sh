#!/bin/bash

>log       //每次测试结果以覆盖的形式写入log
i=2
while [ $i -le 1024 ] 
do
	echo "client nums : $i" >> log
  webbench -c ${i} -t 10 http://192.168.1.122:8080/  | grep  -E  'failed|Speed' >> log   //只输出有关响应速度，成功了多少连接数的那两行
  let i=${i}*2
  echo >>log
done 
