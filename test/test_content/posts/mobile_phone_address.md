+++
title: Android判断手机号码分属 
date: 2016-07-06
+++

# Android判断手机号码分属

一般来说，我们可以通过判断网络链接种类来判断手机是使用哪个运营商号码的。但是有时候不大准而且万一用户禁了网络链接就判断不了了。所以最靠谱的方案还是通过手机的IMSI码来判断。

## 什么是IMSI码

## Android上获取IMSI码

通过`Context`上下文获取IMSI：

```
TelephonyManager tel = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
String imsi = tel.getSubscriberId();
```
