/*

エラー表示関数等
エラー内容書いてぶん投げてください
（メッセージボックス等もあります。）
*/


#pragma once

#include <windows.h>



inline void Error(TCHAR* error_text)
{

	MessageBox(nullptr,error_text, "エラー", MB_ICONWARNING);

	return;

}


inline bool YesNoCheckBox(LPCSTR TextMessage,LPCSTR TitleMessage = "")
{
	int CheckButton;

	CheckButton = 
		MessageBox(nullptr,TextMessage,TitleMessage,MB_YESNO);

	if(CheckButton == IDYES)
	{//Yesを押した場合trueを
		return true;
	}
	else
	{//違うならfalseを
		return false;
	}
}


