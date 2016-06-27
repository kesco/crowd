+++
title: 用Android发送短信

+++

# 调用原生App发送短信

有时候，我们不方便把短信功能嵌入到我们的App上去（容易让客户觉得我们偷发短信），所以我们只有调用手机内置的短信应用发送短信。调用流程非常方便，不需要特殊的权限，和别的调用一样，都是通过Intent机制进行的，简要代码如下：

```
Intent sendIntent = new Intent(Intent.ACTION_VIEW); 	/* 申明Intent对象 */
sendIntent.setData(Uri.parse("smsto:")); 		/* 设置Data URI */
sendIntent.setType("vnd.android-dir/mms-sms"); 		/* 设置Intent类型 */
sendIntent.putExtra("address"  , "15814094377;"); 	/* 设置发送目的号码 */
sendIntent.putExtra("sms_body", "default content"); 	/* 短信内容 */
startActivity(sendIntent); 				/* 调起应用 */
```
