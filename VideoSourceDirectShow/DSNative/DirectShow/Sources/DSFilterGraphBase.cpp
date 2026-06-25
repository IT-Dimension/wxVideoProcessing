
#include "../Include/DSFilterGraphBase.h"

DSFilterGraphBase::DSFilterGraphBase()
	: m_pGraphBuilder(NULL)
#ifdef USE_DS_OBJECT
	, DSObjectBase()
#endif	
	, m_pMediaControl(NULL)
	, m_pVideoWindow(NULL)
	, m_pMediaEventEx(NULL)
	, m_pMediaSeeking(NULL)
	, m_hWnd(NULL)
	, m_hWndNotify(NULL)
	, m_bShouldPreview(true)
	, m_pBasicAudio(NULL)
	, m_pBasicVideo(NULL)
	, m_bMute(false)
	, m_iVolume(0)
#ifdef ROT_DEBUG
	, m_pDSYoooROTFilter(NULL)
#endif
{
}

DSFilterGraphBase::~DSFilterGraphBase()
{
	Stop();
	CloseInterfaces();
}

STDMETHODIMP DSFilterGraphBase::Start()
{
	HRESULT hr = S_OK;
	if (!m_pMediaControl)
	{
		hr = InitInterfaces();
		CHECKRET(hr == S_OK,hr);
		CHECKRET(m_pMediaControl,E_FAIL);
	}
	SetVolume(m_iVolume);
	hr = S_FALSE;
	while (hr == S_FALSE)
	{
		hr = m_pMediaControl->Run();
		if (hr == S_FALSE) Sleep(50);
	}
	return hr;
}

STDMETHODIMP DSFilterGraphBase::Stop()
{
	CHECKRET(m_pMediaControl,E_FAIL);
	m_pMediaControl->Stop();
	return CloseInterfaces();
}

STDMETHODIMP DSFilterGraphBase::Pause()
{
	if (!m_pMediaControl)
	{
		HRESULT hr = Start();
		hr = InitInterfaces();
		CHECKRET(hr == S_OK,hr);
	}
	return m_pMediaControl->Pause();
}

HRESULT DSFilterGraphBase::InitInterfaces()
{
	try
	{
		CloseInterfaces();
		HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, 
			IID_IGraphBuilder, (void **)&m_pGraphBuilder);
		SAFECHECK(hr);

#ifdef ROT_DEBUG
		m_pDSYoooROTFilter = new DSYoooROTFilter();
		hr = m_pDSYoooROTFilter->AddToFilterGraph(m_pGraphBuilder);
		SAFECHECK(hr);
#endif
		return S_OK;
	}
	catch (...)
	{
		return E_FAIL;
	}
}

HRESULT DSFilterGraphBase::CloseInterfaces()
{
#ifdef ROT_DEBUG
	if (m_pGraphBuilder && m_pDSYoooROTFilter) m_pDSYoooROTFilter->RemoveFromFilterGraph(m_pGraphBuilder);
	SAFEDELETE(m_pDSYoooROTFilter);
#endif
	if (m_pMediaEventEx)
	{
		m_pMediaEventEx->SetNotifyWindow(NULL,NULL,NULL);
		SAFERELEASE(m_pMediaEventEx);
	}	
	if (m_pVideoWindow)
	{
		m_pVideoWindow->put_Visible(FALSE);
		m_pVideoWindow->put_Owner(NULL);
		SAFERELEASE(m_pVideoWindow);
	}
	SAFERELEASE(m_pMediaSeeking);
	SAFERELEASE(m_pBasicVideo);
	SAFERELEASE(m_pBasicAudio);
	SAFERELEASE(m_pMediaControl);
	SAFERELEASE(m_pGraphBuilder);
	return S_OK;
}

HWND DSFilterGraphBase::get_NotifyWindow()
{
	return m_hWndNotify;
}

void DSFilterGraphBase::set_NotifyWindow(HWND hWnd)
{
	m_hWndNotify = hWnd;
}

