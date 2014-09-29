配置drcom.conf,第一行为认证服务器地址，形如，“10.1.1.10”
注意，需要英文双引号
例如，
server = "10.1.1.10" #your auth server ip
username = ""
password = ""
然后将配置文件放进/var/drcom.conf
运行$python  [filepath]/StartDrcom0.4.1_openwrt.py
你也可以将它加入开机启动项


