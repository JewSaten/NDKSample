# NDKSample
JNI AES加解密


## 使用方法

```
 String source = "hello world";
 byte[] encodedStr = AESUtils.AESEncode(source.getBytes());
 Log.e(TAG, "encodedStr: " + new String(encodedStr));
 Log.e(TAG, "decodedStr: " + new String(AESUtils.AESDecode(encodedStr)));
 ```

## 结果打印

![img2](/img/img2.png "img2")

## 联系我
- **Gmail：** satenjew@gmail.com
- **工作QQ：** 1076727856