HWND DSFilterGraphBase::get_PreviewWindow()
{
	return m_hWnd;
}

void DSFilterGraphBase::set_PreviewWindow(HWND hWnd)
{
	m_hWnd = hWnd;
}

bool DSFilterGraphBase::get_ShouldPreview()
{
	return m_bShouldPreview;
}

void DSFilterGraphBase::set_ShouldPreview(bool bShouldPreview)
{
	m_bShouldPreview = bShouldPreview;
	SettingUpVideoWindow();
}

HRESULT DSFilterGraphBase::PreparePlayback()
{
	HRESULT hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl,(void**)&m_pMediaControl);
	SAFECHECKCLOSE(hr);
	hr = m_pGraphBuilder->QueryInterface(IID_IBasicVideo,(void**)&m_pBasicVideo);
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaSeeking,(void**)&m_pMediaSeeking);
	hr = m_pGraphBuilder->QueryInterface(IID_IVideoWindow,(void**)&m_pVideoWindow);	
	if (hr != E_NOINTERFACE && m_pBasicVideo) SettingUpVideoWindow();
	if (m_hWndNotify)
	{
		hr = m_pGraphBuilder->QueryInterface(IID_IMediaEventEx,(void**)&m_pMediaEventEx);
		hr = m_pMediaEventEx->SetNotifyWindow((OAHWND)m_hWndNotify,WM_GRAPH_NOTIFY,(LONG_PTR)this);
		//::SetWindowLong(m_hWnd, GWL_STYLE, ::GetWindowLong(m_hWnd, GWL_STYLE) &~SS_GRAYRECT);
	}
	return hr;
}

HRESULT DSFilterGraphBase::ProcessStepMessage()
{
	HRESULT bFinished = S_OK;
	if (m_pMediaEventEx)
	{
		long lEventCode;
		LONG_PTR lParam1,lParam2;
		HRESULT hr = m_pMediaEventEx->GetEvent(&lEventCode,&lParam1,&lParam2,INFINITE);
		if (SUCCEEDED(hr))
		{
			bFinished = lEventCode == EC_STEP_COMPLETE ? S_FALSE : NOERROR;
			hr = m_pMediaEventEx->FreeEventParams(lEventCode,lParam1,lParam2);
			ASSERT(hr == S_OK);
		}
	}
	return bFinished;
}

HRESULT DSFilterGraphBase::ProcessGraphMessage()
{
	HRESULT bFinished = S_OK;
	if (m_pMediaEventEx)
	{
		long lEventCode;
		LONG_PTR lParam1,lParam2;
		//HRESULT hr = m_pMediaEventEx->GetEvent(&lEventCode,&lParam1,&lParam2,INFINITE);
		HRESULT hr = m_pMediaEventEx->GetEvent(&lEventCode,&lParam1,&lParam2,50);
		if (SUCCEEDED(hr))
		{
			bFinished = lEventCode == EC_COMPLETE ? S_FALSE : NOERROR;
			hr = m_pMediaEventEx->FreeEventParams(lEventCode,lParam1,lParam2);
			ASSERT(hr == S_OK);
		}
	}
	return bFinished;
}

bool DSFilterGraphBase::IsAudioSupported()
{
	if (!m_pBasicAudio)
	{
		if (m_pGraphBuilder)
		{
			if (SUCCEEDED(m_pGraphBuilder->QueryInterface(IID_IBasicAudio,(void**)&m_pBasicAudio)))
			{
				return true;
			}
		}
		return false;
	}
	return true;
}

