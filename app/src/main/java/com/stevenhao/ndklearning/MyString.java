package com.stevenhao.ndklearning;

/**
 * @Autor : yunlong20
 * @Date : on 2024-04-17
 * @Description :
 */
public class MyString {
    String str;
    StringBuffer stringBuffer;
    MyString() {
       str = "asdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdf" +
               "asdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfa"+
               "asdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdf" +
        "asdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdf" +
               "fasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdf" +
               "sdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdfasdfaasdf";

        stringBuffer = new StringBuffer(str);
    }
}
