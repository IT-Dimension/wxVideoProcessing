#include "stdwx.h"
#ifdef linux
#include <wx/validate.h>
#include <wx/textctrl.h>
#endif
#include "wxFPValidator.h"
#include <math.h>
#include <wx/filepicker.h>
#include <wx/spinctrl.h>

wxFPValidator::wxFPValidator(double * valPtr)
{
	Initialize();
	m_pDouble = valPtr;	
}

wxFPValidator::wxFPValidator(int * valPtr)
{
	Initialize();
	m_pInt = valPtr;	
}

wxFPValidator::wxFPValidator(long * valPtr)
{
	Initialize();
	m_pLong = valPtr;	
}

wxFPValidator::wxFPValidator(wxString * valPtr)
{
	Initialize();
	m_pString = valPtr;	
}

wxFPValidator::wxFPValidator(const wxFPValidator & validator)
: m_pDouble(validator.m_pDouble), m_pInt(validator.m_pInt), 
m_pLong(validator.m_pLong), m_pString(validator.m_pString)
{
}

wxFPValidator::~wxFPValidator()
{
}

void wxFPValidator::Initialize()
{
	m_pDouble = nullptr;
	m_pInt = nullptr;
	m_pLong = nullptr;
	m_pString = nullptr;
}

wxValidator* wxFPValidator::Clone() const
{
	return new wxFPValidator(*this);
}

bool wxFPValidator::TransferFromWindow()
{
	if(!m_validatorWindow) 
	{
		return false;
	}
#if wxUSE_TEXTCTRL
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)) )
    {
        wxTextCtrl * pControl = (wxTextCtrl*) m_validatorWindow;
		if(m_pDouble)
		{
			return pControl->GetValue().ToDouble(m_pDouble);
		}
		else if(m_pInt)
		{
			double tmp;
			bool res = pControl->GetValue().ToDouble(&tmp);
			if(tmp >= INT_MIN && tmp <= INT_MAX && (fmod(tmp,1) == 0))
			{
				*m_pInt = (int)tmp;
			} else res = false;
			return res;
		}
		else if(m_pLong)
		{
			double tmp;
			bool res = pControl->GetValue().ToDouble(&tmp);
			if(tmp >= LONG_MIN && tmp <= LONG_MAX && (fmod(tmp,1) == 0))
			{
				*m_pLong = (long)tmp;
			} else res = false;
			return res;
		}
		else if(m_pString)
		{
			wxString tmp = pControl->GetValue();
			if(!tmp.IsEmpty())
			{
				*m_pString = tmp;
				return true;
			}
			return false;
		}
	}
	else
#endif

#if wxUSE_SPINCTRL
		if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinCtrl)) )
		{
			wxSpinCtrl * pControl = (wxSpinCtrl*) m_validatorWindow;
			
			if(m_pLong)
			{
				int tmp = pControl->GetValue();
				*m_pLong = (long)tmp;
				return true;
			}
		}
		else
#endif

#if wxUSE_FILEPICKERCTRL
	if (m_validatorWindow->IsKindOf(CLASSINFO(wxFilePickerCtrl)))
	{
		wxFilePickerCtrl * pControl = (wxFilePickerCtrl*) m_validatorWindow;
		if (m_pString)
		{
			*m_pString = pControl->GetPath();
			return true;
		}
		else
			return false;
	}
	else
#endif
		return false;
	return false;
}

bool wxFPValidator::TransferToWindow()
{	
	if(!m_validatorWindow) 
	{
		return false;
	}
#if wxUSE_SPINCTRL
	if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinCtrl)) )
	{
		wxSpinCtrl * pControl = (wxSpinCtrl *) m_validatorWindow;
		if(m_pLong)
		{
			pControl->SetValue((int)*m_pLong);
			return true;
		}
	}
	else
#endif
#if wxUSE_TEXTCTRL
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)) )
    {
        wxTextCtrl * pControl = (wxTextCtrl*) m_validatorWindow;
		if(m_pDouble)
		{
			pControl->SetValue(wxString::Format(wxT("%1.6f"), *m_pDouble));
			return true;
		}
		else if(m_pInt)
		{
			pControl->SetValue(wxString::Format(wxT("%i"), *m_pInt));
			return true;
		}
		else if(m_pLong)
		{
			pControl->SetValue(wxString::Format(wxT("%l"), *m_pLong));
			return true;
		}
		else if(m_pString)
		{
			pControl->SetValue(*m_pString);
			return true;
		}
	}
	else
#endif
#if wxUSE_FILEPICKERCTRL
	if (m_validatorWindow->IsKindOf(CLASSINFO(wxFilePickerCtrl)))
	{
		wxFilePickerCtrl * pControl = (wxFilePickerCtrl*) m_validatorWindow;
		if (m_pString)
		{
			pControl->SetPath(*m_pString);
			return true;
		}
		else
			return false;
	}
	else
#endif
		return false;
	return false;
}

bool wxFPValidator::Validate(wxWindow * parent)
{	
	if(!m_validatorWindow) 
	{
		return false;
	}
#if wxUSE_TEXTCTRL
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)) )
    {
        wxTextCtrl * pControl = (wxTextCtrl*) m_validatorWindow;		
		if(m_pDouble != NULL)
		{			
			return pControl->GetValue().ToDouble(m_pDouble);
		}
		else if(m_pInt != NULL)
		{
			double tmp;
			bool res = pControl->GetValue().ToDouble(&tmp);
			if(tmp >= INT_MIN && tmp <= INT_MAX && (fmod(tmp,1) == 0))
			{
				*m_pInt = (int)tmp;
			} else res = false;
			return res;
		}
		else if(m_pLong != NULL)
		{
			double tmp;
			bool res = pControl->GetValue().ToDouble(&tmp);
			if(tmp >= LONG_MIN && tmp <= LONG_MAX && (fmod(tmp,1) == 0))
			{
				*m_pLong = (long)tmp;
			} else res = false;
			return res;
		}
		else if(m_pString != NULL)
		{
			return !pControl->GetValue().IsEmpty();
		}
	}
	else
#endif
#if wxUSE_TEXTCTRL
		if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinCtrl)) )
		{
			wxSpinCtrl * pControl = (wxSpinCtrl*) m_validatorWindow;		
			if(m_pLong != NULL)
			{
				int tmp = pControl->GetValue();
				*m_pLong = (long)tmp;
				return true;
			}
		}
		else
#endif
#if wxUSE_FILEPICKERCTRL
	if (m_validatorWindow->IsKindOf(CLASSINFO(wxFilePickerCtrl)))
	{
		return true;
	}
	else
#endif
		return false;
	return false;
}
