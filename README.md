Drcom-for-HHUTIAN
=================

Drcom的登陆器，用于河海大学文天学院

原始代码是很久以前由两个 Python 初学者写的，质量糟糕

感谢某个作者的mkpkt...说真的这个玩意应该自己去抓包的，不过既然能用也就照着抄了。

这个学校用的应该是u62（或者U64？）基于python构建的Win框架

在前人的基础上去掉了所有的配置文件，我不知道这算不算是一个优点

已知的BUG？

1.绑定mac和ip仍然不能用

2.偶尔申请61440端口仍然会失败，这个后期会扔个try进去调试

错误对照表...

0x01 有人正在使用这个账号，且是有线的方式
0x02 服务器繁忙，请稍候重新登录
0x03 帐号或密码错误
0x04 本帐号的累计时间或流量已超出限制
0x05 本帐号暂停使用
0x07 IP地址不匹配，本帐号只能在指定IP地址上使用
0x0b MAC(物理)地址不匹配，本帐号只能在指定的IP和MAC(物理)地址上使用
0x14 本帐IP地址太多
0x15 客户端版本不正确
0x16 本帐号只能在指定的Mac和IP上使用
0x17 你的PC设置了静态IP,请改为动态获取方式(DHCP),然后重新登录
0x18 - 0x1c 保留错误信息

以上，再次感谢前人的努力。
