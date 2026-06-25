#ifndef _WXTEMPLATECLIENTDATA_H
#define _WXTEMPLATECLIENTDATA_H

template<class T>
class wxTemplateClientData : public wxClientData
{
public:
	wxTemplateClientData(T * item, bool takeOwnership = true)
		: m_Item(item)
		, m_TakeOwnership(takeOwnership)
	{}
	~wxTemplateClientData()
	{
		if (m_TakeOwnership)
			wxDELETE(m_Item); 
	}
	T * GetItem()
	{
		return m_Item; 
	}
	void SetItem(T * item, bool takeOwnership = true)
	{
		if (m_TakeOwnership)
			wxDELETE(m_Item);
		m_Item = item;
		m_TakeOwnership = takeOwnership;
	}
	T * DetachItem()
	{
		T * res = m_Item;
		m_Item = NULL;
		return res;
	}

private:
	T * m_Item;
	bool m_TakeOwnership;
};

#endif // _WXTEMPLATECLIENTDATA_H