void DSFilterGraphBase::SettingUpVideoWindow()
{
	if (m_pVideoWindow != NULL)
	{
		if (m_hWnd)
		{
			m_pVideoWindow->put_Owner((OAHWND)m_hWnd);
			m_pVideoWindow->put_MessageDrain((OAHWND)m_hWnd);
			m_pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
			ResizeVideoWindow();
		}
		if (!m_bShouldPreview)
		{
			m_pVideoWindow->put_AutoShow(OAFALSE);
			m_pVideoWindow->put_Visible(OAFALSE);
		}
		else
		{
			m_pVideoWindow->put_AutoShow(OATRUE);
			m_pVideoWindow->put_Visible(OATRUE);
		}
	}
}
void DSFilterGraphBase::ResizeVideoWindow()
{
	if (m_hWnd)
	{
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		ResizeVideoWindow(rect);
	}
}

void DSFilterGraphBase::ResizeVideoWindow(RECT rect)
{
	if (m_pVideoWindow != NULL) m_pVideoWindow->SetWindowPosition(rect.left, rect.top, rect.right, rect.bottom);
}

int DSFilterGraphBase::get_Volume()
{
	return m_iVolume;
}
void DSFilterGraphBase::set_Volume(int nVolume)
{
	m_iVolume = nVolume;
	if (!m_bMute)
	{
		SetVolume(m_iVolume);
	}
}

bool DSFilterGraphBase::get_Mute()
{
	return m_bMute;
}

void DSFilterGraphBase::set_Mute(bool bMute)
{
	m_bMute = bMute;
	if (!m_bMute)
	{
		SetVolume(m_iVolume);
	}
	else
	{
		SetVolume(-10000);
	}
}


HRESULT DSFilterGraphBase::SetVolume(int nVolume)
{
	if (!m_pBasicAudio)
	{
		if (m_pGraphBuilder)
		{
			HRESULT hr = m_pGraphBuilder->QueryInterface(IID_IBasicAudio,(void**)&m_pBasicAudio);
			if (FAILED(hr)) return hr;
		}
		return E_NOINTERFACE;
	}
	return m_pBasicAudio->put_Volume(nVolume);
}

void DSFilterGraphBase::set_VideoSize(RECT _rect)
{
	if (m_pBasicVideo != NULL)
	{
		HRESULT hr;
		LONG lLeft,lTop,lWidth,lHeight;
		lLeft	= _rect.left;
		lTop	= _rect.top;
		lHeight	= _rect.bottom - lTop;
		lWidth	= _rect.right - lLeft;
		hr		= m_pBasicVideo->SetDestinationPosition(lLeft,lTop,lWidth,lHeight);
		ASSERT(hr == S_OK);
	}
}

RECT DSFilterGraphBase::get_VideoSize()
{
	RECT _rect;
	_rect.left = 0;
	_rect.top = 0;
	_rect.bottom = 0;
	_rect.right = 0;
	if (m_pBasicVideo != NULL)
	{
		HRESULT hr;
		LONG lLeft,lTop,lWidth,lHeight;
		hr = m_pBasicVideo->GetDestinationPosition(&lLeft,&lTop,&lWidth,&lHeight);
		ASSERT(hr == S_OK);
		_rect.left = lLeft;
		_rect.top = lTop;
		_rect.bottom = lTop + lHeight;
		_rect.right = lLeft + lWidth;
	}
	return _rect;
}

RECT DSFilterGraphBase::get_SourceVideoSize()
{
	RECT _rect;
	_rect.left = 0;
	_rect.top = 0;
	_rect.bottom = 0;
	_rect.right = 0;
	if (m_pBasicVideo != NULL)
	{
		HRESULT hr;
		LONG lLeft,lTop,lWidth,lHeight;
		hr = m_pBasicVideo->GetSourcePosition(&lLeft,&lTop,&lWidth,&lHeight);
		ASSERT(hr == S_OK);
		_rect.left = lLeft;
		_rect.top = lTop;
		_rect.bottom = lTop + lHeight;
		_rect.right = lLeft + lWidth;
	}
	return _rect;
}

bool DSFilterGraphBase::IsRunning()
{
	if (m_pMediaControl)
	{
		OAFilterState _state;
		HRESULT hr = m_pMediaControl->GetState(50,&_state);
		ASSERT(hr == S_OK);
		if (SUCCEEDED(hr))
		{
			return (_state != State_Stopped);
		}
	}
	return false;
}

