#include<ComObj.hpp>

String GetXmlHttp()
{
    Variant xmlHttp;
    String strurl= "http://www.baidu.com";

    xmlHttp = CreateOleObject("WinHttp.WinHttpRequest.5.1");
    xmlHttp.OleProcedure(WideString("Open"),WideString("GET"),WideString(strurl),false);
    xmlHttp.OleProcedure("Send");

    String resptext=xmlHttp.OlePropertyGet(WideString("ResponseText"));
    print(resptext);
}