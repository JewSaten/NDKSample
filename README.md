# NDKSample
JNI AES加解密

[JNI的两种注册方式比较](https://jewsaten.github.io/2017/09/25/jni-simple/)

## 使用方法

```
 String source = "hello world";
 byte[] encodedStr = AESUtils.AESEncode(source.getBytes());
 Log.e(TAG, "encodedStr: " + new String(encodedStr));
 Log.e(TAG, "decodedStr: " + new String(AESUtils.AESDecode(encodedStr)));
 ```

## 结果打印

![img2](/img/img2.png "img2")

## 联系

- **Gmail：** satenjew@gmail.com
- **个人博客：** [https://jewsaten.github.io/](https://jewsaten.github.io/)