bool DSFilterGraphBase::IsPaused()
{
	if (m_pMediaControl)
	{
		OAFilterState _state;
		HRESULT hr = m_pMediaControl->GetState(50,&_state);
		ASSERT(hr == S_OK);
		if (SUCCEEDED(hr))
		{
			return (_state == State_Paused);
		}
	}
	return false;
}
REFERENCE_TIME DSFilterGraphBase::get_Position()
{
	if (m_pMediaSeeking)
	{
		REFERENCE_TIME _time;
		HRESULT hr = m_pMediaSeeking->GetCurrentPosition(&_time);
		ASSERT(hr == S_OK);
		if (SUCCEEDED(hr)) return _time;
	}
	return -1;
}
void DSFilterGraphBase::set_Position(REFERENCE_TIME _time)
{
	if (m_pMediaSeeking)
	{
		REFERENCE_TIME _stop = 0;
		HRESULT hr = m_pMediaSeeking->SetPositions(&_time,AM_SEEKING_AbsolutePositioning,&_stop,AM_SEEKING_NoPositioning);
		//ASSERT(hr == S_OK);
	}
}

REFERENCE_TIME DSFilterGraphBase::get_Duration()
{
	if (m_pMediaSeeking)
	{
		REFERENCE_TIME _time;
		HRESULT hr = m_pMediaSeeking->GetDuration(&_time);
		ASSERT(hr == S_OK);
		if (SUCCEEDED(hr)) return _time;
	}
	return -1;
}

IBaseFilter * DSFilterGraphBase::GetVideoRenderer()
{
	IBaseFilter * _filter = NULL;
	if (m_pGraphBuilder)
	{
		HRESULT hr;
		IEnumFilters * pEnumFilters;
		hr = m_pGraphBuilder->EnumFilters(&pEnumFilters);
		ASSERT(hr == S_OK);
		if (SUCCEEDED(hr))
		{
			IBaseFilter * pFilter;
			ULONG ulFetched;
			hr = pEnumFilters->Next(1,&pFilter,&ulFetched);
			while (hr != S_FALSE)
			{
				IVideoWindow * pVideoWindow;
				hr = pFilter->QueryInterface(IID_IVideoWindow,(void**)&pVideoWindow);
				if (SUCCEEDED(hr))
				{
					SAFERELEASE(pVideoWindow);
					_filter = pFilter;
					break;
				}
				SAFERELEASE(pFilter);
				hr = pEnumFilters->Next(1,&pFilter,&ulFetched);
			}
			SAFERELEASE(pEnumFilters);
		}
	}
	return _filter;
}

IBaseFilter * DSFilterGraphBase::GetAudioRenderer()
{
	IBaseFilter * _filter = NULL;
	if (m_pGraphBuilder)
	{
		HRESULT hr;
		IEnumFilters * pEnumFilters;
		hr = m_pGraphBuilder->EnumFilters(&pEnumFilters);
		ASSERT(hr == S_OK);
		if (SUCCEEDED(hr))
		{
			IBaseFilter * pFilter;
			ULONG ulFetched;
			hr = pEnumFilters->Next(1,&pFilter,&ulFetched);
			while (hr != S_FALSE)
			{
				IBasicAudio * pBasicAudio;
				hr = pFilter->QueryInterface(IID_IBasicAudio,(void**)&pBasicAudio);
				if (SUCCEEDED(hr))
				{
					SAFERELEASE(pBasicAudio);
					_filter = pFilter;
					//_filter = new DSFilterBase(pFilter,true);
					break;
				}
				SAFERELEASE(pFilter);
				hr = pEnumFilters->Next(1,&pFilter,&ulFetched);
			}
			SAFERELEASE(pEnumFilters);
		}
	}
	return _filter;
}