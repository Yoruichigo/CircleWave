/*

�G���[�\���֐���
�G���[���e�����ĂԂ񓊂��Ă�������
�i���b�Z�[�W�{�b�N�X��������܂��B�j
*/


#pragma once

#include <windows.h>



inline void Error(TCHAR* error_text)
{

	MessageBox(nullptr,error_text, "�G���[", MB_ICONWARNING);

	return;

}


inline bool YesNoCheckBox(LPCSTR TextMessage,LPCSTR TitleMessage = "")
{
	int CheckButton;

	CheckButton = 
		MessageBox(nullptr,TextMessage,TitleMessage,MB_YESNO);

	if(CheckButton == IDYES)
	{//Yes���������ꍇtrue��
		return true;
	}
	else
	{//�Ⴄ�Ȃ�false��
		return false;
	}
}


